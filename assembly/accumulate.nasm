section .data			; Sezione contenente dati inizializzati

section .bss			; Sezione contenente dati non inizializzati

section .text			; Sezione contenente il codice macchina


;Loop Unrolling 64, 16, 4, 1
;TESTATA

;________________________Funzione________________________
;void accumulate(MATRIX dest, MATRIX source, int dim);
global accumulate      ; rende la funzione visibile all’esterno

; Posizione dei parametri nel Recordi di Attivazione della funzione
; (i primi 8 bytes sono occupati dall’indirizzo di ritorno e da EBP)
dest   equ   8     ; puntatore a float destination, occupa 32 bit (4 bytes)
src    equ   12    ; puntatore a float source, occupa 32 bit (4 bytes)
d      equ   16    ; intero a 32 bit rappresenta la dimensione da accumulare

accumulate:
;------------------salva i parametri base------------------
    push    ebp       ; salva il Base Pointer
    mov     ebp, esp  ; il Base Pointer punta al Record di Attivazione corrente
    push    eax
    push    ecx
    push    edx
;    pushad


;---------lettura dei parametri dal Recordi di Attivazione---------
    mov     eax, [ebp+dest] ;in eax ho l'indirizzo di partenza di dest
    mov     ecx, [ebp+src]  ;in ecx ho l'indirizzo di partenza di source
    mov     edx, [ebp+d]    ;in edx ho la lunghezza dei vettori

