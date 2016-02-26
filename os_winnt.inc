

; OS Specific Functions
align 4
read_sector: ; eax:#, edi: buffer re
	push	ebx ecx edx esi edi
	;
	mov	esi, hdimg
	shl	eax, 9
	add	esi, eax
	shl	ecx, 7
	cld
	rep	movsd
	;
	pop	edi esi edx ecx ebx
	xor	eax, eax
	ret












align 4
os_malloc: ; eax = bytes, out: eax=0|handler
	push	ebx
	mov	ebx, dword[malloc]
	add	dword[malloc], eax
	mov	eax, ebx
	pop	ebx
ret

align 4
os_free: ; eax = handler

ret

align 4
os_memmove: ; ESI<, EDI>, ECX
	push	esi edi ecx
	cld
	rep	movsb
	pop	ecx edi esi
ret