//  
// Simula��o de inc�ndio em uma floresta.
// Baseada no c�digo proposto por David Joiner.
//
// Uso: firesim <tamanho-do-problema> <nro. experimentos> <probab. maxima> 

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "Random.h"
#include "Forest.h"
#include <omp.h>
#include <sys/time.h>


/*
 * Tempo (wallclock) em microssegundos
 */ 
long 
wtime()
{
   struct timeval t;
   gettimeofday(&t, NULL);
   return t.tv_sec*1000000 + t.tv_usec;
}

void
checkCommandLine(int argc, char** argv, int& size, int& trials, int& probs, int& nthreads, int& chunk)
{
   if (argc > 1) {
      size = atoi(argv[1]);
   }   
   if (argc > 2) {
      trials = atoi(argv[2]);
   }
   if (argc > 3) {
      probs = atoi(argv[3]);
   }   
   if (argc > 4) {
       nthreads = atoi(argv[4]);
   }
   if (argc > 5) {
       chunk = atoi(argv[5]);
   }
}

int 
main(int argc, char* argv[]) 
{
   
   // par�metros dos experimentos
   int forest_size = 30;
   int n_trials = 5000;
   int n_probs = 101;
   int n_threads = 2;
   int chunk = 1;

   double* percent_burned; // percentuais queimados (sa�da)
   double* prob_spread;    // probabilidades (entrada)
   double prob_min = 0.0;
   double prob_max = 1.0;
   double prob_step;
   int base_seed = 100;

   long start_time, end_time;

   checkCommandLine(argc, argv, forest_size, n_trials, n_probs, n_threads, chunk);
    
   try {

      Random rand;

      prob_spread = new double[n_probs];
      percent_burned = new double[n_probs];

      prob_step = (prob_max - prob_min)/(double)(n_probs-1);
     
      Forest* forest;
      int ip, it;

      start_time = wtime();
      printf("Probabilidade, Percentual Queimado\n");

      // para cada probabilidade, calcula o percentual de �rvores queimadas
      omp_set_num_threads(n_threads);
      #pragma omp parallel shared(prob_spread, percent_burned) private(forest, ip, it)
      {    
      forest = new Forest(forest_size);
      #pragma omp for schedule(static, chunk)
      for (ip = 0; ip < n_probs; ip++) {

         prob_spread[ip] = prob_min + (double) ip * prob_step;
         percent_burned[ip] = 0.0;
         rand.setSeed(base_seed+ip); // nova seq��ncia de n�meros aleat�rios
         
        
         // executa v�rios experimentos  
         for (it = 0; it < n_trials; it++) {
           
            // queima floresta at� o fogo apagar
            forest->burnUntilOut(forest->centralTree(), prob_spread[ip], rand);
            percent_burned[ip] += forest->getPercentBurned();
         }

         // calcula m�dia dos percentuais de �rvores queimadas
         percent_burned[ip] /= n_trials;

         // mostra resultado para esta probabilidade
         printf("%lf, %lf\n", prob_spread[ip], percent_burned[ip]);
      }
      }
      end_time = wtime();
      printf("%d thread(s), %ld usec\n", n_threads, (long) (end_time - start_time));

      delete[] prob_spread;
      delete[] percent_burned;
   }
   catch (std::bad_alloc)
   {
      std::cerr << "Erro: alocacao de memoria" << std::endl;
      return 1;
   }

   return 0;
}

