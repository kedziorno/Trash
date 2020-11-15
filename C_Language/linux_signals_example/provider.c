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
// man 7 pthread_create

#define PROGRAM_NAME "provider"
#define FIFO_NAME "fifo"
#define FIFO_BUFFER 64

int signalnum, opt;
int num_threads;
double add_size;
int time_diff;
pthread_attr_t attr;
int s = -1, tnum;
int stack_size = -1;
void *res;
int unlink_return = 0;
int fifo_pipe;
int fifo_flags;
int open_pipe;
fd_set fds;
double *dinfo;
struct thread_info *tinfo;
int *sinfo;

struct thread_info {
	pthread_t thread_id;
	int thread_num;

	double add_size;
	int time_diff;
	int signal;
};

#define handle_error_en(en, msg) \
do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

#define handle_error(msg) \
do { perror(msg); exit(EXIT_FAILURE); } while (0)

static void *
thread_start(void *arg) {
	struct thread_info *tinfo = arg;

	double start_size = 0;

	double old_size = 0;

	// losujemy zachowanie
	printf("%s : In thread 0x%X : get a behaviour for signal [0 = kill , 1 = ignore , 2 = return]\n", PROGRAM_NAME, tinfo->thread_id);
	int behavior = random() % 2;
	printf("%s : In thread 0x%X : we have behavior [%d]\n", PROGRAM_NAME, tinfo->thread_id, behavior);

	while (1) {
		printf("%s : In thread 0x%X : stored value for thread number [%d] is [%lf]\n", PROGRAM_NAME, tinfo->thread_id, tinfo->thread_num, dinfo[tinfo->thread_num]);
		if (old_size != dinfo[tinfo->thread_num]) {
			if (dinfo[tinfo->thread_num] > 0) { // ... "druga wart jest liczba zmmiennoprzecinkowa ze znakiem, i okresla ile ma byc odjete od zasobu"
				if (dinfo[tinfo->thread_num] > start_size) { // "- gdy parametr jest dodatni, to nie moze byc wiekszy od zasobu"
					printf("%s : In thread 0x%X : start_size < 0 , so we break loop\n", PROGRAM_NAME);
				}
				start_size -= dinfo[tinfo->thread_num];
			}
			if (dinfo[tinfo->thread_num] < 0) { // ... "(lub dodane, gdy jest ujemna)".
				start_size += dinfo[tinfo->thread_num];
				if (start_size <= 0) { // ... "- gdy parametr jest ujemny, to dodanie do zasobu nie moze spowodowac przepelnienia"
					printf("%s : In thread 0x%X : After add parameter start_size < 0 , so we break loop\n", PROGRAM_NAME);
				}
			}
			old_size = dinfo[tinfo->thread_num];
		}

		start_size += tinfo->add_size;
		printf("%s : In thread 0x%X : After add harvest we have %lf\n", PROGRAM_NAME, tinfo->thread_id, start_size);

		printf("%s : In thread 0x%X : Sleep for %d seconds\n", PROGRAM_NAME, tinfo->thread_id, tinfo->time_diff);
		struct timeval t = { tinfo->time_diff , 0 };
		select(open_pipe + 1, &fds, NULL, NULL, &t);

		if (sinfo[tinfo->thread_num] == signalnum) {
			// check if we have signal
			// 1-3 random case function for signal
			if (behavior == 0) { // kill
				printf("%s : In thread 0x%X : We get signal, so we kill / exit thread number %d\n", PROGRAM_NAME, tinfo->thread_id, tinfo->thread_num);
				sinfo[tinfo->thread_num] = -1; // ??? -1 info for thread.join
				return (int*)&signalnum;
			}
			if (behavior == 1) { // ignore
				printf("%s : In thread 0x%X : We get signal, so ignore this signal for thread number %d\n", PROGRAM_NAME, tinfo->thread_id, tinfo->thread_num);
				sinfo[tinfo->thread_num] = 0; // reset the tuple
			}
			if (behavior == 2) { // return
				printf("%s : In thread 0x%X : We get signal, so return the same for thread number %d\n", PROGRAM_NAME, tinfo->thread_id, tinfo->thread_num);
				//sinfo[tinfo->thread_num] = signalnum; // ???
			}
		}
	}
}

