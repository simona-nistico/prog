#ifndef datatypes
#define datatypes
//Siccome pqnn e testindex usano questo file, senza queste righe, l'header viene
//definito ppiù volte

#include <xmmintrin.h>	//Usato dalle 4 funzioni sotto per gestire la memoria

#define	MATRIX float*
#define	VECTOR float*
#define true 1
#define false 0


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

	//_______Inserire qui i campi necessari a memorizzare i Quantizzatori_______

	//Per ogni punto (riga) viene indicato il centroide prodotto più vicino e la distanza da esso
	//centroid_of_point[i][j] = centroide a minima distanza dap punti i del sottogruppo j
	//Corrisponde alla funzione q(x) := dato il punto, restituisce l'indice del suo centroide
	int* centroid_of_point;


	//Codebooks dei vari sottogruppi
	//Dimensione: m*k righe, d_star cioè d/m colonne
	//Per andare da un sottogruppo all'altro bisgna avanzare di m righe
	//Le colonne sono d_start cioè d/m
	MATRIX centroids;


	//Struttura dati che contiene le distanze tra i centroidi finali
	//Triangolo superiore della matrice delle distanze che è simmetrica
	VECTOR distances_between_centroids;


	//TEST
  MATRIX coarse_centroids;

//TEST
  int* lista_invertita;

	// Test
	// Struttura dati che memorizza il centroide coarse di ciascun punto
	int* coarse_centroid_of_point;
	//
	// TEST
	int* punti_caricati; // Ha kc celle e memorizza il numero di punti in ogni
	                     //centroide coarse
	// TEST
	int* celle_prima;    // Ha kc celle e memorizza la posizione di inizio per
	                     // la lista relativa al centroide i
	// TEST
	// Matrice che tiene memoria dei residui
	MATRIX residuals;
	// ...
	// ...
	// ...
	//
} params;


//_____________________Funzioni esterne scritte in assembly_____________________
extern float distance(VECTOR x1, VECTOR x2, int d);
extern VECTOR residual(VECTOR x,VECTOR centroid,int d);
extern float objective_function(int n,int m, MATRIX distances_from_centroids);
extern void memset_float(float* array, float val, int dim );



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
	return (MATRIX) get_block(sizeof(float),rows*cols);
}


void dealloc_matrix(MATRIX mat) {
	free_block(mat);
}

//----------Stampa tutti i punti-----------
void print_matrix(int rows, int cols, int k, MATRIX data, char c){
  printf("Numero punti: %d\nDimensione di ogni punto: %d\n", rows, cols);
  for (int i = 0; i < rows; i++) {
		if( c=='c'){
			if( i%k == 0) printf("Gruppo %d\n", i/k);
    	printf("Centroide n %d:\t", i%k);
		}else if( c=='p')
			printf("Punto n %d:\t", i);

    for (int j = 0; j < cols; j++)
      printf("%8.2f\t", data[i*cols+j] );
    printf("\n");
	}
	printf("\n\n");
}

//----------Stampa tutti i punti interi-----------
void print_matrix_int(int rows, int cols, int k, int* data, char c){
  printf("Numero punti: %d\nDimensione di ogni punto: %d\n", rows, cols);
  for (int i = 0; i < rows; i++) {
		if( c=='c'){
			if( i%k == 0) printf("Gruppo %d\n", i/k);
    	printf("Centroide n %d:\t", i%k);
		}else if( c=='p')
			printf("Punto n %d:\t", i);

    for (int j = 0; j < cols; j++)
      printf("%d\t", data[i*cols+j] );
    printf("\n");
	}
	printf("\n\n");
}

//----------Stampa il quantizzatore-----------
void print_quantizer(int m, int* data){
	printf("Ogni numero rapresenta l'indice del centroide più vicino per quel gruppo.\n");
  printf("Quantizzatore: [");
  for (int j = 0; j < m; j++){
      printf("%3d", data[j] );
			if( j!=m-1 )
				printf(", ");
	}
	printf("]\n\n");
}

//----------Stampa la matrice triangolare superiore-----------
void print_centroids_distances(int k, int m, float* data){
	int val = 0;
	for( int g=0; g<m; g++){
		printf("Gruppo %d\n", g);
		printf(" A->");
		for (int j = 0; j < k; j++) printf("      %d ", j);
		printf("\n");
	  for (int i = 0; i < k; i++) {
			printf("DA->%d ", i);
			for( int j=-1; j< i; j++)	printf("        ");
			for( int j=i+1; j<k; j++) 	printf("  %6.2f", data[val++] );
			printf("\n");
		}
	}//for gruppo
}


/** La funzione seguente calcola il valore della funzione obiettivo
  * che deve essere minimizzata al fine avere un buon insieme di centroidi
	* C = min   sum[ dist^2(y, centroide(y) )  ]
	* COSTO: n
  * n =  numero di punti del data set
  * TODO: parallelizzare la somma in assembly
  */
