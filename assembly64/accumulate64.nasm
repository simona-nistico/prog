section .data			; Sezione contenente dati inizializzati

section .bss			; Sezione contenente dati non inizializzati

section .text			; Sezione contenente il codice macchina

global accumulate

accumulate:
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
		;rsi (r5) = indirizzo di partenza dell'array source
		;rdx (r3) = dim

		mov rax,rdi ; rax= dest
		mov rcx,rsi ; rcx= source
		;dim è già in rdx

for_128:

		cmp rdx, 128	       ; Confronto n*m < 8 ?
		jl for_16               ; Se edx è strettamente minore di 64, gestisco il residuo

                ;Loop Unrolling 1: 8 valori
		vmovaps ymm0,[rax]      ; copio i primi 8 valori di dest in ymm0
		vaddps  ymm0,[rcx]      ; aggiungo a ymm0 i primi 8 valori di source
		vmovaps [rax],ymm0      ; ricopio la somma dei primi 8 valori di dest e source in dest

                ;Loop Unrolling 2: 8 valori
		vmovaps ymm0,[rax+32]      ; copio i secondi 8 valori di dest in ymm0
		vaddps  ymm0,[rcx+32]      ; aggiungo a ymm0 i secondi 8 valori di source
		vmovaps [rax+32],ymm0      ; ricopio la somma dei secondi 8 valori di dest e source in dest

                ;Loop Unrolling 3: 8 valori
		vmovaps ymm0,[rax+64]      ; copio i terzi 8 valori di dest in ymm0
		vaddps  ymm0,[rcx+64]      ; aggiungo a ymm0 i terzi 8 valori di source
		vmovaps [rax+64],ymm0      ; ricopio la somma dei terzi 8 valori di dest e source in dest

                ;Loop Unrolling 4: 8 valori
		vmovaps ymm0,[rax+96]      ; copio i quarti 8 valori di dest in ymm0
		vaddps  ymm0,[rcx+96]      ; aggiungo a ymm0 i quarti 8 valori di source
		vmovaps [rax+96],ymm0      ; ricopio la somma dei quarti 8 valori di dest e source in dest

                ;Loop Unrolling 5: 8 valori
		vmovaps ymm0,[rax+128]      ; copio i quarti 8 valori di dest in ymm0
		vaddps  ymm0,[rcx+128]      ; aggiungo a ymm0 i quinti 8 valori di source
		vmovaps [rax+128],ymm0      ; ricopio la somma dei quinti 8 valori di dest e source in dest

                ;Loop Unrolling 6: 8 valori
		vmovaps ymm0,[rax+160]      ; copio i quarti 8 valori di dest in ymm0
		vaddps  ymm0,[rcx+160]      ; aggiungo a ymm0 i sesti 8 valori di source
		vmovaps [rax+160],ymm0      ; ricopio la somma dei sesti 8 valori di dest e source in dest

                ;Loop Unrolling 7: 8 valori
		vmovaps ymm0,[rax+192]      ; copio i quarti 8 valori di dest in ymm0
		vaddps  ymm0,[rcx+192]      ; aggiungo a ymm0 i settimi 8 valori di source
		vmovaps [rax+192],ymm0      ; ricopio la somma dei settimi 8 valori di dest e source in dest

                ;Loop Unrolling 8: 8 valori
		vmovaps ymm0,[rax+224]      ; copio i quarti 8 valori di dest in ymm0
		vaddps  ymm0,[rcx+224]      ; aggiungo a ymm0 gli ottavi 8 valori di source
		vmovaps [rax+224],ymm0      ; ricopio la somma degli ottavi 8 valori di dest e source in dest


				;Loop Unrolling 9: 8 valori
		vmovaps ymm0,[rax+256]      ; copio i primi 8 valori di dest in ymm0
		vaddps  ymm0,[rcx+256]      ; aggiungo a ymm0 i primi 8 valori di source
		vmovaps [rax+256],ymm0      ; ricopio la somma dei primi 8 valori di dest e source in dest

				;Loop Unrolling 10: 8 valori
		vmovaps ymm0,[rax+288]      ; copio i secondi 8 valori di dest in ymm0
		vaddps  ymm0,[rcx+288]      ; aggiungo a ymm0 i secondi 8 valori di source
		vmovaps [rax+288],ymm0      ; ricopio la somma dei secondi 8 valori di dest e source in dest

				;Loop Unrolling 11: 8 valori
		vmovaps ymm0,[rax+320]      ; copio i terzi 8 valori di dest in ymm0
		vaddps  ymm0,[rcx+320]      ; aggiungo a ymm0 i terzi 8 valori di source
		vmovaps [rax+320],ymm0      ; ricopio la somma dei terzi 8 valori di dest e source in dest

				;Loop Unrolling 12: 8 valori
		vmovaps ymm0,[rax+352]      ; copio i quarti 8 valori di dest in ymm0
		vaddps  ymm0,[rcx+352]      ; aggiungo a ymm0 i quarti 8 valori di source
		vmovaps [rax+352],ymm0      ; ricopio la somma dei quarti 8 valori di dest e source in dest

				;Loop Unrolling 13: 8 valori
		vmovaps ymm0,[rax+384]      ; copio i quarti 8 valori di dest in ymm0
		vaddps  ymm0,[rcx+384]      ; aggiungo a ymm0 i quinti 8 valori di source
		vmovaps [rax+384],ymm0      ; ricopio la somma dei quinti 8 valori di dest e source in dest

				;Loop Unrolling 14: 8 valori
		vmovaps ymm0,[rax+416]      ; copio i quarti 8 valori di dest in ymm0
		vaddps  ymm0,[rcx+416]      ; aggiungo a ymm0 i sesti 8 valori di source
		vmovaps [rax+416],ymm0      ; ricopio la somma dei sesti 8 valori di dest e source in dest

				;Loop Unrolling 15: 8 valori
		vmovaps ymm0,[rax+448]      ; copio i quarti 8 valori di dest in ymm0
		vaddps  ymm0,[rcx+448]      ; aggiungo a ymm0 i settimi 8 valori di source
		vmovaps [rax+448],ymm0      ; ricopio la somma dei settimi 8 valori di dest e source in dest

				;Loop Unrolling 16: 8 valori
		vmovaps ymm0,[rax+480]      ; copio i quarti 8 valori di dest in ymm0
		vaddps  ymm0,[rcx+480]      ; aggiungo a ymm0 gli ottavi 8 valori di source
		vmovaps [rax+480],ymm0      ; ricopio la somma degli ottavi 8 valori di dest e source in dest


  	sub rdx, 128            ;sottraggo i 32 elementi già presi
    add rax, 512           ;mi sposto di 32 elementi (256 posizioni)
		add rcx, 512           ;mi sposto di 32 elementi (256 posizioni)
		jmp for_128