void signal_handler(int signum) {
	if (signum == 2) { //CTRL+C
		printf("Receive signal number == 2\n");

		for (size_t index = 0; index < num_threads; index++) {
			s = pthread_join(tinfo[index].thread_id, &res);
			if (s != 0) {
				handle_error_en(s, "pthread_join in signal_handler");
			}
			printf("Joined with thread %d; returned value was %d\n", tinfo[index].thread_num, (char *) res);
		}

		s = pthread_attr_destroy(&attr);
		if (s != 0) {
			handle_error_en(s, "pthread_attr_destroy");
		}
	} else {
		printf("Unknown signal = %d\n", signum);
	}
}

int main(int argc, char *argv[])
{
	char *validateIT, *vITp;

	while ((opt = getopt(argc, argv, "t:s:h:")) != -1) {
		switch (opt) {
			case 't':
				num_threads = atoi(optarg);
			break;
			case 's':
				signalnum = atoi(optarg);
			break;
			case 'h':
				validateIT	= optarg;
			break;
			default:
				fprintf(stderr, "Usage: %s [-t number_threads] [-s signal_number] [-h IncreaseValue/TimeInterval]\n", argv[0]);
				exit(10);
		}
	}

	// buffer overflow, but nevermind
	int in_increase = 1;
	int in_interval = 0;
	char increase[32] = { 0 };
	char interval[32] = { 0 };
	vITp = validateIT;
	int index1 = 0;
	int index2 = 0;
	while (*vITp != 0) {
		if (*vITp == '/') {
			in_increase = 0;
			in_interval = 1;
			vITp++;
			continue;
		}
		if (in_increase == 0 && in_interval == 1) {
			interval[index2++] = *vITp;
			vITp++;
		}
		if (in_increase == 1 && in_interval == 0) {
			increase[index1++] = *vITp;
			vITp++;
		}
	}
	increase[index1] = 0;
	interval[index2] = 0;

	add_size = atof(increase);
	time_diff = atoi(interval);

	printf("%s : Start program with arguments / num threads : %d , signal : %d , add_size : %lf , time_diff : %d\n", PROGRAM_NAME, num_threads, signalnum, add_size, time_diff);

	printf("%s : mkfifo name %s\n", PROGRAM_NAME, FIFO_NAME);
	fifo_pipe = mkfifo(FIFO_NAME, 0600);
	if (fifo_pipe == -1) {
		printf("%s : Failed mkfifo name %s with error : %s\n", PROGRAM_NAME, FIFO_NAME, strerror(errno));
	} else {
		printf("%s : mkfifo name %s with fd %d\n", PROGRAM_NAME, FIFO_NAME, fifo_pipe);
	}

	// install signal handler
	if (signal(2, signal_handler) == SIG_ERR) {
		printf("%s : error on signal handler : %s\n", PROGRAM_NAME, FIFO_NAME, strerror(errno));
	} else {
		printf("%s : signal handler installed\n", PROGRAM_NAME);
	}

	s = pthread_attr_init(&attr);
	if (s != 0) {
		handle_error_en(s, "pthread_attr_init");
	}

	if (stack_size > 0) {
		s = pthread_attr_setstacksize(&attr, stack_size);
		if (s != 0) {
			handle_error_en(s, "pthread_attr_setstacksize");
		}
	}

	tinfo = calloc(num_threads, sizeof(struct thread_info));
	if (tinfo == NULL) {
		handle_error("tinfo , calloc");
	}

	dinfo = calloc(num_threads, sizeof(double));
	if (dinfo == NULL) {
		handle_error("dinfo , calloc");
	}

	sinfo = calloc(num_threads, sizeof(int));
	if (dinfo == NULL) {
		handle_error("sinfo , calloc");
	}

	for (size_t index = 0; index < num_threads; index++) {
		tinfo[index].thread_num = index;
		tinfo[index].time_diff = time_diff;
		tinfo[index].add_size = add_size;
		s = pthread_create(&tinfo[index].thread_id, &attr, &thread_start, &tinfo[index]);
		if (s != 0) {
			handle_error_en(s, "pthread_create");
		}
	}

	FD_ZERO(&fds);
	FD_SET(fifo_pipe, &fds);

	while(1) {
		// check signal table always after parse data from pipe, if we have -1 - join thread
		for (size_t index = 0; index < num_threads; index++) {
			if (sinfo[tinfo[index].thread_num] == -1) {
				printf("Signal -1 in thread number %d\n", tinfo[index].thread_num);
				s = pthread_join(tinfo[index].thread_id, &res);
				if (s != 0) {
					handle_error_en(s, "pthread_join in while main");
				} else {
					printf("Joined with thread %d; returned value was %d\n", tinfo[index].thread_num, (int*) res);
				}
			}
		}

		open_pipe = open(FIFO_NAME, O_RDONLY);
		if (open_pipe == -1) {
			printf("%s : Failed to open pipe %s\n", PROGRAM_NAME, FIFO_NAME);
			exit(3);
		} else {
			printf("%s : pipe %s opened at %d\n", PROGRAM_NAME, FIFO_NAME, open_pipe);
		}

		fifo_flags = fcntl(open_pipe, F_GETFL, 0);
		printf("%s : set pipe to nonblock mode\n", PROGRAM_NAME);
		fcntl(fifo_pipe, F_SETFL, fifo_flags | O_NONBLOCK);
		int fifo_first_number = -1;
		double fifo_second_number = -1;

		char fifo_buffer[FIFO_BUFFER];
		memset(&fifo_buffer, 0, FIFO_BUFFER);
		ssize_t read_fifo_length = read(open_pipe, fifo_buffer, FIFO_BUFFER);
		if (read_fifo_length < 0) {
			printf("%s : Failed pipe name %s with error : %s\n", PROGRAM_NAME, FIFO_NAME, strerror(errno));
		} else {
			printf("%s : We have %d bytes data on pipe %s , using sscanf ...\n", PROGRAM_NAME, read_fifo_length, FIFO_NAME);
			int n_sscanf;
			errno = 0;
			n_sscanf = sscanf(&fifo_buffer, "signal %d\n", &fifo_first_number);
			if ( n_sscanf == 1) {
				printf("%s : We parse signal with 1 values : thread num = %d\n", PROGRAM_NAME, fifo_first_number);
				if (fifo_first_number >= 0 && fifo_first_number < num_threads) {
					printf("%s : Store signal for thread number = %d\n", PROGRAM_NAME, fifo_first_number);
					sinfo[fifo_first_number] = signalnum;
					printf("%s : Signal value stored : %d\n", PROGRAM_NAME, sinfo[fifo_first_number]);
				} else {
					printf("%s : Thread number out of range (%d)\n", PROGRAM_NAME, num_threads);
				}
			} else if (errno != 0) {
				printf("%s : Failed to parse signal from pipe name %s with error : %s\n", PROGRAM_NAME, FIFO_NAME, strerror(errno));
			} else {
				printf("%s : No matching data (signal)\n", PROGRAM_NAME);
			}
			errno = 0;
			n_sscanf = sscanf(&fifo_buffer, "%d %lf\n", &fifo_first_number, &fifo_second_number);
			if ( n_sscanf == 2) {
				printf("%s : We parse 2 values : thread number = %d , harvest = %lf\n", PROGRAM_NAME, fifo_first_number, fifo_second_number);
				if (fifo_first_number >= 0 && fifo_first_number < num_threads) {
					printf("%s : Store harvest = %lf at thread number = %ld\n", PROGRAM_NAME, fifo_second_number, fifo_first_number);
					dinfo[fifo_first_number] = fifo_second_number;
					printf("%s : Value stored : %lf\n", PROGRAM_NAME, dinfo[fifo_first_number]);
				} else {
					printf("%s : Thread number out of range (%d)\n", PROGRAM_NAME, num_threads);
				}
			} else if (errno != 0) {
				printf("%s : Failed to parse from pipe name %s with error : %s\n", PROGRAM_NAME, FIFO_NAME, strerror(errno));
			} else {
				printf("%s : No matching data\n", PROGRAM_NAME);
			}
		}
		if (close(open_pipe) < 0) {
			printf("%s : Failed close pipe name %s with error : %s\n", PROGRAM_NAME, FIFO_NAME, strerror(errno));
		}
	}

	return 0;
}
