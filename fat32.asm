use32
include 'fat_data.inc'

;TARGETOS EQU 'OS647'

os_rd_sector: ; eax=LBA#, edi: buffer, ecx: count
 if TARGETOS = 'NT'
  call	 read_sector
 end if
ret

if TARGETOS = 'OS647'
 os_malloc: ; eax=size, => eax=handler | -1
 ret

 os_free:   ; eax=handler, => -
 ret
end if

;pushad
;    push    EDI
;    invoke  CreateFile,   paths, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0
;    pop     EDI
;    mov     dword[fhand], eax
;    ; esi
;    mov     esi, EDI
;    mov     ecx, 512
;    invoke  WriteFile,    [fhand], esi, ecx, EBX, 0
;    invoke  CloseHandle,  dword[fhand]
;popad
;pushad
;  invoke  MessageBox,HWND_DESKTOP,paths,paths,MB_OK
;popad


;------------------------------------------------------------------------------;
align 4
fat32_init:	; in: -, out: eax=FAT handler | -1
      mov	eax,  512
      call	os_malloc
      cmp	eax,  -1
      je       .error
      mov	ebx,  eax
      ;
      mov	eax,  0
      mov	edi,  ebx
      mov	ecx,  1
      call	os_rd_sector
      cmp	eax, -1
      jne	@f
      mov	eax,  ebx
      call	os_free
      jmp      .error
  @@: mov	esi,  ebx
      add	esi,  512 - 2 - (4*16)
  @@: cmp	word[esi + 00], 0xAA55
      je       .error_mem1
      cmp	byte[esi + 04], FAT32LBA
      je       .finded
      add	esi,  16
      jmp	@b
      ;
 .finded:
      mov	eax,  65*1024
      call	os_malloc
      cmp	eax,  -1
      je       .error_mem1
      mov	edx,  eax

      ;
      mov	eax,  dword[esi + 08]			   ; StartLBA of Partition.
      mov	edi,  edx				   ;
      mov	ecx,  128				   ; 128 sectors = 64k
      call	os_rd_sector				   ;
      cmp	eax,  -1				   ;
      je       .error_mem2				   ;

      ; 						   ;
      mov	eax,  dword[esi + 08]			   ;
      mov	dword[edx + fat32.fat_start_lba], eax	   ;
      ; 						   ;
      mov	eax,  dword[edx + fat32.fat_sect_perfat]   ; sectors per fat
      shl	eax,  9 				   ; * 512
      mov	dword[edx + fat32.fat_fattblsiz], eax	   ; FATTbl Size in bytes.
      call	os_malloc				   ;
      cmp	eax, -1 				   ;
      je       .error_mem2				   ;
      mov	dword[edx + fat32.fat_fattbladr], eax	   ; FATTbl Addr
      mov	edi,  eax				   ; Buffer
      movzx	eax,   word[edx + fat32.fat_sectbefofat]   ; sectors before FAT Tables...
      add	eax,  dword[edx + fat32.fat_start_lba]	   ; add start LBA of partition.
      mov	ecx,  dword[edx + fat32.fat_sect_perfat]   ; num of sectors.
      call	os_rd_sector
      cmp	eax, -1
      je       .error_mem3
      ; root dir ClusterID:
      mov	eax, dword[edx + fat32.fat_rootcluster]
      mov	dword[edx + fat32.fat_currntdir], eax
      jmp      .ok
      ;
.error_mem3:
      mov	eax,  dword[edx + fat32.fat_fattbladr]	   ; FATTbl Buffer
      call	os_free 				   ;
.error_mem2:						   ;
      mov	eax,  edx				   ; free FAT buffer
      call	os_free 				   ;
.error_mem1:						   ;
      mov	eax,  ebx				   ; free MBR buffer
      call	os_free 				   ;
      jmp      .error					   ;
.error: 						   ;
      mov	eax,  -1				   ;
      ret						   ;
.ok:							   ;
      ; free MBR buffer:				   ;
      mov	eax,  ebx				   ; free MBR buffer
      call	os_free 				   ;
      ;
      ;
      mov	eax,  edx				   ; eax = FAT handler
      ret


