#include "changemac.h"

MODULE(MODULE_CLASS_MISC, changemac, NULL);

void
display_time() {
    struct timeval tv;
    getmicrotime(&tv);
    unsigned int h = (tv.tv_sec / 3600) % (24);
    unsigned int m = (tv.tv_sec / 60) % (60);
    unsigned int s = tv.tv_sec % 60;
    printf("changemac time: %.2d:%.2d:%.2d\n", h, m, s);
}

void
dump_all_ifnet_sdl() {
  struct ifnet *currentif;
  struct ifnet **vectorif = ifindex2ifnet;
  int ifl = if_indexlim;
  int ifn;
  int i;

  printf("dump_all_ifnet_sdl()\n");
  printf("Name\t_len\t_index\t_type\t_nlen\t_alen\t_slen\n");
  for (ifn = 0; ifn < ifl; ifn++) 
  {
     currentif = vectorif[ifn];
     if (currentif) 
     {
       struct sockaddr_dl *p = \
         (struct sockaddr_dl *) currentif->if_sadl;

       printf("%s\t%d\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", \
         currentif->if_xname, \
         p->sdl_len, \
         p->sdl_index, \
         p->sdl_type, \
         p->sdl_nlen, \
         p->sdl_alen, \
         p->sdl_slen);
     }
  }
  printf("\n");
}

void
dump_sysctlnode(const struct sysctlnode *p)
{
    if(!p) 
    { 
        printf("sysctlnode NULL\n"); 
    } 
    else 
    {
        printf("sysctlnode: %p\n", p);
        printf("\tsysctl_num: %d\n", p->sysctl_num);
        printf("\tsysctl_name: %s\n", p->sysctl_name);
        printf("\tsysctl_desc: %s\n", p->sysctl_desc);
        printf("\n");
    }
}

struct ifnet *
get_ifnet_from_name(u_char *name)
{
    struct ifnet *ifp;
    TAILQ_FOREACH(ifp, &ifnet, if_list)
    {
        struct sockaddr_dl *sdp = (struct sockaddr_dl *)ifp;
        if(!strncmp(ifp->if_xname, name, sdp->sdl_nlen))
            return ifp;
    }
    return NULL;
}

u_char *
get_mac_from_ifnet(struct ifnet *p)
{
    struct sockaddr_dl *q = (struct sockaddr_dl *)p->if_sadl;
    u_char *mac = (u_char *)malloc(q->sdl_alen, M_TEMP, M_WAITOK|M_ZERO);
    u_char *data = (u_char *)(q->sdl_data + q->sdl_nlen);
    u_short index;
    
    for(index = 0; index < q->sdl_alen; index++)
        mac[index] = data[index];
    
    return (u_char *)mac;
}

u_char *
convert_data_to_string(struct ifnet *p)
{
        u_char *str = get_mac_from_ifnet(p);
	u_char table[17] = "0123456789abcdef:";
	u_char *ret = (u_char *)malloc(3*6, M_TEMP, M_WAITOK|M_ZERO);
	u_short index;
	for(index = 0; index < 6; index++)
	{
		if(str[index] == '\x00')
		{
			ret[3*index] 	= table[0];
			ret[3*index+1] 	= table[0];
			ret[3*index+2]	= table[16];
		}
		else
		{
			ret[3*index] 	= table[(str[index] >> 4) & 0x0f];
			ret[3*index+1] 	= table[(str[index] >> 0) & 0x0f];
			ret[3*index+2]	= table[16];
		}
	}
	ret[17] = '\0';
	return ret;
}

static int
sysctl_set_new_mac(SYSCTLFN_ARGS)
{
    
}

