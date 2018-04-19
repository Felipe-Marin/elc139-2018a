#include <algorithm>
#include <iostream>
#include <string>
#include <omp.h>

class SharedArray {
private:
   char* array;
   int index;
   int size;
public:
   SharedArray(int n) : size(n), index(0) {
      array = new char[size];
      std::fill(array, array+size, '-');
   }
   ~SharedArray() {
      delete[] array;
   }
   void addChar(char c) {
      array[index] = c;
      spendSomeTime();
      index++;
   }
   int countOccurrences(char c) {
      return std::count(array, array+size, c);
   }
   std::string toString() {
      return std::string(array, size);
   }
private:
   void spendSomeTime() {
      for (int i = 0; i < 10000; i++) {
         for (int j = 0; j < 100; j++) {
         }
      }
   }
};


class ArrayFiller {
private:
   static const int nThreads = 3;
   static const int nTimes = 20;
   SharedArray* array;
   bool mutex;
public:
   ArrayFiller(bool usemutex) {
      array = new SharedArray(nThreads * nTimes);
      mutex = usemutex;
   }
   void fillArrayConcurrently() {
      #pragma omp parallel for schedule(auto)
      for (int i = 0; i < nTimes*nThreads; i++) {
         #pragma omp critical
         array->addChar('A'+omp_get_thread_num());
      }
   }
   void printStats() {
      std::cout << array->toString() << std::endl;
      for (int i = 0; i < nThreads; ++i)
         std::cout << (char) ('A'+i) << "=" 
                   << array->countOccurrences('A'+i) << " ";
      std::cout << std::endl;
   }
   ~ArrayFiller() {
      delete array;
   }
};


int main() {
   omp_set_num_threads(3);
   std::cout << "Each thread should add its char to the array n times (n=20)" << std::endl;
   std::cout << "Correct results should total exactly nThreads*nTimes chars" << std::endl;
   std::cout << "Case 1: no mutex, default scheduling (expecting wrong results)" << std::endl;
   ArrayFiller m1(false);
   m1.fillArrayConcurrently();
   m1.printStats();
   
   std::cout << "Case 2: same as case 1 (possibly getting different results due to scheduling)" << std::endl;
   ArrayFiller m2(false);
   m2.fillArrayConcurrently();
   m2.printStats();
   
   std::cout << "Case 3: using mutex (expecting correct results)" << std::endl;
   ArrayFiller m3(true);
   m3.fillArrayConcurrently();
   m3.printStats();
}
