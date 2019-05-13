//Necessario per poter utilizzare la struttura params definita dal prof
#include "datatypes.h"


//Parametri ottenuti in input
params *input;


//------------------------------------METODI------------------------------------


// Funzione che quantizza il punto dell query.
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

// Funzione che calcola la distanza tra il punto ed il centroide del del sottogruppo
// indicato attraverso l'input, in input si passano inoltre l'indirizzo di partenza
// del punto e quel centroide.
// Versione per il query set.

void testSearch(params* input2){
  printf("\n###########Chiamata a Funzione TestSearch############\n");
  /*
    Mettere qui tutto quello che serve per testare quanto scritto
  */

	//Collego l'input passato dal main con la struttura usata in questo codice
  input = input2;
}
