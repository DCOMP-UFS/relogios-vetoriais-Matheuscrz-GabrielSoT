/**
 * Código base (incompleto) para implementação de relógios vetoriais.
 * Meta: implementar a interação entre três processos ilustrada na figura
 * da URL: 
 * 
 * https://people.cs.rutgers.edu/~pxk/417/notes/images/clocks-vector.png
 * 
 * Compilação: mpicc -o rvet rvet.c
 * Execução:   mpiexec -n 3 ./rvet
 */
 
#include <stdio.h>
#include <string.h>  
#include <mpi.h>     


typedef struct Clock { 
   int p[3];
} Clock;


void Event(int pid, Clock *clock){
   clock->p[pid]++;   
}


void Send(int pid, Clock *clock, int dest){
   Event(pid,clock);
   MPI_SEND(clock, sizeof(Clock), MPI_BYTE, dest, 0, MPI_COMM_WORLD);
   printf("Processo %d enviou para o processo %d: Clock(%d, %d, %d)\n",pid, dest, clock->p[0], clock->p[1], clock->p[2]);
}

void Receive(int pid, Clock *clock, int source){
   Clock received;
   MPI_Recv(&received, sizeof(Clock), MPI_BYTE, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

   for(int i = 0; i < 3; i++){
      if(received.p[i] > clock->p[i]){
         clock->p[i]= received.p[i];
      }
   }

   printf("Processo %d recebeu do processo %d: Clock(%d, %d, %d)\n", pid, source, clock->p[0], clock->p[1], clock->p[2]))
}

// Representa o processo de rank 0
void process0(){
   Clock clock = {{0,0,0}};
   Event(0, &clock);
   printf("Process: %d, Clock: (%d, %d, %d)\n", 0, clock.p[0], clock.p[1], clock.p[2]);

   Send(0, &clock, 1); //Exemplo de uso da função Send

   Receive(0, &clock, 1); //Exemplo de uso da função Receive

   printf("Processo %d, Clock após troca com o processo 1: (%d, %d, %d)\n", 0, clock.p[0], clock.p[1], clock.p[2]);
   // TODO: Executar todo procedimento do processo 0 seguindo a imagem
}

// Representa o processo de rank 1
void process1(){
   Clock clock = {{0,0,0}};
   printf("Process: %d, Clock: (%d, %d, %d)\n", 1, clock.p[0], clock.p[1], clock.p[2]);

   Receive(1, &clock, 0);

   Send(1, %clock, 0);

   printf("Processo %d, Clock após troca com Processo 0: (%d, %d, %d)\n", 1, clock.p[0], clock.p[1], clock.p[2]);
   // TODO: Executar todo procedimento do processo 1 seguindo a imagem
}

// Representa o processo de rank 2
void process2(){
   Clock clock = {{0,0,0}};
   // printf("Process: %d, Clock: (%d, %d, %d)\n", 2, clock.p[0], clock.p[1], clock.p[2]);
   
   // TODO: Executar todo procedimento do processo 2 seguindo a imagem
}

int main(void) {
   int my_rank;               

   MPI_Init(NULL, NULL); 
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 

   if (my_rank == 0) { 
      process0();
   } else if (my_rank == 1) {  
      process1();
   } else if (my_rank == 2) {  
      process2();
   }

   /* Finaliza MPI */
   MPI_Finalize(); 

   return 0;
}  /* main */
