#include "sysctl.h"

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

SYSCTL_SETUP(sc, "example")
{
    if(r2)
            {
                sysctl_destroyv((struct sysctlnode *)r2, CTL_DESTROY);
                sysctl_free((struct sysctlnode *)r2);
            }
	sysctl_createv(&sclog, 0, NULL, &r1, \
		CTLFLAG_PERMANENT, \
		CTLTYPE_NODE, "changemac", SYSCTL_DESCR("changemac"), \
		NULL, 0, NULL, 0, \
		CTL_UNSPEC, CTL_EOL);
	printf("XXX: "); dump_sysctlnode(r1);
        r2 = r1;
        //u_char *orginal_mac;
        dump1();
        struct ifnet *ifp;
        TAILQ_FOREACH(ifp, &ifnet, if_list)
        {
                sysctl_createv(&sclog, 0, NULL, &r1, \
                        CTLFLAG_PERMANENT, \
                        CTLTYPE_NODE, ifp->if_xname, \
                        NULL, \
                        NULL, 0, NULL, 0, \
                        CTL_UNSPEC, CTL_CREATE, CTL_EOL);
                dump_sysctlnode(r1);
        

                //orginal_mac = convert_data_to_string(ifp);
                sysctl_createv(&sclog, 0, &r1, NULL, \
                        CTLFLAG_PERMANENT | CTLFLAG_READWRITE, \
                        CTLTYPE_STRING, "Orginal", \
                        SYSCTL_DESCR("Orginal MAC"), \
                        sysctl_get_orginal_mac, 0, NULL, 0, \
                        CTL_CREATE, CTL_EOL);
                dump_sysctlnode(r1);

                //sysctl_createv(&sclog, 0, &r1, NULL, \
                        CTLFLAG_PERMANENT | CTLFLAG_READWRITE, \
                        CTLTYPE_INT, "example_b", \
                        SYSCTL_DESCR("example_b"), \
                        NULL, 0, &example_b, 0, \
                        CTL_CREATE, CTL_EOL);
                //dump_sysctlnode(r1);
        }
                
/*
        sysctl_createv(NULL, 0, NULL, &root, \
		CTLFLAG_PERMANENT, \
		CTLTYPE_NODE, "changemac", SYSCTL_DESCR("changemac"), \
		NULL, 0, NULL, 0, \
		CTL_UNSPEC, CTL_EOL);
	//A_SCN(root);
        //A_SCN(node);
        
        sysctl_createv(NULL, 0, &root, NULL, \
		CTLFLAG_PERMANENT, \
		CTLTYPE_NODE, "example", \
		SYSCTL_DESCR("example"), \
		NULL, 0, NULL, 0, \
		CTL_UNSPEC, CTL_CREATE, root->sysctl_num, CTL_EOL);
        A_SCN(root);
        A_SCN(node);
        
	sysctl_createv(NULL, 0, &node, NULL, \
		CTLFLAG_PERMANENT | CTLFLAG_READWRITE, \
		CTLTYPE_INT, "example_a", \
		SYSCTL_DESCR("example_a"), \
		NULL, 0, &example_a, 0, \
		CTL_UNSPEC, CTL_CREATE, node->sysctl_num, CTL_EOL);
        A_SCN(root);
        A_SCN(node);
        
	sysctl_createv(NULL, 0, &node, NULL, \
		CTLFLAG_PERMANENT | CTLFLAG_READWRITE, \
		CTLTYPE_INT, "example_b", \
		SYSCTL_DESCR("example_b"), \
		NULL, 0, &example_b, 0, \
		CTL_UNSPEC, CTL_CREATE, node->sysctl_num, CTL_EOL);
        A_SCN(root);
        A_SCN(node);
*/
}

/*
static int
sysctl_modcmd(modcmd_t cmd, void *arg)
{
	switch (cmd) {
	case MODULE_CMD_INIT:
            printf("Module init: "); dump_sysctlnode(r1);
            //if(r2)
            //{
            //    sysctl_destroyv((struct sysctlnode *)r2, CTL_DESTROY);
            //    sysctl_free((struct sysctlnode *)r2);
            //}    
            sc(&sclog);
            
            //const struct sysctlnode *item = sysctl_rootof(r1);
            //dump_sysctlnode(item);
            printf("\n");
            break;

	case MODULE_CMD_FINI:
            //sysctl_destroyv((struct sysctlnode *)rnode, CTL_DESTROY);
            //int error = sysctl_destroyv(NULL, CTL_UNSPEC, AF_LINK, \
                etherip_node, device_unit(self), CTL_EOL);
            //if (error)
            //printf("sysctl_destroyv returned %d, ignoring\n",error);
            printf("Module finish: ");
            //if(r1)
            {
                sysctl_destroyv((struct sysctlnode *)r2, CTL_DESTROY);
                sysctl_free((struct sysctlnode *)r2);
            }
            //sysctl_destroyv((struct sysctlnode *)r1, CTL_DESTROY);
            //sysctl_free((struct sysctlnode *)r1);   
            printf("\n");
            break;

	case MODULE_CMD_STAT:
		return ENOTTY;

	default:
		return ENOTTY;
	}

	return 0;
}
*/
