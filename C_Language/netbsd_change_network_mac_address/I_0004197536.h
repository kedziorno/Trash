/* 
 * File:   tools.h
 * Author: kedzior
 *
 * Created on 6 wrzesień 2013, 12:06
 */

#ifndef TOOLS_H
#define	TOOLS_H

#include "headers.h"

#ifdef	__cplusplus
extern "C" {
#endif
        struct ifnet *
        get_ifnet_from_name(u_char *name);
    
        u_char*                         
        get_mac_from_ifnet(struct ifnet *p);

        u_char *
        convert_data_to_string(struct ifnet *p);

#ifdef	__cplusplus
}
#endif

#endif	/* TOOLS_H */

