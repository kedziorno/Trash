#include <sys/types.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int		lock_reg(int, int, int, off_t, int, off_t);

#define	read_lock(fd, offset, whence, len) \
	lock_reg(fd, F_SETLK, F_RDLCK, offset, whence, len)
#define	readw_lock(fd, offset, whence, len) \
	lock_reg(fd, F_SETLKW, F_RDLCK, offset, whence, len)
#define	write_lock(fd, offset, whence, len) \
	lock_reg(fd, F_SETLK, F_WRLCK, offset, whence, len)
#define	writew_lock(fd, offset, whence, len) \
	lock_reg(fd, F_SETLKW, F_WRLCK, offset, whence, len)
#define	un_lock(fd, offset, whence, len) \
	lock_reg(fd, F_SETLK, F_UNLCK, offset, whence, len)

pid_t	lock_test(int, int , off_t , int , off_t );

#define	is_readlock(fd, offset, whence, len) \
	lock_test(fd, F_RDLCK, offset, whence, len)
#define	is_writelock(fd, offset, whence, len) \
	lock_test(fd, F_WRLCK, offset, whence, len)

int lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len) {
	printf("%s() , param : fd=%d,cmd=%d,type=%d,offset=%d,whence=%d,len=%d\n",__func__,fd,cmd,type,offset,whence,len);
	struct flock lock;
	lock.l_type = type;     /* F_RDLCK, F_WRLCK, F_UNLCK */
	lock.l_start = offset;  /* byte offset relative to l_whence */
	lock.l_whence = whence; /* SEEK_SET, SEEK_CUR, SEEK_END */
	lock.l_len = len;       /* #bytes (0 means to EOF) */
	int ret = fcntl(fd, cmd, &lock);
	if(ret == -1) {
		printf("%s() : ",__func__);
		perror("fcntl");
	} else {
		return ret;
	}
}

pid_t	lock_test(int fd, int type, off_t offset, int whence, off_t len) {
	printf("%s() , param : fd=%d,type=%d,offset=%d,whence=%d,len=%d\n",__func__,fd,type,offset,whence,len);
	struct flock lock;
	lock.l_type = type;     /* F_RDLCK or F_WRLCK */
	lock.l_start = offset;  /* byte offset relative to l_whence */
	lock.l_whence = whence; /* SEEK_SET, SEEK_CUR, SEEK_END */
	lock.l_len = len;       /* #bytes (0 means to EOF) */
	if (fcntl(fd,F_GETLK,&lock) < 0) {
		printf("%s() : ",__func__);
		perror("fcntl");
		exit(1);
	}
	if (lock.l_type == F_UNLCK) {
		printf("%s() : have type F_UNLCK, return 0",__func__);
		return (0);        /* false, region is not locked by another process */
	}
	printf("%s() , OK, owner of lock have pid %d\n",__func__,lock.l_pid);
	return (lock.l_pid); /* true, return pid of lock owner */
}

typedef struct {
	int fd;
	int n;
} filerwlock;

/* Create N read/write locks and returns the id of this cluster of locks. */
filerwlock * filerwlockCreate(char *filename, int n) {
	filerwlock *fl = (filerwlock *)malloc(sizeof(filerwlock));
	if (((fl->fd) = open(filename, O_RDWR | O_CREAT | O_APPEND, S_IWUSR)) < 0) {
		printf("%s() : ",__func__);
		perror("open");
		exit(1);
	} else {
		fl->n = n;
		printf("%s() : We open filename with fd=%d,n=%d\n",__func__,fl->fd,fl->n);
	}
	return fl;
}

/* Delete the cluster of read/write locks associated with fl. */
int filerwlockDelete(filerwlock *fl) {
	printf("%s() : Closing fd=%d\n",__func__,fl->fd);
	if (close(fl->fd) < 0) {
		printf("%s() : ",__func__);
		perror("close");
		exit(1);
	}
	printf("%s() : Free struct filerwlock fd=%d,n=%d\n",__func__,fl->fd,fl->n);
	free(fl);
	return 0;
}

/* Given the read/write lock cluster fl, lock its ith element */
int filerwreadlock(filerwlock *fl, int i) {
	printf("%s() : i=%d\n",__func__,i);
	if ((i < 0) || (i >= fl->n)) {
		printf("filerwlockLock needs i in range 0 .. %d\n", (fl->n)-1);
		exit(0);
	}
	printf("%s() : Calling readw_lock(fd=%d,i=%d,SEEK_SET,1)\n",__func__,fl->fd,i);
	readw_lock(fl->fd, i, SEEK_SET, 1);
}

int filerwwritelock(filerwlock *fl, int i) {
	if ((i < 0) || (i >= fl->n)) {
		printf("filerwlockLock needs i in range 0 .. %d\n", (fl->n)-1);
		exit(0);
	}
	printf("%s() : Calling writew_lock(fd=%d,i=%d,SEEK_SET,1)\n",__func__,fl->fd,i);
	writew_lock(fl->fd, i, SEEK_SET, 1);
}

/* Given the lock cluster fl, unlock its ith element */
int filerwunlock(filerwlock *fl, int i) {
	if ((i < 0) || (i >= fl->n)) {
		printf("filerwlockUnlock needs i in range 0 .. %d\n", (fl->n)-1);
		exit(0);
	}
	printf("%s() : Calling un_lock(fd=%d,i=%d,SEEK_SET,1)\n",__func__,fl->fd,i);
	un_lock(fl->fd, i, SEEK_SET, 1);
}

/* Given the lock cluster fl, it read locks all its elements */
int filerwlongreadlock(filerwlock *fl) {
	printf("%s() : Calling readw_lock(fd=%d,0,SEEK_SET,fl->n=%d)\n",__func__,fl->fd,fl->n);
	readw_lock(fl->fd, 0, SEEK_SET, fl->n);
}

/* Given the lock cluster fl, it unlocks all its elements */
int filerwlongunlock(filerwlock *fl) {
	printf("%s() : Calling un_lock(fd=%d,0,SEEK_SET,fl->n=%d)\n",__func__,fl->fd,fl->n);
	un_lock(fl->fd, 0, SEEK_SET, fl->n);
}
