section .data			; Sezione contenente dati inizializzati
align 16          ;128 / 16

section .bss			; Sezione contenente dati non inizializzati

section .text			; Sezione contenente il codice macchina

;________________________Funzioni esterne definite dal prof________________________

extern get_block
extern free_block

%macro	getmem	2
	mov	eax, %1
	push	eax
	mov	eax, %2
	push	eax
	call	get_block
	add	esp, 8
%endmacro

%macro	fremem	1
	push	%1
	call	free_block
	add	esp, 4
%endmacro


;________________________Funzione________________________
;float test_distance(VECTOR x1, VECTOR x2, int d);
global test_distance      ; rende la funzione visibile all’esterno

; Posizione dei parametri nel Recordi di Attivazione della funzione
; (i primi 8 bytes sono occupati dall’indirizzo di ritorno e da EBP)
x1   equ   8     ; puntatore a float x1, occupa 32 bit (4 bytes)
x2   equ   12    ; puntatore a float x2, occupa 32 bit (4 bytes)
d1   equ   16    ; intero a 32 bit rappresenta la dimensione

test_distance:
;------------------salva i parametri base------------------
    push    ebp       ; salva il Base Pointer
    mov     ebp, esp  ; il Base Pointer punta al Record di Attivazione corrente
;    push    ebx       ; salva i registri da preservare
;    push    esi
;    push    edi
;pushad


;---------lettura dei parametri dal Recordi di Attivazione---------
    mov     eax, [ebp+x1]   ;in eax ho l'indirizzo di partenza di x1
    mov     ecx, [ebp+x2]   ;in ecx ho l'indirizzo di partenza di x2
    mov     edx, [ebp+d1]   ;in edx ho la lunghezza dei vettori

    xorps xmm2, xmm2          ;azzero xmm2 dove terrò la somma

    cmp edx, 8	    ; Confronto edx < 8 ?
    jl for_4_dis    ; Se edx è strettamente minore di 8, gestisco il residuo

for_8_dis:
    movaps xmm0, [eax]    ; in xmm0 metto i primi 4 valori di x1
    movaps xmm1, [ecx]    ; in xmm1 metto i primi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

    movaps xmm0, [eax+16]    ; in xmm0 metto i secondi 4 valori di x1
    movaps xmm1, [ecx+16]    ; in xmm1 metto i secondi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

    sub edx, 8      ;sottraggo i 8 elementi già presi
    add eax, 32     ;mi sposto di 8 elementi (32 posizioni)
    add ecx, 32

    cmp edx, 8	    ; Confronto edx >= 8 ? repeat
	  jge for_8_dis   ; Se edx è più grande o uguale a 8, salto al for

for_4_dis:
    cmp edx, 4	    ; Confronto edx < 4 ? salta al residuo
	  jl for_remain_dis  ; Se mancano meno di 4 elementi vai alla gestione residuo

    movaps xmm0, [eax]    ; in xmm0 metto gli ultimi 4 valori di x1
    movaps xmm1, [ecx]    ; in xmm1 metto gli ultimi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

    sub edx, 4      ;sottraggo i 4 elementi già presi
    add eax, 16     ;mi sposto di 4 elementi (16 posizioni)
    add ecx, 16

for_remain_dis:
    cmp edx, 0	    ; edx == 0? fine
    je end_dis

    movss xmm0, [eax]    ; in xmm0 metto gli ultimi <4 valori di x1
    movss xmm1, [ecx]    ; in xmm1 metto gli ultimi <4 valori di x2

    subss xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulss xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addss xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

    dec edx         ;sottraggo 1 elementi già preso
    add eax, 4     ;mi sposto di 1 elemento (4 posizioni)
    add ecx, 4

	  jmp for_remain_dis    ; salto incondizionato tanto la condizione la vedo dopo

end_dis:
    haddps xmm2, xmm2     ;sommo i primi due valori del registro
    haddps xmm2, xmm2     ;sommo i secondi due valori del registro

    sub esp, 4            ;sposo nel registro in cima allo stack FPU il risultato
    movss [esp], xmm2     ;al fine di prenderlo da c
    fld dword [esp]
    add esp, 4

;------------------ripristina i parametri base------------------
;popad
    ;    pop   edi       ; ripristina i registri da preservare
    ;    pop   esi
    ;    pop   ebx
    mov   esp, ebp  ; ripristina lo Stack Pointer
    pop   ebp       ; ripristina il Base Pointer
    ret             ; ritorna alla funzione chiamante





