[Programação Paralela](https://github.com/Felipe-Marin/elc139-2018a) > T7

Nome: Felipe Marin

# T7: Primeiros passos com MPI

## Parte 1

Código: [dotprod_mpi.c](./dotprod_mpi.c)  
Uso: mpiexec -np <número de processos> dotprod_mpi <nº elementos de cada processo> <nº de repetições>

### Desempenho
|Tamanho do vetor|Repetições|Nº de processos|Tempo médio|Speedup|
|----------------|----------|---------------|-----------|-------|
|1000000         |4000      |1              |12.057543  |       |
|1000000         |4000      |3              | 5.585284  |2.159  |
|1000000         |4000      |5              | 3.525578  |3.420  |
|1000000         |4000      |9              | 2.792566  |4.318  |
|2000000         |4000      |1              |24.453481  |       |
|2000000         |4000      |3              |12.270548  |1.993  |
|2000000         |4000      |5              | 7.781559  |3.142  |
|2000000         |4000      |9              | 7.539788  |3.243  |
|4000000         |4000      |1              |48.566486  |       |
|4000000         |4000      |3              |24.525103  |1.980  |
|4000000         |4000      |5              |15.907895  |3.053  |
|4000000         |4000      |9              |16.254837  |2.988  |
|4000000         |4000      |17             |14.054102  |3.456  |

## Parte 2
Código: [msgpipeline_mpi.c](./msgpipeline_mpi.c) 

## Parte 3
Código 1: [mpi_corrigido1.c](./mpi_corrigido1.c)  
Código 2: [mpi_corrigido2.c](./mpi_corrigido2.c)  


## Referências
Wes Kendall, MPI Tutorial. MPI Send and Receive. [http://mpitutorial.com/tutorials/mpi-send-and-receive/](http://mpitutorial.com/tutorials/mpi-send-and-receive/) 