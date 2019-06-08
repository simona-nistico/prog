section .data			; Sezione contenente dati inizializzati

section .bss			; Sezione contenente dati non inizializzati

section .text			; Sezione contenente il codice macchina


;Loop Unrolling 64, 16, 4, 1
;TESTATA

;________________________Funzione________________________
;float test_distance(VECTOR x1, VECTOR x2, int d);
global distance      ; rende la funzione visibile all’esterno

; Posizione dei parametri nel Recordi di Attivazione della funzione
; (i primi 8 bytes sono occupati dall’indirizzo di ritorno e da EBP)
x1   equ   8     ; puntatore a float x1, occupa 32 bit (4 bytes)
x2   equ   12    ; puntatore a float x2, occupa 32 bit (4 bytes)
d    equ   16    ; intero a 32 bit rappresenta la dimensione

distance:
;------------------salva i parametri base------------------
    push    ebp       ; salva il Base Pointer
    mov     ebp, esp  ; il Base Pointer punta al Record di Attivazione corrente
    push    eax
    push    ecx
    push    edx
;    pushad


;---------lettura dei parametri dal Recordi di Attivazione---------
    mov     eax, [ebp+x1]   ;in eax ho l'indirizzo di partenza di x1
    mov     ecx, [ebp+x2]   ;in ecx ho l'indirizzo di partenza di x2
    mov     edx, [ebp+d]    ;in edx ho la lunghezza dei vettori

    xorps xmm2, xmm2          ;azzero xmm2 dove terrò la somma