for_16:

	 	cmp rdx, 16	       ; Confronto n*m < 8 ?
    jl for_remain              ; Se edx è strettamente minore di 8, gestisco il residuo

                ;Loop Unrolling 1: 8 valori
		vmovaps ymm0,[rax]      ; copio i primi 8 valori di dest in ymm0
		vaddps  ymm0,[rcx]      ; aggiungo a ymm0 i primi 8 valori di source
		vmovaps [rax],ymm0      ; ricopio la somma dei primi 8 valori di dest e source in dest

                ;Loop Unrolling 2: 8 valori
		vmovaps ymm0,[rax+32]      ; copio i secondi 8 valori di dest in ymm0
		vaddps  ymm0,[rcx+32]      ; aggiungo a ymm0 i secondi 8 valori di source
		vmovaps [rax+32],ymm0      ; ricopio la somma dei secondi 8 valori di dest e source in dest


  	sub rdx, 16            ;sottraggo i 32 elementi già presi
    add rax, 64           ;mi sposto di 32 elementi (128 posizioni)
		add rcx, 64           ;mi sposto di 32 elementi (128 posizioni)
		jmp for_16



for_remain:
			cmp edx, 0	    ; edx == 0? fine
			je end

  		vmovss xmm0, [rax]    ; xmm0 = tmp[centroide*d_star]
  		vaddss xmm0, [rcx]     ; xmm0 = xmm0+[src]   ;tmp[centroide*d_star] += ds[i*d+(g*d_star)]
  		vmovss [rax], xmm0    ; tmp[centroide*d_star] = xmm0

  		dec rdx         ;sottraggo 1 elementi già preso
  		add rax, 4     ;mi sposto di 1 elemento (4 posizioni)
  		add rcx, 4

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
