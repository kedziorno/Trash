#include "tools.h"

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

