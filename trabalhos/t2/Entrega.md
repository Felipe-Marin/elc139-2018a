[Programação Paralela](https://github.com/AndreaInfUFSM/elc139-2018a) > T2

# T2: Experiências com Profilers - Programação Paralela
--------------------------------------------------

Nome: Felipe Marin

## Parte 1

|Tamanho do vetor|Repetições|Tempo Total|Tempo init_vectors|tempo dot_product|
|----------------|----------|-----------|------------------|-----------------|
|5000000         |500       |9.43       |0.04              |9.39             |
|5000000         |1000      |18.70      |0.04              |18.66            |
|10000000        |500       |18.75      |0.08              |18.67            |
|10000000        |1000      |37.18      |0.06              |37.12            |
|20000000        |500       |37.42      |0.15              |37.27            |
|20000000        |1000      |74.56      |0.14              |74.41            |

*Tempos em segundos

(a) O perfil é afetado pelas opções de configuração?  
Sim, o tempo de execução da função init_vectors é afetado pelo parâmetro de tamanho dos vetores. O tempo gasto pela função dot_product é afetado por ambos os parâmetros(tamanho do vetor x repetições).

(b) Pelo perfil de execução, há alguma função que poderia ser candidata a paralelização? Por quê?  
A função dot_product poderia ser paralelizada, a multiplicação dos valores nos dois vetores poderia ser dividida em mais de um núcleo.


## Parte 2
### Programa
O programa escolhido é um [gerador de planilhas](/trabalhos/t2/python_profiler/genspreadsheet.py) escrito em Python. Ele recebe como parâmetro o número linhas, colunas e tamanho das células, gerando um arquivo no formato XLSX com os dados da planilha. O script pode ser usado através do terminal, o exemplo a seguir gerará uma planilha com 10000 linhas, 200 colunas e células com 100 caracteres:

   ```
   python genspreadsheet.py 10000 200 100
   ```
    
### Profiler
A linguagem Python já conta com um profiler em sua Standard Library, o cProfiler pode ser usado dentro de um script ou através do terminal. Ele gera os seguintes dados das funções:
* ncalls - número de chamadas
* tottime - tempo gasto na função excluindo o tempo gasto pelas sub-funções chamadas
* percall - tottime dividido por ncalls
* cumtime - tempo cumulativo gasto na função e todas suas sub-funções
* percall - cumtime dividido pelo número de chamadas
* filename:lineno(function) - nome do arquivo, número da linha onde a função está no arquivo, e nome da função

Exemplo de uso dentro de um script:
   
   ```
   import cProfile
   import re
   cProfile.run('re.compile("foo|bar")')
   ```
O cProfiler também pode gerar o profile de um script .py
   
   ```
   python -m cProfile [-o output_file] [-s sort_order] myscript.py
   ```
Os dados salvos no profile podem ser visualizados através do terminal usando o próprio cProfiler ou com algum visualizador com interface gráfica. 
O [SnakeViz](https://jiffyclub.github.io/snakeviz/) é um dos visualizadores disponíveis. Ele gera gráficos e uma tabela com os dados do profile através de uma interface web.
![SnakeViz profile](/trabalhos/t2/images/snakeviz.jpg)
Ele está disponível no [PyPI](https://pypi.python.org/pypi/snakeviz), pode ser instalado através do terminal com o comando:
 
   ```
   pip install snakeviz
   ```
### Resultados
Através do gráfico é possível ver que 93.37% do tempo de execução do programa é gasto pela função 'append()' (em rosa no gráfico) que insere a planilha gerada no arquivo xlsx
![Função append no gráfico](/trabalhos/t2/images/append.jpg)
Também é possível buscar por funções através da tabela. No exemplo foi buscado pelas funções 'generate_sheet' e 'generate_xlsx' do script
![Busca por generate na tabela](/trabalhos/t2/images/tabela.jpg)


## Referências
- Python Software Foundation. Documentação dos Profilers do Python. <a href="https://docs.python.org/2/library/profile.html">https://docs.python.org/2/library/profile.html</a>.
- SnakeViz. Página do SnakeViz com documentação. <a href="https://jiffyclub.github.io/snakeviz/">https://jiffyclub.github.io/snakeviz/</a>.
