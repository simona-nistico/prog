#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>

//Necessario per poter utilizzare la struttura params definita dal prof
#include "datatypes.h"



//Matrice che ogni riga rappresenta un punto e ogni colonna un centroide
//L'elemento m[i][j] rappresenta la distanza del punto i dal centroide j
MATRIX distances_from_centroids; //TODO Verificare se serve questa struttura

//Per ogni punto (riga) viene indicato il centroide più vicino e la distanza da stesso
//centroid_of_point[i][0] = centroide a minima distanza
//centroid_of_point[i][1] = distanza dal centroide
//Corrisponde alla funzione q(x) := dato il punto, restituisce l'indice del suo centroide
MATRIX centroid_of_point;


//Centroidi veri e propri codificati come i punti
MATRIX centroids;

//Parametri ottenuti in input
params *input;


//------------------------------------METODI------------------------------------

/**La funzione seguente seleziona k punti da usare come centroidi iniziali
 * COSTO: d+k*d
 * n =  numero di punti del data set
 * d = numero di dimensioni del data/query set
 * k =  numero di centroidi di ogni sotto-quantizzatore
 * ds = data set
 * k numeri casuali diversi da 0 a n incluso
 */
void generate_centroids(int n, int d, int k, MATRIX ds){

	int number,old_number,i,j;

	// Alloco la matrice dei centroidi
	centroids = alloc_matrix(k,d);

  //rand() function returns an integer value between 0 and RAND_MAX(32767)
	// TODO togliere per avere numeri sempre casuali:  srand(time(NULL));

	/**IDEA:
	  * divido gli n punti in k intervalli e ottengo un range di valori;
		* scelgo un numero casuale tra 0 e range e prendo il punto corrispondente;
		* nel caso peggiore viene scelto sempre il max del range e i punti restano sempre entro n;
		* i restanti punti li divido in k-1 intervalli ottenendo un nuovo range;
		* cosi facendo ottengo dei numeri casuali sempre diversi che coprono l'intervallo da  a n;
		*/

	old_number = rand()%(n/k);
	printf("Centroide 0 scelto il punto numero: %d\n",old_number);

	// Inseriamo il primo centroide nella matrice lungo la riga 0
	for(j=0;j<d;j++)
		centroids[j] = ds[old_number*d+j];

	int range;
	for(i=1;i<k;i++){
		range = (n-old_number)/(k-i);	//trovo il nuovo range tra cui scegliere i valori casuali
  	number = old_number+rand()%range; //seleziono un numero casuale nel range (con +1 sfora il range)
		printf("Centroide %d scelto il punto numero: %d\n",i, number);

		// Inseriamo il centroide nella matrice
		for(j=0;j<d;j++)
			centroids[i*d+j] = ds[number*d+j];

		old_number = number;
 	}//for

	print_matrix(k, d, centroids);

	//TESTATO  	--> 	OK
}//generate_centroids



/** Metodo per calcolare la distanza tra due punti memorizzati row first order
	* COSTO: d
  * d = dimensione dei punti
	* TODO: applicare il loop vectorization cioè più operazioni nello stesso ciclo
  * Oppure più punti alla volta
  */
double distance(VECTOR puntoX, VECTOR puntoY, int d){
	double sum = 0;
	double diff = 0;
	for(int i=0;i<d;i++){
		diff = (puntoX[i]-puntoY[i]);
		sum+= diff*diff;
	}
	return sqrt(sum);
	//TESTATO  	--> 	OK
}//distance



/** La funzione seguente cerca i punti che appartengono ad ogni centroide
	* Logicamente divide lo spazio in celle di Voronoi
	* Per ogni punto del dataset, calcola la di stanza con tutti i centroidi
	* e sceglie la distanza minima
	* COSTO: n*k* O(distance)[cioè d]
  * n =  numero di punti del data set
  * d = numero di dimensioni del data/query set
  * k =  numero di centroidi di ogni sotto-quantizzatore
	* ds = data set
  * TODO: verificare se è possibile sfruttare gli indirizzi ed evitare l'uso
  *       del vettore punto
  */
