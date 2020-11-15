#include <sys/cdefs.h>
//      \
__KERNEL_RCSID(0, "$NetBSD: example.c,v 1.7 2010/10/25 22:41:42 jnemeth Exp $");

#include <sys/param.h>
#include <sys/kernel.h>
#include <sys/module.h>

#include <sys/queue.h>
#include <net/if.h>
#include <net/if_dl.h>
#include <sys/malloc.h>
#include <sys/sysctl.h>

//#include "headers.h"



static void _change_mac(u_short, u_char *);





static void
change_mac(char *if_name, u_char *new_mac)
{
    struct ifnet *ifp;
    TAILQ_FOREACH(ifp, &ifnet, if_list)
    {
        char *name = ifp->if_xname;
        struct sockaddr_dl *p = (struct sockaddr_dl *)ifp->if_sadl;
        if(!strncmp(name, if_name, p->sdl_nlen))
        {
            u_short if_index = p->sdl_index;
            _change_mac(if_index, new_mac);
        }
    }
}

static void
_change_mac(u_short if_index, u_char *new_mac) 
{
        struct ifnet *ifp;
        u_short index;

        TAILQ_FOREACH(ifp, &ifnet, if_list) 
        {
                struct sockaddr_dl *q = (struct sockaddr_dl *) ifp->if_sadl;
                if (if_index == q->sdl_index) 
                {
                        u_char *dest = (u_char *) (q->sdl_data + q->sdl_nlen);
                        for (index = 0; index < q->sdl_alen; index++) 
                                dest[index] = new_mac[index];
                }
        }
}

static void
copy_to_BM(BM *dst, struct sockaddr_dl *q)
{
    u_short index;
    dst->if_index = q->sdl_index;
    strncpy(dst->if_name, q->sdl_data, q->sdl_nlen);
    u_char *p = (u_char *) (q->sdl_data + q->sdl_nlen);
    for (index = 0; index < q->sdl_alen; index++) 
    {
        dst->old_mac[index] = p[index];
    }
}

static void
copy_from_BM(struct sockaddr_dl *q, BM *src)
{
    u_short index;
    u_char *p = (u_char *) (q->sdl_data + q->sdl_nlen);
    for (index = 0; index < q->sdl_alen; index++) 
    {
        p[index] = src->old_mac[index];
    }
}

static void
backup_all_mac()
{
    struct ifnet *ifp;
    
    LIST_INIT(&BM_HEAD);
    TAILQ_FOREACH(ifp, &ifnet, if_list)
    {
        BM *item = (BM *)malloc(sizeof(BM), \
                M_TEMP, M_WAITOK | M_ZERO);
        LIST_INSERT_HEAD(&BM_HEAD, item, BM_LIST);
        
        struct sockaddr_dl *q = (struct sockaddr_dl *)ifp->if_sadl;
        
        copy_to_BM(item, q);
    }
}



static void
_backup_all_mac()
{
        struct ifnet *ifp;
        u_short index;

        TAILQ_FOREACH(ifp, &ifnet, if_list) 
        {
                struct sockaddr_dl *q = (struct sockaddr_dl *) ifp->if_sadl;
                struct backup_mac *new = &backup[q->sdl_index - 1];
                new->if_index = q->sdl_index;
                strncpy(new->if_name, q->sdl_data, q->sdl_nlen);
                u_char *p = (u_char *) (q->sdl_data + q->sdl_nlen);
                for (index = 0; index < q->sdl_alen; index++) 
                {
                        new->old_mac[index] = p[index];
                }
                //strncpy(new->old_mac, \
                (unsigned char *)(q->sdl_data + q->sdl_nlen), \
                q->sdl_alen);
                num_backup++;
        }
}

static int
alloc_backup()
{
        backup = malloc(if_indexlim * sizeof (struct backup_mac), \
                M_TEMP, M_WAITOK | M_ZERO);
        num_backup = 0;
}

static void
free_backup()
{
        free(backup, M_TEMP);
}

u_char *mac;



static void
dupa()
{
    struct ifnet *p;
    TAILQ_FOREACH(p, &ifnet, if_list)
    {
        printf("Int: %s, mac: %s\n", p->if_xname, convert_data_to_string(p));
    }
}

static void
exit()
{
    BM *item;
    LIST_FOREACH(item, &BM_HEAD, BM_LIST)
    {
        LIST_REMOVE(item, BM_LIST);
        free(item, M_TEMP);
    }
}
