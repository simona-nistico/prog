section .data			; Sezione contenente dati inizializzati

section .bss			; Sezione contenente dati non inizializzati

section .text			; Sezione contenente il codice macchina


global residual64

   residual64:
		; ------------------------------------------------------------
		; Sequenza di ingresso nella funzione
		; ------------------------------------------------------------
		push	rbp		; salva il Base Pointer
		mov		rbp, rsp	; il Base Pointer punta al Record di Attivazione corrente
		push	rbx
		push	rcx
		push	rdx
		push	rsi
		push	rdi
		push	r8
		push	r9
		push	r10
		push	r11
		push	r12
		push	r13
		push	r14
		push	r15
		;--------------------------------
		;PARAMETRI
		;--------------------------------
;extern void test_residual(VECTOR res, VECTOR x,VECTOR centroid,int d);

		;rdi (r6) =  indirizzo di partenza dell'array res
		;rsi (r5) =  indirizzo di partenza dell'array x
		;rdx (r3) =  indirizzo di partenza dell'array centroid
		;rcx (r2) =  in rcx si trova il valore di d

		mov 		eax, [ebp+res]		;mi salvo l'indirizzo di partenza del residuo (risultato)
		mov     ebx, [ebp+x]       ;in ebx ho l'indirizzo di partenza di x
		mov     ecx, [ebp+centr]   ;in edx ho l'indirizzo di partenza del centroide
		mov     edi, [ebp+d]      ;in edi ho la lunghezza dei vettori


		mov rax,rdi		;rax = indirizzo di partenza del risultato
		mov rbx,rsi		;rbx = indirizzo di partenza di x

		mov rdi,rcx		;rdi = d
		mov rcx,rdx		;rcx = indirizzo di partenza di centroid


for_32:

	  cmp rdi, 32	       		; Confronto d < 8 ?
  	jl for_8           	  ; Se edx è strettamente minore di 8, gestisco il residuo

		vmovaps ymm0,[rbx]     ; i primi 8 elementi di x vanno in ymm0
		vsubps ymm0,[rcx]      ; sottraggo i primi 8 elementi di cent dai primi 8 elementi di x
		vmovaps [rax], ymm0    ; salvo la differenza dei primi 8 elementi nel risultato

		vmovaps ymm0,[rbx+32]
		vsubps ymm0,[rcx+32]
		vmovaps [rax+32], ymm0

		vmovaps ymm0,[rbx+64]
		vsubps ymm0,[rcx+64]
		vmovaps [rax+64], ymm0

		vmovaps ymm0,[rbx+96]
		vsubps ymm0,[rcx+96]
		vmovaps [rax+96], ymm0

  	sub rdi, 32            ;sottraggo i 32 elementi già presi
    add rax, 128           ;mi sposto di 32 elementi (128 posizioni)
		add rbx, 128
		add rcx, 128

		jmp for_32

for_8:
    cmp rdi, 8	       ; Confronto n*m < 8  ? salta al for4
		jl for_4               ; Se mancano meno di 8 elementi vai al for_4

		vmovaps ymm0,[rbx]     ; i primi 8 elementi di x vanno in ymm0
		vsubps ymm0,[rcx]      ; sottraggo i primi 8 elementi di cent dai primi 8 elementi di x
		vmovaps [rax], ymm0

  	sub rdi, 8            ;sottraggo i 32 elementi già presi
    add rax, 32           ;mi sposto di 8 elementi (32 posizioni)
		add rbx, 32
		add rcx, 32

		jmp for_8              ; salto incondizionato tanto la condizione la vedo dopo

for_4:

		cmp rdi, 4	           ; Confronto rdi < 4 ? salta al residuo
  	jl for_remain          ; Se mancano meno di 4 elementi vai alla gestione residuo

		movaps xmm0, [rbx]     ; in xmm0 metto i primi 4 valori di x
		movaps xmm1, [rcx]     ; in xmm1 metto i primi 4 valori di centr

		subps xmm0, xmm1       ; xmm0 = xmm0-xmm1  (x-centr)  x[i]-centroid[i];
		movaps [rax], xmm0     ; res[i] = x[i]-centroid[i]  salvo nel vettore i valori

		sub rdi, 4             ;sottraggo i 4 elementi già presi
		add rbx, 16            ;mi sposto di 4 elementi (16 posizioni)
		add rcx, 16
		add rax, 16

for_remain:

		cmp rdi, 0	        ; rdi == 0? fine
		je end

		movss xmm0, [rbx]    ; in xmm0 metto gli ultimi <4 valori di x
		movss xmm1, [rcx]    ; in xmm1 metto gli ultimi <4 valori di centroidi

		subss xmm0, xmm1     ; xmm0 = xmm0-xmm1  (x-centr)  x[i]-centroid[i];
		movss [rax], xmm0    ; res[i] = x[i]-centroid[i]  salvo nel vettore i valori

		dec rdi              ;sottraggo 1 elementi già preso
		add rbx, 4           ;mi sposto di 1 elemento (4 posizioni)
		add rcx, 4
		add rax, 4  	     ;mi sposto anche come indice

		jmp for_remain        ; salto incondizionato tanto la condizione la vedo dopo


end:

		; ------------------------------------------------------------
		; Sequenza di uscita dalla funzione
		; ------------------------------------------------------------

		pop	r15
		pop	r14
		pop	r13
		pop	r12
		pop	r11
		pop	r10
		pop	r9
		pop	r8
		pop	rdi
		pop	rsi
		pop	rdx
		pop	rcx
		pop	rbx
		mov	rsp, rbp	; ripristina lo Stack Pointer
		pop	rbp		; ripristina il Base Pointer
		ret			; torna alla funzione C chiamante
