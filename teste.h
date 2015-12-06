
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "structure.h"

int **Alocar_matriz(int n, int m)
{
  int **v;
  int   i;
  if (m < 1 || n < 1) { /* verifica parametros recebidos */
    return (NULL);
  }

  /* aloca as linhas da matriz */
  v = (int **) calloc (n, sizeof(int *));
  
  if (v == NULL) {
    return (NULL);
  }
  /* aloca as colunas da matriz */
  for ( i = 0; i < n; i++ ) {
    v[i] = (int*) calloc (m, sizeof(int));
    
    if (v[i] == NULL) {
       return (NULL);
       }
  }
    
  return (v); /* retorna o ponteiro para a matriz */
}

int **Liberar_matriz (int m, int n, int **v)
{
  int  i;  /* variavel auxiliar */
  if (v == NULL) return (NULL);
  if (m < 1 || n < 1) {  /* verifica parametros recebidos */
     return (v);
     }
  for (i=0; i<m; i++)
   free (v[i]); /* libera as linhas da matriz */
  free (v);      /* libera a matriz */
  return (NULL); /* retorna um ponteiro nulo */
}

Tjob *Alocar_vetor (int n)
{
  Tjob *v;        /* ponteiro para o vetor */
  if (n < 1) {  /* verifica parametros recebidos */
     return (NULL);
     }
  /* aloca o vetor */
  v = (Tjob *) calloc (n, sizeof(Tjob));
  if (v == NULL) {
     return (NULL);
     }
  return (v);    /* retorna o ponteiro para o vetor */
}

int *Liberar_vetor (int n, int *v)
{
  if (v == NULL) return (NULL);
  if (n < 1) { /* verifica parametros recebidos */
     return (NULL);
     }
  free(v);        /* libera o vetor */
  return (NULL);  /* retorna o ponteiro */
}

void SetDistances(Tinstance *instance, FILE *file){
     int i, j, jobFrom, jobTo,setupTime;
     for(i=0;i<instance->size;i++){
          for(j=0;j<instance->size;j++){
             fscanf(file, "%d", &jobFrom);
             fscanf(file, "%d", &jobTo);
             fscanf(file, "%d", &setupTime);
             if(jobFrom==-1)
               instance->Distances[jobTo][jobTo]=setupTime;
             else
               instance->Distances[jobFrom][jobTo]=setupTime;
          }
     }
}

void PrintPopulation(int **pop,int lin, int col){
     int i,j;
//     for(i=0;i<lin;i++) {
  ///      printf("Solucao N. %d\n",i);
        for(j=0;j<col;j++)
            printf("%d\t",pop[lin][j]);
    //}
}


void SetJobs(Tinstance *instance, FILE *file, int flag){
     int i;
     switch(flag){
            case 1:
                 for(i=0; i < instance->size; i++){
                    instance->jobs[i].id=i+1;
                    fscanf(file,"%d",&instance->jobs[i].processTime);
                 }
             break;
             case 2:
                 for(i=0; i < instance->size; i++){
                    fscanf(file,"%d",&instance->jobs[i].weights);
                 }
             break;
             case 3:
                 for(i=0; i < instance->size; i++){
                    fscanf(file,"%d",&instance->jobs[i].duedate);
                 }
             break;
     }
}

void SetInstance(Tinstance *instance, FILE *file){
     fscanf(file,"%d", &instance->id);
     fscanf(file,"%d", &instance->size);
     fscanf(file,"%f", &instance->Tau);
     fscanf(file,"%f", &instance->R);
     fscanf(file,"%f", &instance->Eta);
     fscanf(file,"%d", &instance->P_bar);
     fscanf(file,"%d", &instance->P_min);
     fscanf(file,"%d", &instance->P_max);
     fscanf(file,"%d", &instance->S_bar);
     fscanf(file,"%d", &instance->MaxWeight);
     fscanf(file,"%d", &instance->C_max);
     fscanf(file,"%d", &instance->D_bar);
     
     instance->jobs       = Alocar_vetor(instance->size);
     instance->Distances  = Alocar_matriz(instance->size,instance->size);
     
     SetJobs(instance,file,1);//processTime
     SetJobs(instance,file,2);//weight
     SetJobs(instance,file,3);//duedate
     SetDistances(instance,file);
}

void PrintJobInfo(Tjob *job, int size, char flag){
     int i;
     for(i=0;i<size; i++){
        switch(flag){
              case 't':
                   printf("%d\t",job[i].processTime);
              break;
              case 'w':
                   printf("%d\t",job[i].weights);
              break;
              case 'd':
                   printf("%d\t",job[i].duedate);
              break;
              case 'i':
                   printf("%d\t",job[i].id);
              break;
        }
     }
}

void PrintDistances(Tinstance instance){
     int i,j;
     for(i=0;i<instance.size;i++){
          for(j=0;j<instance.size;j++)
              printf("%d ",instance.Distances[i][j]);
          printf("\n");
     }
}

void GenerateFile(char *FileName, Tinstance instance){
     FILE *output;
     int i,j,data;
     if((output = fopen(FileName,"w")) == NULL){
       exit(1);
    }
    for(i=0;i<instance.size;i++){
       for(j=0;j<instance.size;j++)
           fprintf(output,"%d;",instance.Distances[i][j]);
       fprintf(output,"\n");
    }
}

