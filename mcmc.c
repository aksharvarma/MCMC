#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<limits.h>


#define min(a,b) (((a)>(b))?b:a)

/* This allows generationg of uniform random variables */
#define U ((long double)rand()/RAND_MAX)


int dot(int *a, int *b, int n);
void print_matrix(int **matrix, int rows, int cols);
void print_vector(int *vector, int n);
double score(int beta, int *x, int *g, int n);
int *propose(int w_max, int *x, int *w, int n);
int *mcmc(int iterations, int w_max, int *w, int *g, int n, int beta_start, int beta_increment);

  
int main(){
  srand(time(NULL));
  int i,n=14;
  int mcmc_runs=100;
  int iterations=pow(10,5)/2;
  int *w=(int*)malloc(n*sizeof(int));
  int *g=(int*)malloc(n*sizeof(int));
  int w_max=150;

  int *x=(int*)malloc(n*sizeof(int));
  int *final_x=(int*)malloc(n*sizeof(int));
  int best_haul=-99, haul=0;
  
  double beta_start=0.005, beta_increment=0.005;
  
  for(i=0;i<n;i++){
    scanf("%d ", &w[i]);
  }

  for(i=0;i<n;i++){
    scanf("%d ", &g[i]);
  }

  /* for(i=0;i<n;i++){ */
  /*   printf("w: %d g: %d\n", w[i], g[i]); */
  /* } */
    

  for(i=0;i<mcmc_runs;i++){
    x=mcmc(iterations, w_max, w, g, n, beta_start, beta_increment);
    /* print_vector(x,n); */
    haul=dot(x,g,n);
    if(haul>best_haul){
      final_x=x;
      best_haul=haul;
    }
  }

  printf("The final state is:\n");
  print_vector(final_x,n);
  printf("The haul is worth: %d\n", best_haul);
  
  return 0;
}


void print_matrix(int **matrix, int rows, int cols){
  int i,j;

  for(i=0;i<rows;i++){
    for(j=0;j<cols;j++){
      printf("%d ", matrix[i][j]);
    }
    printf("\n");
  }
  return;
}

void print_vector(int *vector, int n){
  int i;

  printf("[");
  for(i=0;i<n;i++)
    printf("%d ", vector[i]);
  printf("]\n");

  return;
}

int dot(int *a, int *b, int n){
  int i;
  int ans=0;
  
  for(i=0;i<n;i++)
    ans+=a[i]*b[i];

  return ans;
}

double score(int beta, int *x, int *g, int n){
  return exp(beta*dot(x,g,n));
}


int *propose(int w_max, int *x, int *w, int n){
  int i;
  int random_index=rand()%n;
  /* printf("i:%d\n", random_index); */
  int* proposal=(int*)malloc(n*sizeof(int));

  for(i=0;i<n;i++)
    if(i==random_index)
      proposal[i]=!x[i];
    else
      proposal[i]=x[i];

  /* print_vector(proposal,n); */
  if(dot(w,proposal,n)<w_max){
    /* printf("I'm done here.\n"); */
    return proposal;
  }
  else
    propose(w_max,x,w,n);
    
}

int *mcmc(int iterations, int w_max, int *w, int *g, int n, int beta_start, int beta_increment){
  int i;
  int beta=beta_start;
  int *x_current=(int*)calloc(n,sizeof(int));
  int *x_proposed=(int*)malloc(n*sizeof(int));
  int *x_best=(int*)malloc(n*sizeof(int));
  double best_score=0;
  double current_score=0, proposed_score=0, accept_probability;

  for(i=0;i<iterations;i++){
    x_proposed=propose(w_max, x_current, w, n);

    current_score=score(beta,x_current,g,n);
    proposed_score=score(beta,x_proposed,g,n);
    accept_probability=min(1,proposed_score-current_score);

    if(proposed_score>best_score){
      x_best=x_proposed;
      x_current=x_proposed;
    }

    if(U<accept_probability){
      x_current=x_proposed;
    }

    if(i%(iterations/100)==0)
      beta+=beta_increment;
  }
  
  return x_best;
}
