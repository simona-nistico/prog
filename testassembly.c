#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>

#include "datatypes.h"

//_____________________Funzioni esterne scritte in assembly_____________________
//extern float distance(VECTOR x1, VECTOR x2, int d);
extern void test_residual(VECTOR res, VECTOR x,VECTOR centroid,int d);
extern float test_objective(int n,int m, MATRIX distances_from_centroids);
//extern void memset_float(float* array, float val, int dim );
//extern void accumulate(MATRIX dest, MATRIX source, int dim);
//extern void divide( MATRIX dest, MATRIX dividendo, float divisore, int dim);



//---------------------------Test Assembly---------------------------
  VECTOR x = alloc_matrix(1, 37);
  VECTOR y = alloc_matrix(1, 37);
  for(int i=0; i<37; i++){
    x[i] = rand()%20;
    y[i] = rand()%20;
  }

//  printf("Vettore X       : \n");
//  print_matrix(1, 47, 1, x, 'p');
//  printf("Vettore Y       : \n");
//  print_matrix(1, 47, 1, y, 'p');
/*
  for( int i=1; i<47; i++){
    printf("Distanza Assembly: %f\n", distance64(x, y, i) );
    printf("Distanza C       : %f\n\n", distance(x, y, i) );
    if( distance64(x, y, i) !=  distance(x, y, i) ) printf("ERRORE\n");
   }
*/
   VECTOR res = residual(x, y, 37);
   printf("Residual C       : \n");
   print_matrix(1, 37, 1, res, 'p');

   VECTOR res2 = alloc_matrix(1, 37);
   test_residual(res2, x, y, 37);
   printf("Residual Assembly: \n");
   print_matrix(1, 37, 1, res2, 'p');


  MATRIX tmp = alloc_matrix(1,47);
  memset(tmp, 0, 47*sizeof(float));

  printf("TEMP AZZERATA:\n" );
  print_matrix(1, 47, 1, tmp, 'p');

  printf("TEMP ACCUMULATA:\n" );
  //accumulate(tmp, x, 37);
//  divide(tmp, x, 2, 37);
  memset_float64(tmp, 87.25, 47);
  print_matrix(1, 47, 1, tmp, 'p');
//  for (int i = 0; i < 37; i++)
//    printf("%f\t", tmp[i]);


//  printf("TEMP ACCUMULATA2:\n" );
//  accumulate(tmp, y, 37);
//  print_matrix(1, 37, 1, tmp, 'p');

  int n=10;
  int m=37;
  VECTOR distances_from_centroids = alloc_matrix(n, m);
  for(int i=0; i<n; i++)
    for( int j=0; j<m; i++)
        distances_from_centroids[i*n+j] = rand()%20;

  float obiettivo = objective_function(n,m, distances_from_centroids);
  float obiettivoAss = test_objective(n,m, distances_from_centroids);

  printf("Funzione obiettivo C       : %lf\n", obiettivo );
  printf("Funzione obiettivo Assembly: %lf\n\n", obiettivoAss );
