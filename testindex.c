#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>

//Necessario per poter utilizzare la struttura params definita dal prof
#include "datatypes.h"



//Matrice che ogni riga rappresenta un punto e ogni colonna un centroide
//L'elemento m[i][j] rappresenta la distanza del punto i dal centroide j
MATRIX distances_from_centroids; //TODO Verificare se serve questa struttura

//Per ogni punto (riga) viene indicato il centroide prodotto più vicino e la distanza da stesso
//centroid_of_point[i][j] = centroide a minima distanza del sottogruppo j
//Corrisponde alla funzione q(x) := dato il punto, restituisce l'indice del suo centroide
int* centroid_of_point;


//Codebooks dei vari sottogruppi
MATRIX centroids;	//dimensione: m*k righe,d_star(d/m) colonne

//Parametri ottenuti in input
params *input;

//Struttura dati che contiene le distanze tra i centroidi finali
//Triangolo superiore della matrice delle distanze che è simmetrica
VECTOR distances_between_centroids; //TODO scegliere struttura

//Numero di elementi per sottovettore
int d_star;


//------------------------------------METODI------------------------------------

/**La funzione seguente seleziona k punti da usare come centroidi iniziali
 * COSTO: k*(d/m)*m (Per favore Fabio gli dai un'occhiata?)
 * n =  numero di punti del data set
 * d = numero di dimensioni del data/query set
 * k =  numero di centroidi di ogni sotto-quantizzatore
 * ds = data set
 * m = numero di sottoquantizzatori
 * k numeri casuali diversi da 0 a n incluso
 */
void generate_centroids(int n, int d, int k, MATRIX ds, int m, int d_star){

	int number,old_number,i,j;

	// Alloco la matrice dei centroidi
	centroids = alloc_matrix(k*m,d_star);

  //rand() function returns an integer value between 0 and RAND_MAX(32767)
	// TODO togliere per avere numeri sempre casuali:  srand(time(NULL));

	/**IDEA:
	  * divido gli n punti in k intervalli e ottengo un range di valori;
		* scelgo un numero casuale tra 0 e range e prendo il punto corrispondente;
		* nel caso peggiore viene scelto sempre il max del range e i punti restano sempre entro n;
		* i restanti punti li divido in k-1 intervalli ottenendo un nuovo range;
		* cosi facendo ottengo dei numeri casuali sempre diversi che coprono l'intervallo da  a n;
		*/

		//Effettuiamo le operazioni di calcolo per ciasun sottogruppo
		for(int g=0;g<m;g++){
			old_number = rand()%((n-1)/k);
			printf("Centroide 0 scelto il punto numero: %d\n",old_number);

			// Inseriamo il primo centroide nella matrice lungo la riga 0
			for(j=0;j<d_star;j++)
				centroids[j*d_star*g] = ds[old_number*d+d_star*g+j];	//[numero_riga*d+numero_gruppo*(d/m)+j]

			int range;
			for(i=1;i<k;i++){
				range = (n-1-old_number)/(k-i);	//trovo il nuovo range tra cui scegliere i valori casuali
  			number = old_number+rand()%range+1; //seleziono un numero casuale nel range
				printf("Centroide %d scelto il punto numero: %d\n",i, number);

				// Inseriamo il centroide nella matrice
				for(j=0;j<d_star;j++)
					//[riga->(i+group*k)*d_star,colonna->j]     [riga->number*d,colonna->d_star*g+j]
					centroids[(i+group*k)*d_star+j] = ds[number*d+d_star*g+j];

				old_number = number;
 			}//for i
		}//for g

		print_matrix(k*m, d_star, centroids, 'c');

	//DA TESTARE!
}//generate_centroids




/** Metodo per calcolare la distanza tra due porzioni di punti memorizzati row first order
	* COSTO: d_star
  * d_star = dimensione del sottovettore
	* TODO: applicare il loop vectorization cioè più operazioni nello stesso ciclo
  * Oppure più punti alla volta
  */
