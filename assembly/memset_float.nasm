section .data			; Sezione contenente dati inizializzati

section .bss			; Sezione contenente dati non inizializzati

section .text			; Sezione contenente il codice macchina

;Loop Unrolling 64,16, 4, 1
;TESTATA

;________________________Funzione________________________
;void *memset(void *array, float f, size_t d)
global memset_float      ; rende la funzione visibile all’esterno

; Posizione dei parametri nel Recordi di Attivazione della funzione
; (i primi 8 bytes sono occupati dall’indirizzo di ritorno e da EBP)
array  equ   8     ; puntatore a string array, occupa 32 bit (4 bytes)
f      equ   12    ; numero float da copiare, occupa 32 bit (4 bytes)
dim    equ   16    ; intero a 32 bit rappresenta la dimensione

memset_float:
;------------------salva i parametri base------------------
    push    ebp       ; salva il Base Pointer
    mov     ebp, esp  ; il Base Pointer punta al Record di Attivazione corrente
    push    eax
    push    edx
;pushad


;---------lettura dei parametri dal Recordi di Attivazione---------
    mov     eax, [ebp+array]   ;in eax ho l'indirizzo di partenza dell'array
    mov     edx, [ebp+dim]     ;in edx ho la lunghezza del vettore

    movss  xmm0, [ebp+f]       ;prendo nella parte bassa il valore da copiare
    shufps xmm0, xmm0, 0x0     ;copio tale valore dappertutto


;    jmp for_remain

for_64:
    cmp edx, 64	    ; Confronto edx < 64 ?
    jl for_16    ; Se edx è strettamente minore di 64, gestisco il residuo

		;Loop Unrolling 1: 4 valori
    movaps [eax], xmm0    ; assegno il valore ai primi 4 elementi di array
    movaps [eax+16], xmm0    ; assegno il valore ai secondi 4 elementi di array
    movaps [eax+32], xmm0    ; assegno il valore ai primi 4 elementi di array
    movaps [eax+48], xmm0    ; assegno il valore ai primi 4 elementi di array

    movaps [eax+64], xmm0    ; assegno il valore ai primi 4 elementi di array
    movaps [eax+80], xmm0    ; assegno il valore ai secondi 4 elementi di array
    movaps [eax+96], xmm0    ; assegno il valore ai primi 4 elementi di array
    movaps [eax+112], xmm0    ; assegno il valore ai primi 4 elementi di array

    movaps [eax+128], xmm0    ; assegno il valore ai primi 4 elementi di array
    movaps [eax+144], xmm0    ; assegno il valore ai secondi 4 elementi di array
    movaps [eax+160], xmm0    ; assegno il valore ai primi 4 elementi di array
    movaps [eax+176], xmm0    ; assegno il valore ai primi 4 elementi di array

    movaps [eax+192], xmm0    ; assegno il valore ai primi 4 elementi di array
    movaps [eax+208], xmm0    ; assegno il valore ai secondi 4 elementi di array
    movaps [eax+224], xmm0    ; assegno il valore ai primi 4 elementi di array
    movaps [eax+240], xmm0    ; assegno il valore ai primi 4 elementi di array

    sub edx, 64      ;sottraggo i 16 elementi già presi
    add eax, 256     ;mi sposto di 16 elementi (64 posizioni)

    jmp for_64      ;salto incondizionato


for_16:
    cmp edx, 16	    ; Confronto edx < 16 ?
    jl for_4    ; Se edx è strettamente minore di 16, gestisco il residuo

		;Loop Unrolling 1: 4 valori
    movaps [eax], xmm0    ; assegno il valore ai primi 4 elementi di array
    movaps [eax+16], xmm0    ; assegno il valore ai secondi 4 elementi di array
    movaps [eax+32], xmm0    ; assegno il valore ai primi 4 elementi di array
    movaps [eax+48], xmm0    ; assegno il valore ai primi 4 elementi di array

    sub edx, 16      ;sottraggo i 16 elementi già presi
    add eax, 64     ;mi sposto di 16 elementi (64 posizioni)

    jmp for_16      ;salto incondizionato

for_4:

;    jmp end

    cmp edx, 4	    ; Confronto edx < 4 ? salta al residuo
	  jl for_remain   ; Se mancano meno di 4 elementi vai alla gestione residuo

    movaps [eax], xmm0    ; assegno il valore ai primi 4 elementi di array
    sub edx, 4      ;sottraggo i 4 elementi già presi
    add eax, 16     ;mi sposto di 4 elementi (16 posizioni)

		jmp for_4    ; salto incondizionato tanto la condizione la vedo dopo

for_remain:
    cmp edx, 0	    ; edx == 0? fine
    je end

    movss [eax], xmm0    ; in xmm0 metto gli ultimi <4 valori di array

    dec edx         ;sottraggo 1 elementi già preso
    add eax, 4     ;mi sposto di 1 elemento (4 posizioni)

	  jmp for_remain    ; salto incondizionato tanto la condizione la vedo dopo

end:


;------------------ripristina i parametri base------------------
    pop   edx
    pop   eax
    mov   esp, ebp  ; ripristina lo Stack Pointer
    pop   ebp       ; ripristina il Base Pointer
    ret             ; ritorna alla funzione chiamante
