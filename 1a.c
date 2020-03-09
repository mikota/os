#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<sys/time.h>
#include<math.h>

unsigned long broj = 1000000001;
unsigned long zadnji = 1000000001;
int pauza = 0;

void prekidna_rutina(int sig) {
	printf("asdf\n");
}

void periodicki_posao (int sig) {
	printf("zadnji prosti broj = %lu",zadnji);
}

int prost(unsigned long n) {
	unsigned long i, max;
	if ((n & 1) == 0) 
		return 0;
	max = sqrt(n);
	for (i=3; i<=max; i+=2)
		if ((n%i) == 0) return 0;
	return 1;	
}

int main () {
	struct itimerval t;
	//povezivanje 
	sigset(SIGALRM, periodicki_posao);
	t.it_value.tv_sec = 0;
	t.it_value.tv_usec = 500000;
	t.it_interval.tv_sec = 0;
	t.it_interval.tv_usec = 500000;
	printf("Poceo osnovni program PID=%d\n", getpid());

	setitimer (ITIMER_REAL,&t,NULL);
	while(1) {
		if (prost(broj))	
			zadnji = broj;
		broj++;
		pause();
	}
	return 0;
}
