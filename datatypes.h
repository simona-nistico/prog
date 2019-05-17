#ifndef datatypes
#define datatypes
//Siccome pqnn e testindex usano questo file, senza queste righe, l'header viene
//definito ppiù volte

#include <xmmintrin.h>	//Usato dalle 4 funzioni sotto per gestire la memoria

#define	MATRIX	float*
#define	VECTOR	float*


typedef struct {
	char* filename; //
	MATRIX ds; // data set
	MATRIX qs; // query set
	int n; // numero di punti del data set
	int d; // numero di dimensioni del data/query set
	int nq; // numero di punti del query set
	int knn; // numero di ANN approssimati da restituire per ogni query
	int m; // numero di gruppi del quantizzatore prodotto
	int k; // numero di centroidi di ogni sotto-quantizzatore
	int kc; // numero di centroidi del quantizzatore coarse
	int w; // numero di centroidi del quantizzatore coarse da selezionare per la ricerca non esaustiva
	int nr; // dimensione del campione dei residui nel caso di ricerca non esaustiva
	float eps; //
	int tmin; //
	int tmax; //
	int exaustive; // tipo di ricerca: (0=)non esaustiva o (1=)esaustiva
	int symmetric; // tipo di distanza: (0=)asimmetrica ADC o (1=)simmetrica SDC
	int silent;
	int display;
	// nns: matrice row major order di interi a 32 bit utilizzata per memorizzare gli ANN
	// sulla riga i-esima si trovano gli ID (a partire da 0) degli ANN della query i-esima
	//
	int* ANN;
	//
	// Inserire qui i campi necessari a memorizzare i Quantizzatori
	//
	// ...
	// ...
	// ...
	//
} params;


/*
 *
 *	Le funzioni sono state scritte assumento che le matrici siano memorizzate
 * 	mediante un array (float*), in modo da occupare un unico blocco
 * 	di memoria, ma a scelta del candidato possono essere
 * 	memorizzate mediante array di array (float**).
 *
 * 	In entrambi i casi il candidato dovrà inoltre scegliere se memorizzare le
 * 	matrici per righe (row-major order) o per colonne (column major-order).
 *
 * 	L'assunzione corrente è che le matrici siano in row-major order.
 *
 */


void* get_block(int size, int elements) {
	return _mm_malloc(elements*size,16); //32 nel caso pqnn64
}


void free_block(void* p) {
	_mm_free(p);
}


MATRIX alloc_matrix(int rows, int cols) {
	return (MATRIX) get_block(sizeof(double),rows*cols);
}


void dealloc_matrix(MATRIX mat) {
	free_block(mat);
}

//----------Stampa tutti i punti-----------
void print_matrix(int rows, int cols, MATRIX data, char c){
	int i, j;
  printf("Numero punti: %d\nDimensione di ogni punto: %d\n", rows, cols);
  for (i = 0; i < rows; i++) {
		if( c=='c')
    	printf("Centroide n %d:\t", i);
		else if( c=='p')
			printf("Punto n %d:\t", i);

    for (int j = 0; j < cols; j++)
      printf("%18.2f\t", data[i*cols+j] );
    printf("\n");
	}
	printf("\n\n");
}

// Funzione che stampa una matrice di interi
void print_matrix_int(int rows, int cols, int* data, char c, int m){
	int i,j;
  printf("Numero punti: %d\nDimensione di ogni punto: %d\n", rows, cols);
  for (i = 0; i < rows; i++) {
		if( c=='c')
    	printf("Centroide n %d:\t", i);
		else if( c=='p')
			printf("Punto n %d:\t", i);

    for (int j = 0; j < cols; j++)
      printf("%d\t", data[i*cols+j] );
    printf("\n");
	}
	printf("\n\n");
}

/** Funzione che calcola la distanza
  * Costo: O(d)
  *
  */
float distance(VECTOR x1,VECTOR x2,int d){
	float diff,sum = 0;

	// Per ridurre l'overhead dovuto ad ogni passo del ciclo effettuiamo al suo
	// interno più operazioni (assumiamo che la dimensione d sia divisibile per
  // due)
	for(int i=0; i<d/2; i++){
		diff = x1[i]-x2[i];
		sum += diff*diff;
		diff = x1[i+1]-x2[i+1];
		sum += diff*diff;
	}

	return sqrt(sum);
}

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

#endif
