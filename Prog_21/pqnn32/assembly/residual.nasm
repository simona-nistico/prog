section .data			; Sezione contenente dati inizializzati

section .bss			; Sezione contenente dati non inizializzati

section .text			; Sezione contenente il codice macchina

;Loop Unrolling 64, 16, 4, 1
;TESTATA


;________________________Funzione________________________
;VECTOR residual(VECTOR res, VECTOR x,VECTOR centroid, int d)
global residual      ; rende la funzione visibile all’esterno


; Posizione dei parametri nel Recordi di Attivazione della funzione
; (i primi 8 bytes sono occupati dall’indirizzo di ritorno e da EBP)
res     equ   8     ; puntatore a float res dove metto il risultato, occupa 32 bit (4 bytes)
x       equ   12     ; puntatore a float x, occupa 32 bit (4 bytes)
centr   equ   16    ; puntatore a float centr, occupa 32 bit (4 bytes)
d       equ   20    ; intero a 32 bit rappresenta la dimensione

residual:
;------------------salva i parametri base------------------
    push    ebp       ; salva il Base Pointer
    mov     ebp, esp  ; il Base Pointer punta al Record di Attivazione corrente
		push 		eax
		push    ebx       ; salva i registri da preservare
		push    ecx
    push    edi
;pushad



;---------lettura dei parametri dal Recordi di Attivazione---------
		mov 		eax, [ebp+res]		;mi salvo l'indirizzo di partenza del residuo (risultato)
		mov     ebx, [ebp+x]       ;in ebx ho l'indirizzo di partenza di x
		mov     ecx, [ebp+centr]   ;in edx ho l'indirizzo di partenza del centroide
		mov     edi, [ebp+d]      ;in edi ho la lunghezza dei vettori

;    jmp for_remain

