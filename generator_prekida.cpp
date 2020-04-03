#include<cstdio>
#include<iostream>
#include<csignal>
#include<unistd.h>
#include<cstdlib>
#include<ctime>
#include<array>
int pid = 0;

void prekidna_rutina(int sig) {
    kill(pid, SIGKILL);
    exit(0);
}

int main(int argc, char* argv[]) {
    time_t t;
    srand((unsigned) time(&t));

    std::array<int,4> signals = {SIGUSR1,SIGUSR2,SIGINT,SIGALRM};
    pid = atoi(argv[1]);
    sigset(SIGINT, prekidna_rutina);
    int time_to_sleep;
    while(1) {
        sleep(3);
        if (rand()%2) sleep(1);
        if (rand()%2) sleep(1);
        int sig_to_send = signals[rand() % 4];
        kill(pid, sig_to_send);
    }
}