for_128:
    cmp edx, 128	    ; Confronto edx < 64 ?
    jl for_16    ; Se edx è strettamente minore di 64, gestisco il residuo

		;Loop Unrolling 1: 4 valori
    movups xmm0, [eax]    ; in xmm0 metto i primi 4 valori di x1
    movups xmm1, [ecx]    ; in xmm1 metto i primi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

    ;Loop Unrolling 2: 4 valori
    movups xmm0, [eax+16]    ; in xmm0 metto i secondi 4 valori di x1
    movups xmm1, [ecx+16]    ; in xmm1 metto i secondi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

    ;Loop Unrolling 3: 4 valori
    movups xmm0, [eax+32]    ; in xmm0 metto i primi 4 valori di x1
    movups xmm1, [ecx+32]    ; in xmm1 metto i primi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

    ;Loop Unrolling 4: 4 valori
    movups xmm0, [eax+48]    ; in xmm0 metto i secondi 4 valori di x1
    movups xmm1, [ecx+48]    ; in xmm1 metto i secondi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

    ;Loop Unrolling 5: 4 valori
    movups xmm0, [eax+64]    ; in xmm0 metto i primi 4 valori di x1
    movups xmm1, [ecx+64]    ; in xmm1 metto i primi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

    ;Loop Unrolling 6: 4 valori
    movups xmm0, [eax+80]    ; in xmm0 metto i secondi 4 valori di x1
    movups xmm1, [ecx+80]    ; in xmm1 metto i secondi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

    ;Loop Unrolling 7: 4 valori
    movups xmm0, [eax+96]    ; in xmm0 metto i primi 4 valori di x1
    movups xmm1, [ecx+96]    ; in xmm1 metto i primi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

    ;Loop Unrolling 8: 4 valori
    movups xmm0, [eax+112]    ; in xmm0 metto i secondi 4 valori di x1
    movups xmm1, [ecx+112]    ; in xmm1 metto i secondi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

    ;Loop Unrolling 9: 4 valori
    movups xmm0, [eax+128]    ; in xmm0 metto i primi 4 valori di x1
    movups xmm1, [ecx+128]    ; in xmm1 metto i primi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

    ;Loop Unrolling 10: 4 valori
    movups xmm0, [eax+144]    ; in xmm0 metto i secondi 4 valori di x1
    movups xmm1, [ecx+144]    ; in xmm1 metto i secondi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

    ;Loop Unrolling 11: 4 valori
    movups xmm0, [eax+160]    ; in xmm0 metto i primi 4 valori di x1
    movups xmm1, [ecx+160]    ; in xmm1 metto i primi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

    ;Loop Unrolling 12: 4 valori
    movups xmm0, [eax+176]    ; in xmm0 metto i secondi 4 valori di x1
    movups xmm1, [ecx+176]    ; in xmm1 metto i secondi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

    ;Loop Unrolling 13: 4 valori
    movups xmm0, [eax+192]    ; in xmm0 metto i primi 4 valori di x1
    movups xmm1, [ecx+192]    ; in xmm1 metto i primi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

    ;Loop Unrolling 14: 4 valori
    movups xmm0, [eax+208]    ; in xmm0 metto i secondi 4 valori di x1
    movups xmm1, [ecx+208]    ; in xmm1 metto i secondi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

    ;Loop Unrolling 15: 4 valori
    movups xmm0, [eax+224]    ; in xmm0 metto i primi 4 valori di x1
    movups xmm1, [ecx+224]    ; in xmm1 metto i primi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

    ;Loop Unrolling 16: 4 valori
    movups xmm0, [eax+240]    ; in xmm0 metto i secondi 4 valori di x1
    movups xmm1, [ecx+240]    ; in xmm1 metto i secondi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

    ;Loop Unrolling 17: 4 valori
    movups xmm0, [eax+256]    ; in xmm0 metto i primi 4 valori di x1
    movups xmm1, [ecx+256]    ; in xmm1 metto i primi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

    ;Loop Unrolling 18: 4 valori
    movups xmm0, [eax+272]    ; in xmm0 metto i secondi 4 valori di x1
    movups xmm1, [ecx+272]    ; in xmm1 metto i secondi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

    ;Loop Unrolling 19: 4 valori
    movups xmm0, [eax+288]    ; in xmm0 metto i primi 4 valori di x1
    movups xmm1, [ecx+288]    ; in xmm1 metto i primi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

    ;Loop Unrolling 20: 4 valori
    movups xmm0, [eax+304]    ; in xmm0 metto i secondi 4 valori di x1
    movups xmm1, [ecx+304]    ; in xmm1 metto i secondi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

    ;Loop Unrolling 21: 4 valori
    movups xmm0, [eax+320]    ; in xmm0 metto i primi 4 valori di x1
    movups xmm1, [ecx+320]    ; in xmm1 metto i primi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

    ;Loop Unrolling 22: 4 valori
    movups xmm0, [eax+336]    ; in xmm0 metto i secondi 4 valori di x1
    movups xmm1, [ecx+336]    ; in xmm1 metto i secondi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

    ;Loop Unrolling 23: 4 valori
    movups xmm0, [eax+352]    ; in xmm0 metto i primi 4 valori di x1
    movups xmm1, [ecx+352]    ; in xmm1 metto i primi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

    ;Loop Unrolling 24: 4 valori
    movups xmm0, [eax+368]    ; in xmm0 metto i secondi 4 valori di x1
    movups xmm1, [ecx+368]    ; in xmm1 metto i secondi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

    ;Loop Unrolling 25: 4 valori
    movups xmm0, [eax+384]    ; in xmm0 metto i primi 4 valori di x1
    movups xmm1, [ecx+384]    ; in xmm1 metto i primi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

    ;Loop Unrolling 26: 4 valori
    movups xmm0, [eax+400]    ; in xmm0 metto i secondi 4 valori di x1
    movups xmm1, [ecx+400]    ; in xmm1 metto i secondi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

    ;Loop Unrolling 27: 4 valori
    movups xmm0, [eax+416]    ; in xmm0 metto i primi 4 valori di x1
    movups xmm1, [ecx+416]    ; in xmm1 metto i primi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

    ;Loop Unrolling 28: 4 valori
    movups xmm0, [eax+432]    ; in xmm0 metto i secondi 4 valori di x1
    movups xmm1, [ecx+432]    ; in xmm1 metto i secondi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

    ;Loop Unrolling 29: 4 valori
    movups xmm0, [eax+448]    ; in xmm0 metto i primi 4 valori di x1
    movups xmm1, [ecx+448]    ; in xmm1 metto i primi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

    ;Loop Unrolling 30: 4 valori
    movups xmm0, [eax+464]    ; in xmm0 metto i secondi 4 valori di x1
    movups xmm1, [ecx+464]    ; in xmm1 metto i secondi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

    ;Loop Unrolling 31: 4 valori
    movups xmm0, [eax+480]    ; in xmm0 metto i primi 4 valori di x1
    movups xmm1, [ecx+480]    ; in xmm1 metto i primi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

    ;Loop Unrolling 31: 4 valori
    movups xmm0, [eax+496]    ; in xmm0 metto i secondi 4 valori di x1
    movups xmm1, [ecx+496]    ; in xmm1 metto i secondi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

    sub edx, 128      ;sottraggo i 128 elementi già presi
    add eax, 512      ;mi sposto di 128 elementi (512 posizioni)
    add ecx, 512

    jmp for_128      ; Salto incondizionato


