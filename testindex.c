#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>

//Necessario per poter utilizzare la struttura params definita dal prof
#include "datatypes.h"



//
//int n;
//int k;
int kc;

//Matrice che ogni riga rappresenta un punto e ogni colonna un centroide
//L'elemento m[i][j] rappresenta la distanza del punto i dal centroide j
MATRIX distancesFromCentroids;

//Punti del dataset da sosituire con quelli veri
//VECTOR ds;

//Array con l'indice dei punti che alla prima iterazione sono anche i centroidi
VECTOR centroidiCasuali;

//Centroidi veri e propri codificati come i punti
MATRIX centroids;

params *input;


//------------------------------------METODI------------------------------------

/**La funzione seguente seleziona k punti da usare come centroidi iniziali
 * n =  numero di punti del data set
 * d = numero di dimensioni del data/query set
 * k =  numero di centroidi di ogni sotto-quantizzatore
   k numeri casuali diversi da 0 a n incluso
 */
void generate_centroids(int n, int d, int k){

	int number,old_number,i,j;

	// Alloco la matrice dei centroidi
	centroids = alloc_matrix(k,d);

  //rand() function returns an integer value between 0 and RAND_MAX(32767)
	// TODO togliere per avere numeri sempre casueli:  srand(time(NULL));

	/**IDEA:
	  * divido gli n punti in k intervalli e ottengo un range di valori;
		* scelgo un numero casuale tra 0 e range e prendo il punto corrispondente;
		* nel caso peggiore viene scelto sempre il max del range e i punti restano sempre entro n;
		* i restanti punti li divido in k-1 intervalli ottenendo un nuovo range;
		* cosi facendo ottengo dei numeri casuali sempre diversi che coprono l'intervallo da  a n;
		*/

	old_number = rand()%(n/k);
	printf("Punto 0 scelto: %d\n",old_number);

	// Inseriamo il primo centroide nella matrice lungo la riga 0
	for(j=0;j<d;j++)
		centroids[j] = input->ds[old_number*d+j];

	int range;
	for(i=1;i<k;i++){
		range = (n-old_number)/(k-i);	//trovo il nuovo range tra cui scegliere i valori casuali
  	number = old_number+rand()%range+1; //seleziono un numero casuale nel range
		printf("Punto %d scelto: %d\n",i, number);

		// Inseriamo il centroide nella matrice
		for(j=0;j<d;j++)
			centroids[i*d+j] = input->ds[number*d+j];

		old_number = number;
 	}//for

	print_matrix(k, d, centroids);

	//TESTATO  	--> 	OK
}//generate_centroids







// la matrice distances deve essere creata una volta sola visto che il metodo seguente deve stare dentro un ciclo
void compute_distance_from_centroids(int n, int d, int k){

	VECTOR punto = alloc_matrix(1,d);	// Alloco il vettore che conterrà di volta in volta il punto
	double distanza;
	double minDist; //Distanza dal minimo centroide
	int minCentr;	//Centroide a minima distanza
	int i,j;

	for(i=0;i<n;i++){	//per ogni punto del dataset devo calcolare la distanza dal primo centroide

		for(j=0;j<d;j++)	//copio il punto dal dataset nel vettore punto
			punto[j] = input->ds[d*i+j];	// Sto supponendo che il dataset sia memorizzato per righe


		//Inizializzo con il primo punto e con la distanza da esso
//		minDist = distance(punto, centroids[0]);
		minCentr = 0;
}
}
/*
		for(j=1;j<kc;j++){ //Verificare la sintassi, l'obiettivo è passare l'indirizzo di partenza
					//del punto i-esimo
input.k
			for(k=0;k<d;k++){
				punto[k] = ds[d*i+k];
			}	// Sto supponendo che il dataset sia memorizzato per righe

			distanza=distance(punto, centroids[j]);

			if(distanza < minDist){
				minDist = distanza;
				minCentr = j;
			}
struct
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


*/
void testIndex(params* input2){
  printf("\n###########Chiamata a Funzione TestIndex############\n");
  /*
    Mettere qui tutto quello che serve per testare quanto scritto

  */
	//Collego l'input passato dal main con la struttura usata in questo codice
  input = input2;


  //generate_centroids(input->n, input->d, input->k);
  generate_centroids(100, 128, 10);	//Test

	//compute_distance_from_centroids(input->n, input->d, input->k);
	compute_distance_from_centroids(100, 128, 10);	//Test
}
