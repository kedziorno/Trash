/* 
 * File:   headers.h
 * Author: kedzior
 *
 * Created on 3 wrzesień 2013, 20:16
 */

#ifndef HEADERS_H
#define	HEADERS_H

#ifdef	__cplusplus
extern "C" {
#endif
    #include <sys/cdefs.h>
    __KERNEL_RCSID\
            (0, "$NetBSD: example.c,v 1.7 2010/10/25 22:41:42 jnemeth Exp $");

    #include <sys/param.h>
    #include <sys/kernel.h>
    #include <sys/module.h>

    #include <sys/sysctl.h>
    #include <sys/malloc.h>
    #include <net/if.h>
    #include <net/if_dl.h>

    struct backup_mac {
        u_char        old_mac[6];
        u_char        if_name[16];
        u_short       if_index;
        LIST_ENTRY(backup_mac) BM_LIST;
    };

    typedef struct backup_mac                   BM;
    LIST_HEAD(BMHEAD, backup_mac)               BM_HEAD;
    struct BMHEAD                               *head;

    struct backup_mac                           *backup;
    u_short                                     num_backup;


    
    void                        dump_sysctlnode(const struct sysctlnode *);
    void                        dump1();
    
#ifdef	__cplusplus
}
#endif

#endif	/* HEADERS_H */

