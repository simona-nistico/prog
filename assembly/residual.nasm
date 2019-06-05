section .data			; Sezione contenente dati inizializzati

section .bss			; Sezione contenente dati non inizializzati

section .text			; Sezione contenente il codice macchina

;TESTATO


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


for_8:
		cmp edi, 8	    ; Confronto edi < 8 ?
		jl for_4       ; Se edi è strettamente minore di 8, gestisco il residuo

		movaps xmm0, [ebx]    ; in xmm0 metto i primi 4 valori di x
		movaps xmm1, [ecx]    ; in xmm1 metto i primi 4 valori di centr

		subps xmm0, xmm1     				 ; xmm0 = xmm0-xmm1  (x-centr)  x[i]-centroid[i];
  	movaps [eax], xmm0     ; res[i] = x[i]-centroid[i]  salvo nel vettore i valori

		movaps xmm0, [ebx+16]    ; in xmm0 metto i secondi 4 valori di x
		movaps xmm1, [ecx+16]    ; in xmm1 metto i secondi 4 valori di centr

		subps xmm0, xmm1      				; xmm0 = xmm0-xmm1  (x-centr)  x[i]-centroid[i];
		movaps [eax+16], xmm0   ; res[i] = x[i]-centroid[i]  salvo nel vettore i valori

		sub edi, 8      ;sottraggo i 8 elementi già presi
		add eax, 32			;mi sposto anche come indice
		add ebx, 32     ;mi sposto di 8 elementi (32 posizioni)
		add ecx, 32

		jmp for_8       ; Se ebx è più grande o uguale a 8, salto al for

for_4:
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
