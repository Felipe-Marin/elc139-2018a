#ifndef __FOREST_H
#define __FOREST_H

#include "Random.h"

//
// Classe que representa uma floresta incendi�vel.
//
class Forest
{
   public:

      Forest(int s, int nthreads);
      ~Forest();

      struct TreePosn
      { 
            int i; 
            int j;
      };

      struct TreePosn centralTree();
      int burnUntilOut(TreePosn start_tree, double prob_spread, Random& r);
      double getPercentBurned();

   private:

      void burn(double prob_spread, Random& r);     
      void grow();
      void lightTree(TreePosn p);
      bool isBurning();
      bool fireSpreads(double prob_spread, Random& r);

   private:

      // A floresta � armazenada em uma matriz.
      // Cada elemento � uma �rvore que pode estar em 4 estados.

      enum Tree {
         Unburnt,       // �rvore n�o queimada 
         Smoldering,    // �rvore come�ando a pegar fogo
         Burning,       // �rvore queimando
         Burnt          // �rvore completamente queimada
      };        

      Tree** t; 
      int size;
      int nthreads;

};

#endif

