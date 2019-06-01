#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <float.h>

//Necessario per poter utilizzare la struttura params definita dal prof
#include "datatypes.h"


//Parametri ottenuti in input
params *input;

//_____________________Funzioni esterne scritte in assembly_____________________
//extern float distance64(VECTOR x1, VECTOR x2, int d);
extern VECTOR test_residual(VECTOR x,VECTOR centroid,int d);
//extern float test_objective(int n,int m, MATRIX distances_from_centroids);
extern void memset_float(float* array, float val, int dim );
extern void accumulate(MATRIX dest, MATRIX source, int dim);
extern void divide( MATRIX dest, MATRIX dividendo, float divisore, int dim);


//------------------------------------METODI------------------------------------

/**La funzione seguente seleziona k punti da usare come centroidi iniziali
 * COSTO: k*(d/m)*m (Per favore Fabio gli dai un'occhiata?)
 * n =  numero di punti del data set
 * d = numero di dimensioni del data/query set
 * k =  numero di centroidi di ogni sotto-quantizzatore
 * ds = data set
 * m = numero di sottoquantizzatori
 */
 void generate_centroids(int n, int d, int k, MATRIX ds, int m, MATRIX centroids){
 		int d_star = d/m;

 		// Alloco la matrice dei centroidi
 		//centroids = alloc_matrix(k*m,d_star);

 		int number = 0; //centroide_scelto
 		for(int g=0;g<m;g++){ //per ogni sottogruppo

 			for(int i=0;i<k;i++){
// 				printf("Gruppo %d centroide %d scelto il punto numero: %d\n", g, i, number);

 				// Inseriamo il centroide nella matrice
 //				for(int j=0;j<d_star;j++)
 					//[riga->(i+group*k)*d_star,colonna->j]     [riga->number*d,colonna->d_star*g+j]
// 					centroids[(i+g*k)*d_star+j] = ds[number*d+d_star*g+j];

        memcpy( &centroids[(i+g*k)*d_star], &ds[number*d+d_star*g], d_star*sizeof(float) );

 				number ++;
  			}//for i
				number = 0;
 		}//for g

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
void points_of_centroid(int n, int d, int k, MATRIX ds, int m, MATRIX centroids, int* centroid_of_point, VECTOR distances_from_centroids){
		int d_star = d/m;

		float distanza;
		float minDist; 	//Distanza minima dal centroide
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
void update_centroids(int n, int d, int k, MATRIX ds, int m, MATRIX centroids,int* centroid_of_point){
		int d_star = d/m;
    int i,j;

    // Matrice temporanea per memorizzare i nuovi centroidi
    MATRIX tmp = alloc_matrix(k,d_star);
    memset(tmp, 0, k*d_star*sizeof(float));

    //Vettore che, per ogni centroide, conta quanti punti appartengono alla sua cella
    int* cont = (int*) calloc(k,sizeof(int)); //usare calloc per inizializzare a 0?

		//Considero tutti i sottogruppi
		for(int g=0;g<m;g++){ //per ogni sottogruppo

			int centroide; //Centroide a cui appartiene il punto corrente

			for(int i=0;i<n;i++){ //per ogni punto del dataset

				centroide = centroid_of_point[i*m+g]; //prendo il centroide di appartenenza
				cont[centroide]++;	//conto un punto in più per questo centroide

/*				for(int j=0;j<d_star;j++)	//sommo tutte le cordinate di punti del centroide
					tmp[centroide*d_star+j] += ds[i*d+(g*d_star)+j];
*/
        accumulate(&tmp[centroide*d_star], &ds[i*d+(g*d_star)], d_star);

			}//for tutti i punti

			// centroids = [riga->(i+group*k)*d_star,colonna->j]
			// dataset = [riga->number*d,colonna->d_star*g+j]

			//Divido ogni somma di cordinate per il numero di punti e lo inserisco come nuovo centroide
			for(int i=0;i<k;i++){	//per ogni centroide

        if( cont[i]!=0 ){
    //      centroids[(i+g*k)*d_star+j] = tmp[i*d_star+j] / (float) cont[i];
          divide(&centroids[(i+g*k)*d_star], &tmp[i*d_star], cont[i], d_star );

//          tmp[i*d_star+j] = 0;
          memset_float(&tmp[i*d_star], 0, d_star);
        } else
          memset_float(&centroids[i*d], -1, d_star);
//      	 centroids[i*d+j] = -1;


//        void divide( MATRIX dest, MATRIX num, int* den, ind dim);
/*				for(int j=0;j<d_star;j++)	//per ogni componente
					if( cont[i]!=0 ){
	 					centroids[(i+g*k)*d_star+j] = tmp[i*d_star+j] / (float) cont[i];
						tmp[i*d_star+j] = 0;
					}else{
						centroids[i*d+j] = -1;
					}
*/
				if( cont[i]==0 )
						printf("###############Nessun punto appartiene al centroide %d\n", i);
         else
						cont[i] = 0;

			}//for tutti i centroidi

	  }//for tutti i sottogruppi

    dealloc_matrix(tmp);
    free(cont);

    // TESTATA
}//update_centroids


/** La funzione calcola e memorizza la distanza tra centroidi, le distanze
  * sono memorizzate considerando solo il triangolo inferiore? della matrice
  * non salvando la parte superiore
  * m = numero di sottoquantizzatori
  * k =  numero di centroidi di ogni sotto-quantizzatore
  * COSTO:
  */
void store_distances(int m, int k, MATRIX centroids){
	//Stiamo salvando le m triangolari inferiori con le distanze tra i centroidi di ogni codebook per risparmiare spazio
	//distances_between_centroids = (VECTOR) malloc(m*(k*(k-1)/2)*sizeof(float));
  input->distances_between_centroids=alloc_matrix(m,(k*(k-1)/2));
  int pos = 0;
  int d_star = input->d/m;
	for(int g=0;g<m;g++){//per ciascun gruppo
		for(int i=0;i<k;i++){//per ogni centroide

    	//La prima distanza è tra il centroide e se stesso e quindi è pari a 0
//      distances_between_centroids[i*(i+1)/2+g*(k*(k+1)/2)] = 0;

			for(int j=i+1;j<k;j++){

				//Calcoliamo e salviamo la distanza

				input->distances_between_centroids[pos++] = distance(&centroids[(g*k+i)*d_star], &centroids[(g*k+j)*d_star], d_star);
      }// for j

		}// for i
	}// for gruppo

  //TESTATA
}//store_distances

float get_distance(int a, int b, int k, int g){
  if( a == b ) return 0;
  if( a >= k || b >= k ) return -1;
  int min = ((a) < (b)) ? (a) : (b); //riga della matrice
  int max = ((a) > (b)) ? (a) : (b); //colonna della matrice
  //min*(min+1)/2+g*(k*(k+1)/2)+max

  /*Spostarsi all'inizio della riga:
   *(k-1) + k-2 +  k-3  + k-4  = 4*k - 4*(4-1)/2 = 4*min - min*(min+1)/2
   *Spostarsi dentro la colonna:
   *dove voglio arrivare - da dove parto - 1 visto che la diagonale non la metto
   */
  //printf("Gruppo %d, distanza da %d a %d: %f\n",g,min,max,
  //  input->distances_between_centroids[g*k*(k-1)/2 + min*k - min*(min+1)/2 +max -min-1] );
  return input->distances_between_centroids[g*k*(k-1)/2 + min*k - min*(min+1)/2 +max -min-1];
  //TESTATO
}//get_distance


//-----------------------------ATTENZIONE-----------------------------
//Sistemare l'arresto del k-means chiedere al prof


/** La funzione seguente calcola il valore della funzione obiettivo
  * che deve essere minimizzata al fine avere un buon insieme di centroidi
	* C = min   sum[ dist^2(y, centroide(y) )  ]
	* COSTO: n
  * n =  numero di punti del data set
  * TODO: parallelizzare la somma in assembly
  */
float objective_function(int n,int m, MATRIX distances_from_centroids){
	   float sum = 0;
     int i=0;
/*
		for(int g=0;g<m;g++){		// per ogni gruppo

			for(int i=0;i<n;i++){    // per ogni punto i, aggiunge distanza(i,q(i))^2
				dist = distances_from_centroids[i*m+g];	//verificare se questo valore è aggiornato
				sum += dist;//*dist; //E' gia al quadrato la distanza restituita
			}

		}// per ogni gruppo
*/

    for(i; i<n*m-4; i+=4){
      sum+=distances_from_centroids[i];
      sum+=distances_from_centroids[i+1];
      sum+=distances_from_centroids[i+2];
      sum+=distances_from_centroids[i+3];
    }

    for(i;i<n*m;i++){
      sum += distances_from_centroids[i];
    }

		return sum;
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
void calculate_centroids(int n, int d, int k, MATRIX ds, float eps, int m,
          MATRIX centroids, int* centroid_of_point, int store){
	int iter = 1;
	int d_star = d/m;

	//Genera Centroidi casuali
	//COSTO: d+k*d

	generate_centroids(n, d, k, ds, m, centroids);
  print_matrix(k*m, d_star, k, centroids, 'c');


	MATRIX distances_from_centroids = (float*) malloc(n*m*sizeof(float));	//la matrice va aggiornata

//	printf("############################ ITERAZIONE %d ############################\n", 0);
	//Divide lo spazio in celle di Voronoi
	//COSTO: n*k* O(distance)[cioè d] = n*k*d
	points_of_centroid(n, d, k, ds, m, centroids, centroid_of_point, distances_from_centroids);
  printf("\nIndici del vettore 'centroids' indicante i centroidi più vicini al punto per sottogruppi\n");
  print_matrix_int(n,m, k, centroid_of_point, 'p');
  printf("\nDistanza AL QUADRATO del centroide più vicino ai punti per sottogruppi\n");
  print_matrix(n,m, k, distances_from_centroids, 'p');


	//Verifica l'ottimalità dei Punti
	//COSTO: n
	float obiettivo = objective_function(n,m, distances_from_centroids);
//  float obiettivoAss = test_objective(n,m, distances_from_centroids);
	float obiettivoPrev = FLT_MAX;
//  printf("Funzione obiettivo C       : %lf\n", obiettivo );
//  printf("Funzione obiettivo Assembly: %lf\n\n", obiettivoAss );

  int tmin = input->tmin;
  int tmax = input->tmax;


//Per la formula della terminazione il prof deve aggiornare le specifiche di progetto sul sito
//	while( ( (obiettivoPrev - obiettivo ) > eps || iter<=tmin) && iter<=tmax) {
	while( iter<=tmax && ( iter<=tmin || (obiettivoPrev - obiettivo ) > eps ) ){
//		printf("############################ ITERAZIONE %d ############################\n", iter);

		//Trova i nuovi centroidi facendo la media dei punti delle celle di Voronoi
		//COSTO: n*d+k*d
		update_centroids(n, d, k, ds, m, centroids, centroid_of_point);
    printf("Nuovi centroidi:\n");
    print_matrix(k*m,d_star, k, centroids, 'c');

		//Cambiati i centroidi, cambiano le celle di Voronoi;
		//determina a quale cella appartengono i Punti
		//COSTO: n*k* O(distance)[cioè d] = n*k*d
    points_of_centroid(n, d, k, ds, m, centroids, centroid_of_point, distances_from_centroids);
    printf("\nIndici del vettore 'centroids' indicante i centroidi più vicini al punto per sottogruppi\n");
    print_matrix_int(n,m, k, centroid_of_point, 'p');
    printf("\nDistanza AL QUADRATO del centroide più vicino ai punti per sottogruppi\n");
    print_matrix(n,m, k, distances_from_centroids, 'p');

		//Verifica l'ottimalità dei Punti
		//COSTO: n
		obiettivoPrev = obiettivo;
    obiettivo = objective_function(n,m, distances_from_centroids);
//    obiettivoAss = test_objective(n,m, distances_from_centroids);
		iter++;
//    printf("Funzione obiettivo C       : %lf\n", obiettivo );
//    printf("Funzione obiettivo Assembly: %lf\n\n", obiettivoAss );

/*    printf("Obiettivo vecchio: %14.2f\nObiettivo corrent: %14.2f\n", obiettivoPrev, obiettivo);
		printf("Variazione obiett: %14.2f\n", (obiettivoPrev - obiettivo) );
//		printf("CONDIZIONE DEL WHILE: %d\n", (obiettivoPrev - obiettivo > eps) );
		if( obiettivoPrev - obiettivo < 0 )
			printf("La funzione obiettivo sta salendo.\n");
    else if( obiettivoPrev - obiettivo == 0 )
        printf("La funzione obiettivo è costante.\n");
*/
  }//while

	free(distances_from_centroids);

  printf("\nCentroidi ottimi finali:\n");
	print_matrix(k*m,d_star, k, centroids, 'c');

  printf("\nIndici del vettore 'centroids' indicante i centroidi più vicini al punto per sottogruppi\n");
  print_matrix_int(n,m, k, centroid_of_point, 'p');

  if(store==1){
    store_distances(m,k, centroids);
    printf("\nDistanze tra centroidi AL QUADRATO:\n");
    print_centroids_distances(k, m, input->distances_between_centroids);
  }

}//calculate_centroids




// ---------- RICERCA NON ESAUSIVA -----------------


// La funzione seguente realizza l'indicizzazione non esaustiva dei punti del
// dataset. Come specificato sull'articolo, il primo passo è realizzare
// un quantizzatore vettoriale ottenuto da un sottogruppo dei punti del dataset.
// Questo sostanzialmente serve ad avere una prima scrematura del dataset.
// Per ogni punto y bisogna anche calcolare il residuo r(y), ovvero y-q(y).
// Quindi realizzare una struttura che per ogni centroide q(y) contenga ogni
// punto y che in forma quantizzata è q(y), accompagnato dal relativo residuo
// che però non è memorizzato esplicitamente bensì c'è un secondo quantizzatore
// prodotto
/*
 * ds = data set
 * coarse_centroids = matrice kc*d che memorizza i kc centroidi coarse
 * residuals =  matrice n*d che memorizza i residui di ciascun punto del dataset
 * centroids = matrice (m*k)*d che memirizza i codebook (grupppi di centroidi)
 *             dei quantizzatori prodotto
 * coarse_centroid_of_point = vettore n*sizeof(int) che memorizza il centroide
 *                            coarse di appartenenza di ciascun punto del
 *                            dataset
 * celle_prima = vettore kc*sizeof(int) che indica quante celle di memoria
 *               precedono la sezione relativa a quel centroide coarse nella
 *               lista invertita
 * punti_caricati = vettore kc*sizeof(int) che in ciascuna cella riporta il
 *                  numero di punti caricati per quel centroide
 * lista_invertita = matrice (m+1)*n che memorizza i residui di ciascun punto e
 *                   il punto di appartenenza, i residui dei punti ottenuti
 *                   dallo stesso quantizzatore prodotto sono memorizzati vicini
 *                  (vedi anche celle_prima)
 * centroid_of_point = matrice n*m che memorizza la quantizzazione prodotto dei
 *                     punti del dataset
 * n = numero di punti del data set
 * n_r = dimensione del campione dei residui nel caso di ricerca non esaustiva
 * k = numero di centroidi di ogni sotto-quantizzatore
 * kc = numero di centroidi del quantizzatore coarse
 * d = numero di dimensioni del data/query set
 * m = numero di gruppi del quantizzatore prodotto
 */
void non_exhaustive_indexing(MATRIX ds, MATRIX coarse_centroids,
                             MATRIX residuals, MATRIX centroids,
                             int* coarse_centroid_of_point,
                             int* celle_prima, int* punti_caricati, int* lista_invertita,
                             int* centroid_of_point,
                             int n, int n_r, int k, int kc, int d, int eps, int m){

  MATRIX distances_from_centroids;
  int d_star = d/m;

  // I centroidi vengono CALCOLATI su n_r punti del Dataset
  // Ora è necessario calcolare il centroide di tutti i punti restanti



  // ponendo m=1 si fa degenerare il quantizzatore prodotto
  // in un quantizzatore coarse
//  printf("\nCalcolo centroidi coarse: \n");

  // Si avvia l'algoritmo per il calcolo dei cenroidi coarse, viene passato n_r
  // perche nella ricerca non esaustiva i centroidi si calcolo su un sottoinsieme
  // dei punti del dataset (si prenderanno i primi n_r)
  calculate_centroids(n_r, d, kc, ds, eps, 1,
            coarse_centroids,coarse_centroid_of_point, 0);

  // Distances from centroids è una matrice che non serve a niente in realtà
  // se non a poter eseguire points_of_centroids (verrà infatti liberata subito dopo)
  distances_from_centroids=alloc_matrix(n,1);

  // Ora che abbiamo ottenuto i centroidi dobbiamo calcolare i centroidi coarse
  // di apartenenza per i punti che non erano stati considerati
  points_of_centroid(n, d, kc, ds, 1, coarse_centroids, coarse_centroid_of_point, distances_from_centroids);

//  printf("\nDistanza AL QUADRATO del centroide COARSE più vicino ai punti (m=1)\n");
//  print_matrix(n,1,1,distances_from_centroids,'p');
//  printf("\nIndici del vettore 'centroids' indicante i centroidi COARSE più vicini al punto (m=1) \n");
//  print_matrix_int(n,1,1,coarse_centroid_of_point,'p');
//  printf("Centroidi COARSE:\n");
//  print_matrix(kc,d,d,coarse_centroids,'c');

  // Deallochiamo la matrice perchè non ci serve più
  dealloc_matrix(distances_from_centroids);


// La matrice dei residui viene riempita
  for(int i=0;i<n;i++)
	   for(int j=0;j<d;j++)
		     residuals[i*d+j]=ds[i*d+j]-coarse_centroids[coarse_centroid_of_point[i]*d + j];


//  printf("\nResidui: \n");
//  print_matrix(n,d,d,residuals,'p');


//  printf("\n---------Calcolo centroidi accurati---------\n");

  // Andiamo adesso a calcolare i centroidi accurati
  calculate_centroids(n_r, d, k, residuals, eps, m,
      centroids,centroid_of_point, input->symmetric);

  // Anche in questo caso bisogna calcolare i quantizzatori di tutti i punti
  // rimasti fuori.
  distances_from_centroids=alloc_matrix(n,m);
  points_of_centroid(n, d, k, residuals,m, centroids, centroid_of_point, distances_from_centroids);

//  printf("\nDistanza AL QUADRATO del centroide ACCURATO più vicino ai punti (m=1)\n");
//  print_matrix(n,1,1,distances_from_centroids,'p');
//  printf("\nIndici del vettore 'centroids' indicante i centroidi ACCURATI più vicini al punto (m=1) \n");
//  print_matrix_int(n,1,1,centroid_of_point,'p');
//  printf("Centroidi ACCURATI:\n");
//  print_matrix(k*m,d_star,m,centroids,'c');

  // Deallochiamo distances_from_centroids che non ci serve più, serve solo come
  // struttura di supporto per trovare i centroidi di ciascun punto
  dealloc_matrix(distances_from_centroids);

  // Costruzione lista invertita

  // vettore che contiene in posizione i il numero di punti y tali che qc(y)=i
  // struttura di supporto per la costruzione della lista lista_invertita
  int* punti_quantizzatore_coarse=(int*) calloc(kc,sizeof(int));
  int sommapunti,inizio;

  // per ogni punto i, si incrementa di 1 il numero di punti del quantizzatore
  // coarse a cui appartiene
  for(int i=0;i<n;i++){
  	punti_quantizzatore_coarse[coarse_centroid_of_point[i]]++;
  }

  // Siccome è il primo quantizzatore coarse, la sua sezione si troverà all'inizio
  celle_prima[0]=0;

  //si conta quante posizioni nella lista invertita ci sono prima che inizi la
  // sottolista del centroide i utilizzando il vettore celle_prima
  for(int i=1;i<kc;i++){
	  celle_prima[i]=celle_prima[i-1]+punti_quantizzatore_coarse[i-1]*(m+1);
  }

  // si vanno a riempire le sottoliste della lista lista_invertita inizio è appunto
  // il punto del vettore da cui inizia la sottolista relativa al centroide coarse
  // considerato
  for(int i=0;i<n;i++){
	  	inizio=celle_prima[coarse_centroid_of_point[i]];

      // All'inizio si ha il punto del dataset, a seguire la quantizzazione dei suoi residui
	  	lista_invertita[inizio+punti_caricati[coarse_centroid_of_point[i]]*(m+1)]=i;

      // Andiamo a caricare la quantizzazione
 		  for(int j=1;j<m+1;j++){
		    	lista_invertita[inizio+punti_caricati[coarse_centroid_of_point[i]]*(m+1)+j]=centroid_of_point[i*m+j-1]; //il -1 in centroid_of_residual[..] serve a compensare il fatto che j parta da 1 (ma io in realtà devo partire da 0 per fare questo calcolo)
		    }

  		//centroid_of_point[i*2+1]=punti_caricati[centroid_of_point[i*2]];
	   	punti_caricati[coarse_centroid_of_point[i]]++; //
  }

  // Deallochiamo questa struttura perchè non ci serve più, andessola memorizzazione
  // è mantenuta nella lista_invertita
  free(punti_quantizzatore_coarse);

}


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

/*
//Prendo un sottoinsieme del dataset
	input->n = 100;
	input->d = 12;
	input->k = 8; //2
	input->m = 2;
	input->eps = 10;
/*
	for(int i=0; i<input->n; i++)
		for( int j=0; j<input->d; j++)
 			input->ds[i*input->d+j] = i+j*2.5+rand()%20;

*/


  if((input->kc)>(input->nr)){
    input->kc = input->nr/16;
  }

	int d_star = (input->d)/(input->m);
/*
	printf("Dataset Iniziale\n");
	print_matrix(input->n, input->d, input->n , input->ds, 'p');
*/
//---------------------------Test singole funzioni---------------------------
/*
  input->centroids=alloc_matrix(m*k,d_star);
  input->centroid_of_point=(int*) malloc(n*m*sizeof(int));

  generate_centroids(input->n, input->d, input->k, input->ds, input->m, input->centroids);

	int* centroid_of_point = alloc_matrix(input->n,2);	//forse la metto dentro la funzione next


  float* distances_from_centroids = (float*) malloc(input->m*input->k*sizeof(float));	//la matrice va aggiornata
  points_of_centroid(input->n, input->d, input->k, input->ds, input->centroids, centroid_of_point, distances_from_centroids);


	objective_function(input->n, input->m, distances_from_centroids);


	update_centroids(input->n, input->d, input->k, input->ds);

	points_of_centroid(input->n, input->d, input->k, input->ds);

	objective_function(input->n, input->m);


  int* q = quantize( x, input->k, input->m, d_star, centroids);
  print_quantizer(input->m, q);

  */
/*
//---------------------------Test Assembly---------------------------
  VECTOR x = alloc_matrix(1, 37);
  VECTOR y = alloc_matrix(1, 37);
  for(int i=0; i<37; i++){
    x[i] = rand()%20;
    y[i] = rand()%20;
  }

//  printf("Vettore X       : \n");
//  print_matrix(1, 47, 1, x, 'p');
//  printf("Vettore Y       : \n");
//  print_matrix(1, 47, 1, y, 'p');

  for( int i=1; i<47; i++){
    printf("Distanza Assembly: %f\n", distance64(x, y, i) );
    printf("Distanza C       : %f\n\n", distance(x, y, i) );
    if( distance64(x, y, i) !=  distance(x, y, i) ) printf("ERRORE\n");
   }
/*
   VECTOR res = residual(x, y, 23);
   printf("Residual C       : \n");
   print_matrix(1, 23, 1, res, 'p');

   VECTOR res2 = test_residual(x, y, 23);
   printf("Residual Assembly: \n");
   print_matrix(1, 23, 1, res2, 'p');
*/
/*
  MATRIX tmp = alloc_matrix(1,47);
  memset(tmp, 0, 47*sizeof(float));

  printf("TEMP AZZERATA:\n" );
  print_matrix(1, 47, 1, tmp, 'p');

  printf("TEMP ACCUMULATA:\n" );
  //accumulate(tmp, x, 37);
//  divide(tmp, x, 2, 37);
  memset_float64(tmp, 87.25, 47);
  print_matrix(1, 47, 1, tmp, 'p');
//  for (int i = 0; i < 37; i++)
//    printf("%f\t", tmp[i]);


//  printf("TEMP ACCUMULATA2:\n" );
//  accumulate(tmp, y, 37);
//  print_matrix(1, 37, 1, tmp, 'p');

  printf("ok\n");

*/



//---------------------------Test completo---------------------------
//_______________________Setting parametri input_____________________
  int n = input->n;
  int d = input->d;
  int k = input->k;
  int m = input->m;
//  int d_star = d/m;

//  input->nr = 12;
//  input->kc = 2;

  int nr = input->nr;
  int kc = input->kc;

//  input->exaustive = 0;
//  input->symmetric = 0;

  printf("Esaustiva: %d, Simmetrica: %d\n", input->exaustive, input->symmetric);
  input->centroids=alloc_matrix(m*k,d_star);
  input->centroid_of_point=(int*) malloc(n*m*sizeof(int));


//_______________________Starting Algorithms_____________________

  if(input->exaustive==1){
    calculate_centroids(n, d, k, input->ds, input->eps, m, input->centroids,
                        input->centroid_of_point, input->symmetric);
  } else {
    input->coarse_centroids = alloc_matrix(kc,d);
    input->coarse_centroid_of_point = (int*) malloc(n*sizeof(int));
    input->residuals=alloc_matrix(n,d);
    input->celle_prima=(int*) calloc(kc,sizeof(int));
    input->punti_caricati=(int*) calloc(kc,sizeof(int));
    input->lista_invertita=(int*) malloc(n*(m+1)*sizeof(int));

    non_exhaustive_indexing(input->ds, input->coarse_centroids,
                            input->residuals, input->centroids, input->coarse_centroid_of_point,
                            input->celle_prima, input->punti_caricati, input->lista_invertita,
                            input->centroid_of_point,
                            n, nr, k, kc, d, input->eps, m);

  }

/*
  int d = 47;
  VECTOR test = malloc(d*sizeof(float) );
  print_matrix(1, d, 1, test, 'p');
  memset_float(test, 5.1, d);
  print_matrix(1, d, 1, test, 'p');
//    get_distance(2, 3, input->k, 0);
  VECTOR test2 = malloc(d*sizeof(float) );
    print_matrix(1, d, 1, test2, 'p');
    memcpy(test2, test, d*sizeof(float) );
  print_matrix(1, d, 1, test2, 'p');
*/
}

/**ATTEZIONE:
  ho notato che sia objective.nasm che distance.nasm introducono un ERRORE
  e quindi un dato diverso da C man mano che la dimensione dei risultati
  aumenta. Più valori sommo, più aumenta la differenza tra C e Assembly.
  I KNN finali sono cmq invariati tra le due implementazioni, per ora
  Attualmente, con le funzione esterne incluse, non c'è alcuna differenza.

  Testate "accumulate" e "divide" che implementano loop unrolling per
  il for j=0...d_star
  Queste due funzioni non generano alcun errore di calcolo e riducono
  i tempi di calcolo di qualche centinaia di millisecondi

  Trovare altri punti in cui è possibile inserire queste due funzioni
*/
