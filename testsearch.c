//Necessario per poter utilizzare la struttura params definita dal prof
#include "datatypes.h"
#include <float.h>

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))


//Parametri ottenuti in input
params *input;

//_____________________Funzioni esterne scritte in assembly_____________________
//extern float distance64(VECTOR x1, VECTOR x2, int d);
extern VECTOR residual(VECTOR x,VECTOR centroid,int d);
//*extern float objective_function(int n,int m, MATRIX distances_from_centroids);
extern void memset_float(float* array, float val, int dim );

//--------------------------------FUNZIONI------------------------------------

/** La seguente funzione si occupa di precalcolare le distanze tra i sottogruppi
	* della query ed i centroidi del codebook relativo al sottogruppo
	* x = query, vettore di dimensione d
	* centroids = matrice k*m*d_star contenenti i centroidi degli m codebook del
	*							quantizzatore prodotto
	* d = dimensione del punto
	* m = numero di gruppi
	* k = numero di centroidi per gruppo
	* distances = matrice m*k in cui vengono salvate le distanze
	*/
void computeDistances(VECTOR x, MATRIX centroids, int d, int m, int k, MATRIX distances){
	int i, d_star = d/m;

	// Considero ogni porzione del punto
	for(int g=0;g<m;g++){
		// Per ogni porzione calcolo la distanza da diascun centroide
		for(i=0;i<k;i++){
			distances[g*k+i] = distance(&x[g*d_star],&centroids[(g*k+i)*d_star],d_star);
		}
	}

}// computeDistances

//---------------------------RICERCA ESAUSTIVA--------------------------------

/** Funzione che trova per ogni punto del queryset i k punti del dataset più vicini
	* Per ciascun punto del dataset si prende la sua quantizzazione da centroid_of_point
	* e si mantiengono i k punti più vicini ed un vettore che mantiene
	* in memoria le loro distanze per confrontarli man mano con tutti i punti
  * n =  numero di punti del data set
  * k =  numero di centroidi di ogni sotto-quantizzatore
	* m = numero di gruppi del quantizzatore prodotto
	* knn = numero di ANN (Aproximate Nearest Neighbor) approssimati da restituire per ogni query
  * nq = numero di punti del query set
	* qs = query set
  * d_star = numero di dimensioni per ogni sottogruppo
	*/

void calNearExt(int n, int d, int k, int m, int knn, int nq, MATRIX qs,
				int* ANN, int* centroid_of_point, MATRIX  distances_between_centroids, MATRIX centroids){

	MATRIX dist_cent;
  // Vettore che contiene la quantizzazione del punto
  int* quant;
  int j, g, l, min, max;
	float dist;
  int d_star = d/m;

	if(input->symmetric == 0){
		dist_cent = alloc_matrix(m,k);
	}

	// RICORDA:
	// ANN = (int*) malloc(knn*nq*sizeof(int))
	VECTOR distances = (VECTOR) malloc(knn*sizeof(float));


	for(int i=0;i<nq;i++){

		if(input->symmetric==1){
			// Calcoliamo la quantizzazione del punto
			quant = quantize(&qs[i*d], k, m, d, centroids);
//			printf("QUANTIZZAZIONE PUNTO:\n");
//			print_quantizer(m, quant);
		}else{
			// Calcoliamo la distanza tra le porzioni del punto ed i centroidi dei
			// relativi codebook
			computeDistances(&qs[i*d],centroids,d,m,k,dist_cent);
		}


		// Inizializziamo i primi knn punti ordinandoli in ordine decrescente
		// Così se è > del primo elemento è inutile fare i confronti


		float test = 0;
    for(j=0;j<knn;j++)  distances[j] = FLT_MAX;
//		memset_float( distances, FLT_MAX, knn);


		// Vediamo quali sono i punti del dataset più vicini
		for(j=0;j<n;j++){
			// Calcoliamo la distanza tra le due quantizzazioni
			dist = 0.0;

			if(input->symmetric==1){
				for(g=0;g<m;g++){
//					printf("Voglio sapere la distanza tra il centroide %d e il centroide %d\n",quant[g],  centroid_of_point[j*m+g]);
					dist += get_distance(quant[g], centroid_of_point[j*m+g],k,g);
//					printf(" Distanza parziale: %f\n", get_distance(quant[g], centroid_of_point[j*m+g],k,g));
				}// for l
			}else{
				for(g=0;g<m;g++){
					// Prendiamo la distanza tra la porzione g della query ed il centroide
					// della porzione g del punto del dataset
					dist+= dist_cent[g*k+centroid_of_point[j*m+g]];
				}// for l
			}

//			printf("Distanza del punto %d del queryset dal punto %d del dataset: %f \n",i,j,dist);
//			printf("%10.2f,\t", dist);

			if(distances[0]>dist){
				l = 0;
				while(l<knn-1 && distances[l+1]>dist){
					distances[l] = distances [l+1];
					ANN[i*knn+l] = ANN[i*knn+l+1];
					l++;
				}// while
				distances[l] = dist;
				ANN[i*knn+l] = j;
			}// if

		}// for j

		// Una volta terminato questo ciclo in ANN[i] ci saranno i knn punti del
		// dataset più vicini alla query
		if(input->symmetric==1){
			free(quant);
		}

	}// for i

	if(input->symmetric==0){
		dealloc_matrix(dist_cent);
	}

	dealloc_matrix(distances);

	// TESTATA! :D
}// calNearExtS