align 4
fat32_rdcluster:   ; ecx=num, edx=handler, eax=ID, edi:buffer
      push	ebx ecx edx esi edi
      mov	ebx,  edx
      dec	eax					   ; Skip two	       ;
      dec	eax					   ;  service clusters ;
      ;
      movzx	edx,   word[ebx + fat32.fat_sectbefofat]   ; sectors before FAT Tables...
      add	edx,  dword[ebx + fat32.fat_start_lba]	   ; add start LBA of partition.
      push	eax   edx   ecx
      mov	ecx,  dword[ebx + fat32.fat_sect_perfat]
      movzx	eax,   byte[ebx + fat32.fat_copies]
      mul	ecx					   ; EDX:EAX=SPFat * FATCopies
      pop	ecx   edx
      add	edx,  eax
      pop	eax
      ; EAX cluster#->EAX sector#
      push	edx
      movzx	esi,   byte[ebx + fat32.fat_secperclust]
      mul	esi
      pop	edx
      add	eax,  edx
      ;
      call	os_rd_sector
      ;
      pop	edi esi edx ecx ebx
ret ; eax=readed bytes | -1

align 4
fat32_listindode: ; EBX=proc, ECX=inodeSz in sectors, EDI: address.
      pushad
      ;
      mov	 esi, edi
      shl	 ecx, 9 				   ; *512
   .next:
      cmp	 byte[esi + 00], 0xE5
      je	.skip
      cmp	 byte[esi + 00], 0x05
      je	.skip
      cmp	 byte[esi + 00], 0x00
      je	.finish
      call	 ebx ; esi: string
 .skip:
      add	 esi, 32
      sub	 ecx, 32
      cmp	 ecx, 0
      jl	.finish
      jmp	.next
      ;
 .finish:
      ;
      popad
ret ; out: -




align 4
fat32_nextcluster:  ; eax=ClusterID, edx=Handler
      push	ebx ecx edx esi edi
      ;
      mov	ebx, edx
      shl	eax, 2					   ; *4
      mov	edx, dword[ebx + fat32.fat_fattbladr]	   ; FAT Table
      mov	ecx, dword[ebx + fat32.fat_fattblsiz]	   ; FAT Table Size
      cmp	eax, ecx
      jae      .err
      ;
      add	edx, eax
      mov	eax, dword[edx + 00]
      and	eax, 0x0FFFFFFF
      cmp	eax, 0x0FFFFFFF
      je       .lst
      jmp      .znd
 .lst:xor	eax, eax
 .znd:jmp      .ret
 .err:mov	eax, -1
 .ret:pop	edi esi edx ecx ebx
      ret
; eax=IdOfCluster





align 4
fat32_ls:  ; eax=handler, ebx=proc.addr(esi=string,edx=pointer to inode)
      push	ebx ecx edx esi edi
      ;
      mov	edx,  eax
      ;
      movzx	eax,  byte[edx + fat32.fat_secperclust]    ; sectores per clst ;
      shl	eax,  9 				   ; * 512 (sect.size) ;
      call	os_malloc				   ;		       ;
      cmp	eax,  -1				   ;		       ;
      je       .error					   ;		       ;
      mov	edi,  eax				   ;		       ;
      ; 						   ;		       ;
      mov	eax,  dword[edx + fat32.fat_currntdir]	   ;		       ;
 .nxt:mov	esi,  eax				   ; save CurrCluster  ;
      mov	ecx,  1 				   ; 1 cluster	       ;
      call	fat32_rdcluster 			   ; ecx and edi set   ;
      cmp	eax,  -1				   ;		       ;
      je       .error_fm				   ; err + free buffer ;
      ; list inode (edi=addr, ecx=inodeSz in sectors, ebx=proc):	       ;

      call	fat32_listindode			   ; srEDI szECX prEBX ;
      mov	eax,  esi				   ; restore clusterId ;
      call	fat32_nextcluster			   ; EDX:handle, EAX:cl;
      cmp	eax,  -1
      je       .error_fm
      or	eax,  eax
      jz       .znd
      jmp      .nxt
      ;
   .znd:
      xor	eax,  eax
      jmp      .ret
   .error_fm:
      mov	eax,  edi
      call	os_free
   .error:
      mov	eax,  -1
   .ret:
      pop	edi esi edx ecx ebx
      ret
; eax=-1 -> error


align 4
fat32_strcmp:  ; esi, edi: strings, ecx: string size
      push	ecx
  @@: dec	ecx
      mov	al, byte[esi + ecx]
      cmp	byte[edi + ecx], al
      jnz      .er
      or	ecx, ecx
      jz       .ok
      jmp	@b
 .er: mov	eax, -1
      jmp      .en
 .ok: xor	eax, eax
 .en: pop	ecx
