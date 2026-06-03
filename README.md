# Projeto-LP2
# Trabalho Prático 1 - LP II

**Nome:** [Genyaltt Soares da Silva Junior]  
**Matrícula:** [20240037227]  

---

## 1. Problema Escolhido
* **P1: Multiplicação de matrizes grandes padrão: mapa por linhas (sem merge)**
* *Descrição:* O programa calcula a multiplicação de duas matrizes quadradas ($A$ e $B$) de tamanho $1200 \times 1200$, gerando uma matriz $C$. A versão concorrente divide o trabalho distribuindo blocos disjuntos de linhas de $C$ para diferentes threads utilizando a biblioteca `pthreads`, sem a necessidade de regiões de exclusão mútua (mutex) ou barreiras durante a computação.

---

## 2. Ambiente de Teste
Como o desenvolvimento e as medições foram realizados através do ambiente online Replit devido a limitações locais de compatibilidade do sistema operacional original com POSIX threads, seguem as especificações da máquina hospedeira e do ambiente do container:

* **CPU:** Intel(R) Xeon(R) Platinum 8581C CPU @ 2.30GHz
* **Núcleos:** Núcleos Físicos (Cores): 4 / Threads (Núcleos Lógicos): 4
* **Sistema Operacional:** Linux (Ambiente Containerizado Replit)
* **Compilador:** GCC (Flags: `-O2 -Wall -Wextra`)

*Nota de Desempenho:* Por se tratar de uma IDE online rodando em uma CPU compartilhada, os tempos de execução podem sofrer pequenas oscilações devido à concorrência com outros usuários do servidor.

---

## 3. Instruções de Compilação e Execução
Conforme as exceções previstas no enunciado para ambientes online que não utilizam o fluxo padrão do CMake, o projeto deve ser compilado e executado por meio do terminal (Shell) do Replit utilizando o comando único abaixo:

```bash```
gcc -O2 -Wall -Wextra -pthread src/*.c -o meu_exe -lm && ./meu_exe 4

---

## 4. Tabela de Desempenho e Escalabilidade
Os valores abaixo representam a média de 5 execuções para cada configuração, garantindo a estabilidade da medição.

Threads,  Tempo Médio (s),  Speedup,  Eficiência

1 (seq),  "2,0080",         "1,00x",  "100,0%"

2      ,  "0,8283",         "2,42x",  "121,0%*"

4      ,  "0,4368",         "4,60x",  "115,0%*"

8      ,  "0,4412",         "4,55x",  "56,9%"

## 5. Discussão de escalabilidade

Com base nos dados coletados na CPU Intel Xeon Platinum 8581C (4 núcleos físicos/lógicos), a discussão dos resultados para o problema P1 é apresentada a seguir:

Limite de Hardware e Contenção: O melhor desempenho foi atingido com 4 threads, apresentando um tempo médio de 0,4368s. Ao subirmos para 8 threads, o tempo de execução estagnou (subindo levemente para 0,4412s) e a eficiência caiu pela metade (de 115% para 56,9%). Isso ocorre porque o processador disponível no Replit possui apenas 4 núcleos físicos/lógicos [conforme o comando lscpu executado anteriormente]. Acima desse limite, as threads passam a disputar as mesmas unidades de execução, gerando um alto overhead de troca de contexto (context switch), o que impede novos ganhos de velocidade.  

Overhead de Gerenciamento e Lei de Amdahl: Embora a multiplicação de matrizes seja altamente paralelizável (CPU-bound), o speedup não é perfeitamente infinito devido à parcela sequencial do programa (como a criação das threads via pthread_create e a junção via pthread_join). Além disso, como estamos em um ambiente compartilhado, a disputa por largura de banda de memória com outros containers pode limitar a escalabilidade, fator comum em problemas que manipulam grandes volumes de dados como o P1.  

