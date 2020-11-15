#include <sys/param.h>
#include <sys/ioctl.h>
#include <sys/fcntl.h>
#include <sys/select.h>
#include <sys/poll.h>
#include <sys/malloc.h>
#include <sys/mutex.h>
#include <sys/proc.h>
#include <sys/kernel.h>
#include <sys/conf.h>
#include <sys/syscall.h>
#include <sys/callout.h>
#include <sys/vnode.h>
#include <sys/once.h>
#include <sys/module.h>
#include <sys/device.h>

MODULE(MODULE_CLASS_MISC, example, NULL);

dev_type_open(changemac_open);
dev_type_close(changemac_close);
dev_type_read(changemac_read);
dev_type_write(changemac_write);

const struct cdevsw 
changemac_cdevsw = {
        changemac_open, 
        changemac_close, 
        changemac_read, 
        changemac_write, 
        noioctl,
        nostop, 
        notty, 
        nopoll, 
        nommap, 
        nokqfilter, 
        D_OTHER,
};

int changemac_open      (dev_t dev, int flags, int ifmt, struct lwp *l);
int changemac_close     (dev_t dev, int flags, int ifmt, struct lwp *l);
int changemac_read      (dev_t dev, struct uio *uio, int ioflag);
int changemac_write     (dev_t dev, struct uio *uio, int ioflag);

int changemac_open(dev_t dev, int flags, int ifmt, struct lwp *l)
{
    // return ENXIO;
    return 0;
}

int changemac_close(dev_t dev, int flags, int ifmt, struct lwp *l)
{
    // return ENXIO;
    return 0;   
}

int changemac_read(dev_t dev, struct uio *uio, int ioflag)
{
    
}

int changemac_write(dev_t dev, struct uio *uio, int ioflag)
{
    
}

static void
init()
{
        // devmajor_t dev_t;
        // devsw_attach("changemac", \
        (const struct bdevsw *)NULL, (devmajor_t *)NULL, \
        (const struct cdevsw *)&changemac_cdevsw, (devmajor_t *)&dev_t);
    
        // const struct cdevsw *cdevsw_lookup(dev_t);
        // printf("devmajor_t : %p, %d\n", dev_t, dev_t); */
    
    struct cfdriver *item;
    printf("allcfdrivers: ");
    LIST_FOREACH(item, &allcfdrivers, cd_list)
    {
        printf("%s, ", item->cd_name);
    }
    
}

static void
exit()
{
    
}

static int
example_modcmd(modcmd_t cmd, void *arg)
{
        switch (cmd) 
        {
                case MODULE_CMD_INIT:
                    init();
                        break;

                case MODULE_CMD_FINI:
                        
                        break;

                case MODULE_CMD_STAT:
                        
                        return ENOTTY;

                default:
                        return ENOTTY;
        }
return 0;
}

