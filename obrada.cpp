#include<cstdio>
#include<iostream>
#include<csignal>
#include<unistd.h>
#include<sys/time.h>
#include<cmath>
#include<array>

#define N 6
#define NO_GP 0

int OZNAKA_CEKANJA[N];
int PRIORITET[N];
int TEKUCI_PRIORITET;
int current_pr;
void prekidna_rutina(int sig);

void print_status(int gpnum, int index, char letter) {
    if (gpnum > 0) std::cout << gpnum;
    else std::cout << '-';
    std::cout << ' ';
    for(int i=0; i<5; i++){
        if (i == index-1) std::cout << letter << ' ';
        else std::cout << "- ";
    }
    std::cout << OZNAKA_CEKANJA[5];
    std::cout << '\n';
}

std::array<int,5> sigpr={SIGUSR1, SIGUSR2, SIGALRM, SIGILL, SIGINT};
void zabrani_prekidanje(){
    for (int sig_i : sigpr){
        sighold(sig_i);
    }
}

void dozvoli_prekidanje(){
    for (int sig_i : sigpr){
        sigrelse(sig_i);
    } 
}
 
void obrada_signala(int i){
    for(int signal_i : sigpr){
        sigset(signal_i, prekidna_rutina);
    }
    print_status(NO_GP, i, 'P');
    for(int sec=1; sec<6; sec++){
        print_status(NO_GP,i,'0'+sec);
        sleep(1);
    }
    print_status(NO_GP, i, 'K');
}

void prekidna_rutina(int sig){
   int n=-1;
   zabrani_prekidanje();
   for(int i=0; i<5; i++){
        if (sig == sigpr[i]) n = i+1;
   }
   print_status(NO_GP, n, 'X');
   OZNAKA_CEKANJA[n]++;
   int temp = -1;
   while(temp != 0){
        temp = 0;
        for(int i=current_pr; i<N; i++){
            if (OZNAKA_CEKANJA[i] > 0){
                temp = i;
            }
        }
        if (temp > 0) {
            OZNAKA_CEKANJA[temp]--;
            PRIORITET[temp] = current_pr;
            dozvoli_prekidanje();
            obrada_signala(temp);
            zabrani_prekidanje();
        }
   }
   dozvoli_prekidanje();
}

void ubij(int sig) {
    std::cout << "\nKilled\n";
    exit(0);
}

int main ( void )
{
    for(int signal_i : sigpr){
        sigset(signal_i, prekidna_rutina);
    }
    sigset(SIGKILL,ubij);
    std::cout << "Proces obrade prekida, PID=" << getpid() << '\n';
    printf("G 1 2 3 4 5\n");
    printf("-----------\n");
    for(int i=0; i<10; i++){
        sleep(1);
        print_status(i+1,-1,'E');
    }
    printf ("Zavrsio osnovni program\n");
    return 0;
}
