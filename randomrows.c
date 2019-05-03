#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>

int n;
int k;
int kc; 

//Matrice che ogni riga rappresenta un punto e ogni colonna un centroide
//L'elemento m[i][j] rappresenta la distanza del punto i dal centroide j
double** distancesFromCentroids; 

//Punti del dataset da sosituire con quelli veri
double* punti;

//Array con l'indice dei punti che alla prima iterazione sono anche i centroidi
int* centroidiCasuali;

//Centroidi veri e propri da codificare come i punti
double** centroids;

//Matrice di due colonne che per ogni indice i che rappresenta il punto corrispondente
//memorizza in [i][0] il centroide corrispondendente e in [i][1] la distanza dal centroide
double** centroideDelPunto;

//genera k numeri casuali diversi da 0 a n incluso
int* generate(int n, int k){

	int* numbers = calloc(k,sizeof(int));

	srand(time(NULL));
	numbers[0]=rand()%(n/k);
	printf("%d\n",numbers[0]);
	
	int range;
	for(int i=1;i<k;i++){
	
		range = (n-numbers[i-1])/(k-i);
		printf("Range: %d  ",range );
  		numbers[i]=numbers[i-1]+rand()%range+1;
		printf("%d\n",numbers[i]);
 	}

//Mettere nella matrice centroids i punti corrispondenti agli indici trovati

}

// la matrice distances deve essere creata una volta sola visto che il metodo seguente deve stare dentro un ciclo
double** computeDistancesFromCentroids(){

        double** distances = calloc(n,sizeof(double));
	double minDist;
	int minCentr;
	for(int i=0;i<n;i++){
		distances[i]=calloc(kc,sìzeof(double));
		distances[i][0] = distance((punti+i*d)*, centroids[0]);
		minDist = distances[i][0];
		minCentr = 0; 
		for(int j=1;j<kc;j++){ //Verificare la sintassi, l'obiettivo è passare l'indirizzo di partenza
					//del punto i-esimo
			distances[i][j]=distance((punti+i*d)*, centroids[j]);
			if(distances[i][j] < minDist){
				minDist = distances[i][j];
				minCentr = j;
			}

		}
		
		centroideDelPunto[i][0] = minCentr;
		centroideDelPunto[i][1] = minDist;
	}
	return distances;

}

float* centroids(int* starting);

//Si può applicare il loop vectorization cioè più operazioni nello stesso ciclo
//Oppure più punti alla volta
double distance(int* puntoX, int* puntoY){ 

	double sum=0;
	double diff=0;
	for(int i=0;i<d;i++){
		diff = (puntoX[i]-puntoY[i]);
		sum+= diff*diff;	
	}
	return sqrt(sum);
}

//Per  ogni punto si ha il centroide di appartenenza (q(x))

//Per ogni centroide bisogna trovare il punto medio e sostituirlo ai centroidi di partenza

//Ripetere finchè la distanza tra il centroide di prima e quello nuovo non è inferiore ad epsilon

int main(){

  generate(100,9);
  return 0;

}
