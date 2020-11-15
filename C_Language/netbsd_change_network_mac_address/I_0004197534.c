#include <sys/param.h>
#include <sys/kernel.h>
#include <sys/module.h>

#include <sys/sysctl.h>
#include <sys/malloc.h>
#include <net/if.h>
#include <net/if_dl.h>

//#include "headers.h"







static void
dump_backup()
{
    printf("\ndump_backup():\n");
    printf("IF_INDEX\tIF_NAME\tOLD_MAC\n");
    BM *item;
    LIST_FOREACH(item, &BM_HEAD, BM_LIST)
    {
        printf("%d\t\t", item->if_index);
        printf("%s\t", item->if_name);
        printf("%02x:%02x:%02x:%02x:%02x:%02x\n", \
                item->old_mac[0], \
                item->old_mac[1], \
                item->old_mac[2], \
                item->old_mac[3], \
                item->old_mac[4], \
                item->old_mac[5]);
    }
}

void
_dump_backup() 
{
        printf("\n*** dump_backup()\n");
        u_short index;
        printf("IF_INDEX\tIF_NAME\tOLD_MAC\n");
        for (index = 0; index < num_backup; index++) 
        {
                struct backup_mac *p = &backup[index];
                printf("%d\t\t", p->if_index);
                printf("%s\t", p->if_name);
                printf("%02x:%02x:%02x:%02x:%02x:%02x\n", \
                        p->old_mac[0], \
                        p->old_mac[1], \
                        p->old_mac[2], \
                        p->old_mac[3], \
                        p->old_mac[4], \
                        p->old_mac[5]);
        }
        printf("\n");
}

void
list_interfaces()
{
        struct ifnet *p;

        printf("\"Active\" interfaces: ");
        TAILQ_FOREACH(p, &ifnet, if_list) {
                printf("%s ", p->if_xname);
        }
        printf(".\n");
}

void
dump_sdl_data(struct sockaddr_dl *p) 
{
  char *q = (char *) p->sdl_data;
  u_short index;
  //printf("\n*** dump_sdl_data(%p)\n", p);
  for (index = 0; index < 12; index++) 
  {
        printf("%x:", (unsigned char) q[index]);
  }
  //printf("\n");
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


void
list_mac_addr_1() {
  struct ifnet *currentif; // current interface p.
  struct ifnet **vectorif = \
  ifindex2ifnet; // vector of interfaces
  int ifl = if_indexlim; // len. interface vector
  int ifn; // current interface n.
  int i; // temp

  printf("\n*** list_mac_addr_1()\n");
  for (ifn = 0; ifn < ifl; ifn++) 
  {
    currentif = vectorif[ifn];
    if (currentif) 
    {
      struct sockaddr_dl *p = \
        (struct sockaddr_dl *) currentif->if_sadl;
      char *addr = (char *) (p->sdl_data + p->sdl_nlen);

      printf("Name: %s\tMAC address: ", currentif->if_xname);
      /*
      for(i = 0; i < 6; i++)
      {
        if(i < 5)
          printf("%02x:", addr[i]);
        else
          printf("%02x", addr[i]);
      }
      printf("\n");
      */
      dump_sdl_data(p);
      printf("\n");
    }
  }
  printf("\n");
}

void
dump1() {
  struct ifnet *currentif; // current interface p.
  struct ifnet **vectorif = \
  ifindex2ifnet; // vector of interfaces
  int ifl = if_indexlim; // len. interface vector
  int ifn; // current interface n.
  int i; // temp

  printf("\n*** dump1()\n");
  printf("Name\tsdl_len\tsdl_index\tsdl_type\tsdl_nlen\tsdl_alen\tsdl_slen\n");
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