double distance_subgroup(int x,int y,int group){
	double sum = 0;
	double diff = 0;

	for(int j=0;j<d_star;j++){
		diff = ds[x*d+d_star*group+j] - ds[y*d+d_star*group+j]
		sum += diff*diff;
	}
	return sqrt(sum);
	//DA TESTARE!
}//distance

double dist_cent_ds(int point,int centr,int group){
	double sum,diff;

	for(int j=0;j<d_star;j++){
		diff = ds[point*d+group*d_star+j]-centroids[(centr+group*k)*d_star+j];
		sum += diff*diff;
	}

	return sqrt(sum);
	//DA TESTARE!
}

//ATTENZIONE! DA RIVEDERE
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
void points_of_centroid(int n, int d, int k, MATRIX ds, int m, int d_star){

	//VECTOR punto = alloc_matrix(1,d);	// Alloco il vettore che conterrà di volta in volta il punto
	//Si può evitare dal momento che posso accedere ai singoli elementi
	//usando l'aritmetica dei puntatori

	double distanza;
	double minDist; 	//Distanza minima dal centroide
	int minCentr;			//Centroide a minima distanza
	int i,j,c; //i = indice per i punti, j = indice per dimensioni, c = indice per centroidi

	for(int g=0;g<m;g++){// devo considerare il codebook di ogni sottogruppo

		for(i=0;i<n;i++){	//per ogni punto del dataset devo trovare il centroide più vicino

			/*
			for(j=0;j<d;j++)	//copio il punto dal dataset nel vettore punto
				punto[j] = input->ds[d*i+j];	// Dataset memorizzato per righe

				*/

			//Inizializzo con il primo punto e con la distanza da esso
			minDist = dist_cent_ds(i,0,g);
			minCentr = 0;
			//printf("Distanza del punto %i dal centroide  0: %f\n", i, minDist);


			for(c=1;c<k;c++){ //Calcolo la distanza tra il punto attuale e ogni centroide

				/*for(j=0;j<d;j++)
					punto[j] = input->ds[d*i+j]; // Dataset memorizzato per righe
					*/

				distanza = dist_cent_ds(i,0,g);
				//printf("Distanza del punto %i dal centroide %i: %f\n", i, c, distanza);

				if(distanza < minDist){
					minDist = distanza;
					minCentr = c;
				}//if
			}//for ogni centroide

			//Salvo i risultati trovati
			//printf("Distanza del punto %i dal centroide minimo %i: %f\n", i, minCentr, minDist);
			centroid_of_point[i*m+g] = minCentr;

			//vedere se è necessario salvarlo in una struttura dati ed eventualmente
			//utilizzare una struttura dati a parte (distances_from_centroids)
			//centroid_of_point[2*i+1] = minDist;
		}//for ogni punto

	}// for ogni sottogruppo

	// Liberiamo lo spazio (verificare se si può utilizzare la stessa funzione della matrice)
	//dealloc_matrix(punto);
	printf("Colonna 0: indice del vettore 'centroids' indicante il centroide più vicino al punto\n");
	printf("Colonna 1: distanza tra il punto e il centroide\n");
	print_matrix(n,m, centroid_of_point, 'p');

	//DA TESTARE!
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
void update_centroids(int n, int d, int k, MATRIX ds, int m, int d_star){

		// Matrice temporanea per memorizzare i nuovi centroidi
		MATRIX tmp = alloc_matrix(k,d_star);

		//Vettore che, per ogni centroide, conta quanti punti appartengono alla sua cella
		int* cont = (int*) malloc(k,sizeof(int)); //usare calloc per inizializzare a 0?

		int centroide; //Centroide a cui appartiene il punto corrente

		//Considero tutti i sottogruppi
		for(int g=0;g<m;g++){ //per ogni sottogruppo

			for(int i=0;i<n;i++){ //per ogni punto del dataset

				centroide = centroid_of_point[i*m+g]; //prendo il centroide di appartenenza
				cont[centroide]++;	//conto un punto in più per questo centroide

				for(int j=0;j<d_star;j++)	//sommo tutte le cordinate di punti del centroide
					tmp[centroide*d_star+j] += ds[i*d+(g*d_star)+j];

			}//for tutti i punti

			//Divido ogni somma di cordinate per il numero di punti e lo inserisco come nuovo centroide
			for(int i=0;i<k;i++){	//per ogni centroide

				for(int j=0;j<d_star;j++)	//per ogni componente
					if( cont[i]!=0 )
						centroids[(i+g*k)*d_star+j] = tmp[i*d_star+j] / (double) cont[i];
						tmp[i*d_star+j] = 0;
					else
						centroids[i*d+j] = -1;

				if( cont[i]==0 )
						printf("###############Nessun punto appartiene al centroide %d\n", i);
				else
						cont[i] = 0;

			}//for tutti i centroidi
	  }//for tutti i sottogruppi

		dealloc_matrix(tmp);
		free(cont);

		printf("Nuovi centroidi:\n");
		print_matrix(k*m,d_star,centroids, 'c');

		//DA TESTARE
}//update_centroids


/** La funzione calcola e memorizza la distanza tra centroidi, le distanze
  * sono memorizzate considerando solo il triangolo superiore della matrice
	* non salvando la parte superiore
	*/
// TODO
void store_distances(){}


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
		int i;

		for(int g=0;g<m;g++){		// per ogni gruppo

			for(i=0;i<n;i++){    // per ogni punto i, aggiunge distanza(i,q(i))^2
				dist = distances_from_centroids[i*m+g];	//verificare se questo valore è aggiornato
				sum += dist*dist;
			}

		}// per ogni gruppo

		printf("Funzione obiettivo: %lf\n\n", sum );
		return sum;

		// Controllare se corretto

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
void calculate_centroids(int n, int d, int k, MATRIX ds, float eps,int m,int d_star){
	int iter = 1;

	//Genera Centroidi casuali
	//COSTO: d+k*d
	generate_centroids(n, d, k, ds, m, d_star);

	centroid_of_point = (int*) malloc(n*m,sizeof(int));	//la matrice va aggiornata

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
		update_centroids(n, d, k, ds, m, d_star);

		//Cambiati i centroidi, cambiano le celle di Voronoi;
		//determina a quale cella appartengono i Punti
		//COSTO: n*k* O(distance)[cioè d] = n*k*d
		points_of_centroid(n, d, k, ds, m, d_star);

		//Verifica l'ottimalità dei Punti
		//COSTO: n
		obiettivoPrev = obiettivo;
		double obiettivo = objective_function(n);
		iter++;

		printf("Variazione funzione obiettivo: %lf\n", (obiettivoPrev - obiettivo) );
		printf("CONDIZIONE DEL WHILE: %d\n", ((obiettivoPrev - obiettivo ) >eps));
		if( obiettivoPrev - obiettivo < 0 ){
			printf("La funzione obiettivo sta salendo:\n");
			printf("Obiettivo vecchio: %lf\nObiettivo corrent: %lf\n", obiettivoPrev, obiettivo);
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

	for(int i=0; i<12; i++)
		for( int j=0; j<4; j++)
 			input->ds[i*4+j] = i+j*2.5+rand()%20;

//Prendo un sottoinsieme del dataset
	input->n = 12;
	input->d = 4;
	input->k = 4; //2
	input->m = 1;
	input->eps = 20;

	d_star = d/m;

	printf("Dataset Iniziale\n");
	print_matrix(input->n, input->d, input->ds, 'p');

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


	calculate_centroids(input->n, input->d, input->k, input->ds, input->eps,input->m,d_star);

}
