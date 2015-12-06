#include <stdio.h>
#include <stdlib.h>
#include "teste.h"
#include "ferreira.h"

int main (int arg, char *argv[])
{
    FILE *files;
    Tinstance instance;
    Tjob *solution,*x, *solutionRand1 , *solutionRand2, *solutionFinal;
    int **GSolutions;
    double m;
    double index, Bigger;
    int i, time = 0, last = 0, j, qtdLoop, rand1, rand2, qtdSolutions = 1000,
        time1, time2, objective, NonOptimal,objectiveGr, ObjectiveGRASP, ObjectiveGEN;
    
    if((files = fopen("instancias/wt_sds_1.txt","r")) == NULL){
      printf("nao foi possivel abrir ");
      getchar();
      exit(1);
    }
    
    SetInstance(&instance,files); // Inicializa a instancia do problema
    solution = SetSolution(solution,instance.size); // Inicializa a variavel que guardara a solucao
    
    printf("\n\nSOLUCAO INICIAL\n\n");
    PrintJobInfo(instance.jobs,instance.size,'i'); // Imprime a ordem inicial do problema
    
    NonOptimal = Objective(instance, instance.jobs,'n'); // Calcula o valor da funcao objetivo original
    printf("Solucao sem otimizacao %d\n",NonOptimal);

    printf("\n\nSOLUCAO GULOSA\n\n");
    solution = GreedySolution(solution, instance); // Gera a solucao Gulosa
    PrintJobInfo(solution,instance.size,'i');  // Imprime a ordem apos a solucao gulosa
    
    objective        = Objective(instance, solution, 's'); // Imprime o valor da funcao objetivo da solucao gulosa
    printf("Solucao Gulosa %d",objective);
    
    GSolutions = Alocar_matriz(qtdSolutions, instance.size);
    GSolutions = GenSolutions(qtdSolutions, instance.size, solution, GSolutions);

    qtdLoop = Randomize(0.85 * qtdSolutions);

    do { 
        rand1 = Randomize(qtdSolutions);
        rand2 = Randomize(qtdSolutions);      

        time1 = Objective(instance, solution[rand1],'s');
        time2 = Objective(instance, solution[rand2],'s');

        if(time1 < time2){
            solutionRand1 = solution[rand1];
        }else{
            solutionRand1 = solution[rand2];
        }

        rand1 = Randomize(qtdSolutions);
        rand2 = Randomize(qtdSolutions);      

        time1 = Objective(instance, solution[rand1],'s');
        time2 = Objective(instance, solution[rand2],'s');        

        if(time1 < time2){
            solutionRand2 = solution[rand1];
        }else{
            solutionRand2 = solution[rand2];
        }

        solutionFinal = Crossover(solutionRand1, solutionRand2, instance.size);
        solutionFinal = Mutacao(solutionFinal);

        replace();

        qtdLoop--;
    }while(qtdLoop > 0);


}
