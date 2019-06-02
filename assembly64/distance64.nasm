section .data			; Sezione contenente dati inizializzati

section .bss			; Sezione contenente dati non inizializzati

section .text			; Sezione contenente il codice macchina

;TESTATA

global distance64

distance64:
		; ------------------------------------------------------------
		; Sequenza di ingresso nella funzione
		; ------------------------------------------------------------
		push	rbp				; salva il Base Pointer
		mov		rbp, rsp	; il Base Pointer punta al Record di Attivazione corrente
		push	rax				; salva i registri generali
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

		;rdi (r6) = indirizzo di partenza dell'array x1
		;rsi (r5) = indirizzo di partenza dell'array x2
		;rdx (r5) = dimensione n degli array x ed y

		;--------------------------------
		;ESECUZIONE
		;---------------------------------

		mov rax,rdi ; metto in rax l'indirizzo di partenza di x1
		mov rcx,rsi ; metto in rcx l'indirizzo di partenza di x2
		;la dimensione n dei vettori si trova già in rdx

		vxorps	ymm2, ymm2 ; azzero ymm2 dove metterò la somma

   for_32:
	 		cmp rdx, 32	    ; Confronto edx < 32 ?
  			jl for_8      ; Se edx è strettamente minore di 32, gestisco il residuo

			;Loop Unrolling 1: 8 valori
		  vmovups ymm0, [rax]    ; in ymm0 metto i primi 8 valori di x1
 		  vmovups ymm1, [rcx]    ; in ymm1 metto i primi 8 valori di x2

  		vsubps ymm0, ymm1      ; ymm0 = ymm0-ymm1  (x1-x2)  diff = x1[i]-x2[i];
 		  vmulps ymm0, ymm0      ; ymm0 = ymm0*ymm0           diff*diff;
 		  vaddps ymm2, ymm0      ; ymm2 = ymm2+ymm0           sum += diff*diff

		  ;Loop Unrolling 2: 8 valori
		  vmovups ymm0, [rax+32]    ; in ymm0 metto i secondi 8 valori di x1
 		  vmovups ymm1, [rcx+32]    ; in ymm1 metto i secondi 8 valori di x2

  		vsubps ymm0, ymm1      ; ymm0 = ymm0-ymm1  (x1-x2)  diff = x1[i]-x2[i];
 		  vmulps ymm0, ymm0      ; ymm0 = ymm0*ymm0           diff*diff;
 		  vaddps ymm2, ymm0      ; ymm2 = ymm2+ymm0           sum += diff*diff

		  ;Loop Unrolling 3: 8 valori
		  vmovups ymm0, [rax+64]    ; in ymm0 metto i terzi 8 valori di x1
 		  vmovups ymm1, [rcx+64]    ; in ymm1 metto i terzi 8 valori di x2

  		vsubps ymm0, ymm1      ; ymm0 = ymm0-ymm1  (x1-x2)  diff = x1[i]-x2[i];
 		  vmulps ymm0, ymm0      ; ymm0 = ymm0*ymm0           diff*diff;
 		  vaddps ymm2, ymm0      ; ymm2 = ymm2+ymm0           sum += diff*diff

 		  ;Loop Unrolling 4: 8 valori
		  vmovups ymm0, [rax+96]    ; in ymm0 metto i quarti 8 valori di x1
 		  vmovups ymm1, [rcx+96]   ; in ymm1 metto i quarti 8 valori di x2

  		vsubps ymm0, ymm1      ; ymm0 = ymm0-ymm1  (x1-x2)  diff = x1[i]-x2[i];
 		  vmulps ymm0, ymm0      ; ymm0 = ymm0*ymm0           diff*diff;
 		  vaddps ymm2, ymm0      ; ymm2 = ymm2+ymm0           sum += diff*diff


 		  sub rdx, 32      ;sottraggo i 32 elementi già presi
 		  add rax, 128     ;mi sposto di 32 elementi (128 posizioni)
 		  add rcx, 128

		  jmp for_32   	; Verifico dopo la condizione


    for_8:
  		cmp rdx, 8	    ; Confronto edx < 8 ? salta al for4
	  		jl for_4   			; Se mancano meno di 8 elementi vai alla gestione residuo

 		  vmovups ymm0, [rax]    ; in ymm0 metto gli ultimi 8 valori di x1
 		  vmovups ymm1, [rcx]    ; in ymm1 metto gli ultimi 8 valori di x2

 		  vsubps ymm0, ymm1      ; ymm0 = ymm0-ymm1  (x1-x2)  diff = x1[i]-x2[i];
  		vmulps ymm0, ymm0      ; ymm0 = ymm0*ymm0           diff*diff;
  		vaddps ymm2, ymm0      ; ymm2 = ymm2+ymm0           sum += diff*diff

		  sub rdx, 8      ;sottraggo gli 8 elementi già presi
		  add rax, 32     ;mi sposto di 8 elementi (32 posizioni)
		  add rcx, 32

	  	jmp for_8    ; salto incondizionato tanto la condizione la vedo dopo

		for_4:
		  cmp rdx, 4	    ; Confronto edx < 4 ? salta al for_remainder
		  jl for_remain     ; Se mancano meno di 4 elementi vai alla gestione residuo

		  movaps xmm0, [rax]    ; in xmm0 metto gli ultimi <4 valori di x1
 		  movaps xmm1, [rcx]    ; in xmm1 metto gli ultimi <4 valori di x2

 		  subps xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
 		  mulps xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
 		  addps xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

  		sub rdx,4         ;sottraggo 4 elementi già presi
 		  add rax, 16     ;mi sposto di 1 elemento (4 posizioni)
		  add rcx, 16

	  	jmp for_4    ; salto incondizionato tanto la condizione la vedo dopo

    for_remain:
   		 cmp rdx, 0	    ; edx == 0? fine
    		 je end

   		 movss xmm0, [rax]    ; in xmm0 metto gli ultimi <4 valori di x1
    	 movss xmm1, [rcx]    ; in xmm1 metto gli ultimi <4 valori di x2

  		 subss xmm0, xmm1      ; xmm0 = xmm0-xmm1  (x1-x2)  diff = x1[i]-x2[i];
   		 mulss xmm0, xmm0      ; xmm0 = xmm0*xmm0           diff*diff;
   		 addss xmm2, xmm0      ; xmm2 = xmm2+xmm0           sum += diff*diff

  		 dec rdx         ;sottraggo 1 elementi già preso
  		 add rax, 4      ;mi sposto di 1 elemento (4 posizioni)
  		 add rcx, 4

	  	 jmp for_remain    ; salto incondizionato tanto la condizione la vedo dopo

	    end:
																								  ;ymm2= [y1|y2|y3|y4|y5|y6|y7|y8]
																								  ;	   			A1    A2    B1    B2
				vhaddps ymm2,ymm2     ; prima riduzione   ymm2= [A1|A2|A1|A2|B1|B2|B1|B2]
		                                    ;      					A1+A2=A3    B1+B2=B3
				vhaddps ymm2,ymm2     ; seconda riduzione ymm2=	[A3|A3|A3|A3|B3|B3|B3|B3]
				vperm2f128 ymm0,ymm2,ymm2,1 ;             ymm0= [X |X |X |X |A3|A3|A3|A3]
				vaddss xmm0,xmm2      ; A3+B3 finisce in testa a xmm0 (verrà quindi ritornato in uscita)


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
		pop	rbp		; ripristina il Base Pointer
		ret			; torna alla funzione C chiamante
