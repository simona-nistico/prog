#include <stdio.h>
#include <float.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <xmmintrin.h>

#define	MATRIX float*
#define	VECTOR float*
/*
float distance(VECTOR x1,VECTOR x2,int d){
	float diff,sum = 0;
	int i;
	// Si lavora su gruppi di 4
	for(i=0; i<=d-4; i+=4){
		diff = x1[i]-x2[i];
		sum += diff*diff;
		diff = x1[i+1]-x2[i+1];
		sum += diff*diff;
		diff = x1[i+2]-x2[i+2];
		sum += diff*diff;
		diff = x1[i+3]-x2[i+3];
		sum += diff*diff;
	}

	// Processiamo un eventuale residual_centroids
	for(i;i<d;i++){
		diff = x1[i]-x2[i];
		sum += diff*diff;
	}

	return sum;

	// TESTATA
}//distance
*/

void brute_force(float* ds, float* qs, int n,int nq, int d, int knn){

	float* v1=malloc(d*sizeof(float));
	float* v2=malloc(d*sizeof(float));
	int* NN=malloc(nq*knn*sizeof(float));
	int l;
	float dist;
	for(int i=0;i<nq;i++){ // punto del qs i

		for(int j=0;j<d;j++){
			v1[j]=qs[i*d+j];
		}


  	VECTOR distances = (VECTOR) malloc(knn*sizeof(float));

    for(int j=0;j<knn;j++)
			distances[j] = FLT_MAX;

		for(int j=0;j<n;j++){ // punto del ds j
			for(int k=0;k<d;k++){
				v2[k]=ds[j*d+k];
			}
			dist=distance(v1,v2,d);
			//printf("Distanza al QUADRATO tra la query %d ed il punto del dataset %d: %f\n",i,j, dist);
			if(distances[0]>dist){
				l = 0;
				while(l<knn-1 && distances[l+1]>dist){
					distances[l] = distances [l+1];
					NN[i*knn+l] = NN[i*knn+l+1];
					l++;
				}// while
				distances[l] = dist;
				NN[i*knn+l] = j;
			}// if

		}


	}

	for(int i=0;i<nq;i++){
		printf("I %d punti più vicini alla query %d sono: ",knn,i);
		for(int j=0;j<knn;j++){
			printf("%d\t",NN[i*knn+j]);
		}
		printf("\n");
	}

}

/*
int main() {
	int n=4;
	int d=2;
	int nq=2;
	int knn=2;
	float* ds=malloc(n*d*sizeof(float));
	float* qs=malloc(nq*d*sizeof(float));

	ds[0]=1.0;	ds[1]=0.0;
	ds[2]=3.3;  ds[3]=4.0;
	ds[4]=2.0;	ds[5]=5.0;
	ds[6]=7,0;	ds[7]=3.0;

	qs[0]=0.5;	qs[1]=1.0;
	qs[2]=3.0;	qs[3]=2.5;

	brute_force(ds,qs,n,nq,d,knn);

}
*/