//-------------------------- RICERCA NON ESAUSTIVA ---------------------------


void NoExaSearch(MATRIX ds, MATRIX qs, MATRIX centroids, MATRIX coarse_centroids,
	int* lista_invertita, int* celle_prima, int* punti_caricati,
	int* ANN, int d, int w, int k, int kc, int knn,int m,int nq){

	MATRIX dist_cent;
	// Per ogni punto cerchiamo i w centroidi coarse più vicini
	int* c_coarse = (int*) calloc(w, sizeof(int));

	int* quantization;
	VECTOR dist;
	VECTOR res;
	int j,g,l,p,inizio,centroide;
	int d_star = d/m;
	float distanza;

	if(input->symmetric == 0){
		dist_cent = alloc_matrix(m,k);
	}

//	printf("\nPunti di ciascun centroide coarse\n");
//	print_matrix_int(kc,1,1,punti_caricati,'c');

	for(int i=0;i<nq;i++){

		dist = alloc_matrix(1,w);

		// Settiamo il vettore delle distanze al massimo float rappresentabile
		for(j=0;j<w;j++){
			dist[j] = FLT_MAX;
		}// for j
//		memset_float( dist, FLT_MAX, w);


		// Vediamo quali sono i centroidi coarse più vicini
		for(j=0;j<kc;j++){
			// Calcoliamo la distanza tra le due quantizzazioni

	//			printf("Queryset: %p\n", &qs[i*d+l*d_star] );
//				printf("CoarseCe: %p\n\n", &coarse_centroids[j] );
			distanza = distance(&qs[i*d],&coarse_centroids[j*d],d);


//			printf("\nDistanza dal centroide coarse: %f\n", distanza);

			if(dist[0]>distanza){
				l = 0;
				while(l<w-1 && dist[l+1]>distanza){
					dist[l] = dist[l+1];
					c_coarse[l] = c_coarse[l+1];
					l++;
				}// while

				dist[l] = distanza;
				c_coarse[l] = j;
			}// if
		}// for j

		// Ora sappiamo in quali insiemi di punti dobbiamo cercare, ciascuna
		// distanza rappresenta il residuo al quadrato

		// Iniziamo la ricerca tra i punti che si trovano nelle celli di Voronoi
		// dei w centroidi coarse

//		printf("\nMatrice delle distanze: \n");print_matrix(1,w,w,dist,'p');
//		print_matrix(1,w,w,dist,'p');

		dealloc_matrix(dist);

		dist = alloc_matrix(1,knn);


		for(j=0;j<knn;j++){		dist[j] = FLT_MAX;	}// for j
//		memset_float( dist, FLT_MAX, knn);

//		print_matrix(1,knn,knn,dist,'p');


		for(j=0;j<w;j++){
			centroide = c_coarse[j];
//			printf("\nCentroide coarse: %d\n", centroide);

			// Calcoliamo il residuo del punto del queryset considerato
			res = residual(&qs[i*d],&coarse_centroids[centroide*d],d);

			// Delimitiamo il nostro vettore per poterlo scandire
			inizio = celle_prima[centroide];

			if(input->symmetric == 1){
				quantization = quantize(res,k,m,d,centroids);
			}else{
				// Calcoliamo la distanza tra le porzioni del punto ed i centroidi dei
				// relativi codebook
				computeDistances(res,centroids,d,m,k,dist_cent);
			}

			// Calcoliamo i knn punti del queryset i cui residui sono più vicini ai
			// residui del punto
			//printf("\nPunti presenti: %d\n", punti_caricati[centroide]);

			for(p=0;p<punti_caricati[centroide];p++){
				// Calcoliamo la distanza tra la quantizzazione del residuo ed il
				// residuo del punto
				distanza = 0;

				if(input->symmetric == 1){
					for(g=0;g<m;g++){
						//l = lista_invertita[inizio+p*(m+1)+g+1];
						distanza += get_distance(quantization[g], lista_invertita[inizio+p*(m+1)+g+1], k, g);
					}
				}else{
					for(g=0;g<m;g++){
						// Nel primo elemento c'è il punto, quindi si shifta di 1
						l = lista_invertita[inizio+p*(m+1)+g+1];
						// Prendiamo la distanza tra la porzione g della query ed il centroide
						// della porzione g del punto del dataset
						distanza += dist_cent[g*k+l];
					}// for l
				}

//				printf("\nDistanza: %f \n",distanza);

				if(dist[0]>distanza){
					l = 0;
					while(l<knn-1 && dist[l+1]>distanza){
						dist[l] = dist[l+1];
						ANN[i*knn+l] = ANN[i*knn+l+1];
						l++;
					}// while
					dist[l] = distanza;
					ANN[i*knn+l] = lista_invertita[inizio+p*(m+1)];
				}// if

//				print_matrix(1,knn,knn,dist,'p');

			}//for p

//			printf("\nk centroidi più vicini: \n");
//			print_matrix_int(nq,knn,knn,ANN,'p');

			// A questo punto abbiamo ottenuto i knn punti del queryset pi� vicini
			// tra quelli che ricadevano tra i w centroidi coarse

		}// for j

	}//for i

	if(input->symmetric==1){
		free(quantization);
	}else{
		dealloc_matrix(dist_cent);
	}


	free(c_coarse);
	dealloc_matrix(dist);
	dealloc_matrix(res);

	// TESTATA

} // NoExaSearch



