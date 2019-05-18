//Necessario per poter utilizzare la struttura params definita dal prof
#include "datatypes.h"

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))


//Parametri ottenuti in input
params *input;


//---------------------------RICERCA ESAUSTIVA--------------------------------

/** Funzione che trova per ogni punto del queryset i k punti del dataset più vicini
	* per ciascun punto del dataset si prende la sua quantizzazione da centroid_of_point
	* e si mantiengono i k punti più vicini ed un vettore che mantiene
	* in memoria le loro distanze per confrontarli man mano con tutti i punti
	*/
void calNearExtS(MATRIX qs, int* ANN, int* centroid_of_point,MATRIX  distances_between_centroids, int k, int m, int knn, int nq, int n, int d_star){
	// RICORDA:
	// ANN = (int*) malloc(knn*nq*sizeof(int))
	VECTOR distances = (VECTOR) malloc(knn*sizeof(double));
	// Vettore che contiene la quantizzazione del punto
	int* quantizzation;
	float distance;
	int j,l;

	for(int i=0;i<nq){

		if(input->symmetric==1){
			// Calcoliamo la quantizzazione del punto
			quantizzation = quantize(&qs[i*d], m, d_star);
		}

		// Inizializziamo i primi knn punti ordinandoli in ordine decrescente
		// Così se è > del primo elemento è inutile fare i confronti
		for(j=0;j<knn;j++){
			distances[j] = FLT_MAX;
		}// for j

		// Vediamo quali sono i punti del dataset più vicini
		for(j=0;j<n;j++){
			// Calcoliamo la distanza tra le due quantizzazioni
			distance = 0;

			if(input->symmetric==1){
				for(l=0;l<m;l++){
					// Prendere il massimo ed il minimo mi serve per potermi muovere sulla
					// triangolare superiore che contiene le distanze
					min = min(centroids[j*m+l],quantizzation[l]);
					max = max(centroids[j*m+l],quantizzation[l]);
					distance += distances_between_centroids[min*(min+1)/2+g*(k*(k+1)/2)+max];
				}// for l
			}else{
				for(l=0;l<m;l++){
					distance += distance(&qs[i*d+l*d_star],&centroids[centroid_of_point[j*m+l]],d_star)
				}// for l
			}

			if(distances[0]>distance){
				l = 0;
				while(distances[l+1]>distance && l<knn-1){
					distances[l] = distances [l+1];
					ANN[i*knn+l] = ANN[i*knn+l];
					l++;
				}// while
				distances[l] = distance;
				ANN[i*knn+l] = j;
			}// if

		}// for j

		// Una volta terminato questo ciclo in ANN[i] ci saranno i knn punti del
		// dataset più vicini alla query

		free(quantizzation);

	}// for i

	dealloc_matrix(distances);

}// calNearExtS


//-------------------------- RICERCA NON ESAUSTIVA ---------------------------

void NoExaSearch(MATRIX ds,MATRIX centroids_coarse,int* ANN, int d, int knn,int m,int nq, int d_star){
	// Per ogni punto cerchiamo i w centroidi coarse più vicini
	int* c_coarse = (int*) malloc(w*sizeof(int));
	int* quantization;
	VECTOR dist,res;
	int j,l,p,inizio,centroide;

	for(int i=0;i<nq,i++){

		dist= alloc_matrix(1,w);

		// Settiamo il vettore delle distanze al massimo float rappresentabile
		for(j=0;j<w;j++){
			dist[j] = FLT_MAX;
		}// for j

		// Vediamo quali sono i centroidi coarse pi� vicini
		for(j=0;j<kc;j++){
			// Calcoliamo la distanza tra le due quantizzazioni
			distance = 0;

			for(l=0;l<m;l++){
				distance += distance(&qs[i*d+l*d_star],&centroids_coarse[j],d);
			}// for l

			if(distances[0]>distance){
				l = 0;
				while(distances[l+1]>distance && l<knn-1){
					dist[l] = distances [l+1];
					c_coarse[l] = c_coarse[l+1];
					l++;
				}// while
				dist[l] = distance;
				c_coarse[l] = j;
			}// if
		}// for j

		// Ora sappiamo in quali insiemi di punti dobbiamo cercare, ciascuna
		// distanza rappresenta il residuo al quadrato

		// Iniziamo la ricerca tra i punti che si trovano tra i w centroidi coarse

		dealloc_matrix(dist);
		dist = alloc_matrix(knn*sizeof(int));

		for(j=0;j<knn;j++){
			dist[j] = FLT_MAX;
		}// for j

		for(j=0;j<w,j++){
			centroide = c_coarse[j]

			// Calcoliamo il residuo del punto del queryset considerato
			res = residual(&ds[i*d],&coarse_centroids[centroide],d);

			// Delimitiamo il nostro vettore per poterlo scandire
			inizio = celle_prima[centroide]*(m+1);

			if(input->symmetric){
				quantizzation = quantize(res,m,d_star);
			}

			// Calcoliamo i knn punti del queryset i cui residui sono più vicini ai
			// residui del punto
			for(p=0;p<punti_caricati[centroide];p++){
				// Calcoliamo la distanza tra la quantizzazione del residuo ed il
				// residuo del punto
				distance = 0;

				if(input->symmetric==1){
					for(l=0;l<m;l++){
						// min e max servono per trovare gli indici per accedere alla
						// struttura tiangolare che contiene le distanze eliminando i
						// duplicati
						min = min(centroids[j*m+l],quantizzation[l]);
						max = max(centroids[j*m+l],quantizzation[l]);
						distance += distances_between_centroids[min*(min+1)/2+g*(k*(k+1)/2)+max];
					}
				}else{
					for(l=0;l<m;l++){
						distance += distance(res,&centroids[lista_invertita[inizio+p*(m+1)+l]],d_star);
					}// for l
				}

				if(distances[0]>distance){
					l = 0;
					while(distances[l+1]>distance && l<knn-1){
						dist[l] = distances [l+1];
						ANN[i*m+l] = ANN[i*m+l+1];
						l++;
					}// while
					dist[l] = distance;
					ANN[i*m+l] = lista_invertita[inizio+p*(m+1)+l+1];
				}// if


			}//for p

			// A questo punto abbiamo ottenuto i knn punti del queryset pi� vicini
			// tra quelli che ricadevano tra i w centroidi coarse

		}// for j

	}//for i

	free(c_coarse);
	free(quantizzation);
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
}