Tjob *SetSolution(Tjob *solution, int size){
    int i;
    solution = Alocar_vetor(size);
    for(i=0; i < size; i++){
       solution[i].id             = 0;
       solution[i].processTime    = 0;
       solution[i].weights        = 0;
       solution[i].duedate        = 0;
    }
    return solution;
}

int Objective(Tinstance instance, Tjob *solution, char flag){
    int i, sum = 0, from = 0,to;
    sum = instance.Distances[from][from];
    for(i=0;i<instance.size;i++){
        if(flag == 's')
           sum += solution[i].processTime;
           else
           sum += instance.jobs[i].processTime;
        if(i%2==0){
          from = (flag=='s')?solution[i].id-1:instance.jobs[i].id-1;
          }
        else {
             to = (flag=='s')?solution[i].id-1:instance.jobs[i].id-1;
             sum += instance.Distances[from][to];
        }
    }
    return sum;
}

int Randomize(int max){
     return rand()%max;
}

Tjob *RandSolution(Tjob *solution, int size){
	srand(time(NULL));
     Tjob *auxSolution;
     int pos,j,k, i,tem=1;
     auxSolution = SetSolution(solution,size);
     for(i = 0;i<size;i++){
           do{
               pos = Randomize(size);
           }while(auxSolution[pos].id != 0);
           auxSolution[pos].id          = solution[i].id;
           auxSolution[pos].weights     = solution[i].weights;
           auxSolution[pos].duedate     = solution[i].duedate;
           auxSolution[pos].processTime = solution[i].processTime;
     }
     solution = auxSolution;
     return solution;
}

int **GenSolutions(int row, int col, Tjob *solution, int **nsolutions){
     int i,j;
     Tjob *auxSolution;
     auxSolution = solution;
     for(i = 0;i < row; i++){
        auxSolution = RandSolution(auxSolution,col);
        for(j=0;j<col;j++){
           nsolutions[i][j] = auxSolution[j].id;
        }
     }
     return nsolutions;
}


int IsJobAlocated(int jobID, Tjob *solution){
    int i=0;
    while(solution[i].id !=0){
         if(solution[i].id == jobID){
           return 1;
         }
         i++;
    }
    return 0;
}

double ProcessTimeAverage(Tjob *solution, Tinstance instance){
      int i, sum=0,count=0;
      for(i=0;i<instance.size; i++){
         if(!IsJobAlocated(instance.jobs[i].id,solution)){
            sum += instance.jobs[i].processTime;
            count++;
         }
      }
      return sum/count;
}


double tardiness_factor(Tinstance instance){
      return 1-((double)instance.D_bar/(double)instance.C_max);
}

double SetupTimeAverage(Tjob *solution, Tinstance instance){
      int i,j, sum=0,count=0,flag=1;
      double media;
      for(i=0;i<instance.size; i++){
         if(!IsJobAlocated(instance.jobs[i].id,solution)){
            for(j=0;j<instance.size;j++){
                sum +=  instance.Distances[i][j];
                count++;
            }
         }
      }
      media =(double)sum/(double)count;
      return media;
}

double severity_factor(Tinstance instance, Tjob *solution){
      return SetupTimeAverage(solution,instance)/ProcessTimeAverage(solution,instance);
}


Tjob *GreedySolution(Tjob *solution, Tinstance instance){
      int i, j,last=0, time=0;
      for(i=0;i<instance.size;i++){
          last = BestValue(solution,instance,last,time);
          solution[i].id          = instance.jobs[last].id;
          solution[i].processTime = instance.jobs[last].processTime;
          solution[i].weights     = instance.jobs[last].weights;
          solution[i].duedate     = instance.jobs[last].duedate;
          time += instance.jobs[last].processTime;
       }
       return solution;
}

double k1(Tinstance instance, Tjob *solution){
    double aux;
    aux = 1.2*log((double)instance.size)-(double)instance.R;
    if(tardiness_factor(instance) < 0.5)
       aux -= 0.5;
    if(severity_factor(instance,solution) < 0.5 && instance.size < 5)
       aux -= 0.5;
    return aux;
}

double k2(Tinstance instance, Tjob *solution){
      double aux, tardiness = tardiness_factor(instance);
      if(tardiness < 0.8)
         aux = 1.8;
      else aux = 2.0;
      aux = tardiness / (aux * sqrt(severity_factor(instance,solution)));
      return aux;
}


double ATCS(int time, int last, int next, Tinstance instance, Tjob *solution){
      double exp,k,b;
      double base = (double)instance.jobs[next].weights/(double)instance.jobs[next].processTime;
      int max = (instance.jobs[next].duedate - instance.jobs[next].processTime - time);
      int nextTime = instance.Distances[last][next];
      double tunningK1 = k1(instance, solution) * ProcessTimeAverage(solution, instance);
      double tunningk2 = k2(instance, solution)*SetupTimeAverage(solution, instance) ;
      if( max < 0)
         max = 0;
      exp =(-((double)max/tunningK1 - (double)nextTime/tunningk2));
      exp = 1/exp;
      if(base > 0)
        base = pow(base,exp);
      else base = 0;
      return base;
}


int BestValue(Tjob *solution, Tinstance instance, int last, int time){
      int Bigger=0, j;
      double index;
      for (j=0;j<instance.size;j++){
          index = ATCS(time,last,j,instance,solution);
          if(index >= Bigger && !IsJobAlocated(j+1,solution)){
             Bigger = index;
             last = j;
          }
      }
      return last;
}
