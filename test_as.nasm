section .data			; Sezione contenente dati inizializzati

;uno:		dd		1.0
;
align 16      ;128 / 16
;inizio:		dd		1.0, 2.0, 3.0, 4.0

section .bss			; Sezione contenente dati non inizializzati

;alignb 16
;ris:		resd	1

section .text			; Sezione contenente il codice macchina


global test_distance      ; rende la funzione visibile all’esterno

; Posizione dei parametri nel Recordi di Attivazione della funzione
; (i primi 8 bytes sono occupati dall’indirizzo di ritorno e da EBP)

; intestazione in C
;int test_distance(VECTOR x1, VECTOR x2, int d);

x1   equ   8     ; puntatore a float x1, occupa 32 bit (4 bytes)
x2   equ   12    ; puntatore a float x2, occupa 32 bit (4 bytes)
d    equ   16    ; intero a 32 bit rappresenta la dimensione


test_distance:
    ;
    ; sequenza di ingresso nella funzione
    ;
    push    ebp       ; salva il Base Pointer
    mov     ebp, esp  ; il Base Pointer punta al Record di Attivazione corrente
;    push    ebx       ; salva i registri da preservare
;    push    esi
;    push    edi

    ;
    ; lettura dei parametri dal Recordi di Attivazione
    ;

    mov     eax, [ebp+x1]   ;in eax ho l'indirizzo di partenza di x1
    mov     ecx, [ebp+x2]   ;in ecx ho l'indirizzo di partenza di x2
    mov     edx, [ebp+d]    ;in edx ho la lunghezza dei vettori

    xorps xmm2, xmm2          ;azzero xmm2 dove terrò la somma

;LOOP UNROLLING???

for:
    movaps xmm0, [eax]    ; in xmm0 metto i primi 4 valori di x1
    movaps xmm1, [ecx]    ; in xmm1 metto i primi 4 valori di x2

    subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
    mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
    addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

    sub edx, 4      ;sottraggo i 4 elementi già presi
    add eax, 16     ;mi sposto di 4 elementi (16 posizioni)
    add ecx, 16

    cmp edx, 4	    ; Confronto edx con 8
	  jge for         ; Se edx è più grande o uguale a 8, salto al for

for_res:
    ;devo finire di gestire la parte residua dei vettori

    haddps xmm2, xmm2     ;sommo i primi due valori del registro
    haddps xmm2, xmm2     ;sommo i secondi due valori del registro

    sub esp, 4            ;sposo nel registro in cima allo stack il risultato
    movss [esp], xmm2     ;al fine di prenderlo da c
    fld dword [esp]
    add esp, 4



    ;    pop   edi       ; ripristina i registri da preservare
    ;    pop   esi
    ;    pop   ebx
        mov   esp, ebp  ; ripristina lo Stack Pointer
        pop   ebp       ; ripristina il Base Pointer
        ret             ; ritorna alla funzione chiamante


;Versione che usa la Floating Point Unit
;------------------------------------
;    mov   esi, [ebp+x1] ; ESI = address of vector x1
;     mov   edi, [ebp+x2] ; EDI = address of vector x2
;     mov   ebx, [ebp+d]  ; EBX = Number of elements in vectors
;
;   Traverse vectors in reverse order
;     fldz                ; Load zero on top of stack (sum starts at zero)
;
;     Sum the square of differences
;    nextvec:
;     Load the current vector elements
;     fld   dword [edi+ebx*4-4]
;     fld   dword [esi+ebx*4-4]
;     fsubp           ; Find the difference
;     fmul  st0       ; Square the value
;     faddp           ; Add it to the current sum
;     dec   ebx       ; Go to previous element in the array
;     jnz   nextvec   ; If EBX is zero we are finished

;;     fsqrt           ; Distance = sqrt of sum of the square of the differences
                     ; Distance is at the top of the stack in ST0
                     ; Functions returning a float place the result in ST0
                     ; to the caller

;-----------------------------
