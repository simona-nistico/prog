section .data			; Sezione contenente dati inizializzati

section .bss			; Sezione contenente dati non inizializzati

section .text			; Sezione contenente il codice macchina

global objective_function64

objective_function64:
		; ------------------------------------------------------------
		; Sequenza di ingresso nella funzione
		; ------------------------------------------------------------
		push	rbp		; salva il Base Pointer
		mov		rbp, rsp	; il Base Pointer punta al Record di Attivazione corrente
		push	rax		; salva i registri generali
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

		;rdi (r6) = n
		;rsi (r5) = m
		;rdx (r3) = distances_from_centroids


		;--------------------------------
		;ESECUZIONE
		;---------------------------------

		mov   rbx, rdi       ;in rbx ho il numero di punti (n)
		mov 	rcx, rsi       ;in rcx ho il numero di gruppi (m)
		; in rdx ci sono già la distanze dai centroidi
		imul rbx,rcx
		vxorps ymm0,ymm0


	for_32:
		cmp rbx, 8	       ; Confronto n*m < 8 ?
			jl for_8         ; Se edx è strettamente minore di 8, gestisco il residuo

		vaddps ymm0,[rdx]      ;sum += distances_from_centroids 0...8
		vaddps ymm0,[rdx+32]
		vaddps ymm0,[rdx+64]
		vaddps ymm0,[rdx+96]

  	sub rbx, 32            ;sottraggo i 32 elementi già presi
    add rdx, 128           ;mi sposto di 32 elementi (128 posizioni)

    jmp for_32             ; Verifico dopo se andare ad 8

  for_8:
		cmp rbx, 8	       ; Confronto n*m < 8  ? salta al for4
		jl for_4           ; Se mancano meno di 8 elementi vai alla gestione residuo

		vaddps ymm0, [rdx]

		sub rbx, 8             ;sottraggo gli 8 elementi già presi
    add rdx, 32            ;mi sposto di 8 elementi (32 posizioni)

		jmp for_8              ; salto incondizionato tanto la condizione la vedo dopo

  for_4:
		cmp rbx, 4	       ; Confronto n*m < 4 ? salta al residuo
	  jl for_remain      ; Se mancano meno di 4 elementi vai alla gestione residuo

		addps xmm0,[rdx]       ;sum += distances_from_centroids 0...4

		sub rbx, 4             ;sottraggo i 4 elementi già presi
		add rdx, 16            ;mi sposto di 4 elementi (16 posizioni)

		jmp for_4				; salto incondizionato tanto la condizione la vedo dopo

 	for_remain:

		cmp rbx, 0	    ; n*m == 0? fine
  		je end

 		addss xmm0, [rdx]    ; in xmm0 metto gli ultimi <4 valori di x1

		dec rbx         ;sottraggo 1 elementi già preso
		add rdx, 4     ;mi sposto di 1 elemento (4 posizioni)

    jmp for_remain    ; salto incondizionato tanto la condizione la vedo dopo

  end:
					  ;ymm2= [y1|y2|y3|y4|y5|y6|y7|y8]
					  ;	   A1    A2    B1    B2

		vhaddps ymm0,ymm0                ; prima riduzione   ymm2= [A1|A2|A1|A2|B1|B2|B1|B2]
                                                          ;      A1+A2=A3    B1+B2=B3
		vhaddps ymm0,ymm0                ; seconda riduzione ymm2= [A3|A3|A3|A3|B3|B3|B3|B3]
		vperm2f128 ymm1,ymm0,ymm0,1      ;                   ymm0= [X |X |X |X |A3|A3|A3|A3]
		vaddss xmm0,xmm1                 ; A3+B3 finisce in testa a xmm0 (verrà quindi ritornato in uscita)


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
		pop	rax
		mov	rsp, rbp	; ripristina lo Stack Pointer
		pop	rbp				; ripristina il Base Pointer
		ret						; torna alla funzione C chiamante
