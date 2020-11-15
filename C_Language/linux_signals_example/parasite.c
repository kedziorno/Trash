#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>

#define PROGRAM_NAME "parasite"
#define FIFO_BUFFER 64
#define FIFO_NAME "fifo"

int signal_num = -1, destination_pid = -1;
float time_interval = -1;
float start_register = 0;
int opt, fifo_pipe, open_pipe, fifo_flags;
fd_set fds;

void signal_handler(int signum) {
	printf("Receiving signal number %d\n", signum);
	start_register = start_register + (start_register * 0.25); // after receive signal (eg 16 - SIGUSR1/2?) we add +25% to value
	printf("+25procent start_register = %lf\n", start_register);
	if (signal(signum, signal_handler) == SIG_ERR) {
		printf("error on install new handler : %s\n", strerror(errno));
	} else {
		printf("install new handler\n");
	}
}

void signal_handler_rt(int signum) {
	printf("Receiving signal RT_ number %d\n", signum);
	start_register = start_register - (start_register * 0.20); // after receive signal (eg 16 - SIGUSR1/2?) we add -20% to value
	printf("-20procent start_register = %lf\n", start_register);
	if (signal(signum, signal_handler_rt) == SIG_ERR) {
		printf("error on install new RT handler : %s\n", strerror(errno));
	} else {
		printf("install new RT handler\n");
	}
}

//https://stackoverflow.com/a/14598879
int random_number(int min_num, int max_num)
{
				int result = 0, low_num = 0, hi_num = 0;

				if (min_num < max_num)
				{
								low_num = min_num;
								hi_num = max_num + 1; // include max_num in output
				} else {
								low_num = max_num + 1; // include max_num in output
								hi_num = min_num;
				}

				srand(time(NULL));
				result = (rand() % (hi_num - low_num)) + low_num;
				return result;
}