static int
sysctl_get_orginal_mac(SYSCTLFN_ARGS)
{
    //#define SYSCTLFN_ARGS const int *name, u_int namelen, \
	void *oldp, size_t *oldlenp, \
	const void *newp, size_t newlen, \
	const int *oname, struct lwp *l, \
	const struct sysctlnode *rnode
    /*
    printf("sysctl_get_orginal_mac:\n");
    printf("\tconst int *name: %p\n", name);
    printf("\tu_int namelen: %d\n", namelen);
    printf("\tvoid *oldp: %p\n", oldp);
    printf("\tsize_t *oldlenp: %p\n", oldlenp);
    printf("\tconst void *newp: %p\n", newp);
    printf("\tsize_t newlen: %d\n", newlen);
    printf("\tconst int *oname: %p\n", oname);
    printf("\tstruct lwp *l: %p\n", l);
    printf("\tconst struct sysctlnode *rnode: %p\n", rnode);
    */
            
    int error = 0;
    struct sysctlnode node;
    
    if(namelen != 0)
        return EINVAL;
    
    node = *rnode;
    struct sysctlnode *parent = (struct sysctlnode *)node.sysctl_parent;
    //printf("node.sysctl_name: %s\n", parent->sysctl_name);
    node.sysctl_data = \
            convert_data_to_string(get_ifnet_from_name(parent->sysctl_name));
    
    
    error = sysctl_lookup(SYSCTLFN_CALL(&node));
    
    if(error)
        return error;
    
    
    return 0;
}

void
changemac_sysctl_init(struct sysctllog *sclog)
{

	sysctl_createv(&sclog, 0, NULL, &r1, \
		CTLFLAG_PERMANENT, \
		CTLTYPE_NODE, "changemac", SYSCTL_DESCR("changemac"), \
		NULL, 0, NULL, 0, \
		CTL_UNSPEC, CTL_EOL);
	//printf("XXX: "); dump_sysctlnode(r1);
        r2 = r1;
        //u_char *orginal_mac;
        //dump_all_ifnet_sdl();
        struct ifnet *ifp;
        TAILQ_FOREACH(ifp, &ifnet, if_list)
        {
                sysctl_createv(&sclog, 0, NULL, &r1, \
                        CTLFLAG_PERMANENT, \
                        CTLTYPE_NODE, ifp->if_xname, \
                        NULL, \
                        NULL, 0, NULL, 0, \
                        CTL_UNSPEC, CTL_CREATE, CTL_EOL);
                //dump_sysctlnode(r1);
        

                //orginal_mac = convert_data_to_string(ifp);
                sysctl_createv(&sclog, 0, &r1, NULL, \
                        CTLFLAG_PERMANENT | CTLFLAG_READONLY, \
                        CTLTYPE_STRING, "orginal", \
                        SYSCTL_DESCR("Orginal MAC"), \
                        sysctl_get_orginal_mac, 0, NULL, 0, \
                        CTL_CREATE, CTL_EOL);
                //dump_sysctlnode(r1);

                //sysctl_createv(&sclog, 0, &r1, NULL, \
                        CTLFLAG_PERMANENT | CTLFLAG_READWRITE, \
                        CTLTYPE_INT, "example_b", \
                        SYSCTL_DESCR("example_b"), \
                        NULL, 0, &example_b, 0, \
                        CTL_CREATE, CTL_EOL);
                //dump_sysctlnode(r1);
        }
}

void changemac_sysctl_exit()
{
        sysctl_destroyv((struct sysctlnode *)r2, CTL_DESTROY);
        sysctl_free((struct sysctlnode *)r2);    
}

static void
init()
{
        changemac_sysctl_init(sclog);
}

static void
exit()
{
        changemac_sysctl_exit();
}

static int
changemac_modcmd(modcmd_t cmd, void *arg)
{
        switch (cmd) {
                case MODULE_CMD_INIT:
                    printf("changemac loaded. ");
                    display_time();
                        init();
                break;

                case MODULE_CMD_FINI:
                        printf("changemac unloaded.\n");
                        display_time();
                        exit();
                break;

                case MODULE_CMD_STAT:
                        return ENOTTY;

                default:
                        return ENOTTY;
        }
return 0;
}

