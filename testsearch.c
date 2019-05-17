//Necessario per poter utilizzare la struttura params definita dal prof
#include "datatypes.h"

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))


//Parametri ottenuti in input
params *input;


<<<<<<< HEAD
//--------------------------- RICERCA ESAUSTIVA --------------------------------
=======
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

//-------------------------RICERCA NON ESAUSTIVA---------------

void smallest_elements(MATRIX values, int* indexes, MATRIX returned_values, int* returned_indexes, int n_values, int n_indexes){

	int i,j,l;

	for(i=0;i<n_indexes;i++){
		returned_values[i] = FLT_MAX;
		returned_indexes[i]=-1;
	}

	for(i=0;j<n_values;i++){
			if(returned_values[0]>values[i]){
				j=1
				while(j<n_indexes && returned_values[j]>values[i]){
					j++
				}
				l=j;
				for(j=j-1;j>-1;j--){
					returned_values[j]]=returned_values[j+1];
					returned_indexes[j]=returned_indexes[j+1];
				}
				returned_values[l]=values[i];
				returned_indexes[l]=indexes[i];
				}
			}
	}

}

void calculateNearestNonExt(MATRIX qs, int nq, int n, int knn, int kc, int w, boolean symmetric){


	int* nearest_coarse = (int*) malloc(w*sizeof(double));
	int* coarse_index = (int*) malloc(n*sizeof(double));
	VECTOR nearest_distance = alloc_matrix(w,1);
	VECTOR distances = alloc_matrix(n,1);

	VECTOR  residual_distances;
	int* residual_indexes;

	int i,j,l,candidate_nearest,punti_ispezionati;
	float distance;

	for(i=0;i<nq;i++){

		for(j=0;j<kc;j++){
			distances[j]=distance(&qs[i*d],&coarse_centroids[k*d],d);
			coarse_index[j]=j;
		}

		smallest_elements(distances, coarse_index, nearest_distance, nearest_coarse, kc, w)
		dealloc_matrix(distances);
		free(coarse_index);

		// in nearest_coarse ora si trovano gli indici dei w centroidi più vicini ad x
		// in nearest_distance le relative distanze

		VECTOR residual = alloc_matrix(d,1);

		for(j=0;j<w;j++){
				candidate_nearest+=punti_quantizzatore_coarse[nearest_coarse[j]];
		}

		residual_distances=alloc_matrix(candidate_nearest,1);
		residual_indexes= (int*) malloc(candidate_nearest*sizeof(double));
		result_residual_distances=alloc_matrix(knn,1);
		result_residual_indexes= (int*) malloc(knn*sizeof(double));

		for(j=0;j<knn;j++){
			result_residual_distances[j]=FLT_MAX;
			result_residual_indexes[j]=-1;
		}


		for(j=0;j<w;j++){
				 // in residual va il residuo del punto i
				 // rispetto al centroide j
				for(l=0;l<d;l++){
					residual[l]=qs[i*d+l]-coarse_centroids[nearest_coarse[j]*d+l];
				}
				inizio=celle_prima[nearest_coarse[j]];
				punti_ispezionati=0;

				for(l=0;l<punti_quantizzatore_coarse[nearest_coarse[j]];l++){
					if(symmetric){
						// quantizzare x
						// fare dist(q(r(x)),q(r(y))) e salvarla in distance
					}
					else {
						// fare dist((r(x),q(r(y))) e salvarla in distance
					}
					residual_indexes[punti_ispezionati+l]=lista_invertita[inizio+punti_ispezionati*(m+1)+m];
					residual_distances[punti_ispezionati+l]=distance;
					punti_ispezionati++;
				}

		}

	}

	dealloc_matrix(residual_distances);
	free(residual_indexes);

}

//---------------------------RICERCA ESAUSTIVA--------------------------------
>>>>>>> f684e2762d6aeb1692db9cfcc484aacb993c7ccc

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
		// Calcoliamo la quantizzazione del punto
		quantizzation = quantize(&qs[i*d], m, d_star);

		// Inizializziamo i primi knn punti ordinandoli in ordine decrescente
		// Così se è > del primo elemento è inutile fare i confronti
		for(j=0;j<knn;j++){
			distances[j] = FLT_MAX;
		}// for j

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
			}// for l

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

void calNearExtA(MATRIX qs, int* ANN, int* centroid_of_point, int k, int m, int knn, int nq, int n, int d_star){
	// RICORDA:
	// ANN = (int*) malloc(knn*nq*sizeof(int))
	VECTOR distances = (VECTOR) malloc(knn*sizeof(double));

	float distance;
	int j,l;

	for(int i=0;i<nq){

		// Inizializziamo i primi knn punti ordinandoli in ordine decrescente
		// Così se è > del primo elemento è inutile fare i confronti
		for(j=0;j<knn;j++){
			distances[j] = FLT_MAX;
		}// for j

		// Vediamo quali sono i punti del dataset più vicini
		for(j=0;j<n;j++){
			// Calcoliamo la distanza tra le due quantizzazioni
			distance = 0;

			for(l=0;l<m;l++){
				distance += distance(&qs[i*d+l*d_star],&centroids[centroid_of_point[j*m+l]],d_star)
			}// for l

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

	}// for i

	dealloc_matrix(distances);

}// calNearExtA

void exaustiveSearch(MATRIX qs, int* ANN, int* centroid_of_point, int k, int m, int knn, int nq, int n, int d_star){
	if(input -> symmetric ==1 ){
		calNearExtS(qs, ANN, centroid_of_point, k, m, knn, nq, n, d_star);
	}else{
		calNearExtA(MATRIX qs, ANN, centroid_of_point, k, m, knn, nq, n, d_star)
	}
}

//-------------------------- RICERCA NON ESAUSTIVA ---------------------------

void NoExaSearch(MATRIX ds,MATRIX centroids_coarse,int* ANN, int d, int knn,int m,int nq, int d_star){
	// Per ogni punto cerchiamo i w centroidi coarse più vicini
	int* c_coarse = (int*) malloc(w*sizeof(int));
	VECTOR dist;
	int j,l,p,inizio,centroide;

	for(int i=0;i<nq,i++){

		dist= alloc_matrix(1,w);

		// Settiamo il vettore delle distanze al massimo float rappresentabile
		for(j=0;j<w;j++){
			dist[j] = FLT_MAX;
		}// for j

		// Vediamo quali sono i centroidi coarse più vicini
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

		for(j=0;j<w<,j++){
			// Delimitiamo il nostro vettore per poterlo scandire
			centroide = c_coarse[j]
			inizio = celle_prima[centroide]*(m+1);

			// Calcoliamo i knn punti del queriset i cui residui sono più vicini ai
			// residui del punto
			for(p=0;p<punti_caricati[centroide];p++){
				// Calcoliamo la distanza tra la quantizzazione del residuo ed il
				// residuo del punto
				distance = 0;

				for(l=0;l<m;l++){
					distance += distance(&residuals[i*d+l*d_star],&centroids[lista_invertita[inizio+p*(m+1)+l]],d_star);
				}// for l

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

			// A questo punto abbiamo ottenuto i knn punti del queryset più vicini
			// tra quelli che ricadevano tra i w centroidi coarse

		}// for j

	}//for i

	free(c_coarse);
	dealloc_matrix(dist);

} // NoExaSearch

void testSearch(params* input2){
  printf("\n###########Chiamata a Funzione TestSearch############\n");
  /*
    Mettere qui tutto quello che serve per testare quanto scritto
  */

	//Collego l'input passato dal main con la struttura usata in questo codice
  input = input2;
}