int main(int argc, char *argv[])
{
	printf("%s : PID = %d\n", PROGRAM_NAME, getpid());

	while ((opt = getopt(argc, argv, "s:p:d:v:")) != -1) {
		switch (opt) {
			case 's':
				signal_num = atoi(optarg);
			break;
			case 'p':
				destination_pid = atoi(optarg);
			break;
			case 'd':
				time_interval	= atof(optarg);
			break;
			case 'v':
				start_register	= atof(optarg);
			break;
			default:
				fprintf(stderr, "Usage: %s [-s signal] [-p pid] [-d time_interval] [-v start_register]\n", argv[0]);
				exit(10);
		}
	}

	printf("%s : Start program with arguments / signal : %d , pid : %d , time_interval : %lf , start_register : %lf\n", PROGRAM_NAME, signal_num, destination_pid, time_interval, start_register);

	printf("%s : mkfifo name %s\n", PROGRAM_NAME, FIFO_NAME);
	fifo_pipe = mkfifo(FIFO_NAME, 0600);
	if (fifo_pipe == -1) {
		printf("%s : mkfifo name %s with error : %s\n", PROGRAM_NAME, FIFO_NAME, strerror(errno));
	} else {
		printf("%s : mkfifo name %s with fd %d\n", PROGRAM_NAME, FIFO_NAME, fifo_pipe);
	}

	// block SIG_PIPE - to report pre-death, implement signal handler
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set,SIGPIPE);
	if(sigprocmask(SIG_BLOCK, &set, NULL)==0){
		printf("Blocking SIGPIPE...\n");
	}

	struct sigaction sig_new, sig_old;
	sig_new.sa_handler = signal_handler;
	if (sigaction(signal_num, &sig_new, &sig_old) < 0) {
		printf("%s : sigaction failed : %s\n", PROGRAM_NAME, strerror(errno));
	} else {
		printf("%s : sigaction installed\n", PROGRAM_NAME);
	}

	for (size_t index = SIGRTMIN; index <= SIGRTMAX; index++) {
		struct sigaction sig_new, sig_old;
		sig_new.sa_handler = signal_handler_rt;
		sig_new.sa_flags = SI_USER;
		if (sigaction(index, &sig_new, &sig_old) < 0) {
			printf("%s : sigaction failed on install RT_ number %d : %s\n", PROGRAM_NAME, index, strerror(errno));
		} else {
			//printf("%s : sigaction installed RT_ signal %d\n", PROGRAM_NAME, index);
		}
	}
	FD_ZERO(&fds);
	FD_SET(fifo_pipe, &fds);

	size_t loop_idx = 1;

	while(1) {
		printf("%d\n", loop_idx++);
		open_pipe = open(FIFO_NAME, O_RDWR);
		if (open_pipe == -1) {
			printf("%s : Failed to open pipe %s : %s\n", PROGRAM_NAME, FIFO_NAME, strerror(errno));
			exit(3);
		} else {
			printf("%s : pipe %s opened WR at %d\n", PROGRAM_NAME, FIFO_NAME, open_pipe);
		}

		fifo_flags = fcntl(open_pipe, F_GETFL, 0);
		printf("%s : set pipe to nonblock mode\n", PROGRAM_NAME);
		fcntl(fifo_pipe, F_SETFL, fifo_flags | O_NONBLOCK);

		char fifo_buffer[FIFO_BUFFER];
		memset(&fifo_buffer, 0, FIFO_BUFFER);
		snprintf(&fifo_buffer, FIFO_BUFFER, "%d %d %lf", getpid(), destination_pid, start_register);

		ssize_t write_fifo_length = write(open_pipe, fifo_buffer, strlen(fifo_buffer));
		if (write_fifo_length < 0) {
			printf("%s : Failed pipe name %s with error : %s\n", PROGRAM_NAME, FIFO_NAME, strerror(errno));
		} else {
			printf("%s : write %d bytes to pipe name %s [%s]\n", PROGRAM_NAME, write_fifo_length, FIFO_NAME, fifo_buffer);
		}

		if (close(open_pipe) < 0) {
			printf("%s : Failed close pipe name %s with error : %s\n", PROGRAM_NAME, FIFO_NAME, strerror(errno));
		} else {
			printf("%s : close pipe name %s\n", PROGRAM_NAME, FIFO_NAME);
		}

		printf("%s : Sleep for %fl deciseconds\n", PROGRAM_NAME, time_interval);
		sigset_t sst;
		siginfo_t sit;
		struct timespec ts;
		ts.tv_sec = 0.1 * time_interval;
		ts.tv_nsec = 0;
		sigemptyset(&sst);
		sigaddset(&sst, signal_num);
		int rs = 0;
		if (rs = sigtimedwait(&sst, &sit, &ts) == -1) {
			printf("%s : Error in sigtimedwait : %s\n", PROGRAM_NAME, strerror(errno));
		} else { // we have signal before timout
			printf("%s : sigtimedwait ok , returned %d\n", PROGRAM_NAME, rs);
			printf("%s : sit.si_code   %d\n", PROGRAM_NAME, sit.si_code);
			printf("%s : sit.si_errno  %d\n", PROGRAM_NAME, sit.si_errno);
			printf("%s : sit.si_signo  %d\n", PROGRAM_NAME, sit.si_signo);
			printf("%s : sit._sifields %d\n", PROGRAM_NAME, sit._sifields);
			continue;
		}
		// we send reminder
		printf("%s : Send reminder\n", PROGRAM_NAME);
		int rt_number = random_number(SIGRTMIN, SIGRTMAX);
		printf("%s : Random SIGRTMIN-SIGRTMAX : %d\n", PROGRAM_NAME, rt_number);
		//int rt_sigqueueinfo(pid_t tgid, int sig, siginfo_t *uinfo);
		int rt_sqi = 0;
		union sigval value_sv;
		value_sv.sival_int = rt_number; // send the SIGRT_ number
		if (rt_sqi = sigqueue(destination_pid, rt_number, value_sv) == -1) {
			printf("%s : Error in reminder / sigqueue : %s\n", PROGRAM_NAME, strerror(errno));
		} else {
			printf("%s : sigqueue returned : %d\n", PROGRAM_NAME, rt_sqi);
		}

	}

	return 0;
}
