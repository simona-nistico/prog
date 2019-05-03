#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>

#define	MATRIX		double*
#define	VECTOR		double*

int n;
int k;
int kc;

//Matrice che ogni riga rappresenta un punto e ogni colonna un centroide
//L'elemento m[i][j] rappresenta la distanza del punto i dal centroide j
MATRIX distancesFromCentroids;

//Punti del dataset da sosituire con quelli veri
VECTOR ds;

//Array con l'indice dei punti che alla prima iterazione sono anche i centroidi
VECTOR centroidiCasuali

//Centroidi veri e propri da codificare come i punti
MATRIX centroids;


//genera k numeri casuali diversi da 0 a n incluso
void generate_centroids(int n, int k){

	int number,old_number,i,j;

	// Alloco la matrice dei centroidi
	centroids = alloc_matrix(k,d);

	srand(time(NULL));
	old_number=rand()%(n/k);
	printf("%d\n",old_number);

	// Inseriamo il centroide nella matrice
	for(j=0;j<d;j++){
		centroids[i*d+j] = ds[old_number*d+i];
	}

	int range;
	for(i=1;i<k;i++){

		range = (n-old_number)/(k-i);
		printf("Range: %d  ",range );
  	number=old_number+rand()%range+1;
		printf("%d\n",numbers[i]);

		// Inseriamo il centroide nella matrice
		for(j=0;j<d;j++){
			centroids[i*d+j] = ds[number*d+i];
		}

		old_number = number;

 	}

//Mettere nella matrice centroids i punti corrispondenti agli indici trovati

}

// la matrice distances deve essere creata una volta sola visto che il metodo seguente deve stare dentro un ciclo
void computeDistancesFromCentroids(){

	VECTOR punto = alloc_matrix(1,k);	// Alloco il vettore che conterrà di volta in volta il punto
	double minDist,distanza;
	int minCentr;
	int i,j,k;

	for(i=0;i<n;i++){

		for(k=0;k<d;k++){
			punto[k] = ds[d*i+k];	// Sto supponendo che il dataset sia memorizzato per righe
		}

		minDist = distance(punto, centroids[0]);
		minCentr = 0;

		for(j=1;j<kc;j++){ //Verificare la sintassi, l'obiettivo è passare l'indirizzo di partenza
					//del punto i-esimo

			for(k=0;k<d;k++){
				punto[k] = ds[d*i+k];
			}	// Sto supponendo che il dataset sia memorizzato per righe

			distanza=distance(punto, centroids[j]);

			if(distanza < minDist){
				minDist = distanza;
				minCentr = j;
			}

		}

		centroideDelPunto[i][0] = minCentr;
		centroideDelPunto[i][1] = minDist;
	}

	// Liberiamo lo spazio (verificare se si può utilizzare la stessa funzione della matrice)
	dealloc_matrix(punto);

}

//Si può applicare il loop vectorization cioè più operazioni nello stesso ciclo
//Oppure più punti alla volta
double distance(VECTOR puntoX, VECTOR puntoY){

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