ret ; eax: -1 | 0



align 4
fat32_findfile:  ; eax=handler, esi=filename
      push	ebx ecx edx esi edi
      ;
      mov	ebx,  esi
      mov	edx,  eax
      ;
      movzx	eax,  byte[edx + fat32.fat_secperclust]    ; sectores per clst ;
      shl	eax,  9 				   ; * 512 (sect.size) ;
      call	os_malloc				   ;		       ;
      cmp	eax,  -1				   ;		       ;
      je       .error					   ;		       ;
      mov	edi,  eax				   ;		       ;
      ; 						   ;		       ;
      mov	eax,  dword[edx + fat32.fat_currntdir]	   ;		       ;
 .nxt:mov	esi,  eax				   ; save CurrCluster  ;
      mov	ecx,  1 				   ; 1 cluster	       ;
      call	fat32_rdcluster 			   ; ecx and edi set   ;
      cmp	eax,  -1				   ;		       ;
      je       .error_fm				   ; err + free buffer ;
      ; list inode (edi=addr, ecx=inodeSz in sectors, ebx=proc):	       ;





      push	 ecx  edi
      ;
      mov	 esi, edi
      shl	 ecx, 9 				   ; *512
   .next:
      cmp	 byte[esi + 00], 0xE5
      je	.skip
      cmp	 byte[esi + 00], 0x05
      je	.skip
      cmp	 byte[esi + 00], 0x00
      je	.finish

      push	 ecx edi
      mov	 ecx, 11
      mov	 edi, ebx
      call	 fat32_strcmp	; esi, edi: strings, ecx: string size
      pop	 edi ecx
      cmp	 eax, -1
      je	.skip
      mov	 ax, word[esi + 20] ;26
      shl	 eax, 16
      mov	 ax, word[esi + 26]
      jmp	.finish

 .skip:
      add	 esi, 32
      sub	 ecx, 32
      cmp	 ecx, 0
      jl	.finish
      jmp	.next
      ;
 .finish:
      ;
      pop	 edi ecx
      cmp	 eax, -1
      jne	.znd


      ;call	 fat32_listindode			    ; srEDI szECX prEBX ;
      mov	eax,  esi				   ; restore clusterId ;
      call	fat32_nextcluster			   ; EDX:handle, EAX:cl;
      cmp	eax,  -1
      je       .error_fm
      or	eax,  eax
      jz       .znd
      jmp      .nxt
      ;
   .znd:
      ;xor	 eax,  eax
      jmp      .ret
   .error_fm:
      mov	eax,  edi
      call	os_free
   .error:
      mov	eax,  -1
   .ret:
      pop	edi esi edx ecx ebx
      ret
; eax=-1 -> error || StartCluster

align 4
fat32_cd:  ; eax=handler, esi=dirname
ret
; eax=-1 -> error

align 4
fat32_stat: ; esi = filename, ecx=filesize, edx = rights,
ret
; eax=-1 -> error

align 4
fat32_readfile: ; eax=handler, esi = filename, edi = buffer, ecx = size
      mov	ebx, eax
      ;
      push	ebx ecx edx esi edi
      mov	eax, ebx
      call	fat32_findfile	; eax=handler, esi=filename
      pop	edi esi edx ecx ebx
      cmp	eax, -1
      jne	@f
      ret
  @@:


      push	edi  eax
      mov	ecx, 1
      mov	edx, ebx
      call	fat32_rdcluster ;fat32_rdcluster    ; ecx=num, edx=handler, eax=ID, edi:buffer
      pop	edx  edi
      cmp	eax, -1
      je	@f
      ;
      movzx	eax,  byte[ebx + fat32.fat_secperclust]    ; sectores per clst ;
      shl	eax,  9 				   ; * 512 (sect.size) ;
      add	edi,  eax
      ;
      mov	eax,  edx
      mov	edx,  ebx
      call	fat32_nextcluster			   ; EDX:handle, EAX:cl;
      cmp	eax, -1
      je	@f

     ; print cluster #
     ; pushad
     ; add	 al, 0x30
     ; mov	 byte[number], al
     ; invoke	 MessageBox,HWND_DESKTOP, number, str_findfile,MB_OK
     ; popad

      jmp	@b


  @@:

      ; fat32_nextcluster: ; eax=ClusterID, edx=Handler
ret
; eax=-1 -> error