void testSearch(params* input2){
  printf("\n###########Chiamata a Funzione TestSearch############\n");
  /*
    Mettere qui tutto quello che serve per testare quanto scritto
  */

/*
	//Collego l'input passato dal main con la struttura usata in questo codice
  input = input2;

	//---------------------------Dati piccoli per il test---------------------------

  //	for(int i=0; i<12; i++)
	//		for( int j=0; j<4; j++)
	// 			input->ds[i*4+j] = i+j*2.5+rand()%20;

*/

	//Prendo un sottoinsieme del dataset
	//	input->n = 12;
	//	input->d = 4;
	//	input->k = 4; //2
	//	input->m = 2;
	//	input->eps = 15;

	printf("Dataset Iniziale\n");
	print_matrix(input->n, input->d, input->n , input->ds, 'p');

	//---------------------------Test singole funzioni---------------------------

  // Prendo un sottoinsieme del query set originale
//  input->nq = 1;
//  input->knn = 1;
//	input->w = 1;

  int nq = input->nq;
  int knn = input-> knn;
  int k = input->k;
  int n = input->n;
  int d = input->d;
  int m = input->m;
	int kc = input->kc;
	int w = input->w;




//  printf("Queryset Iniziale\n");
//  print_matrix(input->nq, input->d, input->nq , input->qs, 'p');


	//---------------------------Test completo---------------------------
  input->ANN = (int*) malloc(nq*knn*sizeof(int));
  memset(input->ANN, -1, nq*knn*sizeof(int));
//	print_matrix_int(nq,knn,knn, input->ANN,'p');


	if(input->exaustive == 1){
		calNearExt(n, d, k, m, knn, nq,
								input->qs, input->ANN, input->centroid_of_point,
								input->distances_between_centroids, input->centroids);
	} else {
		NoExaSearch(input->ds, input->qs, input->centroids, input->coarse_centroids,
								input->lista_invertita, input->celle_prima, input->punti_caricati,
								input->ANN, d, w, k, kc, knn, m, nq);
	}


//	print_matrix_int(nq,knn,knn,input->ANN,'p');

	for(int i=0;i<nq;i++){
		printf("I %d punti più vicini alla query %d sono: ",knn,i);
		for(int j=0;j<knn;j++){
			printf("%d\t",input->ANN[i*knn+j]);
		}
		printf("\n");
	}

//  print_matrix_int(nq,knn,knn,input->ANN,'p');


}