for_64:
    cmp edx, 64	    ; Confronto edx < 64 ?
    jl for_16     ; Se edx è strettamente minore di 64, gestisco il residuo

  	;Loop Unrolling 1: 4 valori
    movaps xmm0, [eax]    ; xmm0 = tmp[centroide*d_star]
    addps xmm0, [ecx]     ; xmm0 = xmm0+[src]   ;tmp[centroide*d_star] += ds[i*d+(g*d_star)]
    movaps [eax], xmm0    ; tmp[centroide*d_star] = xmm0

  	;Loop Unrolling 2: 4 valori
    movaps xmm0, [eax+16]    ; xmm0 = tmp[centroide*d_star]
    addps xmm0, [ecx+16]     ; xmm0 = xmm0+[src]   ;tmp[centroide*d_star] += ds[i*d+(g*d_star)]
    movaps [eax+16], xmm0    ; tmp[centroide*d_star] = xmm0

  	;Loop Unrolling 3: 4 valori
    movaps xmm0, [eax+32]    ; xmm0 = tmp[centroide*d_star]
    addps xmm0, [ecx+32]     ; xmm0 = xmm0+[src]   ;tmp[centroide*d_star] += ds[i*d+(g*d_star)]
    movaps [eax+32], xmm0    ; tmp[centroide*d_star] = xmm0

  	;Loop Unrolling 4: 4 valori
    movaps xmm0, [eax+48]    ; xmm0 = tmp[centroide*d_star]
    addps xmm0, [ecx+48]     ; xmm0 = xmm0+[src]   ;tmp[centroide*d_star] += ds[i*d+(g*d_star)]
    movaps [eax+48], xmm0    ; tmp[centroide*d_star] = xmm0

    ;Loop Unrolling 5: 4 valori
    movaps xmm0, [eax+64]    ; xmm0 = tmp[centroide*d_star]
    addps xmm0, [ecx+64]     ; xmm0 = xmm0+[src]   ;tmp[centroide*d_star] += ds[i*d+(g*d_star)]
    movaps [eax+64], xmm0    ; tmp[centroide*d_star] = xmm0

    ;Loop Unrolling 6: 4 valori
    movaps xmm0, [eax+80]    ; xmm0 = tmp[centroide*d_star]
    addps xmm0, [ecx+80]     ; xmm0 = xmm0+[src]   ;tmp[centroide*d_star] += ds[i*d+(g*d_star)]
    movaps [eax+80], xmm0    ; tmp[centroide*d_star] = xmm0

    ;Loop Unrolling 7: 4 valori
    movaps xmm0, [eax+96]    ; xmm0 = tmp[centroide*d_star]
    addps xmm0, [ecx+96]     ; xmm0 = xmm0+[src]   ;tmp[centroide*d_star] += ds[i*d+(g*d_star)]
    movaps [eax+96], xmm0    ; tmp[centroide*d_star] = xmm0

    ;Loop Unrolling 8: 4 valori
    movaps xmm0, [eax+112]    ; xmm0 = tmp[centroide*d_star]
    addps xmm0, [ecx+112]     ; xmm0 = xmm0+[src]   ;tmp[centroide*d_star] += ds[i*d+(g*d_star)]
    movaps [eax+112], xmm0    ; tmp[centroide*d_star] = xmm0

    ;Loop Unrolling 9: 4 valori
    movaps xmm0, [eax+128]    ; xmm0 = tmp[centroide*d_star]
    addps xmm0, [ecx+128]     ; xmm0 = xmm0+[src]   ;tmp[centroide*d_star] += ds[i*d+(g*d_star)]
    movaps [eax+128], xmm0    ; tmp[centroide*d_star] = xmm0

    ;Loop Unrolling 10: 4 valori
    movaps xmm0, [eax+144]    ; xmm0 = tmp[centroide*d_star]
    addps xmm0, [ecx+144]     ; xmm0 = xmm0+[src]   ;tmp[centroide*d_star] += ds[i*d+(g*d_star)]
    movaps [eax+144], xmm0    ; tmp[centroide*d_star] = xmm0

    ;Loop Unrolling 11: 4 valori
    movaps xmm0, [eax+160]    ; xmm0 = tmp[centroide*d_star]
    addps xmm0, [ecx+160]     ; xmm0 = xmm0+[src]   ;tmp[centroide*d_star] += ds[i*d+(g*d_star)]
    movaps [eax+160], xmm0    ; tmp[centroide*d_star] = xmm0

    ;Loop Unrolling 12: 4 valori
    movaps xmm0, [eax+176]    ; xmm0 = tmp[centroide*d_star]
    addps xmm0, [ecx+176]     ; xmm0 = xmm0+[src]   ;tmp[centroide*d_star] += ds[i*d+(g*d_star)]
    movaps [eax+176], xmm0    ; tmp[centroide*d_star] = xmm0

    ;Loop Unrolling 13: 4 valori
    movaps xmm0, [eax+192]    ; xmm0 = tmp[centroide*d_star]
    addps xmm0, [ecx+192]     ; xmm0 = xmm0+[src]   ;tmp[centroide*d_star] += ds[i*d+(g*d_star)]
    movaps [eax+192], xmm0    ; tmp[centroide*d_star] = xmm0

    ;Loop Unrolling 14: 4 valori
    movaps xmm0, [eax+208]    ; xmm0 = tmp[centroide*d_star]
    addps xmm0, [ecx+208]     ; xmm0 = xmm0+[src]   ;tmp[centroide*d_star] += ds[i*d+(g*d_star)]
    movaps [eax+208], xmm0    ; tmp[centroide*d_star] = xmm0

    ;Loop Unrolling 15: 4 valori
    movaps xmm0, [eax+224]    ; xmm0 = tmp[centroide*d_star]
    addps xmm0, [ecx+224]     ; xmm0 = xmm0+[src]   ;tmp[centroide*d_star] += ds[i*d+(g*d_star)]
    movaps [eax+224], xmm0    ; tmp[centroide*d_star] = xmm0

    ;Loop Unrolling 16: 4 valori
    movaps xmm0, [eax+240]    ; xmm0 = tmp[centroide*d_star]
    addps xmm0, [ecx+240]     ; xmm0 = xmm0+[src]   ;tmp[centroide*d_star] += ds[i*d+(g*d_star)]
    movaps [eax+240], xmm0    ; tmp[centroide*d_star] = xmm0


    sub edx, 64      ;sottraggo i 16 elementi già presi
    add eax, 256     ;mi sposto di 16 elementi (64 posizioni)
    add ecx, 256

    jmp for_64      ; Salto incondizionato


