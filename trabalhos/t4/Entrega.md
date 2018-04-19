[Programação Paralela](https://github.com/Felipe-Marin/elc139-2018a) > T4

Nome: Felipe Marin


# T4: Scheduling com OpenMP 
--------------------------------------------------

Código: [ThreadABC_OMP.cpp](./ThreadABC_OMP.cpp)

## Saídas do programa com diferentes 'schedule'

### Static
```
Case 1: no mutex, default scheduling (expecting wrong results)
CABCBACBACACBACBACBACBACBACBACBACBACBACBACBACBACBACBACBACB--
A=19 B=19 C=20
Case 2: same as case 1 (possibly getting different results due to scheduling)
CBACABCABCACBACBACBACBACBACBACBACBCABCABACBACBACBACBACBACB--
A=19 B=19 C=20
Case 3: using mutex (expecting correct results)
BAAAAAAAAAAAAAAAAAAAACCCCCCCCCCCCCCCCCCCCBBBBBBBBBBBBBBBBBBB
A=20 B=20 C=20
```

### Dynamic
```
Case 1: no mutex, default scheduling (expecting wrong results)
CACBABCABCABCABCACBACBABCABCABCABCABCABCABCABCABCABCABCABC--
A=19 B=19 C=20
Case 2: same as case 1 (possibly getting different results due to scheduling)
CBACABCABCABCABCABCABCACBACBACBACBACBACBACBACBACBACBACBACB--
A=19 B=19 C=20
Case 3: using mutex (expecting correct results)
ABBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBAC
A=2 B=57 C=1
```

### Guided
```
Case 1: no mutex, default scheduling (expecting wrong results)
CABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABC--
A=19 B=19 C=20
Case 2: same as case 1 (possibly getting different results due to scheduling)
CABACBACBACBACBACBACBACBACBACBACBACBACBABCABCABCABCABCABC---
A=19 B=19 C=19
Case 3: using mutex (expecting correct results)
ABBBBBBBBBBBBBBBBBBBBBBBBBBBAAAAAAAAAABBCCCCCCCCCCCCCCCCCCCC
A=11 B=29 C=20
```

### Runtime
```
Case 1: no mutex, default scheduling (expecting wrong results)
CACBABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCACBABC--
A=19 B=19 C=20
Case 2: same as case 1 (possibly getting different results due to scheduling)
BACBACBACBACBACBABCABCABCABCABCABCABCABCABCABCACBACBACBACB--
A=19 B=20 C=19
Case 3: using mutex (expecting correct results)
CBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBCA
A=1 B=57 C=2
```

### Auto
```
Case 1: no mutex, default scheduling (expecting wrong results)
CCBACABCABABCABCABCABCABCABCABCABCABCABCABCBACBACBACBACBAC--
A=19 B=19 C=20
Case 2: same as case 1 (possibly getting different results due to scheduling)
ACBABCACBACABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCAB--
A=20 B=19 C=19
Case 3: using mutex (expecting correct results)
BAAAAAAAAAAAAAAAAAAAACCCCCCCCCCCCCCCCCCCCBBBBBBBBBBBBBBBBBBB
A=20 B=20 C=20
```

## Referências
Blaise Barney, Lawrence Livermore National Laboratory. OpenMP Tutorials. [https://computing.llnl.gov/tutorials/openMP/](https://computing.llnl.gov/tutorials/openMP/) 