for_64:
    cmp edx, 64	    ; Confronto edx < 64 ?
    jl for_16    ; Se edx è strettamente minore di 64, gestisco il residuo

		;Loop Unrolling 1: 4 valori
    movups xmm0, [eax]    ; in xmm0 metto i primi 4 valori di x1
    movups xmm1, [ecx]    ; in xmm1 metto i primi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

		;Loop Unrolling 2: 4 valori
    movups xmm0, [eax+16]    ; in xmm0 metto i secondi 4 valori di x1
    movups xmm1, [ecx+16]    ; in xmm1 metto i secondi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

		;Loop Unrolling 3: 4 valori
		movups xmm0, [eax+32]    ; in xmm0 metto i primi 4 valori di x1
    movups xmm1, [ecx+32]    ; in xmm1 metto i primi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

		;Loop Unrolling 4: 4 valori
    movups xmm0, [eax+48]    ; in xmm0 metto i secondi 4 valori di x1
    movups xmm1, [ecx+48]    ; in xmm1 metto i secondi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

    ;Loop Unrolling 5: 4 valori
    movups xmm0, [eax+64]    ; in xmm0 metto i primi 4 valori di x1
    movups xmm1, [ecx+64]    ; in xmm1 metto i primi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

    ;Loop Unrolling 6: 4 valori
    movups xmm0, [eax+80]    ; in xmm0 metto i secondi 4 valori di x1
    movups xmm1, [ecx+80]    ; in xmm1 metto i secondi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

    ;Loop Unrolling 7: 4 valori
    movups xmm0, [eax+96]    ; in xmm0 metto i primi 4 valori di x1
    movups xmm1, [ecx+96]    ; in xmm1 metto i primi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

    ;Loop Unrolling 8: 4 valori
    movups xmm0, [eax+112]    ; in xmm0 metto i secondi 4 valori di x1
    movups xmm1, [ecx+112]    ; in xmm1 metto i secondi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

    ;Loop Unrolling 9: 4 valori
    movups xmm0, [eax+128]    ; in xmm0 metto i primi 4 valori di x1
    movups xmm1, [ecx+128]    ; in xmm1 metto i primi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

    ;Loop Unrolling 10: 4 valori
    movups xmm0, [eax+144]    ; in xmm0 metto i secondi 4 valori di x1
    movups xmm1, [ecx+144]    ; in xmm1 metto i secondi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

    ;Loop Unrolling 11: 4 valori
    movups xmm0, [eax+160]    ; in xmm0 metto i primi 4 valori di x1
    movups xmm1, [ecx+160]    ; in xmm1 metto i primi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

    ;Loop Unrolling 12: 4 valori
    movups xmm0, [eax+176]    ; in xmm0 metto i secondi 4 valori di x1
    movups xmm1, [ecx+176]    ; in xmm1 metto i secondi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

    ;Loop Unrolling 13: 4 valori
    movups xmm0, [eax+192]    ; in xmm0 metto i primi 4 valori di x1
    movups xmm1, [ecx+192]    ; in xmm1 metto i primi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

    ;Loop Unrolling 14: 4 valori
    movups xmm0, [eax+208]    ; in xmm0 metto i secondi 4 valori di x1
    movups xmm1, [ecx+208]    ; in xmm1 metto i secondi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

    ;Loop Unrolling 15: 4 valori
    movups xmm0, [eax+224]    ; in xmm0 metto i primi 4 valori di x1
    movups xmm1, [ecx+224]    ; in xmm1 metto i primi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

    ;Loop Unrolling 16: 4 valori
    movups xmm0, [eax+240]    ; in xmm0 metto i secondi 4 valori di x1
    movups xmm1, [ecx+240]    ; in xmm1 metto i secondi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff


    sub edx, 64      ;sottraggo i 16 elementi già presi
    add eax, 256     ;mi sposto di 16 elementi (64 posizioni)
    add ecx, 256

	  jmp for_64      ; Salto incondizionato

for_16:
    cmp edx, 16	    ; Confronto edx < 16 ?
    jl for_4    ; Se edx è strettamente minore di 16, gestisco il residuo

		;Loop Unrolling 1: 4 valori
    movups xmm0, [eax]    ; in xmm0 metto i primi 4 valori di x1
    movups xmm1, [ecx]    ; in xmm1 metto i primi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

		;Loop Unrolling 2: 4 valori
    movups xmm0, [eax+16]    ; in xmm0 metto i secondi 4 valori di x1
    movups xmm1, [ecx+16]    ; in xmm1 metto i secondi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

		;Loop Unrolling 3: 4 valori
		movups xmm0, [eax+32]    ; in xmm0 metto i primi 4 valori di x1
    movups xmm1, [ecx+32]    ; in xmm1 metto i primi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

		;Loop Unrolling 4: 4 valori
    movups xmm0, [eax+48]    ; in xmm0 metto i secondi 4 valori di x1
    movups xmm1, [ecx+48]    ; in xmm1 metto i secondi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff


    sub edx, 16      ;sottraggo i 16 elementi già presi
    add eax, 64     ;mi sposto di 16 elementi (64 posizioni)
    add ecx, 64

	  jmp for_16      ; Salto incondizionato

for_4:
    cmp edx, 4	    ; Confronto edx < 4 ? salta al residuo
	  jl for_remain   ; Se mancano meno di 4 elementi vai alla gestione residuo

    movups xmm0, [eax]    ; in xmm0 metto gli ultimi 4 valori di x1
    movups xmm1, [ecx]    ; in xmm1 metto gli ultimi 4 valori di x2

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
;    popad
    pop   edx
    pop   ecx
    pop   eax
    mov   esp, ebp  ; ripristina lo Stack Pointer
    pop   ebp       ; ripristina il Base Pointer
    ret             ; ritorna alla funzione chiamante
