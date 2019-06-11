
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

for_64:

		cmp rdx, 64	       ; Confronto n*m < 8 ?
		jl for_32               ; Se edx è strettamente minore di 64, gestisco il residuo

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


  	sub rdx, 64            ;sottraggo i 32 elementi già presi
    add rax, 256           ;mi sposto di 32 elementi (256 posizioni)
		add rcx, 256           ;mi sposto di 32 elementi (256 posizioni)
		jmp for_64


for_32:

	 	cmp rdx, 32	       ; Confronto n*m < 8 ?
    jl for_8               ; Se edx è strettamente minore di 8, gestisco il residuo

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


  	sub rdx, 32            ;sottraggo i 32 elementi già presi
    add rax, 128           ;mi sposto di 32 elementi (128 posizioni)
		add rcx, 128           ;mi sposto di 32 elementi (128 posizioni)
		jmp for_32


for_8:
		cmp rdx, 8	       ; Confronto n*m < 8  ? salta al for4
		jl for_remain               ; Se mancano meno di 8 elementi vai alla gestione residuo

		vmovaps ymm0,[rax]      ; copio i primi 8 valori di dest in ymm0
		vaddps  ymm0,[rcx]      ; aggiungo a ymm0 i primi 8 valori di source
		vmovaps [rax],ymm0      ; ricopio la somma dei primi 8 valori di dest e source in dest

		sub rdx, 8             ;sottraggo gli 8 elementi già presi
    add rax, 32            ;mi sposto di 8 elementi (32 posizioni)
		add rcx, 32	       ;mi sposto di 8 elementi (32 posizioni)

		jmp for_8              ; salto incondizionato tanto la condizione la vedo dopo


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
