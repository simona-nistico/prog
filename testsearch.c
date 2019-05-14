//Necessario per poter utilizzare la struttura params definita dal prof
#include "datatypes.h"

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))


//Parametri ottenuti in input
params *input;


//------------------------------------METODI------------------------------------


/** Funzione che effettua la quantizzazione della query
	* COSTO: =(d_star)
	*/
int* quantize(int x, int m, int d_star){
	// Centroidi che quantizzano ciascuna porzione
	int* cents = (int*) malloc(m,sizeof(int))
	int centr,j;
	float min,dist;

	// Calcoliamo il centroide di ciascun sottogruppo
	for(int g=0;g<m;g++){
		// Calcoliamo il centroide più vicino del sottogruppo
		centr = 0;
		min = distance(&x[m*d_star],&centroids[m*k*d_star],d_star);

		//Considero tutti i punti del codeboock del sottogruppo prendendo quello a
		//dimensione minima
		for(j=1;j<k;j++){
			tmp = distance(&x[m*d_star],&centroids[(m*k+i)*d_star],d_star);
			if(dist < min){
				centr = j;
				min = dist;
			}

			// Abbiamo trovato il centroide della porzione
			cents[i] = centr;
		}
	}
	return cents;
}

//---------------------------RICERCA ESAUSTIVA--------------------------------

/** Funzione che trova per ogni punto del queryset i k punti del dataset più vicini
	* per ciascun punto del dataset si prende la sua quantizzazione da centroid_of_point
	* e si mantiengono i k punti più vicini ed un vettore che mantiene
	* in memoria le loro distanze per confrontarli man mano con tutti i punti
	*/
void calculateNearestExt(MATRIX qs, int* ANN, int* centroid_of_point, int k, int knn, int nq, int n, int d_star){
	// RICORDA:
	// ANN = (int*) malloc(knn*nq*sizeof(int))
	VECTOR distances = (VECTOR) malloc(knn*sizeof(double));
	// Inizializziamo tutto il vettore con
	int* quantizzation;
	float distance;
	int j,l;

	for(int i=0;i<nq){
		// Calcoliamo la quantizzazione del punto
		quantizzation = quantize(&qs[i*d], m, d_star);

		// Inizializziamo i primi knn punti ordinandoli in ordine decrescente
		// Così se è > del primo elemento è inutile fare i confronti
		for(j=0;j<knn;j++){
			distances[i*knn+j] = FLT_MAX;
		}

		// Vediamo quali sono i punti del dataset più vicini
		for(j=0;j<n;j++){
			// Calcoliamo la distanza tra le due quantizzazioni
			distance = 0;
			for(l=0;l<m;l++){
				// Prendere il massimo ed il minimo mi serve per potermi muovere sulla
				// triangolare superiore che contiene le distanze
				min = min(centroids[j*m+l],quantizzation[l]);
				max = max(centroids[j*m+l],quantizzation[l]);
				distance += distances_between_centroids[min*(min+1)/2+g*(k*(k+1)/2)+max];
			}

			if(distances[0]>distance){
				l = 0;
				while(distances[l+1]>distance && l<knn-1){
					distances[l] = distances [l+1];
					ANN[i*knn+l] = ANN[i*knn+l];
					l++;
				}
				distances[l] = distance;
				ANN[i*knn+l] = j;
			}

		}

		// Una volta terminato questo ciclo in ANN[i] ci saranno i knn punti del
		// dataset più vicini alla query

		free(quantizzation);

	}

	free(distances);

}

//------------------------- FINE RICERCA ESAUSTIVA ---------------------------

void testSearch(params* input2){
  printf("\n###########Chiamata a Funzione TestSearch############\n");
  /*
    Mettere qui tutto quello che serve per testare quanto scritto
  */

	//Collego l'input passato dal main con la struttura usata in questo codice
  input = input2;
}
