#include <cstdio>
#include <iostream>
#include <csignal>
#include <unistd.h>
#include <sys/time.h>
#include <cmath>

unsigned long broj = 1000000001;
unsigned long zadnji = 1000000001;
int pauza = 0;

void printaj_zadnjeg() {
	std::cout << "Zadnji prosti broj jest: " << zadnji << std::endl;
}

void prekidna_rutina(int sig) {
	printaj_zadnjeg();
}

void postavi_pauzu(){
	pauza = 1 - pauza;
}

void periodicki_posao (int sig) {
	printaj_zadnjeg();
    postavi_pauzu();
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
        while(pauza)
		    pause();
	}
	return 0;
}