for_64:
		cmp edi, 64	    ; Confronto edi < 64 ?
		jl for_16       ; Se edi è strettamente minore di 64, gestisco il residuo

    ;Loop Unrolling 1: 4 valori
		movaps xmm0, [ebx]    ; in xmm0 metto i primi 4 valori di x
		movaps xmm1, [ecx]    ; in xmm1 metto i primi 4 valori di centr
		subps xmm0, xmm1     				 ; xmm0 = xmm0-xmm1  (x-centr)  x[i]-centroid[i];
  	movaps [eax], xmm0     ; res[i] = x[i]-centroid[i]  salvo nel vettore i valori

    ;Loop Unrolling 2: 4 valori
		movaps xmm0, [ebx+16]    ; in xmm0 metto i secondi 4 valori di x
		movaps xmm1, [ecx+16]    ; in xmm1 metto i secondi 4 valori di centr
		subps xmm0, xmm1      				; xmm0 = xmm0-xmm1  (x-centr)  x[i]-centroid[i];
		movaps [eax+16], xmm0   ; res[i] = x[i]-centroid[i]  salvo nel vettore i valori

    ;Loop Unrolling 3: 4 valori
    movaps xmm0, [ebx+32]    ; in xmm0 metto i primi 4 valori di x
		movaps xmm1, [ecx+32]    ; in xmm1 metto i primi 4 valori di centr
		subps xmm0, xmm1     				 ; xmm0 = xmm0-xmm1  (x-centr)  x[i]-centroid[i];
  	movaps [eax+32], xmm0     ; res[i] = x[i]-centroid[i]  salvo nel vettore i valori

    ;Loop Unrolling 4: 4 valori
		movaps xmm0, [ebx+48]    ; in xmm0 metto i secondi 4 valori di x
		movaps xmm1, [ecx+48]    ; in xmm1 metto i secondi 4 valori di centr
		subps xmm0, xmm1      				; xmm0 = xmm0-xmm1  (x-centr)  x[i]-centroid[i];
		movaps [eax+48], xmm0   ; res[i] = x[i]-centroid[i]  salvo nel vettore i valori

    ;Loop Unrolling 5: 4 valori
		movaps xmm0, [ebx+64]    ; in xmm0 metto i primi 4 valori di x
		movaps xmm1, [ecx+64]    ; in xmm1 metto i primi 4 valori di centr
		subps xmm0, xmm1     				 ; xmm0 = xmm0-xmm1  (x-centr)  x[i]-centroid[i];
  	movaps [eax+64], xmm0     ; res[i] = x[i]-centroid[i]  salvo nel vettore i valori

    ;Loop Unrolling 6: 4 valori
		movaps xmm0, [ebx+80]    ; in xmm0 metto i secondi 4 valori di x
		movaps xmm1, [ecx+80]    ; in xmm1 metto i secondi 4 valori di centr
		subps xmm0, xmm1      				; xmm0 = xmm0-xmm1  (x-centr)  x[i]-centroid[i];
		movaps [eax+80], xmm0   ; res[i] = x[i]-centroid[i]  salvo nel vettore i valori

    ;Loop Unrolling 7: 4 valori
    movaps xmm0, [ebx+96]    ; in xmm0 metto i primi 4 valori di x
		movaps xmm1, [ecx+96]    ; in xmm1 metto i primi 4 valori di centr
		subps xmm0, xmm1     				 ; xmm0 = xmm0-xmm1  (x-centr)  x[i]-centroid[i];
  	movaps [eax+96], xmm0     ; res[i] = x[i]-centroid[i]  salvo nel vettore i valori

    ;Loop Unrolling 8: 4 valori
		movaps xmm0, [ebx+112]    ; in xmm0 metto i secondi 4 valori di x
		movaps xmm1, [ecx+112]    ; in xmm1 metto i secondi 4 valori di centr
		subps xmm0, xmm1      				; xmm0 = xmm0-xmm1  (x-centr)  x[i]-centroid[i];
		movaps [eax+112], xmm0   ; res[i] = x[i]-centroid[i]  salvo nel vettore i valori

    ;Loop Unrolling 9: 4 valori
    movaps xmm0, [ebx+128]    ; in xmm0 metto i primi 4 valori di x
    movaps xmm1, [ecx+128]    ; in xmm1 metto i primi 4 valori di centr
    subps xmm0, xmm1     				 ; xmm0 = xmm0-xmm1  (x-centr)  x[i]-centroid[i];
    movaps [eax+128], xmm0     ; res[i] = x[i]-centroid[i]  salvo nel vettore i valori

    ;Loop Unrolling 10: 4 valori
    movaps xmm0, [ebx+144]    ; in xmm0 metto i secondi 4 valori di x
    movaps xmm1, [ecx+144]    ; in xmm1 metto i secondi 4 valori di centr
    subps xmm0, xmm1      				; xmm0 = xmm0-xmm1  (x-centr)  x[i]-centroid[i];
    movaps [eax+144], xmm0   ; res[i] = x[i]-centroid[i]  salvo nel vettore i valori

    ;Loop Unrolling 11: 4 valori
    movaps xmm0, [ebx+160]    ; in xmm0 metto i primi 4 valori di x
    movaps xmm1, [ecx+160]    ; in xmm1 metto i primi 4 valori di centr
    subps xmm0, xmm1     				 ; xmm0 = xmm0-xmm1  (x-centr)  x[i]-centroid[i];
    movaps [eax+160], xmm0     ; res[i] = x[i]-centroid[i]  salvo nel vettore i valori

    ;Loop Unrolling 12: 4 valori
    movaps xmm0, [ebx+176]    ; in xmm0 metto i secondi 4 valori di x
    movaps xmm1, [ecx+176]    ; in xmm1 metto i secondi 4 valori di centr
    subps xmm0, xmm1      				; xmm0 = xmm0-xmm1  (x-centr)  x[i]-centroid[i];
    movaps [eax+176], xmm0   ; res[i] = x[i]-centroid[i]  salvo nel vettore i valori

    ;Loop Unrolling 13: 4 valori
    movaps xmm0, [ebx+192]    ; in xmm0 metto i primi 4 valori di x
    movaps xmm1, [ecx+192]    ; in xmm1 metto i primi 4 valori di centr
    subps xmm0, xmm1     				 ; xmm0 = xmm0-xmm1  (x-centr)  x[i]-centroid[i];
    movaps [eax+192], xmm0     ; res[i] = x[i]-centroid[i]  salvo nel vettore i valori

    ;Loop Unrolling 14: 4 valori
    movaps xmm0, [ebx+208]    ; in xmm0 metto i secondi 4 valori di x
    movaps xmm1, [ecx+208]    ; in xmm1 metto i secondi 4 valori di centr
    subps xmm0, xmm1      				; xmm0 = xmm0-xmm1  (x-centr)  x[i]-centroid[i];
    movaps [eax+208], xmm0   ; res[i] = x[i]-centroid[i]  salvo nel vettore i valori

    ;Loop Unrolling 15: 4 valori
    movaps xmm0, [ebx+224]    ; in xmm0 metto i primi 4 valori di x
    movaps xmm1, [ecx+224]    ; in xmm1 metto i primi 4 valori di centr
    subps xmm0, xmm1     				 ; xmm0 = xmm0-xmm1  (x-centr)  x[i]-centroid[i];
    movaps [eax+224], xmm0     ; res[i] = x[i]-centroid[i]  salvo nel vettore i valori

    ;Loop Unrolling 16: 4 valori
    movaps xmm0, [ebx+240]    ; in xmm0 metto i secondi 4 valori di x
    movaps xmm1, [ecx+240]    ; in xmm1 metto i secondi 4 valori di centr
    subps xmm0, xmm1      				; xmm0 = xmm0-xmm1  (x-centr)  x[i]-centroid[i];
    movaps [eax+240], xmm0   ; res[i] = x[i]-centroid[i]  salvo nel vettore i valori


		sub edi, 64      ;sottraggo i 16 elementi già presi
		add eax, 256			;mi sposto anche come indice
		add ebx, 256     ;mi sposto di 16 elementi (64 posizioni)
		add ecx, 256

		jmp for_64       ; Se ebx è più grande o uguale a 8, salto al for


