#ifndef datatypes
#define datatypes
//Siccome pqnn e testindex usano questo file, senza queste righe, l'header viene
//definito più volte

#include <xmmintrin.h> //Per usare _mm_malloc


//#define	MATRIX  float*
//#define	VECTOR  float*


//----------Stampa tutti i punti-----------
void print_matrix(int rows, int cols, int k, MATRIX data, char c){
  printf("Numero punti: %d\nDimensione di ogni punto: %d\n", rows, cols);
  for (int i = 0; i < rows; i++) {
		if( c=='c'){
			if( i%k == 0) printf("Gruppo %d\n", i/k);
    	printf("Centroide n %d:\t", i%k);
		}else if( c=='p')
//			printf("Punto n %d:\t", i);

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
float objective_function(int n,int m, MATRIX distances_from_centroids){
	   float sum = 0;
     int i;

		for(int g=0;g<m;g++){		// per ogni gruppo

			for(int i=0;i<n;i++)   // per ogni punto i, aggiunge distanza(i,q(i))^2
				sum += distances_from_centroids[i*m+g];	//verificare se questo valore è aggiornato
		}// per ogni gruppo

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
*/
		return sqrt(sum);


		// Controllare se corretto -> sembra di si
}//objective_function


/** Funzione che calcola la distanza
  * Costo: O(d)
  * versione ottimizzata
  */
float distance(VECTOR x1,VECTOR x2,int d){
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
	for(i;i<d;i++){
		diff = x1[i]-x2[i];
		sum += diff*diff;
	}

	return sum;

	// TESTATA
}//distance


// Funzione che calcola il residuo, versione ottimizzata
// Esegue il calcolo: r(x) = x-q_c(x)
VECTOR residual(VECTOR x,VECTOR centroid,int d){
	//VECTOR res = alloc_matrix(1,d);
	VECTOR res = (VECTOR) _mm_malloc(1*d*sizeof(float),16);

	int i=0;

	for(i;i<=d-4;i+=4){
		res[i] = x[i]-centroid[i];
		res[i+1] = x[i+1]-centroid[i+1];
		res[i+2] = x[i+2]-centroid[i+2];
		res[i+3] = x[i+3]-centroid[i+3];
	}

	// Ciclo per eventuali cicli residui
	for(i;i<d;i++)
		res[i] = x[i]-centroid[i];
	return res;

	// TESTATA: OK!
}//residual


void memset_float( MATRIX data, float val, int dim){
	for( int j=0; j<dim; j++)
		data[j] = val;
}

void accumulate( MATRIX dest, MATRIX orig, int dim){
	for(int j=0;j<dim;j++)
		dest[j] += orig[j];
}

void divide( MATRIX dest, MATRIX dividendo, float divisore, int dim){
	for(int j=0;j<dim;j++)
		dest[j] = dividendo[j] / divisore;
}



#endif
