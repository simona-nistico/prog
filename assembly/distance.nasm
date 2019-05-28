section .data			; Sezione contenente dati inizializzati

section .bss			; Sezione contenente dati non inizializzati

section .text			; Sezione contenente il codice macchina

;TESTATO

;________________________Funzione________________________
;float test_distance(VECTOR x1, VECTOR x2, int d);
global test_distance      ; rende la funzione visibile all’esterno

; Posizione dei parametri nel Recordi di Attivazione della funzione
; (i primi 8 bytes sono occupati dall’indirizzo di ritorno e da EBP)
x1   equ   8     ; puntatore a float x1, occupa 32 bit (4 bytes)
x2   equ   12    ; puntatore a float x2, occupa 32 bit (4 bytes)
d    equ   16    ; intero a 32 bit rappresenta la dimensione

test_distance:
;------------------salva i parametri base------------------
    push    ebp       ; salva il Base Pointer
    mov     ebp, esp  ; il Base Pointer punta al Record di Attivazione corrente
;pushad


;---------lettura dei parametri dal Recordi di Attivazione---------
    mov     eax, [ebp+x1]   ;in eax ho l'indirizzo di partenza di x1
    mov     ecx, [ebp+x2]   ;in ecx ho l'indirizzo di partenza di x2
    mov     edx, [ebp+d]    ;in edx ho la lunghezza dei vettori

    xorps xmm2, xmm2          ;azzero xmm2 dove terrò la somma

    cmp edx, 16	    ; Confronto edx < 16 ?
    jl for_4    ; Se edx è strettamente minore di 16, gestisco il residuo

for_16:
		;Loop Unrolling 1: 4 valori
    movaps xmm0, [eax]    ; in xmm0 metto i primi 4 valori di x1
    movaps xmm1, [ecx]    ; in xmm1 metto i primi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

		;Loop Unrolling 2: 4 valori
    movaps xmm0, [eax+16]    ; in xmm0 metto i secondi 4 valori di x1
    movaps xmm1, [ecx+16]    ; in xmm1 metto i secondi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

		;Loop Unrolling 3: 4 valori
		movaps xmm0, [eax+32]    ; in xmm0 metto i primi 4 valori di x1
    movaps xmm1, [ecx+32]    ; in xmm1 metto i primi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

		;Loop Unrolling 4: 4 valori
    movaps xmm0, [eax+48]    ; in xmm0 metto i secondi 4 valori di x1
    movaps xmm1, [ecx+48]    ; in xmm1 metto i secondi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff


    sub edx, 16      ;sottraggo i 16 elementi già presi
    add eax, 64     ;mi sposto di 16 elementi (64 posizioni)
    add ecx, 64

    cmp edx, 16	    ; Confronto edx >= 8 ? repeat
	  jge for_16   ; Se edx è più grande o uguale a 8, salto al for

for_4:
    cmp edx, 4	    ; Confronto edx < 4 ? salta al residuo
	  jl for_remain   ; Se mancano meno di 4 elementi vai alla gestione residuo

    movaps xmm0, [eax]    ; in xmm0 metto gli ultimi 4 valori di x1
    movaps xmm1, [ecx]    ; in xmm1 metto gli ultimi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

    sub edx, 4      ;sottraggo i 4 elementi già presi
    add eax, 16     ;mi sposto di 4 elementi (16 posizioni)
    add ecx, 16

		jmp for_4    ; salto incondizionato tanto la condizione la vedo dopo

for_remain:
    cmp edx, 0	    ; edx == 0? fine
    je end

    movss xmm0, [eax]    ; in xmm0 metto gli ultimi <4 valori di x1
    movss xmm1, [ecx]    ; in xmm1 metto gli ultimi <4 valori di x2

    subss xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulss xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addss xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

    dec edx         ;sottraggo 1 elementi già preso
    add eax, 4     ;mi sposto di 1 elemento (4 posizioni)
    add ecx, 4

	  jmp for_remain    ; salto incondizionato tanto la condizione la vedo dopo

end:
    haddps xmm2, xmm2     ;sommo i primi due valori del registro
    haddps xmm2, xmm2     ;sommo i secondi due valori del registro

    sub esp, 4            ;sposo nel registro in cima allo stack FPU il risultato
    movss [esp], xmm2     ;al fine di prenderlo da c
    fld dword [esp]
    add esp, 4

;------------------ripristina i parametri base------------------

    mov   esp, ebp  ; ripristina lo Stack Pointer
    pop   ebp       ; ripristina il Base Pointer
    ret             ; ritorna alla funzione chiamante