void points_of_centroid(int n, int d, int k, MATRIX ds){

	//VECTOR punto = alloc_matrix(1,d);	// Alloco il vettore che conterrà di volta in volta il punto
	//Si può evitare dal momento che posso accedere ai singoli elementi
	//usando l'aritmetica dei puntatori

	double distanza;
	double minDist; //Distanza dal minimo centroide
	int minCentr;	//Centroide a minima distanza
	int i,j,c; //i = indice per i punti, j = indice per dimensioni, c = indice per centroidi

	for(i=0;i<n;i++){	//per ogni punto del dataset devo trovare il centroide più vicino

		/*
		for(j=0;j<d;j++)	//copio il punto dal dataset nel vettore punto
			punto[j] = input->ds[d*i+j];	// Dataset memorizzato per righe

			*/

		//Inizializzo con il primo punto e con la distanza da esso
		minDist = distance(ds+d*i, centroids, d);
		minCentr = 0;
		//printf("Distanza del punto %i dal centroide  0: %f\n", i, minDist);


		for(c=1;c<k;c++){ //Calcolo la distanza tra il punto attuale e ogni centroide

			/*for(j=0;j<d;j++)
				punto[j] = input->ds[d*i+j]; // Dataset memorizzato per righe
				*/

			distanza = distance(ds+d*i, centroids+d*c, d);
			//printf("Distanza del punto %i dal centroide %i: %f\n", i, c, distanza);

			if(distanza < minDist){
				minDist = distanza;
				minCentr = c;
			}//if
		}//for ogni centroide

		//Salvo i risultati trovati
		//printf("Distanza del punto %i dal centroide minimo %i: %f\n", i, minCentr, minDist);
		centroid_of_point[2*i] = minCentr;
		centroid_of_point[2*i+1] = minDist;
	}//for ogni punto

	// Liberiamo lo spazio (verificare se si può utilizzare la stessa funzione della matrice)
	//dealloc_matrix(punto);
	printf("Colonna 0: indice del vettore 'centroids' indicante il centroide più vicino al punto\n");
	printf("Colonna 1: distanza tra il punto e il centroide\n");
	print_matrix(n,2, centroid_of_point);

	//TESTATO  	--> 	OK
}//points_of_centroid





/** La funzione seguente calcola il nuovo centroide facendo la media geometrica dei punti
	* Per ogni punto del dataset, fa la media (componente per componente)
	* di tutti i punti che appartengono alla stessa cella
	* COSTO: n*d+k*d
  * n =  numero di punti del data set
  * d = numero di dimensioni del data/query set
  * k =  numero di centroidi di ogni sotto-quantizzatore
	* ds = data set
  * TODO: parallelizzare la somma in assembly
  */
void update_centroids(int n, int d, int k, MATRIX ds){
		// Matrice temporanea per memorizzare i nuovi centroidi
		MATRIX tmp = alloc_matrix(k,d);

		//Vettore che, per ogni centroide, conta quanti punti appartengono alla sua cella
		VECTOR cont = malloc(k*sizeof(double));

		int centroide; //Centroide a cui appartiene il punto corrente

		for(int i=0;i<n;i++){ //per ogni punto del dataset
			centroide = centroid_of_point[2*i]; //prendo il centroide di appartenenza
			cont[centroide]++;	//conto un punto in più per questo centroide

			for(int j=0;j<d;j++)	//sommo tutte le cordinate di punti del centroide
				tmp[centroide*d+j] += ds[i*d+j];
		}//for tutti i punti

		//Divido ogni somma di cordinate per il numero di punti e lo inserisco come nuovo centroide
		for(int i=0;i<k;i++){
			for(int j=0;j<d;j++)
				centroids[i*d+j] = tmp[i*d+j] / (double) cont[i];
		}//for tutti i centroidi

		dealloc_matrix(tmp);
		free(cont);

		printf("Nuovi centroidi:\n");
		print_matrix(k,d,centroids);

		//TESTATO  	--> 	OK
}//update_centroids




//-----------------------------ATTENZIONE-----------------------------
//forse l'arresto non dipende dalla funzione obiettivo ma da:
// dist(vecchio centroide, nuovo centroide) < epsilon

/** La funzione seguente calcola il valore della funzione obiettivo
  * che deve essere minimizzata al fine avere un buon insieme di centroidi
	* C = min   sum[ dist^2(y, centroide(y) )  ]
	* COSTO: n
  * n =  numero di punti del data set
  * TODO: parallelizzare la somma in assembly
  */
double objective_function(int n){
		double sum = 0, dist = 0;

		for(int i=0;i<n;i++){    // per ogni punto i, aggiunge distanza(i,q(i))^2
			dist = centroid_of_point[2*i+1];	//verificare se questo valore è aggiornato
			sum += dist*dist;
		}

		printf("Funzione obiettivo: %lf\n\n", sum );
		return sum;
}//objective_function



