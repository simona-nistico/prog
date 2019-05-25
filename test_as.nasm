section .data			; Sezione contenente dati inizializzati

;uno:		dd		1.0
;
;align 16
;inizio:		dd		1.0, 2.0, 3.0, 4.0

section .bss			; Sezione contenente dati non inizializzati

;alignb 16
;vec2:		resq	4

section .text			; Sezione contenente il codice macchina


global test_distance      ; rende la funzione visibile all’esterno

; Posizione dei parametri nel Recordi di Attivazione della funzione
; (i primi 8 bytes sono occupati dall’indirizzo di ritorno e da EBP)

; intestazione in C
;int test_distance(VECTOR x1, VECTOR x2, int d);

x1   equ   8     ; puntatore a float x1, occupa 32 bit (4 bytes)
x2   equ   12    ; puntatore a float x2, occupa 32 bit (4 bytes)
d    equ   16    ; intero a 32 bit rappresenta la dimensione

num1 equ 10
num2 equ 12


test_distance:

    ;
    ; sequenza di ingresso nella funzione
    ;
    push    ebp       ; salva il Base Pointer
    mov     ebp, esp  ; il Base Pointer punta al Record di Attivazione corrente
    push    ebx       ; salva i registri da preservare
    push    esi
    push    edi

    ;
    ; lettura dei parametri dal Recordi di Attivazione
    ;
  ;  movaps xmm0, [ebp+x1]  ; legge x1
  ;  movaps xmm1, [ebp+x2]  ; legge x2
     ;fadd [ebp+d]

    ;fld num1   ; This pushes num1 onto the stack at the address of st(0)
    ;fld num2   ; This pushes num2 onto the stack at the address of st(1)
    ;faddp      ; This multiplies st(1) and st(0), stores the result at st(0), and then pops the variable stored at st(1) off the stack


  ;  diff = x1[i]-x2[i];
  ;  sum += diff*diff;
  ;  mov ecx, [ebp+d]  ; legge d
    ;
    ; corpo della funzione
    ;

  ;  mov eax, ecx; asmfunc restituisce un numero intero

    ;
    ; sequenza di uscita dalla funzione
    ;
    pop   edi       ; ripristina i registri da preservare
    pop   esi
    pop   ebx
    mov   esp, ebp  ; ripristina lo Stack Pointer
    pop   ebp       ; ripristina il Base Pointer
    ret             ; ritorna alla funzione chiamante
