#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>

#define	MATRIX		double*
#define	VECTOR		double*

int n;
int k;
int kc;
int m;
double epsilon;

// Numero di elementi per sottogruppo
int ns = d/m;

//Matrice di due colonne che per ogni indice i che rappresenta il punto corrispondente
//memorizza in [i][0] il centroide corrispondendente e in [i][1] la distanza dal centroide
MATRIX centroideDelPunto;

//Punti del dataset da sosituire con quelli veri
VECTOR ds;

//Array con l'indice dei punti che alla prima iterazione sono anche i centroidi
VECTOR centroidiCasuali

//Centroidi veri e propri da codificare come i punti
MATRIX centroids;


//genera k numeri casuali diversi da 0 a n incluso
void generate_centroids(int n, int k){

	int number,old_number,i,j;

	// Alloco la matrice dei centroidi
	centroids = alloc_matrix(k,d);

	srand(time(NULL));
	old_number=rand()%(n/k);
	printf("%d\n",old_number);

	// Inseriamo il centroide nella matrice
	for(j=0;j<d;j++){
		centroids[i*d+j] = ds[old_number*d+i];
	}

	int range;
	for(i=1;i<k;i++){

		range = (n-old_number)/(k-i);
		printf("Range: %d  ",range );
  	number=old_number+rand()%range+1;
		printf("%d\n",numbers[i]);

		// Inseriamo il centroide nella matrice
		for(j=0;j<d;j++){
			centroids[i*d+j] = ds[number*d+i];
		}

		old_number = number;

 	}

//Mettere nella matrice centroids i punti corrispondenti agli indici trovati

}

// la matrice distances deve essere creata una volta sola visto che il metodo seguente deve stare dentro un ciclo
void computeDistancesFromCentroids(){

	VECTOR punto = alloc_matrix(1,d);	// Alloco il vettore che conterrà di volta in volta il punto
  VECTOR centroide = alloc_matrix(1,d); // Alloco il vettore che conterrà di volta in volta il centroide
	double minDist,distanza;
	int minCentr;
	int i,j,l;

	for(i=0;i<n;i++){

		for(l=0;l<d;l++){
			punto[l] = ds[d*i+l];	// Sto supponendo che il dataset sia memorizzato per righe
		}

		for(l=0;l<d;l++){
			centroide[l] = centroids[d*0+l];	// Sto supponendo che il dataset sia memorizzato per righe
		}

		minDist = distance(punto, centroide);
		minCentr = 0;

		for(j=1;j<k;j++){ //Verificare la sintassi, l'obiettivo è passare l'indirizzo di partenza
					//del punto i-esimo

			for(l=0;l<d;l++){
				centroide[l] = centroids[d*j+l];	// Sto supponendo che il dataset sia memorizzato per righe
			}

			distanza=distance(punto, centroids[j]);

			if(distanza < minDist){
				minDist = distanza;
				minCentr = j;
			}

		}

		centroideDelPunto[i][0] = minCentr;
		centroideDelPunto[i][1] = minDist;
	}

	// Liberiamo lo spazio (verificare se si può utilizzare la stessa funzione della matrice)
	dealloc_matrix(punto);
	dealloc_matrix(centroide);

}

//Si può applicare il loop vectorization cioè più operazioni nello stesso ciclo
//Oppure più punti alla volta
double distance(VECTOR puntoX, VECTOR puntoY){

	double sum=0;
	double diff=0;
	for(int i=0;i<d;i++){
		diff = (puntoX[i]-puntoY[i]);
		sum+= diff*diff;
	}
	return sqrt(sum);
}

//Per  ogni punto si ha il centroide di appartenenza (q(x))

//Per ogni centroide bisogna trovare il punto medio e sostituirlo ai centroidi di partenza

//Ripetere finchè la distanza tra il centroide di prima e quello nuovo non è inferiore ad epsilon


// Funzione che calcola il nuovo centroide facendo la media geometrica dei punti
void aggiorna_centroidi(){
		// Creiamo una matrice che ci consenta di effettuare l'aggiornamento dei centroidi
		MATRIX tmp = alloc_matrix(k,d);
		VECTOR c = VECTOR malloc(k,sizeof(double));
		int i,j,centroide;

		for(i=0;i<n;i++){
			centroide = centroideDelPunto[i][0];
			c[centroide]++;
			for(j=0;j<d;j++){
				tmp[centroide*d+j] += ds[i*d+j];
			}
		}

		for(i=0;i<k;i++){
			for(j=0;j<d;j++){
				centroids[i*d+j] = tmp[i*d+j] / c[i];
			}
		}

		dealloc_matrix(tmp);
		free(c);
}

//calcola la somma dei quadrati delle distanze dei punti dai rispettivi centroidi

double sumOfDistances(){

		int i;
		double sum=0;

		for(i=0;i<n;i++){    // per ogni punto i, aggiunge distanza(i,q(i))^2
			sum+=centroideDelPunto[i*2+1]*centroideDelPunto[i*2+1];
		}

		return sum;

}

//calcola i centroidi partendo dall'inizializzazione ed aggiornando i centroidi
//fino a quando la somma delle distanze dai centroidi non è inferiore ad epsilon

void calculate_centroids(){

		generate_centroids(n,k);
  	centroideDelPunto = alloc_matrix(n,2)
  	computeDistancesFromCentroids();
  	double sommadistanze=sumOfDistances();

  	while(sommadistanze>epsilon) {

				aggiorna_centroidi();
    		computeDistancesFromCentroids();
    		sommadist=sumOfDistances();
  	}

}


// Funzione che quantizza il punto dell query.
int* quantize(int x){
	// Centroidi che quantizzano ciascuna porzione
	int* cents = (int*) malloc(m,sizeof(int))
	int centr;
	double dist,tmp;

	// Calcoliamo il centroide di ciascun sottogruppo
	for(int i=0;i<ns;i++){
		// Calcoliamo il centroide più vicino del sottogruppo
		centr = 0;
		dist = dist_cent(x,0,i);
		for(int j=1;j<k;j++){
			tmp = dist_cent(x,j,i);
			if(tmp < dist){
				centr = j;
				dist = tmp;
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
double dist_cent_qs(int point,int centr,int group){
	double sum,diff;

	for(int i=0;i<ns;i++){
		diff = qs[point*d+group*ns+i]-centroids[centr*d+group*ns+i];
		sum = diff*diff;
	}

	return sqrt(sum);
}


int main(){

  generate(100,9);
  return 0;

}