for_16:
		cmp edi, 16	    ; Confronto edi < 16 ?
		jl for_4       ; Se edi è strettamente minore di 8, gestisco il residuo

    ;Loop Unrolling 1: 4 valori
		movaps xmm0, [ebx]    ; in xmm0 metto i primi 4 valori di x
		movaps xmm1, [ecx]    ; in xmm1 metto i primi 4 valori di centr
		subps xmm0, xmm1     				 ; xmm0 = xmm0-xmm1  (x-centr)  x[i]-centroid[i];
  	movaps [eax], xmm0     ; res[i] = x[i]-centroid[i]  salvo nel vettore i valori

    ;Loop Unrolling 2: 4 valori
		movaps xmm0, [ebx+16]    ; in xmm0 metto i secondi 4 valori di x
		movaps xmm1, [ecx+16]    ; in xmm1 metto i secondi 4 valori di centr
		subps xmm0, xmm1      				; xmm0 = xmm0-xmm1  (x-centr)  x[i]-centroid[i];
		movaps [eax+16], xmm0   ; res[i] = x[i]-centroid[i]  salvo nel vettore i valori

    ;Loop Unrolling 3: 4 valori
    movaps xmm0, [ebx+32]    ; in xmm0 metto i primi 4 valori di x
		movaps xmm1, [ecx+32]    ; in xmm1 metto i primi 4 valori di centr
		subps xmm0, xmm1     				 ; xmm0 = xmm0-xmm1  (x-centr)  x[i]-centroid[i];
  	movaps [eax+32], xmm0     ; res[i] = x[i]-centroid[i]  salvo nel vettore i valori

    ;Loop Unrolling 4: 4 valori
		movaps xmm0, [ebx+48]    ; in xmm0 metto i secondi 4 valori di x
		movaps xmm1, [ecx+48]    ; in xmm1 metto i secondi 4 valori di centr
		subps xmm0, xmm1      				; xmm0 = xmm0-xmm1  (x-centr)  x[i]-centroid[i];
		movaps [eax+48], xmm0   ; res[i] = x[i]-centroid[i]  salvo nel vettore i valori


		sub edi, 16      ;sottraggo i 16 elementi già presi
		add eax, 64			;mi sposto anche come indice
		add ebx, 64     ;mi sposto di 16 elementi (64 posizioni)
		add ecx, 64

		jmp for_16       ; Se ebx è più grande o uguale a 8, salto al for

for_4:

;    jmp end

		cmp edi, 4	    ; Confronto edi < 4 ? salta al residuo
  	jl for_remain   ; Se mancano meno di 4 elementi vai alla gestione residuo

		movaps xmm0, [ebx]    ; in xmm0 metto gli ultimi 4 valori di x
		movaps xmm1, [ecx]    ; in xmm1 metto gli ultimi 4 valori di centr

		subps xmm0, xmm1      			 ; xmm0 = xmm0-xmm1  (x-centr)  x[i]-centroid[i];
		movaps [eax], xmm0     ; res[i] = x[i]-centroid[i]  salvo nel vettore i valori

		add eax, 16			;mi sposto anche come indice
		sub edi, 4      ;sottraggo i 4 elementi già presi
		add ebx, 16     ;mi sposto di 4 elementi (16 posizioni)
		add ecx, 16

for_remain:
		cmp edi, 0	    ; edi == 0? fine
		je end

		movss xmm0, [ebx]    ; in xmm0 metto gli ultimi <4 valori di x
		movss xmm1, [ecx]    ; in xmm1 metto gli ultimi <4 valori di centroid

		subss xmm0, xmm1      		 ; xmm0 = xmm0-xmm1  (x-centr)  x[i]-centroid[i];
		movss [eax], xmm0    ; res[i] = x[i]-centroid[i]  salvo nel vettore i valori

		dec edi         ;sottraggo 1 elementi già preso
		add eax, 4  		;mi sposto anche come indice
		add ebx, 4     ;mi sposto di 1 elemento (4 posizioni)
		add ecx, 4

		jmp for_remain     ; salto incondizionato tanto la condizione la vedo dopo

end:

;------------------ripristina i parametri base------------------
;popad
    pop   edi       ; ripristina i registri da preservare
		pop		ecx
    pop   ebx
		pop 	eax
    mov   esp, ebp  ; ripristina lo Stack Pointer
    pop   ebp       ; ripristina il Base Pointer
    ret             ; ritorna alla funzione chiamante
