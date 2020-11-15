/* 
 * File:   changemac.h
 * Author: kedzior
 *
 * Created on 6 wrzesień 2013, 12:02
 */

#ifndef CHANGEMAC_H
#define	CHANGEMAC_H

#ifdef	__cplusplus
extern "C" {
#endif
        #include <sys/cdefs.h>
        __KERNEL_RCSID\
        (0, "$NetBSD: example.c,v 1.7 2010/10/25 22:41:42 jnemeth Exp $");

        #include <sys/param.h>
        #include <sys/kernel.h>
        #include <sys/module.h>
        #include <sys/time.h>

        #include <sys/sysctl.h>
        #include <sys/malloc.h>
        #include <net/if.h>
        #include <net/if_dl.h>

        /* BM */
        struct backup_mac {
                u_char        old_mac[6];
                u_char        if_name[16];
                u_short       if_index;
                LIST_ENTRY(backup_mac) BM_LIST;
        };

        typedef struct backup_mac BM;
        LIST_HEAD(BMHEAD, backup_mac) BM_HEAD;
        struct BMHEAD *head;

        struct backup_mac *backup;
        u_short num_backup;
    
        /* Tools */
        u_char *get_mac_from_ifnet(struct ifnet *p);
        u_char *convert_data_to_string(struct ifnet *p);
        struct ifnet *get_ifnet_from_name(u_char *name);
        
        /* Sysctl */
        const struct sysctlnode *r1 = NULL, *r2 = NULL ;
        struct sysctllog *sclog = NULL;
        void changemac_sysctl_init(struct sysctllog *sclog);
        void changemac_sysctl_exit();
        
        /* Dump */
        void dump_sysctlnode(const struct sysctlnode *node);
        void dump_all_ifnet_sdl();
#ifdef	__cplusplus
}
#endif

#endif	/* CHANGEMAC_H */