;________________________Funzione________________________
;VECTOR residual(VECTOR x,VECTOR centroid,int d)
global test_residual      ; rende la funzione visibile all’esterno

; Posizione dei parametri nel Recordi di Attivazione della funzione
; (i primi 8 bytes sono occupati dall’indirizzo di ritorno e da EBP)
x       equ   8     ; puntatore a float x, occupa 32 bit (4 bytes)
centr   equ   12    ; puntatore a float centr, occupa 32 bit (4 bytes)
d2      equ   16    ; intero a 32 bit rappresenta la dimensione

test_residual:
;------------------salva i parametri base------------------
    push    ebp       ; salva il Base Pointer
    mov     ebp, esp  ; il Base Pointer punta al Record di Attivazione corrente
    push    ebx       ; salva i registri da preservare
		push    ecx
    push    esi
    push    edi
;pushad



;---------lettura dei parametri dal Recordi di Attivazione---------
    mov     edi, [ebp+d2]      ;in edi ho la lunghezza dei vettori
		;poiché 'getmem' è una chiamata a c e usa gli altri registri, devo
		;prelevare dopo gli altri valori
    getmem 4, edi              ;in eax ho l'indirizzo di partenza del vettore residual
                               ; 4 bytes per il float e dimensione d
		push eax									 ;mi salvo l'indirizzo di partenza

		mov     ebx, [ebp+x]       ;in ebx ho l'indirizzo di partenza di x
		mov     ecx, [ebp+centr]   ;in edx ho l'indirizzo di partenza del centroide
		mov 	  esi, 0					 	 ;i=0

		cmp edi, 8	    ; Confronto edi < 8 ?
		jl for_4_res       ; Se edi è strettamente minore di 8, gestisco il residuo

for_8_res:

		movaps xmm0, [ebx]    ; in xmm0 metto i primi 4 valori di x
		movaps xmm1, [ecx]    ; in xmm1 metto i primi 4 valori di centr

		subps xmm0, xmm1     				 ; xmm0 = xmm0-xmm1  (x-centr)  x[i]-centroid[i];
  	movaps [eax], xmm0     ; res[i] = x[i]-centroid[i]  salvo nel vettore i valori

		movaps xmm0, [ebx+16]    ; in xmm0 metto i secondi 4 valori di x
		movaps xmm1, [ecx+16]    ; in xmm1 metto i secondi 4 valori di centr

		subps xmm0, xmm1      				; xmm0 = xmm0-xmm1  (x-centr)  x[i]-centroid[i];
		movaps [eax+16], xmm0   ; res[i] = x[i]-centroid[i]  salvo nel vettore i valori

		sub edi, 8      ;sottraggo i 8 elementi già presi
		add ebx, 32     ;mi sposto di 8 elementi (32 posizioni)
		add ecx, 32
		add eax, 32			;mi sposto anche come indice

		cmp edi, 8	    ; Confronto edi >= 8 ? repeat
		jge for_8_res       ; Se ebx è più grande o uguale a 8, salto al for

for_4_res:
		cmp edi, 4	    ; Confronto edi < 4 ? salta al residuo
  	jl for_remain_res      ; Se mancano meno di 4 elementi vai alla gestione residuo

		movaps xmm0, [ebx]    ; in xmm0 metto gli ultimi 4 valori di x
		movaps xmm1, [ecx]    ; in xmm1 metto gli ultimi 4 valori di centr

		subps xmm0, xmm1      			 ; xmm0 = xmm0-xmm1  (x-centr)  x[i]-centroid[i];
		movaps [eax], xmm0     ; res[i] = x[i]-centroid[i]  salvo nel vettore i valori

		sub edi, 4      ;sottraggo i 4 elementi già presi
		add ebx, 16     ;mi sposto di 4 elementi (16 posizioni)
		add ecx, 16
		add eax, 16			;mi sposto anche come indice

