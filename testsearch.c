//Necessario per poter utilizzare la struttura params definita dal prof
#include "datatypes.h"
#include <float.h>


#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))


//Parametri ottenuti in input
params *input;


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

  // Vettore che contiene la quantizzazione del punto
  int* quant;
  int j, g, l, min, max;
	float dist;
  int d_star = d/m;

	// RICORDA:
	// ANN = (int*) malloc(knn*nq*sizeof(int))
	VECTOR distances = (VECTOR) malloc(knn*sizeof(float));


	for(int i=0;i<nq;i++){

		if(input->symmetric==1){
			// Calcoliamo la quantizzazione del punto
			quant = quantize(&qs[i*d], k, m, d, centroids);
//			printf("QUANTIZZAZIONE PUNTO:\n");
//			print_quantizer(m, quant);
		}

		// Inizializziamo i primi knn punti ordinandoli in ordine decrescente
		// Così se è > del primo elemento è inutile fare i confronti


		float test = 0;
    for(j=0;j<knn;j++)  distances[j] = FLT_MAX;


		// Vediamo quali sono i punti del dataset più vicini
		for(j=0;j<n;j++){
			// Calcoliamo la distanza tra le due quantizzazioni
			dist = 0.0;

			if(input->symmetric==1){
				for(g=0;g<m;g++){
					// Prendere il massimo ed il minimo mi serve per potermi muovere sulla
					// triangolare superiore che contiene le distanze
					//min = min(centroids[j*m+g],quant[g]);
					//max = max(centroids[j*m+g],quant[g]);
//					printf("Voglio sapere la distanza tra il centroide %d e il centroide %d\n",quant[g],  centroid_of_point[j*m+g]);
					dist += get_distance(quant[g], centroid_of_point[j*m+g],k,g);
//					printf(" Distanza parziale: %f\n", get_distance(quant[g], centroid_of_point[j*m+g],k,g));
				}// for l
			}else{
				for(g=0;g<m;g++){
					dist+= distance(&qs[i*d+g*d_star],&centroids[(k*g+centroid_of_point[j*m+g])*d_star],d_star);
				}// for l
			}

//			printf("Distanza del punto %d del queryset dal punto %d del dataset: %f \n",i,j,dist);
			//printf("%10.2f,\t", dist);

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

	dealloc_matrix(distances);

	// TESTATA! :D
}// calNearExtS


//-------------------------- RICERCA NON ESAUSTIVA ---------------------------


void NoExaSearch(MATRIX ds, MATRIX qs, MATRIX centroids, MATRIX coarse_centroids,
	int* lista_invertita, int* celle_prima, int* punti_caricati,
	int* ANN, int d, int w, int k, int kc, int knn,int m,int nq){
	// Per ogni punto cerchiamo i w centroidi coarse più vicini
	int* c_coarse = (int*) malloc(w*sizeof(int));
	int* quantization;
	VECTOR dist;
	VECTOR res;
	int j,g,l,p,inizio,centroide;
	int d_star = d/m;
	float distanza;

	printf("\nPunti di ciascun centroide coarse\n");
	print_matrix_int(kc,1,1,punti_caricati,'c');

	for(int i=0;i<nq;i++){

		dist = alloc_matrix(1,w);

		// Settiamo il vettore delle distanze al massimo float rappresentabile
		for(j=0;j<w;j++){
			dist[j] = FLT_MAX;
		}// for j

		// Vediamo quali sono i centroidi coarse più vicini
		for(j=0;j<kc;j++){
			// Calcoliamo la distanza tra le due quantizzazioni
			distanza = 0;

			for(l=0;l<m;l++){
				distanza += distance(&qs[i*d+l*d_star],&coarse_centroids[j],d);
			}// for l

			printf("\nDistanza dal centroide coarse: %f\n", distanza);

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

		printf("\nMatrice delle distanze: \n");
		print_matrix(1,w,w,dist,'p');

		dealloc_matrix(dist);

		dist = alloc_matrix(1,knn);

		for(j=0;j<knn;j++){
			dist[j] = FLT_MAX;
		}// for j

		print_matrix(1,knn,knn,dist,'p');

		for(j=0;j<w;j++){
			centroide = c_coarse[j];
			printf("\nCentroide coarse: %d\n", centroide);

			// Calcoliamo il residuo del punto del queryset considerato
			res = residual(&ds[i*d],&coarse_centroids[centroide],d);

			// Delimitiamo il nostro vettore per poterlo scandire
			inizio = celle_prima[centroide];

			if(input->symmetric == 1){
				quantization = quantize(res,k,m,d_star,coarse_centroids);
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
						distanza += distance(res,&centroids[l],d_star);
					}// for l
				}

				printf("\nDistanza: %f \n",distanza);

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

				print_matrix(1,knn,knn,dist,'p');

			}//for p

			printf("\nk centroidi più vicini: \n");
			print_matrix_int(nq,knn,knn,ANN,'p');

			// A questo punto abbiamo ottenuto i knn punti del queryset pi� vicini
			// tra quelli che ricadevano tra i w centroidi coarse

		}// for j

	}//for i

	if(input->symmetric==1){
		free(quantization);
	}

	free(c_coarse);
	dealloc_matrix(dist);
	dealloc_matrix(res);

} // NoExaSearch


void testSearch(params* input2){
  printf("\n###########Chiamata a Funzione TestSearch############\n");
  /*
    Mettere qui tutto quello che serve per testare quanto scritto
  */

	//Collego l'input passato dal main con la struttura usata in questo codice
  input = input2;

	//---------------------------Dati piccoli per il test---------------------------

  //	for(int i=0; i<12; i++)
	//		for( int j=0; j<4; j++)
	// 			input->ds[i*4+j] = i+j*2.5+rand()%20;

	//Prendo un sottoinsieme del dataset
	//	input->n = 12;
	//	input->d = 4;
	//	input->k = 4; //2
	//	input->m = 2;
	//	input->eps = 15;
/*
		printf("Dataset Iniziale\n");
		print_matrix(input->n, input->d, input->n , input->ds, 'p');

	//---------------------------Test singole funzioni---------------------------
*/
  // Prendo un sottoinsieme del query set originale
  input->nq = 5;
  input->knn = 3;
	input->w = 1;

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
/*		NoExaSearch(input->ds, input->qs, input->centroids, input->coarse_centroids,
								input->lista_invertita, input->celle_prima, input->punti_caricati,
								input->ANN, d, w, k, kc, knn, m, nq);
*/	}

//  print_matrix_int(nq,knn,knn,input->ANN,'p');



}
