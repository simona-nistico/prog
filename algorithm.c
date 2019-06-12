#include "pqnn64c.c"
//La riga sopra viene cambiata in automatico a seconda del run che si lancia
#include "utils.h"

//#define min(a, b) (((a) < (b)) ? (a) : (b))
//#define max(a, b) (((a) > (b)) ? (a) : (b))


/**La funzione seguente seleziona k punti da usare come centroidi iniziali
 * COSTO: k*(d/m)*m (Per favore Fabio gli dai un'occhiata?)
 * n =  numero di punti del data set
 * d = numero di dimensioni del data/query set
 * k =  numero di centroidi di ogni sotto-quantizzatore
 * ds = data set
 * m = numero di sottoquantizzatori
 */
 void generate_centroids(int n, int d, int k, MATRIX ds, int m, MATRIX centroids,
                          MATRIX distances_from_centroids, int* centroid_of_point){

   float distanza;
   float minDist; 	//Distanza minima dal centroide
   int minCentr;			//Centroide a minima distanza
   int i, y;
   int c, g;

   int d_star = d/m;

   //Vettore che, per ogni centroide, conta quanti punti appartengono alla sua cella
   //MATRIX tmp = alloc_matrix(k*m,d_star);
   //int* cont = (int*) calloc(k*m,sizeof(int)); //usare calloc per inizializzare a 0?

  	for(y=0;y<k*d;y+=d_star){

       //i = y/d;
       //g = (y%d)/d_star;

  			// Inseriamo il centroide nella matrice
 			 memcpy( &centroids[y/d*d_star+(y%d)*k], &ds[y], d_star*sizeof(float) );
       // OK!

       distances_from_centroids[y/d_star] = 0;
       centroid_of_point[y/d_star] = y/d;
       // OK!

  	}// settaggio centroidi

   for(y;y<n*d;y+=d_star){	//per ogni punto del dataset devo trovare il centroide più vicino

      i = y/d;
      g = (y%d)/d_star;

     //Inizializzo con il primo punto e con la distanza da esso
     // centroids = [riga->(i+group*k)*d_star,colonna->j]
     // dataset = [riga->number*d,colonna->d_star*g+j]
     minDist = distance(&ds[y],&centroids[g*k*d_star],d_star); //(0+group*k)
     minCentr = 0;

     for(c=1;c<k;c++){ //Calcolo la distanza tra il punto attuale e ogni centroide

       distanza = distance(&ds[y],&centroids[(g*k+c)*d_star],d_star);
 //			printf("Distanza del punto %i dal centroide %i del gruppo %d: %f\n", i, c, g, distanza);

       if(distanza < minDist){
         minDist = distanza;
         minCentr = c;
       }//if
     }//for ogni centroide

       //Salvo i risultati trovati
 //				printf("MIN: Distanza del punto %i dal centroide minimo %i del gruppo %d: %f\n", i, minCentr, g, minDist);
     centroid_of_point[i*m+g] = minCentr;
     distances_from_centroids[i*m+g] = minDist;

     //print_matrix(k*m,d_star,m,tmp,'c');

   }//for ogni punto

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

    for(int y=0;y<n*d;y+=d_star){	//per ogni punto del dataset devo trovare il centroide più vicino

       i = y/d;
       g = (y%d)/d_star;

 			//Inizializzo con il primo punto e con la distanza da esso
 			// centroids = [riga->(i+group*k)*d_star,colonna->j]
 			// dataset = [riga->number*d,colonna->d_star*g+j]
 			minDist = distance(&ds[y],&centroids[g*k*d_star],d_star); //(0+group*k)
 			minCentr = 0;

 			for(c=1;c<k;c++){ //Calcolo la distanza tra il punto attuale e ogni centroide

 				distanza = distance(&ds[y],&centroids[(g*k+c)*d_star],d_star);
 //			printf("Distanza del punto %i dal centroide %i del gruppo %d: %f\n", i, c, g, distanza);

 				if(distanza < minDist){
 					minDist = distanza;
 					minCentr = c;
 				}//if
 			}//for ogni centroide

 				//Salvo i risultati trovati
 //				printf("MIN: Distanza del punto %i dal centroide minimo %i del gruppo %d: %f\n", i, minCentr, g, minDist);
 			centroid_of_point[i*m+g] = minCentr;
 	    distances_from_centroids[i*m+g] = minDist;

      //print_matrix(k*m,d_star,m,tmp,'c');

 		}//for ogni punto

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
void update_centroids(int n, int d, int k, MATRIX ds, int m, MATRIX centroids, int* centroid_of_point){
		int d_star = d/m;
    int i,j,centroide;

    // Matrice temporanea per memorizzare i nuovi centroidi
    //MATRIX tmp = alloc_matrix(k*m,d_star);
    memset_float(centroids, 0, k*m*d_star);

    //Vettore che, per ogni centroide, conta quanti punti appartengono alla sua cella
    int* cont = (int*) calloc(k*m,sizeof(int)); //usare calloc per inizializzare a 0?

    for(int y=0;y<n*m;y++){	//per ogni punto del dataset devo trovare il centroide più vicino

       centroide = centroid_of_point[y]; //prendo il centroide di appartenenza
       cont[(y%m)*k+centroide]++;	//conto un punto in più per questo centroide

       //printf("%d\n", cont[(y%m)*k+centroide]);

       //print_matrix(1,d_star,d_star,&centroids[((y%m)*k+centroide)],'p');

       accumulate(&centroids[((y%m)*k+centroide)*d_star], &ds[y*d_star], d_star);

       //print_matrix(1,d_star,d_star,&centroids[((y%m)*k+centroide)],'p');

 		}//for ogni punto

    //print_matrix(m*k,d_star,d_star,tmp,'p');


		//Divido ogni somma di cordinate per il numero di punti e lo inserisco come nuovo centroide
		for(int i=0;i<k*m;i++){	//per ogni centroide

      if( cont[i]!=0 ){
				divide(&centroids[i*d_star], cont[i], d_star);
				//memset(&tmp[i*d_star], 0, d_star*sizeof(float));
      } //else
					//printf("\n######## Nessun punto appartiene al centroide #########");

	  }//for tutti i sottogruppi

    //dealloc_matrix(tmp);
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
void store_distances(params* input){

	//Stiamo salvando le m triangolari inferiori con le distanze tra i centroidi di ogni codebook per risparmiare spazio
	//distances_between_centroids = (VECTOR) malloc(m*(k*(k-1)/2)*sizeof(float));

  MATRIX centroids = input->centroids;
  int d = input->d;
  int m = input->m;
  int k = input->k;


  int i, g, j, p;
  int d_star = d/m;

  input->distances_between_centroids=alloc_matrix(m*k,k);
  MATRIX distances_between_centroids = input->distances_between_centroids;

	//print_matrix(k*m,d_star,m,centroids,'c');

	for(g=0;g<m;g++){//per ciascun gruppo
    for(i=0;i<k;i++){
      distances_between_centroids[(g*k+i)*k+i] = 0;
		  for(int j=i+1;j<k;j++){
			   //Calcoliamo e salviamo la distanza
			  distances_between_centroids[(g*k+i)*k+j] = distance(&centroids[(g*k+i)*d_star], &centroids[(g*k+j)*d_star], d_star);
        distances_between_centroids[(g*k+j)*k+i] = distances_between_centroids[(g*k+i)*k+j];
      }// for j
    }
	}// for p

  //TESTATA
}//store_distances

/** La funzione ci consente di accedere facilmente alla struttura dati che contiene
	* le distanze tra i centroidi
	* a = numero del primo centroide
	* b = numero del secondo centroide
	* g = numero del gruppo
	*/
/*
float get_distance(int a, int b, int k, int g){
  if( a == b ) return 0;
  if( a >= k || b >= k ) return -1;
  int min = ((a) < (b)) ? (a) : (b); //riga della matrice
  int max = ((a) > (b)) ? (a) : (b); //colonna della matrice
  //min*(min+1)/2+g*(k*(k+1)/2)+max
  //Spostarsi all'inizio della riga:
  //(k-1) + k-2 +  k-3  + k-4  = 4*k - 4*(4-1)/2 = 4*min - min*(min+1)/2
  //Spostarsi dentro la colonna:
  //dove voglio arrivare - da dove parto - 1 visto che la diagonale non la metto
  //printf("Gruppo %d, distanza da %d a %d: %f\n",g,min,max,
  //  input->distances_between_centroids[g*k*(k-1)/2 + min*k - min*(min+1)/2 +max -min-1] );
  return input->distances_between_centroids[g*k*(k-1)/2 + min*k - min*(min+1)/2 +max -min-1];
  //TESTATO
}//get_distance
*/


//----------------------------------------------------------------------------

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
void calculate_centroids(int n, int d, int k, MATRIX ds, float eps, int m, MATRIX centroids,
                         int* centroid_of_point, int tmin, int tmax){

  int iter = 1;
	int d_star = d/m;

	//Genera Centroidi casuali
	//COSTO: d+k*d

  MATRIX distances_from_centroids = alloc_matrix(n,m);	//la matrice va aggiornata

	generate_centroids(n, d, k, ds, m, centroids, distances_from_centroids, centroid_of_point);
//  print_matrix(k*m, d_star, k, centroids, 'c');


//	printf("############################ ITERAZIONE %d ############################\n", 0);
	//Divide lo spazio in celle di Voronoi
//	//COSTO: n*k* O(distance)[cioè d] = n*k*d
	//points_of_centroid(n, d, k, ds, m, centroids, centroid_of_point, distances_from_centroids);
//  printf("\nIndici del vettore 'centroids' indicante i centroidi più vicini al punto per sottogruppi\n");
//  print_matrix_int(n,m, k, centroid_of_point, 'p');
//  printf("\nDistanza AL QUADRATO del centroide più vicino ai punti per sottogruppi\n");
//  print_matrix(n,m, k, distances_from_centroids, 'p');

	//Verifica l'ottimalità dei Punti
	//COSTO: n
	float obiettivo = objective_function(n,m, distances_from_centroids);
	float obiettivoPrev = FLT_MAX;
  //printf("Funzione obiettivo: %lf\n", obiettivo );

//Per la formula della terminazione il prof deve aggiornare le specifiche di progetto sul sito
//	while( ( (obiettivoPrev - obiettivo ) > eps || iter<=tmin) && iter<=tmax) {
	while( iter<=tmax && ( iter<=tmin || abs(obiettivoPrev - obiettivo )/obiettivoPrev > eps ) ){
		//printf("############################ ITERAZIONE %d ############################\n", iter);

		//Trova i nuovi centroidi facendo la media dei punti delle celle di Voronoi
		//COSTO: n*d+k*d
		update_centroids(n, d, k, ds, m, centroids, centroid_of_point);

//    printf("Nuovi centroidi:\n");
//    print_matrix(k*m,d_star, k, centroids, 'c');

		//Cambiati i centroidi, cambiano le celle di Voronoi;
		//determina a quale cella appartengono i Punti
		//COSTO: n*k* O(distance)[cioè d] = n*k*d
    points_of_centroid(n, d, k, ds, m, centroids, centroid_of_point, distances_from_centroids);
//    printf("\nIndici del vettore 'centroids' indicante i centroidi più vicini al punto per sottogruppi\n");
//    print_matrix_int(n,m, k, centroid_of_point, 'p');
//    printf("\nDistanza AL QUADRATO del centroide più vicino ai punti per sottogruppi\n");
//    print_matrix(n,m, k, distances_from_centroids, 'p');



		//Verifica l'ottimalità dei Punti
		//COSTO: n
		obiettivoPrev = obiettivo;
    obiettivo = objective_function(n,m, distances_from_centroids);
		iter++;
    //printf("Funzione obiettivo: %lf\n", obiettivo );

//    printf("Obiettivo vecchio: %14.2f\nObiettivo corrent: %14.2f\n", obiettivoPrev, obiettivo);
//		printf("Variazione obiett: %14.2f\n", (obiettivoPrev - obiettivo) );
  }//while

	dealloc_matrix(distances_from_centroids);

//  printf("\nCentroidi ottimi finali:\n");
//	print_matrix(k*m,d_star, k, centroids, 'c');

//  printf("\nIndici del vettore 'centroids' indicante i centroidi più vicini al punto per sottogruppi\n");
//  print_matrix_int(n,m, k, centroid_of_point, 'p');


}//calculate_centroids

/** Funzione che effettua la quantizzazione della query
  * cents = vettore in cui sarà memorizzata la quantizzazione
  * x = punto di cui restituire il quantizzatore
	* k = centroidi per ciascun sottogruppo
	* m = numero totale di gruppi
	* d_star = dimensione di ogni gruppo
	* La funzione concatena i quantizzatori di ogni sottogruppo
	* Esegue: q(x) = ( q1(u1(x)), q2(u2(x)), ....)
	* COSTO: = g*k*(d_star)
	*/
void quantize(int* cents, MATRIX x, int k, int m, int d, VECTOR centroids, int points){

  int d_star = d/m;

  float distanza;
  float minDist; 	//Distanza minima dal centroide
  int minCentr;			//Centroide a minima distanza
  int i,j; //i = indice per i punti, j = indice per dimensioni
  int c,g; //c = indice per centroidi, g = indice per gruppi

  for(int y=0;y<points*d;y+=d_star){	//per ogni punto del dataset devo trovare il centroide più vicino

    i = y/d;
    g = (y%d)/d_star;

 		//Inizializzo con il primo punto e con la distanza da esso
 		// centroids = [riga->(i+group*k)*d_star,colonna->j]
 		// dataset = [riga->number*d,colonna->d_star*g+j]
 		minDist = distance(&x[y],&centroids[g*k*d_star],d_star); //(0+group*k)
 		minCentr = 0;

 		for(c=1;c<k;c++){ //Calcolo la distanza tra il punto attuale e ogni centroide

 			distanza = distance(&x[y],&centroids[(g*k+c)*d_star],d_star);
 //		printf("Distanza del punto %i dal centroide %i del gruppo %d: %f\n", i, c, g, distanza);

 			if(distanza < minDist){
 				minDist = distanza;
 				minCentr = c;
 			}//if
		}//for ogni centroide

 			//Salvo i risultati trovati
 //			printf("MIN: Distanza del punto %i dal centroide minimo %i del gruppo %d: %f\n", i, minCentr, g, minDist);
 		cents[i*m+g] = minCentr;
    //print_matrix(k*m,d_star,m,tmp,'c');

 	}//for ogni punto

	//TESTATA
}//quantize

// ------------------------- RICERCA NON ESAUSIVA --------------------------


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
void non_exhaustive_indexing(params* input){

  MATRIX ds = input->ds;
  int n = input->n;
  int nr = input->nr;
  int k = input->k;
  int kc = input->kc;
  int d = input->d;
  float eps = input->eps;
  int m = input->m;
  int tmin = input->tmin;
  int tmax = input->tmax;
  int d_star = d/m;
  int i;

  // I centroidi vengono CALCOLATI su n_r punti del Dataset
  // Ora è necessario calcolare il centroide di tutti i punti restanti

  // ponendo m=1 si fa degenerare il quantizzatore prodotto
  // in un quantizzatore coarse
 //  printf("\nCalcolo centroidi coarse: \n");

  input->coarse_centroid_of_point = (int*) malloc(n*sizeof(int));
	input->coarse_centroids = alloc_matrix(kc,d);
  int* coarse_centroid_of_point = input->coarse_centroid_of_point;
  MATRIX coarse_centroids = input->coarse_centroids;

  // Si avvia l'algoritmo per il calcolo dei cenroidi coarse, viene passato n_r
  // perche nella ricerca non esaustiva i centroidi si calcolo su un sottoinsieme
  // dei punti del dataset (si prenderanno i primi n_r)
  calculate_centroids(nr, d, kc, ds, eps, 1, coarse_centroids, coarse_centroid_of_point,
                      tmin, tmax);

  // Distances from centroids è una matrice che non serve a niente in realtà
  // se non a poter eseguire points_of_centroids (verrà infatti liberata subito dopo)

  // Ora che abbiamo ottenuto i centroidi dobbiamo calcolare i centroidi coarse
  // di apartenenza per i punti che non erano stati considerati
  //for(i=nr;i<n;i++){
    quantize(&coarse_centroid_of_point[nr], &ds[nr*d], kc, 1, d, coarse_centroids, (n-nr));
  //}

//  printf("\nDistanza AL QUADRATO del centroide COARSE più vicino ai punti (m=1)\n");
//  print_matrix(n,1,1,distances_from_centroids,'p');
//  printf("\nIndici del vettore 'centroids' indicante i centroidi COARSE più vicini al punto (m=1) \n");
//  print_matrix_int(n,1,1,coarse_centroid_of_point,'p');
//  printf("Centroidi COARSE:\n");
//  print_matrix(kc,d,d,coarse_centroids,'c');

  // Deallochiamo la matrice perchè non ci serve più
  MATRIX residuals = alloc_matrix(n,d);


// La matrice dei residui viene riempita
  for(int i=0;i<n;i++)
			residual(&residuals[i*d],&ds[i*d],&coarse_centroids[coarse_centroid_of_point[i]*d],d);
  dealloc_matrix(ds);


//  printf("\nResidui: \n");
//  print_matrix(n,d,d,residuals,'p');


//  printf("\n---------Calcolo centroidi accurati---------\n");

  // Andiamo adesso a calcolare i centroidi accurati
	input->centroids = alloc_matrix(m*k,d_star);
	input->centroid_of_point = (int*) malloc(n*m*sizeof(int));
  MATRIX centroids = input->centroids;
  int* centroid_of_point = input->centroid_of_point;

  calculate_centroids(nr, d, k, residuals, eps, m,
      centroids,centroid_of_point, tmin, tmax);

  // Anche in questo caso bisogna calcolare i quantizzatori di tutti i punti
  // rimasti fuori.
	//distances_from_centroids=alloc_matrix(n,m);
  //points_of_centroid(n, d, k, residuals,m, centroids, centroid_of_point, distances_from_centroids);
  //for(i=nr;i<n;i++){
    quantize(&centroid_of_point[nr*m], &residuals[nr*d], k, m, d, centroids, (n-nr));

    dealloc_matrix(residuals);
  //}

//  printf("\nDistanza AL QUADRATO del centroide ACCURATO più vicino ai punti (m=1)\n");
//  print_matrix(n,1,1,distances_from_centroids,'p');
//  printf("\nIndici del vettore 'centroids' indicante i centroidi ACCURATI più vicini al punto (m=1) \n");
//  print_matrix_int(n,1,1,centroid_of_point,'p');
//  printf("Centroidi ACCURATI:\n");
//  print_matrix(k*m,d_star,m,centroids,'c');

  // Deallochiamo distances_from_centroids che non ci serve più, serve solo come
  // struttura di supporto per trovare i centroidi di ciascun punto

  // Costruzione lista invertita
	input->lista_invertita = (int*) malloc(n*(m+1)*sizeof(int));
  int* lista_invertita = input->lista_invertita;

  // vettore che contiene in posizione i il numero di punti y tali che qc(y)=i
  // struttura di supporto per la costruzione della lista lista_invertita
  int* punti_quantizzatore_coarse = (int*) calloc(kc,sizeof(int));
  int sommapunti,inizio;

  // per ogni punto i, si incrementa di 1 il numero di punti del quantizzatore
  // coarse a cui appartiene
  for(int i=0;i<n;i++){
  	punti_quantizzatore_coarse[coarse_centroid_of_point[i]]++;
  }

	input->celle_prima = (int*) calloc(kc,sizeof(int));
  int* celle_prima = input->celle_prima;

  // Siccome è il primo quantizzatore coarse, la sua sezione si troverà all'inizio
  celle_prima[0]=0;

  //si conta quante posizioni nella lista invertita ci sono prima che inizi la
  // sottolista del centroide i utilizzando il vettore celle_prima
  for(int i=1;i<kc;i++){
	  celle_prima[i]=celle_prima[i-1]+punti_quantizzatore_coarse[i-1]*(m+1);
  }

	input->punti_caricati = (int*) calloc(kc,sizeof(int));
  int* punti_caricati = input->punti_caricati;

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

/* NON CANCELLARE
void padding(params* input){
  int n = input->n;
  int nq = input->nq;
  int d = input->d;
  int m = input->m;
  int d_star = d/m;
  MATRIX ds = input->ds;
  MATRIX qs = input->qs;

  //Quante colonne devo aggiungere
  int pad = 4 - ((d_star) % 4) ;

  MATRIX ds2 = alloc_matrix(n, d+pad*m);
  for( int i=0; i<n; i++)
    for( int g=0; g<m; g++){
      memcpy( &ds2[ i*(d+pad*m)+g*(d_star+pad) ], &ds[ i*d+g*d_star ], d_star*sizeof(float) );
      memset( &ds2[ i*(d+pad*m)+g*(d_star+pad)+d_star+1 ], 0, pad*sizeof(float) );
    }


  MATRIX qs2 = alloc_matrix(nq, d+pad*m);
  for( int i=0; i<nq; i++)
    for( int g=0; g<m; g++){
      memcpy( &qs2[ i*(d+pad*m)+g*(d_star+pad) ], &qs[ i*d+g*d_star ], d_star*sizeof(float) );
      memset( &qs2[ i*(d+pad*m)+g*(d_star+pad)+d_star+1 ], 0, pad*sizeof(float) );
    }

  input->d = d+pad*m;
  input->ds = ds2;
  input->qs = qs2;
}
*/


void indexing(params* input){
//  if( (input->d / input->m) % 4 != 0 )
//    padding(input);


/*movups noPad
Indexing time = 57.416 secs
Searching time = 41.961 secs
movaps newPad
Indexing time = 56.146 secs
Searching time = 42.275 secs
*/

  //print_matrix(20, input->d, input->k, input->qs,'p');

  int d_star = input->d/input->m;

	if(input->nr > input->n){
		input->nr = input->n/2;
	}

	if(input->k > input->n){
		input->k = input->n/16;
	}

	if(input->kc > input->nr){
		input->kc = input->nr/16;
	}

  //input->n = 20;
  //input->k = 4;

  printf("Esaustiva: %d, Simmetrica: %d\n", input->exaustive, input->symmetric);

  //----------Stampa tutti i punti-----------
//  print_matrix(input->n, input->d, input->k, input->qs,'p');


//_______________________Starting Algorithms_____________________

  if(input->exaustive==1){
		input->centroids=alloc_matrix(input->m*input->k,d_star);
		input->centroid_of_point=(int*) malloc(input->n*input->m*sizeof(int));
    calculate_centroids(input->n, input->d, input->k, input->ds, input->eps, input->m, input->centroids,
                        input->centroid_of_point, input->tmin, input->tmax);
    dealloc_matrix(input->ds);
  } else {
    non_exhaustive_indexing(input);

  }

  if(input->symmetric == 1){
    		store_distances(input);
  }

	//print_matrix(input->k * input->m, d_star, input->k , input->centroids, 'c');

}

//--------------------------------- SEARCHING ----------------------------------


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

void exaSearchSim(int n, int d, int k, int m, int knn, int nq, MATRIX qs,
				int* ANN, int* centroid_of_point, MATRIX  distances_between_centroids, MATRIX centroids){

  // Vettore che contiene la quantizzazioni del punto
  int* quant;
  int j, g, l, min, max;
	float dist;
  int d_star = d/m;

  quant = (int*) malloc(nq*m*sizeof(int));


	// RICORDA:
	// ANN = (int*) malloc(knn*nq*sizeof(int))
	VECTOR distances = (VECTOR) malloc(knn*sizeof(float));

  // Calcoliamo la quantizzazione dei punti
  quantize(quant, qs, k, m, d, centroids, nq);

	for(int i=0;i<nq;i++){

		// Inizializziamo i primi knn punti ordinandoli in ordine decrescente
		// Così se è > del primo elemento è inutile fare i confronti
		memset_float(distances, FLT_MAX, knn);

		// Vediamo quali sono i punti del dataset più vicini
		for(j=0;j<n;j++){
			// Calcoliamo la distanza tra le due quantizzazioni
			dist = 0.0;

			for(g=0;g<m;g++){
//				printf("Voglio sapere la distanza tra il centroide %d e il centroide %d\n",quant[g],  centroid_of_point[j*m+g]);
        //max = max(quant[i*m+g], centroid_of_point[j*m+g]);
        //min = min(quant[i*m+g], centroid_of_point[j*m+g]);
        dist += distances_between_centroids[(g*k+quant[i*m+g])*k+centroid_of_point[j*m+g]];
//				printf(" Distanza parziale: %f\n", get_distance(quant[g], centroid_of_point[j*m+g],k,g));
			}// for l

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

	}// for i


  free(quant);


	dealloc_matrix(distances);

	// TESTATA! :D
}// exaSearchSim

void exaSearchAsim(int n, int d, int k, int m, int knn, int nq, MATRIX qs,
				int* ANN, int* centroid_of_point, MATRIX centroids){

	MATRIX dist_cent;
  // Vettore che contiene la quantizzazione del punto
  int j, g, l, min, max;
	float dist;
  int d_star = d/m;

	dist_cent = alloc_matrix(m,k);

	// RICORDA:
	// ANN = (int*) malloc(knn*nq*sizeof(int))
	VECTOR distances = (VECTOR) malloc(knn*sizeof(float));


	for(int i=0;i<nq;i++){

		// Calcoliamo la distanza tra le porzioni del punto ed i centroidi dei
		// relativi codebook
		computeDistances(&qs[i*d],centroids,d,m,k,dist_cent);



		// Inizializziamo i primi knn punti ordinandoli in ordine decrescente
		// Così se è > del primo elemento è inutile fare i confronti


		memset_float(distances, FLT_MAX, knn);


		// Vediamo quali sono i punti del dataset più vicini
		for(j=0;j<n;j++){
			// Calcoliamo la distanza tra le due quantizzazioni
			dist = 0.0;

			for(g=0;g<m;g++){
				// Prendiamo la distanza tra la porzione g della query ed il centroide
				// della porzione g del punto del dataset
				dist+= dist_cent[g*k+centroid_of_point[j*m+g]];
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

	}// for i

	dealloc_matrix(dist_cent);
	dealloc_matrix(distances);

	// TESTATA! :D
}// exaSearchAsim


//-------------------------- RICERCA NON ESAUSTIVA ---------------------------


void NoExaSearchAsim(MATRIX ds, MATRIX qs, MATRIX centroids, MATRIX coarse_centroids,
  int* lista_invertita, int* celle_prima, int* punti_caricati, int* ANN,
  int d, int w, int k, int kc, int knn, int m, int nq){

	MATRIX dist_cent;
	// Per ogni punto cerchiamo i w centroidi coarse più vicini
	int* c_coarse = (int*) calloc(w, sizeof(int));

	VECTOR dist;
	VECTOR res;
	int j,g,l,p,inizio,centroide,min,max;
	int d_star = d/m;
	float distanza;

//	printf("\nPunti di ciascun centroide coarse\n");
//	print_matrix_int(kc,1,1,punti_caricati,'c');


  res = alloc_matrix(1,d);

	for(int i=0;i<nq;i++){

		dist = alloc_matrix(1,w);

		// Settiamo il vettore delle distanze al massimo float rappresentabile
		memset_float( dist, FLT_MAX, w);


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

		memset_float( dist, FLT_MAX, knn);

    dist_cent = alloc_matrix(m,k);

//		print_matrix(1,knn,knn,dist,'p');

		for(j=0;j<w;j++){
			centroide = c_coarse[j];
//			printf("\nCentroide coarse: %d\n", centroide);

			// Calcoliamo il residuo del punto del queryset considerato
			residual(res, &qs[i*d], &coarse_centroids[centroide*d], d);

			// Delimitiamo il nostro vettore per poterlo scandire
			inizio = celle_prima[centroide];

			// Calcoliamo la distanza tra le porzioni del punto ed i centroidi dei
			// relativi codebook
			computeDistances(res,centroids,d,m,k,dist_cent);



			// Calcoliamo i knn punti del queryset i cui residui sono più vicini ai
			// residui del punto
			//printf("\nPunti presenti: %d\n", punti_caricati[centroide]);

			for(p=0;p<punti_caricati[centroide];p++){
				// Calcoliamo la distanza tra la quantizzazione del residuo ed il
				// residuo del punto
				distanza = 0;

				for(g=0;g<m;g++){
  				// Nel primo elemento c'è il punto, quindi si shifta di 1
  				l = lista_invertita[inizio+p*(m+1)+g+1];
  				// Prendiamo la distanza tra la porzione g della query ed il centroide
  				// della porzione g del punto del dataset
  				distanza += dist_cent[g*k+l];
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


	dealloc_matrix(dist_cent);


	free(c_coarse);
	dealloc_matrix(dist);
	dealloc_matrix(res);

	// TESTATA

} // NoExaSearchAsim

void NoExaSearchSim(MATRIX ds, MATRIX qs, MATRIX centroids, MATRIX coarse_centroids,
  MATRIX distances_between_centroids, int* lista_invertita, int* celle_prima,
  int* punti_caricati, int* ANN, int d, int w, int k, int kc, int knn, int m,
  int nq){

	// Per ogni punto cerchiamo i w centroidi coarse più vicini
	int* c_coarse = (int*) calloc(w, sizeof(int));

	int* quantization;
	VECTOR dist;
	VECTOR res;
	int j,g,l,p,inizio,centroide,min,max;
	int d_star = d/m;
	float distanza;

//	printf("\nPunti di ciascun centroide coarse\n");
//	print_matrix_int(kc,1,1,punti_caricati,'c');


  res = alloc_matrix(1,d);

	for(int i=0;i<nq;i++){

		dist = alloc_matrix(1,w);

		// Settiamo il vettore delle distanze al massimo float rappresentabile
		memset_float( dist, FLT_MAX, w);


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

		memset_float( dist, FLT_MAX, knn);

    // Effettuiamo la quantizzazione del punto
    quantization = (int*) malloc(m*sizeof(int));

//		print_matrix(1,knn,knn,dist,'p');

		for(j=0;j<w;j++){
			centroide = c_coarse[j];
//			printf("\nCentroide coarse: %d\n", centroide);

			// Calcoliamo il residuo del punto del queryset considerato
			residual(res, &qs[i*d], &coarse_centroids[centroide*d], d);

			// Delimitiamo il nostro vettore per poterlo scandire
			inizio = celle_prima[centroide];

			quantize(quantization,res,k,m,d,centroids,1);


			// Calcoliamo i knn punti del queryset i cui residui sono più vicini ai
			// residui del punto
			//printf("\nPunti presenti: %d\n", punti_caricati[centroide]);

			for(p=0;p<punti_caricati[centroide];p++){
				// Calcoliamo la distanza tra la quantizzazione del residuo ed il
				// residuo del punto
				distanza = 0;

				for(g=0;g<m;g++){
					l = lista_invertita[inizio+p*(m+1)+g+1];
					//max = max(quantization[g], l);
					//min = min(quantization[g], l);
					distanza += distances_between_centroids[(g*k+l)*k+quantization[g]];
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

	free(quantization);


	free(c_coarse);
	dealloc_matrix(dist);
	dealloc_matrix(res);

	// TESTATA

} // NoExaSearchSim



void searching(params* input){


  //printf("________________Chiamata a Search________________\n" );
	if(input->exaustive==1){
    if(input->symmetric==1){
  		exaSearchSim(input->n, input->d, input->k, input->m, input->knn, input->nq, input->qs,
  							input->ANN, input->centroid_of_point, input->distances_between_centroids,
  							input->centroids);
    }else{
      exaSearchAsim(input->n, input->d, input->k, input->m, input->knn, input->nq, input->qs,
  							input->ANN, input->centroid_of_point, input->centroids);
    }
	} else {
    if(input->symmetric==1){
  		NoExaSearchSim(input->ds, input->qs, input->centroids, input->coarse_centroids,
        input->distances_between_centroids, input->lista_invertita, input->celle_prima,
        input->punti_caricati, input->ANN, input->d, input->w, input->k, input->kc,
        input->knn, input->m, input->nq);
    }else{
      NoExaSearchAsim(input->ds, input->qs, input->centroids, input->coarse_centroids,
        input->lista_invertita, input->celle_prima, input->punti_caricati, input->ANN,
        input->d, input->w, input->k, input->kc, input->knn, input->m, input->nq);
    }
	}

  //print_matrix_int(input->nq, input->knn, input->knn, input->ANN,'p');
}
