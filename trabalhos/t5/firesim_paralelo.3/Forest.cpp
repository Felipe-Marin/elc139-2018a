#include "Forest.h"
#include <omp.h>

Forest::Forest(int s, int nthreads) : size(s), nthreads(nthreads) 
{
   omp_set_num_threads(nthreads);
   t = new Tree*[size];
   for (int i = 0; i < size; i++)
      t[i] = new Tree[size];
   grow();
}

Forest::~Forest()
{
   for (int i = 0; i < size; i++)
      delete[] t[i];
   delete[] t;
}

struct Forest::TreePosn 
Forest::centralTree()
{
   struct TreePosn p = {size/2, size/2};
   return p;
}

int
Forest::burnUntilOut(TreePosn start_tree, double prob_spread, Random& r) 
{
   int count;

   grow();
   lightTree(start_tree);

   // queima a floresta at� terminar o fogo
   count = 0;
   while (isBurning()) {
      burn(prob_spread, r);
      count++;
   }

   return count;
}

double 
Forest::getPercentBurned() 
{
   int total = size*size-1;
   int sum = 0;
   int i, j;
   // calcula quantidade de �rvores queimadas
   #pragma omp parallel for schedule(static) shared(sum) private(i, j)
   for (i = 0; i < size; i++) {
      for (j = 0; j < size; j++) {
         if (t[i][j] == Burnt) {
            #pragma omp critical
            sum++;
         }
      }
   }
   // retorna percentual de �rvores queimadas
   return ((double)(sum-1)/(double)total);
}

void 
Forest::burn(double prob_spread, Random& r) 
{
   int i, j;
   #pragma omp parallel for schedule(static) private(i, j)
   for (i = 0; i < size; i++) {
      for (j = 0; j < size; j++) {
         if (t[i][j] == Burning) // �rvores pegando fogo
            t[i][j] = Burnt;     // ficam completamente queimadas
         if (t[i][j] == Smoldering) // �rvores come�ando a pegar fogo
            t[i][j] = Burning;      // ficam queimando
      }
   }

   // �rvores n�o queimadas come�am a pegar fogo
   #pragma omp parallel for schedule(static) private(i,j)
   for (i = 0;  i < size; i++) {
      for (j = 0; j < size; j++) {
         if (t[i][j] == Burning) {
            if (i != 0) { // �rvore ao norte
               if (t[i-1][j] == Unburnt && fireSpreads(prob_spread, r)) {
                  t[i-1][j] = Smoldering;
               }
            }
            if (i != size-1) { // �rvore ao sul
               if (t[i+1][j] == Unburnt && fireSpreads(prob_spread, r)) {
                  t[i+1][j] = Smoldering;
               }
            }
            if (j != 0) { // �rvore a oeste
               if (t[i][j-1] == Unburnt && fireSpreads(prob_spread, r)) {
                  t[i][j-1] = Smoldering;
               }
            }
            if (j != size-1) { // �rvore a leste
               if (t[i][j+1] == Unburnt && fireSpreads(prob_spread, r)) {
                  t[i][j+1] = Smoldering;
               }
            }
         }
      }
   }
}

void 
Forest::grow()
{
   int i, j;
   #pragma omp parallel for schedule(static) private(i, j)
   for (i = 0; i < size; i++)
      for (j = 0; j < size; j++)
         t[i][j] = Unburnt;
}

void 
Forest::lightTree(TreePosn p) 
{ 
   t[p.i][p.j] = Smoldering;
}

bool
Forest::isBurning()
{
   int i, j;
   bool isBurningOrSmoldering = false;
   #pragma omp parallel for schedule(static) private(i, j)
   for (i = 0; i < size; i++) {
      for (j = 0; j < size; j++) {
         if (t[i][j] == Smoldering || t[i][j] == Burning) {
            #pragma omp critical
            isBurningOrSmoldering = true;
         }
      }
   }
   return isBurningOrSmoldering;
}

bool
Forest::fireSpreads(double prob_spread, Random& r) 
{
   if (r.nextDouble() < prob_spread) 
      return true;
   else
      return false;
}
