FAT32LBA = 0x0C

virtual at 0
  fat32:
      .fat_jmp	       dw 00		; 2 bytes   EB 58		      ;
      .fat_nop	       db 00		;	    90			      ;
      .fat_osname      db 8 dup 0	; 8 bytes			      ;
      .fat_bps	       dw 0x0000	; =512 dw, Bytes per Sector	      ; 512
      .fat_secperclust db 0x00		; db, Sectors per Cluster - 1, 2, 4,  ; 1
					;     8, 16, 32, 64 or 128 sectors    ;
      .fat_sectbefofat dw 0x0000	; dw, Reserved sectors in front of    ; 32
					;     the FAT(s) incl. the Bootsector ;
      .fat_copies      db 0x00		; db, Number of FAT copies, Normaly 2 ;
      .fat_unused_00   dd 0x00000000	; 4 bytes, Unused by FAT32	      ;
      .fat_media       db 0x00		; db, Media Descriptor = 0xF8	      ;
      .fat_unised_01   dw 0x0000	;				      ;
      .fat_bios_spt    dw 0x0000	; dw, Sectors per Track 	      ; 63
      .fat_bios_heads  dw 0x0000	; dw, Heads -"- 		      ; 16
      .fat_part_addr   dd 0x00000000	; dd, The number of sectors on the    ; 63
					;     disk from the start of the      ;
					;     partition to the beginning      ;
					;     of the first FAT32 volume.      ;
      .fat_total_sect  dd 0x00000000	; dd, Number of sectors in the part.  ; 2048
      .fat_sect_perfat dd 0x00000000	; dd, Sectors per FAT		      ;
      .fat_activefat   dw 0x0000	; dw, Active FAT #		      ;
      .fat_version     dw 0x0000	; dw, FAT32 Drive Version	      ;
      .fat_rootcluster dd 0x00000000	; dd, Cluster number for the start    ; 0x02
					;     of the Root Directory Table     ;
      .fat_fsinfo_cl   dw 0x0000	; dw, Sector number from the start    ; 0x01
					;     of the partition, for the       ;
					;     File System Information Sector  ;
      .fat_boot_copy   dw 0x0000	; dw, Sector number from the start    ; 6
					;     of the partition, for	      ;
					;     the Backup Boot Sector	      ;
      .fat_unused_03   dd 3 dup 0	; 12b, Reserved 		      ;
       ; Ext. BIOS Para- meter Block:	;				      ;
      .fat_bios_drive  db 0x00		; db, Logical Drive Number -  0x80    ;
      .fat_bios_chead  db 0x00		; db, Current Head		      ;
      .fat_extsignatr  db 0x00		; db, Signature 		      ; 0x29
      .fat_serialnumb  dd 0x00000000	; dd, ID - RNDGenerated serial number ;
      .fat_volumename  db 11 dup 0	; 11b, Volume Label - The same as     ;
					;      stored in a special file       ;
					;      in the root directory.	      ;
      .fat_systemtype  db 08  dup 0	; 8b, System ID = 'FAT32 '	      ;
      .fat_bootcode32  db 420 dup 0	; 420b, Free - Used for bootcode      ;
      .fat_execmarker  dw 0x0000	; dw, Executable signature = AA55h    ;
;---------------------------------------;-------------------------------------;

;---------------------------------------;-------------------------------------;
; SECTOR #1:				;				      ;
   ;FSInfo_sector:			;				      ;
      .fsin_sign       dd 0x00000000	; Signature 1 0x41615252	      ;
      .fsin_padd       db 0480 dup 0	; reserved			      ;
      .fsin_sig2       dd 0x00000000	; Signature 2 0x61417272	      ;
      .fsin_free       dd 0x00000000	; dd, num of free clusters or	      ;
					;     0xffffffff if unknown	      ;
      .fsin_strt       dd 0x00000000	; dd, starting cluster for search or  ;
					;     0xffffffff if start == 2	      ;
      .fsin_pad2       db 0012 dup 0	; reserved			      ;
      .fsin_esig       dd 0x00000000	; Signature 'TheEnd'		      ; 0xAA550000
       ;
;---------------------------------------;-------------------------------------;
; notAector, but info bloxk for internal use:
      .fat_fathandle   dd 0x00000000
      .fat_fattbladr   dd 0x00000000
      .fat_fattblsiz   dd 0x00000000
      .fat_start_lba   dd 0x00000000
      .fat_currntdir   dd 0x00000000	; Pointer to current DIR inode
end virtual