/** La funzione seguente calcola i centroidi finali:
  * genera dei centroidi casuali, divide lo spazio in celle di Voronoi,
	* calcola i nuovi centroidi facendo la media dei punti di ogni cella
	* ripete finchè....????
	* C = min   sum[ dist^2(y, centroide(y) )  ]
	* COSTO:
  * n =  numero di punti del data set
  * d = numero di dimensioni del data/query set
  * k =  numero di centroidi di ogni sotto-quantizzatore
	* ds = data set
  */
void calculate_centroids(int n, int d, int k, MATRIX ds, float eps){
	int iter = 1;

	//Genera Centroidi casuali
	//COSTO: d+k*d
	generate_centroids(n, d, k, ds);

	centroid_of_point = alloc_matrix(n,2);	//la matrice va aggiornata

	printf("############################ ITERAZIONE %d ############################\n", 0);
	//Divide lo spazio in celle di Voronoi
	//COSTO: n*k* O(distance)[cioè d] = n*k*d
	points_of_centroid(n, d, k, ds);

	//Verifica l'ottimalità dei Punti
	//COSTO: n
	double obiettivo = objective_function(n);
	double obiettivoPrev = 0.0;


//Per la formula della terminazione il prof deve aggiornare le specifiche di progetto sul sito
	while( (obiettivoPrev - obiettivo ) > eps || iter == 1) {
		printf("############################ ITERAZIONE %d ############################\n", iter);

		//Trova i nuovi centroidi facendo la media dei punti delle celle di Voronoi
		//COSTO: n*d+k*d
		update_centroids(n, d, k, ds);

		//Cambiati i centroidi, cambiano le celle di Voronoi;
		//determina a quale cella appartengono i Punti
		//COSTO: n*k* O(distance)[cioè d] = n*k*d
		points_of_centroid(n, d, k, ds);

		//Verifica l'ottimalità dei Punti
		//COSTO: n
		obiettivoPrev = obiettivo;
		double obiettivo = objective_function(n);
		iter++;

		printf("Variazione funzione obiettivo: %lf\n", (obiettivoPrev - obiettivo) );
		printf("TEST: %d\n", ((obiettivoPrev - obiettivo ) >eps));
		if( obiettivoPrev - obiettivo < 0 ){
			printf("La funzione obiettivo sta salendo:\n");
			printf("Obiettivo vecchio: %lf\nObiettivo corrente: %lf\n", obiettivoPrev, obiettivo);
		}

  }//while

}//calculate_centroids



void testIndex(params* input2){
  printf("\n###########Chiamata a Funzione TestIndex############\n");
  /*
    Mettere qui tutto quello che serve per testare quanto scritto
  */

	//Collego l'input passato dal main con la struttura usata in questo codice
  input = input2;

//---------------------------Visualizza dati veri---------------------------
/*
	printf("Dataset:\n");
	print_matrix(input->n,input->d,input->ds);

	printf("Queryset:\n");
	print_matrix(input->nq,input->d,input->qs);
*/

//---------------------------Dati piccoli per il test---------------------------
/*	for(int i=0; i<4; i++)
		for( int j=0; j<4; j++)
 			input->ds[i*4+j] = i+j*2.5;
*/
//Prendo un sottoinsieme del dataset
	input->n = 12;
	input->d = 4;
	input->k = 2;
	input->eps = 2;

	printf("Dataset Iniziale\n");
	print_matrix(input->n, input->d, input->ds);

//---------------------------Test singole funzioni---------------------------
/*
  generate_centroids(input->n, input->d, input->k, input->ds);
  //generate_centroids(100, 128, 10);	//Test


	//double x[4] = {1, 2, 3, 8};
	//double y[4] = {9, 8, 7, 6};
	//printf("Distanza: %f\n", distance(x, y,4) );

	centroid_of_point = alloc_matrix(input->n,2);	//forse la metto dentro la funzione next


	points_of_centroid(input->n, input->d, input->k, input->ds);


	objective_function(input->n);


	update_centroids(input->n, input->d, input->k, input->ds);

	points_of_centroid(input->n, input->d, input->k, input->ds);

	objective_function(input->n);

	*/


	calculate_centroids(input->n, input->d, input->k, input->ds, input->eps);

}