for_remain_res:
		cmp edi, 0	    ; edi == 0? fine
		je end_res

		movss xmm0, [ebx]    ; in xmm0 metto gli ultimi <4 valori di x
		movss xmm1, [ecx]    ; in xmm1 metto gli ultimi <4 valori di centroid

		subss xmm0, xmm1      		 ; xmm0 = xmm0-xmm1  (x-centr)  x[i]-centroid[i];
		movss [eax], xmm0    ; res[i] = x[i]-centroid[i]  salvo nel vettore i valori

		dec edi         ;sottraggo 1 elementi già preso
		add ebx, 4     ;mi sposto di 1 elemento (4 posizioni)
		add ecx, 4
		add eax, 4  		;mi sposto anche come indice

		jmp for_remain_res     ; salto incondizionato tanto la condizione la vedo dopo

end_res:
		pop eax				;ripristino il valore visto che lo avevo modificato

;------------------ripristina i parametri base------------------
;popad
    pop   edi       ; ripristina i registri da preservare
    pop   esi
		pop		ecx
    pop   ebx
    mov   esp, ebp  ; ripristina lo Stack Pointer
    pop   ebp       ; ripristina il Base Pointer
    ret             ; ritorna alla funzione chiamante




;________________________Funzione________________________
;float objective_function(int n,int m, MATRIX distances_from_centroids)
global test_objective      ; rende la funzione visibile all’esterno

; Posizione dei parametri nel Recordi di Attivazione della funzione
; (i primi 8 bytes sono occupati dall’indirizzo di ritorno e da EBP)
n     equ   8     ; intero a 32 bit rappresenta il numero di punti
m   	equ   12    ; intero a 32 bit rappresenta il numero di gruppi
dist  equ   16    ; puntatore a distances_from_centroids

test_objective:
;------------------salva i parametri base------------------
    push    ebp       ; salva il Base Pointer
    mov     ebp, esp  ; il Base Pointer punta al Record di Attivazione corrente
;    push    ebx       ; salva i registri da preservare,
;		push    ecx
;    push    esi
;    push    edi
		pushad


;---------lettura dei parametri dal Recordi di Attivazione---------
		mov     ebx, [ebp+n]       ;in ebx ho il numero di punti
		mov     ecx, [ebp+m]   		 ;in ecx ho il numero di gruppi
		mov 	  edx, [ebp+dist]    ;in edx ho l'indirizzo di partenza di distances_from_centroids

		xorps xmm0, xmm0          ;azzero xmm0 dove terrò la somma

		imul ebx, [ebp+m]		;ebx = n*m cioè num_punti*num_gruppi

    cmp ebx, 8	    ; Confronto n*m < 8 ?
    jl for_4_obj    ; Se edx è strettamente minore di 8, gestisco il residuo

for_8_obj:
		addps xmm0, [edx]		  ;sum += distances_from_centroids 0...4
		addps xmm0, [edx+16]

    sub ebx, 8      ;sottraggo i 8 elementi già presi
    add edx, 32     ;mi sposto di 8 elementi (32 posizioni)

    cmp ebx, 8	    ; Confronto n*m >= 8 ? repeat
	  jge for_8_obj   ; Se edx è più grande o uguale a 8, salto al for

for_4_obj:
    cmp ebx, 4	    ; Confronto n*m < 4 ? salta al residuo
	  jl for_remain_obj  ; Se mancano meno di 4 elementi vai alla gestione residuo

		addps xmm0, [edx]		  ;sum += distances_from_centroids 0...4

    sub ebx, 4      ;sottraggo i 4 elementi già presi
    add edx, 16     ;mi sposto di 4 elementi (16 posizioni)

for_remain_obj:
    cmp ebx, 0	    ; n*m == 0? fine
    je end_obj

    addps xmm0, [edx]    ; in xmm0 metto gli ultimi <4 valori di x1

    dec ebx         ;sottraggo 1 elementi già preso
    add edx, 4     ;mi sposto di 1 elemento (4 posizioni)

	  jmp for_remain_obj    ; salto incondizionato tanto la condizione la vedo dopo

end_obj:
    haddps xmm0, xmm0     ;sommo i primi due valori del registro
    haddps xmm0, xmm0     ;sommo i secondi due valori del registro

    sub esp, 4            ;sposo nel registro in cima allo stack FPU il risultato
    movss [esp], xmm2     ;al fine di prenderlo da c
    fld dword [esp]
    add esp, 4


;------------------ripristina i parametri base------------------
		popad
;    pop   edi       ; ripristina i registri da preservare
;    pop   esi
;		pop		ecx
;    pop   ebx
    mov   esp, ebp  ; ripristina lo Stack Pointer
    pop   ebp       ; ripristina il Base Pointer
    ret             ; ritorna alla funzione chiamante