for_16:
    cmp edx, 16	    ; Confronto edx < 16 ?
    jl for_4     ; Se edx è strettamente minore di 16, gestisco il residuo

		;Loop Unrolling 1: 4 valori
    movaps xmm0, [eax]    ; xmm0 = tmp[centroide*d_star]
    addps xmm0, [ecx]     ; xmm0 = xmm0+[src]   ;tmp[centroide*d_star] += ds[i*d+(g*d_star)]
    movaps [eax], xmm0    ; tmp[centroide*d_star] = xmm0

		;Loop Unrolling 2: 4 valori
    movaps xmm0, [eax+16]    ; xmm0 = tmp[centroide*d_star]
    addps xmm0, [ecx+16]     ; xmm0 = xmm0+[src]   ;tmp[centroide*d_star] += ds[i*d+(g*d_star)]
    movaps [eax+16], xmm0    ; tmp[centroide*d_star] = xmm0

		;Loop Unrolling 3: 4 valori
    movaps xmm0, [eax+32]    ; xmm0 = tmp[centroide*d_star]
    addps xmm0, [ecx+32]     ; xmm0 = xmm0+[src]   ;tmp[centroide*d_star] += ds[i*d+(g*d_star)]
    movaps [eax+32], xmm0    ; tmp[centroide*d_star] = xmm0

		;Loop Unrolling 4: 4 valori
    movaps xmm0, [eax+48]    ; xmm0 = tmp[centroide*d_star]
    addps xmm0, [ecx+48]     ; xmm0 = xmm0+[src]   ;tmp[centroide*d_star] += ds[i*d+(g*d_star)]
    movaps [eax+48], xmm0    ; tmp[centroide*d_star] = xmm0

    sub edx, 16      ;sottraggo i 16 elementi già presi
    add eax, 64     ;mi sposto di 16 elementi (64 posizioni)
    add ecx, 64

	  jmp for_16      ; Salto incondizionato

for_4:
    cmp edx, 4	    ; Confronto edx < 4 ? salta al residuo
	  jl for_remain   ; Se mancano meno di 4 elementi vai alla gestione residuo

    movaps xmm0, [eax]    ; xmm0 = tmp[centroide*d_star]
    addps xmm0, [ecx]     ; xmm0 = xmm0+[src]   ;tmp[centroide*d_star] += ds[i*d+(g*d_star)]
    movaps [eax], xmm0    ; tmp[centroide*d_star] = xmm0

    sub edx, 4      ;sottraggo i 4 elementi già presi
    add eax, 16     ;mi sposto di 4 elementi (16 posizioni)
    add ecx, 16

		jmp for_4    ; salto incondizionato tanto la condizione la vedo dopo

for_remain:
    cmp edx, 0	    ; edx == 0? fine
    je end

    movss xmm0, [eax]    ; xmm0 = tmp[centroide*d_star]
    addss xmm0, [ecx]     ; xmm0 = xmm0+[src]   ;tmp[centroide*d_star] += ds[i*d+(g*d_star)]
    movss [eax], xmm0    ; tmp[centroide*d_star] = xmm0

    dec edx         ;sottraggo 1 elementi già preso
    add eax, 4     ;mi sposto di 1 elemento (4 posizioni)
    add ecx, 4

	  jmp for_remain    ; salto incondizionato tanto la condizione la vedo dopo

end:

;------------------ripristina i parametri base------------------
;    popad
    pop   edx
    pop   ecx
    pop   eax
    mov   esp, ebp  ; ripristina lo Stack Pointer
    pop   ebp       ; ripristina il Base Pointer
    ret             ; ritorna alla funzione chiamante
