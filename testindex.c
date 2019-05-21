#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>

//Necessario per poter utilizzare la struttura params definita dal prof
#include "datatypes.h"



//Matrice che ogni riga rappresenta un punto e ogni colonna il gruppo
//L'elemento m[i][j] rappresenta la distanza del punto i dal centroide più vicino del
//sottogruppo j
MATRIX distances_from_centroids;

//Per ogni punto (riga) viene indicato il centroide prodotto più vicino e la distanza da esso
//centroid_of_point[i][j] = centroide a minima distanza dap punti i del sottogruppo j
//Corrisponde alla funzione q(x) := dato il punto, restituisce l'indice del suo centroide
int* centroid_of_point;


//Codebooks dei vari sottogruppi
//Dimensione: m*k righe, d_star cioè d/m colonne
//Per andare da un sottogruppo all'altro bisgna avanzare di m righe
//Le colonne sono d_start cioè d/m
MATRIX centroids;

//Parametri ottenuti in input
params *input;

//Struttura dati che contiene le distanze tra i centroidi finali
//Triangolo superiore della matrice delle distanze che è simmetrica
VECTOR distances_between_centroids;

//Numero di elementi per sottovettore
int d_star;


int* centroids_coarse; //matrice dei centroidi del quantizzatore grossolano
                      //di dimensione nxd

//int* coarse_centroid_of_point; // matrice di dimensione nx2 che contiene
                                 // in m[i][0] il centroide del punto
																 //e in m[i][1] la posizione del punto
																 //all'interno della sottolista della lista
																 //invertita relativa al centroide

//MATRIX residuals;

//------------------------------------METODI------------------------------------

/**La funzione seguente seleziona k punti da usare come centroidi iniziali
 * COSTO: k*(d/m)*m (Per favore Fabio gli dai un'occhiata?)
 * n =  numero di punti del data set
 * d = numero di dimensioni del data/query set
 * k =  numero di centroidi di ogni sotto-quantizzatore
 * ds = data set
 * m = numero di sottoquantizzatori
 */
 void generate_centroids(int n, int d, int k, MATRIX ds, int m){
 		int d_star = d/m;

 		// Alloco la matrice dei centroidi
 		centroids = alloc_matrix(k*m,d_star);

 		int number = 0; //centroide_scelto
 		for(int g=0;g<m;g++){ //per ogni sottogruppo

 			for(int i=0;i<k;i++){
// 				printf("Gruppo %d centroide %d scelto il punto numero: %d\n", g, i, number);

 				// Inseriamo il centroide nella matrice
 				for(int j=0;j<d_star;j++)
 					//[riga->(i+group*k)*d_star,colonna->j]     [riga->number*d,colonna->d_star*g+j]
 					centroids[(i+g*k)*d_star+j] = ds[number*d+d_star*g+j];
 				number ++;
  			}//for i
				number = 0;
 		}//for g

 		print_matrix(k*m, d_star, k, centroids, 'c');

    // TESTATA
 }//generate_centroids



/** La funzione seguente cerca i punti che appartengono ad ogni centroide
	* Logicamente divide lo spazio in celle di Voronoi
	* Per ogni punto del dataset, calcola la di stanza con tutti i centroidi
	* e sceglie la distanza minima
	* COSTO: n*k* O(distance)[cioè d]
  * n =  numero di punti del data set
  * d = numero di dimensioni del data/query set
  * k =  numero di centroidi di ogni sotto-quantizzatore
	* ds = data set
	* m = numero di sottoquantizzatori
  */