/*float objective_function(int n,int m, MATRIX distances_from_centroids){
	   float sum = 0;
     int i;
/*
		for(int g=0;g<m;g++){		// per ogni gruppo

			for(int i=0;i<n;i++){    // per ogni punto i, aggiunge distanza(i,q(i))^2
				dist = distances_from_centroids[i*m+g];	//verificare se questo valore è aggiornato
				sum += dist;//*dist; //E' gia al quadrato la distanza restituita
			}

		}// per ogni gruppo
*/
/*
    for(i=0; i<n*m-4; i+=4){
      sum+=distances_from_centroids[i];
      sum+=distances_from_centroids[i+1];
      sum+=distances_from_centroids[i+2];
      sum+=distances_from_centroids[i+3];
    }
*/
/*    for(i=0;i<n;i++){
      sum += distances_from_centroids[i];
    }

		return sum;


		// Controllare se corretto -> sembra di si
}//objective_function


/** Funzione che calcola la distanza
  * Costo: O(d)
  * versione ottimizzata
  */
/*float distance(VECTOR x1,VECTOR x2,int d){
	float diff,sum = 0;
	int i=0;

	// Per ridurre l'overhead dovuto ad ogni passo del ciclo effettuiamo al suo
	// interno più operazioni (assumiamo che la dimensione d sia divisibile per
  // due)

	// Si lavora su gruppi di 4
	for(i; i<=d-4; i+=4){
		diff = x1[i]-x2[i];
		sum += diff*diff;
		diff = x1[i+1]-x2[i+1];
		sum += diff*diff;
		diff = x1[i+2]-x2[i+2];
		sum += diff*diff;
		diff = x1[i+3]-x2[i+3];
		sum += diff*diff;
	}


	// Processiamo un eventuale residual_centroids
<<<<<<< HEAD
	for(i;i<d;i++){
=======
/*	for(i=0;i<d;i++){
>>>>>>> bbefcb37e361f8e58feeb0208e63647fddac0bd7
		diff = x1[i]-x2[i];
		sum += diff*diff;
	}

	return sum;

	// TESTATA
}//distance


// Funzione che calcola il residuo, versione ottimizzata
// Esegue il calcolo: r(x) = x-q_c(x)
VECTOR residual(VECTOR x,VECTOR centroid,int d){
	VECTOR res = alloc_matrix(1,d);
	int i=0;

	for(i;i<=d-4;i+=4){
		res[i] = x[i]-centroid[i];
		res[i+1] = x[i+1]-centroid[i+1];
		res[i+2] = x[i+2]-centroid[i+2];
		res[i+3] = x[i+3]-centroid[i+3];
	}


	// Ciclo per eventuali cicli residui
<<<<<<< HEAD
	for(i;i<d;i++)
=======
/*	for(i=0;i<d;i++)
>>>>>>> bbefcb37e361f8e58feeb0208e63647fddac0bd7
		res[i] = x[i]-centroid[i];

	return res;

	// TESTATA: OK!
}//residual



/** Funzione che effettua la quantizzazione della query
  * x = punto di cui restituire il quantizzatore
	* k = centroidi per ciascun sottogruppo
	* m = numero totale di gruppi
	* d_star = dimensione di ogni gruppo
	* La funzione concatena i quantizzatori di ogni sottogruppo
	* Esegue: q(x) = ( q1(u1(x)), q2(u2(x)), ....)
	* COSTO: = g*k*(d_star)
	*/
int* quantize(VECTOR x, int k, int m, int d, VECTOR centroids){
	// Indice dei centroidi che quantizzano ciascuna porzione
	int* cents = (int*) malloc(m*sizeof(int));
	int centr;
	int d_star = d/m;
	float min,dist;

	for(int g=0; g<m; g++){ //per ogni sottogruppo calcolo il centroide più vicino

		centr = 0;
		//		min = distance(&x[m*d_star],&centroids[m*k*d_star],d_star);
		min = distance(&x[g*d_star],&centroids[g*k*d_star],d_star);
		//printf("Distanza tra il punto e il centroide %d del gruppo %d : %f\n", 0, g, min);

		// centroids = [riga->(i+group*k)*d_star,colonna->j]
		// dataset = [riga->number*d,colonna->d_star*g+j]

		//Considero tutti i punti del codebook del sottogruppo prendendo quello a
		//dimensione minima
		for(int i=1; i<k; i++){
		//		tmp = distance(&x[m*d_star],&centroids[(m*k+i)*d_star],d_star);
			dist = distance(&x[g*d_star],&centroids[(g*k+i)*d_star],d_star);
			//printf("Distanza tra il punto e il centroide %d del gruppo %d : %f\n", i, g, dist);
			if(dist < min){
				centr = i;
				min = dist;
			}

		}//for centroide del gruppo g
		// Abbiamo trovato il centroide del gruppo g
		cents[g] = centr;
	}//for gruppo

	return cents;
	//TESTATA
}//quantize


#endif
