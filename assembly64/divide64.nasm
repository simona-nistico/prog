
section .data			; Sezione contenente dati inizializzati

section .bss			; Sezione contenente dati non inizializzati

section .text			; Sezione contenente il codice macchina

global divide64

divide64:
		; ------------------------------------------------------------
		; Sequenza di ingresso nella funzione
		; ------------------------------------------------------------
		push	rbp		; salva il Base Pointer
		mov	rbp, rsp	; il Base Pointer punta al Record di Attivazione corrente
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
		
		;rdi (r6) = indirizzo di partenza dell'array dest
		;rsi (r5) = indirizzo di partenza dell'array dividendo
		;xmm0 = divisore
		;rdx (r4) = dim

		;mov [rdi],rdx           ; per caricare qualcosa in xmm1 bisogna passare dalla memoria
		;movss xmm1, [rdi]       ;per questo rdx viene prima caricato in testa a rdi

		;movss xmm1,xmm0
		shufps xmm0, xmm0, 0x0          ;copio il valore val su tutto xmm0
		vperm2f128 ymm0,ymm0,ymm0,0     ;copio il valore in xmm0 su tutto ymm0

		mov rax, rdi
	        mov rbx, rsi

	  for_128:

	        cmp rdx, 128	       ; Confronto n*m < 8 ?
                jl for_64               ; Se edx è strettamente minore di 8, gestisco il residuo


                ;Loop Unrolling 1: 8 valori
		vmovaps ymm1,[rbx]      ; copio i primi 8 valori di dest in ymm0
		vdivps  ymm1,ymm0       ; aggiungo a ymm0 i primi 8 valori di source
		vmovaps [rax],ymm1      ; ricopio la somma dei primi 8 valori di dest e source in dest

                ;Loop Unrolling 2: 8 valori
		vmovaps ymm1,[rbx+32]      ; copio i secondi 8 valori di dest in ymm0
		vdivps  ymm1,ymm0      ; aggiungo a ymm0 i secondi 8 valori di source
		vmovaps [rax+32],ymm1      ; ricopio la somma dei secondi 8 valori di dest e source in dest

                ;Loop Unrolling 3: 8 valori
		vmovaps ymm1,[rbx+64]      ; copio i terzi 8 valori di dest in ymm0
		vdivps  ymm1,ymm0          ; aggiungo a ymm0 i terzi 8 valori di source
		vmovaps [rax+64],ymm1      ; ricopio la somma dei terzi 8 valori di dest e source in dest

                ;Loop Unrolling 4: 8 valori
		vmovaps ymm1,[rbx+96]      ; copio i quarti 8 valori di dest in ymm0
		vdivps  ymm1,ymm0          ; aggiungo a ymm0 i quarti 8 valori di source
		vmovaps [rax+96],ymm1      ; ricopio la somma dei quarti 8 valori di dest e source in dest

                ;Loop Unrolling 5: 8 valori
		vmovaps ymm1,[rbx+128]      ; copio i quinti 8 valori di dest in ymm0
		vdivps  ymm1,ymm0       ; aggiungo a ymm0 i quinti 8 valori di source
		vmovaps [rax+128],ymm1      ; ricopio la somma dei quinti 8 valori di dest e source in dest

                ;Loop Unrolling 6: 8 valori
		vmovaps ymm1,[rbx+160]      ; copio i sesti 8 valori di dest in ymm0
		vdivps  ymm1,ymm0      ; aggiungo a ymm0 i sesti 8 valori di source
		vmovaps [rax+160],ymm1      ; ricopio la somma dei sesti 8 valori di dest e source in dest

                ;Loop Unrolling 7: 8 valori
		vmovaps ymm1,[rbx+192]      ; copio i settimi 8 valori di dest in ymm0
		vdivps  ymm1,ymm0          ; aggiungo a ymm0 i settimi 8 valori di source
		vmovaps [rax+192],ymm1      ; ricopio la somma dei settimi 8 valori di dest e source in dest

                ;Loop Unrolling 8: 8 valori
		vmovaps ymm1,[rbx+224]      ; copio i ottavi 8 valori di dest in ymm0
		vdivps  ymm1,ymm0          ; aggiungo a ymm0 gli ottavi 8 valori di source
		vmovaps [rax+224],ymm1      ; ricopio la somma degli ottavi 8 valori di dest e source in dest

                ;Loop Unrolling 1: 8 valori
		vmovaps ymm1,[rbx+256]      ; copio i primi 8 valori di dest in ymm0
		vdivps  ymm1,ymm0       ; aggiungo a ymm0 i primi 8 valori di source
		vmovaps [rax+256],ymm1      ; ricopio la somma dei primi 8 valori di dest e source in dest

                ;Loop Unrolling 2: 8 valori
		vmovaps ymm1,[rbx+288]      ; copio i secondi 8 valori di dest in ymm0
		vdivps  ymm1,ymm0      ; aggiungo a ymm0 i secondi 8 valori di source
		vmovaps [rax+288],ymm1      ; ricopio la somma dei secondi 8 valori di dest e source in dest

                ;Loop Unrolling 3: 8 valori
		vmovaps ymm1,[rbx+320]      ; copio i terzi 8 valori di dest in ymm0
		vdivps  ymm1,ymm0          ; aggiungo a ymm0 i terzi 8 valori di source
		vmovaps [rax+320],ymm1      ; ricopio la somma dei terzi 8 valori di dest e source in dest

                ;Loop Unrolling 4: 8 valori
		vmovaps ymm1,[rbx+352]      ; copio i quarti 8 valori di dest in ymm0
		vdivps  ymm1,ymm0          ; aggiungo a ymm0 i quarti 8 valori di source
		vmovaps [rax+352],ymm1      ; ricopio la somma dei quarti 8 valori di dest e source in dest

                ;Loop Unrolling 5: 8 valori
		vmovaps ymm1,[rbx+384]      ; copio i quinti 8 valori di dest in ymm0
		vdivps  ymm1,ymm0       ; aggiungo a ymm0 i quinti 8 valori di source
		vmovaps [rax+384],ymm1      ; ricopio la somma dei quinti 8 valori di dest e source in dest

                ;Loop Unrolling 6: 8 valori
		vmovaps ymm1,[rbx+416]      ; copio i sesti 8 valori di dest in ymm0
		vdivps  ymm1,ymm0      ; aggiungo a ymm0 i sesti 8 valori di source
		vmovaps [rax+416],ymm1      ; ricopio la somma dei sesti 8 valori di dest e source in dest

                ;Loop Unrolling 7: 8 valori
		vmovaps ymm1,[rbx+448]      ; copio i settimi 8 valori di dest in ymm0
		vdivps  ymm1,ymm0          ; aggiungo a ymm0 i settimi 8 valori di source
		vmovaps [rax+448],ymm1      ; ricopio la somma dei settimi 8 valori di dest e source in dest

                ;Loop Unrolling 8: 8 valori
		vmovaps ymm1,[rbx+480]      ; copio i ottavi 8 valori di dest in ymm0
		vdivps  ymm1,ymm0          ; aggiungo a ymm0 gli ottavi 8 valori di source
		vmovaps [rax+480],ymm1      ; ricopio la somma degli ottavi 8 valori di dest e source in dest


  	        sub rdx, 128            ;sottraggo i 128 elementi già presi
    		add rax, 512           ;mi sposto di 128 elementi (512 posizioni)
		add rbx, 512           ;mi sposto di 128 elementi (512 posizioni)

		jmp for_128

	  for_64:

	        cmp rdx, 64	       ; Confronto n*m < 8 ?
                jl for_32               ; Se edx è strettamente minore di 8, gestisco il residuo


                ;Loop Unrolling 1: 8 valori
		vmovaps ymm1,[rbx]      ; copio i primi 8 valori di dest in ymm0
		vdivps  ymm1,ymm0       ; aggiungo a ymm0 i primi 8 valori di source
		vmovaps [rax],ymm1      ; ricopio la somma dei primi 8 valori di dest e source in dest

                ;Loop Unrolling 2: 8 valori
		vmovaps ymm1,[rbx+32]      ; copio i secondi 8 valori di dest in ymm0
		vdivps  ymm1,ymm0      ; aggiungo a ymm0 i secondi 8 valori di source
		vmovaps [rax+32],ymm1      ; ricopio la somma dei secondi 8 valori di dest e source in dest

                ;Loop Unrolling 3: 8 valori
		vmovaps ymm1,[rbx+64]      ; copio i terzi 8 valori di dest in ymm0
		vdivps  ymm1,ymm0          ; aggiungo a ymm0 i terzi 8 valori di source
		vmovaps [rax+64],ymm1      ; ricopio la somma dei terzi 8 valori di dest e source in dest

                ;Loop Unrolling 4: 8 valori
		vmovaps ymm1,[rbx+96]      ; copio i quarti 8 valori di dest in ymm0
		vdivps  ymm1,ymm0          ; aggiungo a ymm0 i quarti 8 valori di source
		vmovaps [rax+96],ymm1      ; ricopio la somma dei quarti 8 valori di dest e source in dest

                ;Loop Unrolling 5: 8 valori
		vmovaps ymm1,[rbx+128]      ; copio i quinti 8 valori di dest in ymm0
		vdivps  ymm1,ymm0       ; aggiungo a ymm0 i quinti 8 valori di source
		vmovaps [rax+128],ymm1      ; ricopio la somma dei quinti 8 valori di dest e source in dest

                ;Loop Unrolling 6: 8 valori
		vmovaps ymm1,[rbx+160]      ; copio i sesti 8 valori di dest in ymm0
		vdivps  ymm1,ymm0      ; aggiungo a ymm0 i sesti 8 valori di source
		vmovaps [rax+160],ymm1      ; ricopio la somma dei sesti 8 valori di dest e source in dest

                ;Loop Unrolling 7: 8 valori
		vmovaps ymm1,[rbx+192]      ; copio i settimi 8 valori di dest in ymm0
		vdivps  ymm1,ymm0          ; aggiungo a ymm0 i settimi 8 valori di source
		vmovaps [rax+192],ymm1      ; ricopio la somma dei settimi 8 valori di dest e source in dest

                ;Loop Unrolling 8: 8 valori
		vmovaps ymm1,[rbx+224]      ; copio i ottavi 8 valori di dest in ymm0
		vdivps  ymm1,ymm0          ; aggiungo a ymm0 gli ottavi 8 valori di source
		vmovaps [rax+224],ymm1      ; ricopio la somma degli ottavi 8 valori di dest e source in dest


  	        sub rdx, 64            ;sottraggo i 64 elementi già presi
    		add rax, 256           ;mi sposto di 64 elementi (256 posizioni)
		add rbx, 256           ;mi sposto di 64 elementi (256 posizioni)

		jmp for_64

	  for_32:

	        cmp rdx, 32	       ; Confronto n*m < 8 ?
                jl for_16               ; Se edx è strettamente minore di 8, gestisco il residuo


                ;Loop Unrolling 1: 8 valori
		vmovaps ymm1,[rbx]      ; copio i primi 8 valori di dest in ymm0
		vdivps  ymm1,ymm0       ; aggiungo a ymm0 i primi 8 valori di source
		vmovaps [rax],ymm1      ; ricopio la somma dei primi 8 valori di dest e source in dest

                ;Loop Unrolling 2: 8 valori
		vmovaps ymm1,[rbx+32]      ; copio i secondi 8 valori di dest in ymm0
		vdivps  ymm1,ymm0      ; aggiungo a ymm0 i secondi 8 valori di source
		vmovaps [rax+32],ymm1      ; ricopio la somma dei secondi 8 valori di dest e source in dest

                ;Loop Unrolling 3: 8 valori
		vmovaps ymm1,[rbx+64]      ; copio i terzi 8 valori di dest in ymm0
		vdivps  ymm1,ymm0          ; aggiungo a ymm0 i terzi 8 valori di source
		vmovaps [rax+64],ymm1      ; ricopio la somma dei terzi 8 valori di dest e source in dest

                ;Loop Unrolling 4: 8 valori
		vmovaps ymm1,[rbx+96]      ; copio i quarti 8 valori di dest in ymm0
		vdivps  ymm1,ymm0          ; aggiungo a ymm0 i quarti 8 valori di source
		vmovaps [rax+96],ymm1      ; ricopio la somma dei quarti 8 valori di dest e source in dest


  	        sub rdx, 32            ;sottraggo i 32 elementi già presi
    		add rax, 128           ;mi sposto di 32 elementi (128 posizioni)
		add rbx, 128           ;mi sposto di 32 elementi (128 posizioni)

		jmp for_32

	  for_16:
    		cmp rdx, 16	       ; Confronto n*m < 8  ? salta al for4
		jl for_8               ; Se mancano meno di 8 elementi vai alla gestione residuo

   	        vmovaps ymm1, [rbx]     ; xmm0 = tmp[i*d_star]
  	        vdivps ymm1, ymm0       ; xmm0 = xmm0/cont[i]   ;tmp[centroide*d_star]/cont[i]
  	        vmovaps [rax], ymm1     ; centroids[(i+g*k)*d_star] = xmm0

   	        vmovaps ymm1, [rbx+32]     ; xmm0 = tmp[i*d_star]
  	        vdivps ymm1, ymm0       ; xmm0 = xmm0/cont[i]   ;tmp[centroide*d_star]/cont[i]
  	        vmovaps [rax+32], ymm1     ; centroids[(i+g*k)*d_star] = xmm0

   	        sub rdx, 16             ;sottraggo i 16 elementi già presi
 	        add rax, 64            ;mi sposto di 16 elementi (64 posizioni)
                add rbx, 64

		jmp for_16    ; salto incondizionato tanto la condizione la vedo dopo

	  for_8:
    		cmp rdx, 8	       ; Confronto n*m < 8  ? salta al for4
		jl for_4               ; Se mancano meno di 8 elementi vai alla gestione residuo

   	        vmovaps ymm1, [rbx]     ; xmm0 = tmp[i*d_star]
  	        vdivps ymm1, ymm0       ; xmm0 = xmm0/cont[i]   ;tmp[centroide*d_star]/cont[i]
  	        vmovaps [rax], ymm1     ; centroids[(i+g*k)*d_star] = xmm0

   	        sub rdx, 8             ;sottraggo i 8 elementi già presi
 	        add rax, 32            ;mi sposto di 8 elementi (32 posizioni)
                add rbx, 32

		jmp for_8    ; salto incondizionato tanto la condizione la vedo dopo

	  for_4:

   		cmp rdx, 4	    ; Confronto edx < 4 ? salta al residuo
	        jl for_remain   ; Se mancano meno di 4 elementi vai alla gestione residuo
  
   	        movaps xmm1, [rbx]    ; xmm0 = tmp[i*d_star]
   	        divps xmm1, xmm0      ; xmm0 = xmm0/cont[i]   ;tmp[centroide*d_star]/cont[i]
  	        movaps [rax], xmm1    ; centroids[(i+g*k)*d_star] = xmm0

  	        sub rdx, 4      ;sottraggo i 4 elementi già presi
   	        add rax, 16     ;mi sposto di 4 elementi (16 posizioni)
                add rbx, 16

		jmp for_4    ; salto incondizionato tanto la condizione la vedo dopo

     for_remain:
  		cmp rdx, 0	    ; edx == 0? fine
    	        je end

    	        movss xmm1, [rbx]    ; xmm0 = tmp[i*d_star]
    	        divss xmm1, xmm0     ; xmm0 = xmm0/cont[i]   ;tmp[centroide*d_star]/cont[i]
    	        movss [rax], xmm1    ; centroids[(i+g*k)*d_star] = xmm0

  	        dec rdx         ;sottraggo 1 elementi già preso
    	        add rax, 4     ;mi sposto di 1 elemento (4 posizioni)
    	        add rbx, 4

	  	jmp for_remain    ; salto incondizionato tanto la condizione la vedo dopo


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
		pop	rax
		mov	rsp, rbp	; ripristina lo Stack Pointer
		pop	rbp		; ripristina il Base Pointer
		ret			; torna alla funzione C chiamante