void points_of_centroid(int n, int d, int k, MATRIX ds, int m){
		int d_star = d/m;

		double distanza;
		double minDist; 	//Distanza minima dal centroide
		int minCentr;			//Centroide a minima distanza
		int i,j; //i = indice per i punti, j = indice per dimensioni
		int c, g; //c = indice per centroidi, g = indice per gruppi

		for(g=0;g<m;g++){// devo considerare il codebook di ogni sottogruppo

			for(i=0;i<n;i++){	//per ogni punto del dataset devo trovare il centroide più vicino

				//Inizializzo con il primo punto e con la distanza da esso
				// centroids = [riga->(i+group*k)*d_star,colonna->j]
				// dataset = [riga->number*d,colonna->d_star*g+j]
				minDist = distance(&ds[i*d+g*d_star],&centroids[(g*k)*d_star],d_star); //(0+group*k)
				minCentr = 0;
//				printf("Distanza del punto %i dal centroide 0 del gruppo %d: %f\n", i, g, minDist);

				for(c=1;c<k;c++){ //Calcolo la distanza tra il punto attuale e ogni centroide

					distanza = distance(&ds[i*d+g*d_star],&centroids[(c+g*k)*d_star],d_star);
//					printf("Distanza del punto %i dal centroide %i del gruppo %d: %f\n", i, c, g, distanza);

					if(distanza < minDist){
						minDist = distanza;
						minCentr = c;
					}//if
				}//for ogni centroide

				//Salvo i risultati trovati
//				printf("MIN: Distanza del punto %i dal centroide minimo %i del gruppo %d: %f\n", i, minCentr, g, minDist);
				centroid_of_point[i*m+g] = minCentr;
	      distances_from_centroids[i*m+g] = minDist;

			}//for ogni punto

		}// for ogni sottogruppo

		printf("\nIndici del vettore 'centroids' indicante i centroidi più vicini al punto per sottogruppi\n");
		print_matrix_int(n,m, k, centroid_of_point, 'p');
		printf("\nDistanza AL QUADRATO del centroide più vicino ai punti per sottogruppi\n");
		print_matrix(n,m, k, distances_from_centroids, 'p');

    // TESTATA
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
void update_centroids(int n, int d, int k, MATRIX ds, int m){
		int d_star = d/m;
    int i,j;

    // Matrice temporanea per memorizzare i nuovi centroidi
    MATRIX tmp = alloc_matrix(k,d_star);
    memset(tmp, 0, k*d_star*sizeof(float));

    // Ciclo per azzerare la matrice
/*    for(i=0;i<k;i++){
      for(j=0;j<d_star;j++){
        tmp[i*d_star+j] = 0;
        //printf(" %f ", tmp[i*d_star+j] );// = 0;
      }
    }
*/
    //Vettore che, per ogni centroide, conta quanti punti appartengono alla sua cella
    int* cont = (int*) calloc(k,sizeof(int)); //usare calloc per inizializzare a 0?

		//Considero tutti i sottogruppi
		for(int g=0;g<m;g++){ //per ogni sottogruppo

			int centroide; //Centroide a cui appartiene il punto corrente

			for(int i=0;i<n;i++){ //per ogni punto del dataset

				centroide = centroid_of_point[i*m+g]; //prendo il centroide di appartenenza
				cont[centroide]++;	//conto un punto in più per questo centroide

				for(int j=0;j<d_star;j++)	//sommo tutte le cordinate di punti del centroide
					tmp[centroide*d_star+j] += ds[i*d+(g*d_star)+j];

			}//for tutti i punti

			// centroids = [riga->(i+group*k)*d_star,colonna->j]
			// dataset = [riga->number*d,colonna->d_star*g+j]

			//Divido ogni somma di cordinate per il numero di punti e lo inserisco come nuovo centroide
			for(int i=0;i<k;i++){	//per ogni centroide

				for(int j=0;j<d_star;j++)	//per ogni componente
					if( cont[i]!=0 ){
	 					centroids[(i+g*k)*d_star+j] = tmp[i*d_star+j] / (double) cont[i];
						tmp[i*d_star+j] = 0;
					}else{
						centroids[i*d+j] = -1;
					}

				if( cont[i]==0 )
						printf("###############Nessun punto appartiene al centroide %d\n", i);
				else
						cont[i] = 0;

			}//for tutti i centroidi

	  }//for tutti i sottogruppi

    dealloc_matrix(tmp);
    free(cont);

//		printf("Nuovi centroidi:\n");
//		print_matrix(k*m,d_star, k, centroids, 'c');

    // TESTATA
}//update_centroids


/** La funzione calcola e memorizza la distanza tra centroidi, le distanze
  * sono memorizzate considerando solo il triangolo inferiore della matrice
  * non salvando la parte superiore
  * COSTO:
  */
// TODO
/*void store_distances(int m, int k, MATRIX centroids){
	//Stiamo salvando le m triangolari inferiori con le distanze tra i centroidi di ogni codebook per risparmiare spazio
	distances_between_centroids = (VECTOR) malloc(m*(k*(k+1)/2)*sizeof(float));
	int i,j;

	//Effettuiamo le operazioni per ciascun gruppo
	for(int g=0;g<m;g++){
		for(i=0;i<k;i++){
			//La prima distanza è tra il centroide e se stesso e quindi è pari a 0
			distances_between_centroids[i*(i+1)/2+g*(k*(k+1)/2)] = 0;

			for(j=1;j<i;j++){
				//Calcoliamo e salviamo la distanza
				distances_between_centroids[i*(i+1)/2+g*(k*(k+1)/2)+j] = distance(&centroids[(g*k+i)*d_star], &centroids[(g*k+j)*d_star], d_star);
			}// for j

		}// for i

	}// for g

}


//-----------------------------ATTENZIONE-----------------------------
//Sistemare l'arresto del k-means chiedere al prof

/** La funzione seguente calcola il valore della funzione obiettivo
  * che deve essere minimizzata al fine avere un buon insieme di centroidi
	* C = min   sum[ dist^2(y, centroide(y) )  ]
	* COSTO: n
  * n =  numero di punti del data set
  * TODO: parallelizzare la somma in assembly
  */
double objective_function(int n,int m){
		double sum = 0, dist = 0;

		for(int g=0;g<m;g++){		// per ogni gruppo

			for(int i=0;i<n;i++){    // per ogni punto i, aggiunge distanza(i,q(i))^2
				dist = distances_from_centroids[i*m+g];	//verificare se questo valore è aggiornato
				sum += dist;//*dist; //E' gia al quadrato la distanza restituita
			}

		}// per ogni gruppo

		printf("Funzione obiettivo: %lf\n\n", sum );
		return sum;

		// Controllare se corretto -> sembra di si

}//objective_function


// ---------- RICERCA NON ESAUSIVA -----------------


// la matrice distances deve essere creata una volta sola visto che il metodo seguente deve stare dentro un ciclo
/*void compute_distances_from_centroids(MATRIX points,MATRIX centroids,MATRIX centroid_of_point, int num_points,int num_centroids, int dimensions){

	VECTOR punto = alloc_matrix(1,dimensions);	// Alloco il vettore che conterrà di volta in volta il punto
  VECTOR centroide = alloc_matrix(1,dimensions); // Alloco il vettore che conterrà di volta in volta il centroide
	double minDist,distanza;
	int minCentr;
	int i,j,l;

	for(i=0;i<num_points;i++){

		for(l=0;l<dimensions;l++){
			punto[l] = points[dimensions*i+l];	// Sto supponendo che il dataset sia memorizzato per righe
		}

		for(l=0;l<dimensions;l++){
			centroide[l] = centroids[dimensions*0+l];	// Sto supponendo che il dataset sia memorizzato per righe
		}

		minDist = distance(punto, centroide);
		minCentr = 0;

		for(j=1;j<num_centroids;j++){ //Verificare la sintassi, l'obiettivo è passare l'indirizzo di partenza
					//del punto i-esimo

			for(l=0;l<dimensions;l++){
				centroide[l] = centroids[dimensions*j+l];	// Sto supponendo che il dataset sia memorizzato per righe
			}

			distanza=distance(punto, centroide);

			if(distanza < minDist){
				minDist = distanza;
				minCentr = j;
			}

		}

		centroid_of_point[i*2] = minCentr;
		centroid_of_point[i*2+1] = minDist;
	}

	// Liberiamo lo spazio (verificare se si può utilizzare la stessa funzione della matrice)
	dealloc_matrix(punto);
	dealloc_matrix(centroide);

}
/*
void update_coarse_centroids(MATRIX points, MATRIX centroids, MATRIX centroid_of_point, int num_points, int num_centroids, int dimensions){
		// Creiamo una matrice che ci consenta di effettuare l'aggiornamento dei centroidi
		MATRIX tmp = alloc_matrix(k,d);
		VECTOR c = VECTOR malloc(k,sizeof(double));
		int i,j,centroide;

		for(i=0;i<num_points;i++){
			centroide = centroid_of_point[i*2];
			c[centroide]++;
			for(j=0;j<dimensions;j++){
				tmp[centroide*d+j] += points[i*d+j];
			}
		}

		for(i=0;i<num_centroids;i++){
			for(j=0;j<dimensions;j++){
				centroids[i*d+j] = tmp[i*d+j] / c[i];
			}
		}

		dealloc_matrix(tmp);
		free(c);
}

// genera un vettore di length numeri casuali compresi tra 0 e last_index
// diversi tra loro
void generate_random_index(int* indexes, int last_index, int length){

		int number,old_number,i,j;
    srand(time(NULL));
		old_number=rand()%(num_points/num_centroids);
		indexes[0]=old_number;

	  for(i=1;i<num_centroids;i++){

			range = (last_index-old_number)/(length-i);
			number=old_number+rand()%range+1;
			indexes[i]=number;
			old_number=number;

		}
}


// mette result_rows righe di source (che ha dimensions colonne e num_rows righe)
// nella matrice result (entrambe le matrici di float)
void extract_casual_rows(int num_rows, int result_rows, int dimensions, MATRIX result, MATRIX source){

	int* indexes = malloc(result_rows*sizeof(int))
	generate_random_index(indexes,num_rows,result_rows)

	for(i=0;i<result_rows;i++){
		for(j=0;j<dimensions;j++){
				result[i*dimensions+j]=source[indexes[i]*dimensions+j];
		}
	}

}

double sumOfDistances(MATRIX centroid_of_point){

		int i;
		double sum=0;

		for(i=0;i<n;i++){    // per ogni punto i, aggiunge distanza(i,q(i))^2
			sum+=centroid_of_point[i*2+1]*centroid_of_point[i*2+1];
		}

		return sum;

}

void calculate_coarse_quantizer(num_points,num_points_reduced,dimensions,num_centroids,MATRIX dataset,MATRIX coarse_centroids,MATRIX coarse_centroid_of_point){

		MATRIX reduced_dataset = alloc_matrix(num_points_reduced,dimensions);
		MATRIX partial_coarse_centroid_of_point = alloc_matrix(num_points_reduced,2);
		extract_casual_rows(num_points,num_centroids_reduced,dimensions,reduced_dataset,ds);
		extract_casual_rows(num_points_reduced,num_centroids,dimensions,coarse_centroids,reduced_dataset);
  	compute_distances_from_centroids(reduced_dataset,coarse_centroids,coarse_centroid_of_point,num_points_reduced,num_centroids,dimensions);
  	double sommadistanze=sumOfDistances(coarse_centroid_of_point);

  	while(sommadistanze>epsilon) {

				update_coarse_centroids(reduced_dataset,coarse_centroids,coarse_centroid_of_point,num_points_reduced,num_centroids,dimensions);
    		compute_distances_from_centroids(reduced_dataset,coarse_centroids,coarse_centroid_of_point,num_points_reduced,num_centroids,dimensions);
    		sommadist=sumOfDistances(coarse_centroid_of_point);
  	}

		coarse_centroid_of_point=alloc_matrix(num_points,2);
		compute_distances_from_centroids(dataset,coarse_centroids,coarse_centroid_of_point,num_points,num_centroids,dimensions);

}


void calculate_accurate_quantizer(int num_points, int num_centroids, int subgroups, int dimensions){

  int* residual_centroids_indexes=(int*) malloc(num_centroids*sizeof(int));
	generate_random_index(residual_centroids_indexes, num_points-1, )

	for(i=0;i<num_centroids;i++){
		for(j=0;j<subgroups;j++){
			residual_centroids[i*subgroups+j]=residuals[residual_centroids_indexes[i]*subgroups+j];
		}
	}

  points_of_centroid(num_points, dimensions,num_centroids,residuals,residual_centroids,centroid_of_residual, subgroups, d_star);

	double obiettivo = objective_function(num_points,subgroups);
	double obiettivoPrev = 0.0;

	while( (obiettivoPrev - obiettivo ) > eps || iter == 1) {
		update_centroids(num_points, dimensions,num_centroids,residuals,residual_centroids,centroid_of_residual, subgroups, d_star);
		points_of_centroid(num_points, dimensions,num_centroids,residuals,residual_centroids,centroid_of_residual, subgroups, d_star));
		obiettivoPrev = obiettivo;
		double obiettivo = objective_function(num_points,subgroups);
		iter++;

	}//while



}


//La funzione seguente realizza l'indicizzazione non esaustiva dei punti del
//dataset. Come specificato sull'articolo, il primo passo è realizzare
//un quantizzatore vettoriale dei punti del dataset. Questo sostanzialmente
//serve ad avere una prima scrematura del dataset. Per ogni punto y bisogna
// anche calcolare il residuo r(y), ovvero y-q(y). Quindi realizzare una
// struttura che per ogni centroide q(y) contenga ogni punto y che
// in forma quantizzata è q(y), accompagnato dal relativo residuo che però
// non è memorizzato esplicitamente bensì c'è un secondo quantizzatore prodotto

void non_exhaustive_indexing(MATRIX dataset, int num_points, int num_points_reduced, int num_centroids, int dimensions, int dim_star, int subgroups){

	MATRIX coarse_centroids = alloc_matrix(num_centroids,dimensions);
	MATRIX coarse_centroid_of_point = alloc_matrix(num_points,2)

 // Costruzione dei Centroidi Coarse
	calculate_coarse_quantizer(num_points,num_points_reduced,dimensions,num_centroids,dataset,coarse_centroids,coarse_centroid_of_point); // viene realizzato il quantizzatore vettoriale

	residuals=alloc_matrix(num_points,dimensions); //matrice dei residui

	for(i=0;i<num_points;i++){ // La matrice dei residui viene riempita
		for(j=0;j<dimensions;j++){
			residuals[i*dimensions+j]=dataset[i*dimensions+j]-coarse_centroids[coarse_centroid_of_point[i*2]*d + j]
		}
	}

	MATRIX residual_centroids=alloc_matrix(subgroups*num_centroids*d_star);
	distances_from_centroids=alloc_matrix(num_points*subgroups);
	int* centroid_of_residual=(int*) malloc(num_points*subgroups*sizeof(int));

	calculate_accurate_quantizer(num_points, num_centroids, subgroups, dimensions);

	store_distances(subgroups,num_centroids,residual_centroids); // vengono salvate le distanze

	int* punti_quantizzatore_coarse=(int*) calloc(num_centroids*sizeof(int)); //contiene in posizione i il numero di punti y
	                                                       //tali che qc(y)=i
	int* celle_prima= (int*) malloc(num_centroids*sizeof(int)); // contiene in posizione i il numero di celle occupate
	                                        // nella lista invertita prima che inizi la sottolista
																					// relativa al centroide grossolano i
	int* punti_caricati= (int*) calloc(num_centroids*sizeof(int)); //contiene in posizione i il numero di punti y
	                                           //tali che qc(y)=i (parte da 0 e viene calcolato man mano)
	int* lista_invertita= (int*) malloc(num_points*(subgroups+1)*sizeof(int)); // ha m + 1 celle per ogni punto i
	                                                  // quindi tra lista_invertita[celle_prima[coarse_centroid_of_point[i*2]]]
																										// e lista_invertita[celle_prima[coarse_centroid_of_point[i*2]+(m+1)*punti_quantizzatore_coarse[coarse_centroid_of_point[i*2]]]]
																										// si trovano listati, ogni (m+1) posizioni, gli m quantizzatori dei residui rispetto
																										// al quantizzatore coarse e nella cella (m+1)-esima l'indice del punto del dataset in questione
																										// questo coerentemente col fatto che si tratti di liste, non c'è una struttura che permetta di risalire
																										// dal punto alla quantizzazione del suo residuo, ma non è importante, ciò che è importante è avere per ogni
																										// centroide la lista dei punti che ne vengono quantizzati con i residui quantizzati.
	int i,sommapunti;

	for(i=0;i<num_points;i++){ // per ogni punto i, si incrementa di 1 il numero
		                // di punti del quantizzatore coarse
		punti_quantizzatore_coarse[coarse_centroid_of_point[i*2]]++;
	}

	celle_prima[0]=0;

	for(i=1;i<num_centroids;i++){ //si conta quante posizioni nella lista invertita ci sono
                    //prima che inizi la sottolista del centroide i utilizzando
										//un vettore celle_prima
		celle_prima[i]=celle_prima[i-1]+punti_quantizzatore_coarse[i-1]*subgroups;
	}


	for(i=0;i<num_points;i++){ // si vanno a riempire le sottoliste della lista lista_invertita
		                // inizio è appunto il punto del vettore da cui inizia la sottolista
										// ad
		inizio=celle_prima[coarse_centroid_of_point[i*2]];
		lista_invertita[inizio+punti_caricati[coarse_centroid_of_point[i*2]]*subgroups]=i;
		for(j=1;j<subgroups+1;j++){
			lista_invertita[inizio+punti_caricati[coarse_centroid_of_point[i*2]]*subgroups+j]=accurate_centroid_of_point[i*num_points+j];
		}
		coarse_centroid_of_point[i*2+1]=punti_caricati[coarse_centroid_of_point[i*2]];
		punti_caricati[coarse_centroid_of_point[i*2]]++; //
	}

}


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
	* m = numero di gruppi del quantizzatore prodotto
  */
void calculate_centroids(int n, int d, int k, MATRIX ds, float eps, int m){
	int iter = 1;
	int d_start = d/m;

	//Genera Centroidi casuali
	//COSTO: d+k*d
	generate_centroids(n, d, k, ds, m);

	centroid_of_point = (int*) malloc(n*m*sizeof(int));	//la matrice va aggiornata
	distances_from_centroids = (float*) malloc(n*m*sizeof(float));	//la matrice va aggiornata

	printf("############################ ITERAZIONE %d ############################\n", 0);
	//Divide lo spazio in celle di Voronoi
	//COSTO: n*k* O(distance)[cioè d] = n*k*d
	points_of_centroid(n, d, k, ds, m);

	//Verifica l'ottimalità dei Punti
	//COSTO: n
	double obiettivo = objective_function(n,m);
	double obiettivoPrev = 0.0;


//Per la formula della terminazione il prof deve aggiornare le specifiche di progetto sul sito
	while( (obiettivoPrev - obiettivo ) > eps || iter == 1) {
		printf("############################ ITERAZIONE %d ############################\n", iter);

		//Trova i nuovi centroidi facendo la media dei punti delle celle di Voronoi
		//COSTO: n*d+k*d
		update_centroids(n, d, k, ds, m);

		//Cambiati i centroidi, cambiano le celle di Voronoi;
		//determina a quale cella appartengono i Punti
		//COSTO: n*k* O(distance)[cioè d] = n*k*d
		points_of_centroid(n, d, k, ds, m);

		//Verifica l'ottimalità dei Punti
		//COSTO: n
		obiettivoPrev = obiettivo;
		obiettivo = objective_function(n,m);
		iter++;

    printf("Obiettivo vecchio: %14.2f\nObiettivo corrent: %14.2f\n", obiettivoPrev, obiettivo);
		printf("Variazione obiett: %14.2f\n", (obiettivoPrev - obiettivo) );
		printf("CONDIZIONE DEL WHILE: %d\n", (obiettivoPrev - obiettivo > eps) );
		if( obiettivoPrev - obiettivo < 0 )
			printf("La funzione obiettivo sta salendo.\n");
    else if( obiettivoPrev - obiettivo == 0 )
        printf("La funzione obiettivo è costante.\n");

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
	input->m = 2;
	input->eps = 15;

	d_star = (input->d)/(input->m);

	printf("Dataset Iniziale\n");
	print_matrix(input->n, input->d, input->n , input->ds, 'p');

//---------------------------Test singole funzioni---------------------------
/*
  generate_centroids(input->n, input->d, input->k, input->ds, input->m);
  //generate_centroids(100, 128, 10);	//Test


	float x[6] = {1, 2, 3, 8, 7, 2};
	float y[6] = {9, 8, 7, 6, 1, 3};
	printf("Distanza: %f\n", distance(x, y, 5) );

	centroid_of_point = alloc_matrix(input->n,2);	//forse la metto dentro la funzione next


	points_of_centroid(input->n, input->d, input->k, input->ds);


	objective_function(input->n, input->m);


	update_centroids(input->n, input->d, input->k, input->ds);

	points_of_centroid(input->n, input->d, input->k, input->ds);

	objective_function(input->n, input->m);


  int* q = quantize( x, input->k, input->m, d_star, centroids);
  print_quantizer(input->m, q);

  */

//---------------------------Test completo---------------------------
	calculate_centroids(input->n, input->d, input->k,
		input->ds, input->eps, input->m);

}
