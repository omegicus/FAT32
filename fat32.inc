format binary as 'img'
include 'fat_data.inc'
DISKSIZE = (1024*1024*32)/512
PARTSIZE = DISKSIZE - 64*512 ;(1=MBR, 63=Padding)



; HDD SECTOR #0
	DB    446 - ( $ - $$ ) DUP 0

	; MBR PARTITIONS:
	PART1:	DB 16 DUP 0
	PART2:
		DB	0x80		; 00 ACTIVE
		DB	0x00		; 01 START_HEAD
		DW	0x0000		; 02 START CYL/SECT
		DB	FAT32LBA	; 04 Type == OmegicusFS
		DB	0x00		; 05 END HEAD
		DW	0x0000		; 06 END CYL/SECT
		DD	63		; 08 LBA START
		DD	0x0000FFFF	; 12 PART SIZE IN SECTORS... 65k == 32M PARTITION
	PART3:	DB 16 DUP 0
	PART4:	DB 16 DUP 0
	DB	0x55, 0xAA

; HDD SECTOR #1 - #62 incl.
PADD_MBR:	DB	62*512 DUP 0


use32
; HDD ADDR (1'st part, 7E00), sector #63
;---------------------------------------;-------------------------------------;
FAT32_BSS:				;				      ;
       jmp	       bootcode 	; 2 bytes   EB 58		      ;
       nop				;	    90			      ;
       ;				;				      ;
       os_name	       db 'MSWIN4.1'	; 8 bytes			      ;
       bps	       dw 512		; dw, Bytes per Sector on the physical;
					;     medium - Normally 512 bytes     ;
       spc	       db 01		; db, Sectors per Cluster - 1, 2, 4,  ;
					;     8, 16, 32, 64 or 128 sectors    ;
       resfrfat        dw 32  ;?38	    ; dw, Reserved sectors in front of	  ;
					;     the FAT(s) incl. the Bootsector ;
       numfats	       db 2		; db, Number of FAT copies, Normaly 2 ;
       MaxRootEntries  dw 0		;     fat16: dw 512 , FAT32: 0	      ;
       TotalSectors    dw 0		;     fat16: dw 2880, FAT32: 0	      ;
       mediadr	       db 0xF8		; db, Media Descriptor - The same as  ;
					;     in FAT16, but FAT32 is only     ;
					;     allowed on harddrives, so the   ;
					;     value is F8h		      ;
       notused2        dw 0		; dw, Not used in FAT32 	      ;
       spt	       dw 63		; dw, Sectors per Track - The disc    ;
					;     geometry used by partition.     ;
       heads	       dw 16		; dw, Heads -"- 		      ;
       hiddsectors     dd 63		; dd, The number of sectors on the    ;
					;     disk from the start of the      ;
					;     partition to the beginning      ;
					;     of the first FAT32 volume.      ;
       totalsectors    dd 2048		; dd, Number of sectors in the part.  ;
       sectorsperfat   dd 08		; dd, Sectors per FAT		      ;
       fat_flags       dw 0		; dw, Active FAT #		      ;
       fat32ver        dw 0		; dw, FAT32 Drive Version	      ;
					;     (High byte = Major version,     ;
					;     Low byte = Minor version)       ;
       rootcluster     dd 2		; dd, Cluster number for the start    ;
					;     of the Root Directory Table     ;
       fsinfostart     dw 1		; dw, Sector number from the start    ;
					;     of the partition, for the       ;
					;     File System Information Sector  ;
       bpbsstrt        dw 6		; dw, Sector number from the start    ;
					;     of the partition, for	      ;
					;     the Backup Boot Sector	      ;
       db 12	       dup 0		; 12b, Reserved 		      ;
       ; Ext. BIOS Para- meter Block:	;				      ;
       BootDrive db 0x80		; db, Logical Drive Number -	      ;
					;     Normaly 00h for floppies	      ;
					;     and 80h for hard drives.	      ;
       currhead  db 0x00		; db, Current Head		      ;
       ExtenSig  db 0x29		; db, Signature 		      ;
       serialnm  dd 0x00000000		; dd, ID - RNDGenerated serial number ;
       volumlbl  db 'NO NAME    '	; 11b, Volume Label - The same as     ;
					;      stored in a special file       ;
					;      in the root directory.	      ;
       systemid  db 'FAT32   '		; 8b, System ID = 'FAT32 '	      ;
       ;				;				      ;
       bootcode  db 420 dup 0		; 420b, Free - Used for bootcode      ;
       execbtsn  dw 0xAA55		; dw, Executable signature = AA55h    ;
;---------------------------------------;-------------------------------------;
									      ;
									      ;
;---------------------------------------;-------------------------------------;
; SECTOR #1:				;				      ;
FSInfo_sector:				;				      ;
       fsin_sign dd 0x41615252		; Signature 1			      ;
       fsin_padd db 0480 dup 0		; reserved			      ;
       fsin_sig2 dd 0x61417272		; Signature 2			      ;
       fsin_free dd 0x00000000		; dd, num of free clusters or	      ;
					;     0xffffffff if unknown	      ;
       fsin_strt dd 0x00000000		; dd, starting cluster for search or  ;
					;     0xffffffff if start == 2	      ;
       fsin_pad2 db 0012 dup 0		; reserved			      ;
       fsin_esig dd 0xAA550000		; Signature 'TheEnd'		      ;
;---------------------------------------;-------------------------------------;
									      ;
									      ;
; SECTOR #2:								      ;
Bootloader_Stage2: ; as FAT32 official say's... 			      ;
       db 510 dup 0
       dw 0xAA55
									      ;
		; We are now ready to load our second sector of boot code     ;
		; But first, a bit of undocumented information about how      ;
		; Win2k stores it's second sector of boot code. 	      ;
		;							      ;
		; The FAT32 filesystem was designed so that you can store     ;
		; multiple sectors of boot code. The boot sector of a FAT32   ;
		; volume is actually three sectors long. Microsoft extended   ;
		; the BPB so much that you can't fit enough code in the       ;
		; boot sector to make it work. So they extended it. Sector 0  ;
		; is the traditional boot sector, sector 1 is the FSInfo sect,;
		; and sector 2 is used to store extra boot code to make up    ;
		; for the lost space the BPB takes.			      ;
		;							      ;
		; Now this creates an interesting problem. Suppose for example;
		; that the user has Win98 and Win2k installed. The Win2k      ;
		; boot sector is stored at sector 0 and the Win98 bootsect is ;
		; stored as BOOTSECT.DOS on the file system. Now if Win2k were;
		; to store it's second sector of boot code in sector 2 like   ;
		; the fat spec says to do then when you try to dual boot back ;
		; to Win98 the Win98 boot sector will load Win2k's second     ;
		; sector of boot code. Understand? ;-)			      ;
		;							      ;
		; To get around this problem Win2k stores it's second sector  ;
		; of boot code elsewhere. This sector always stored at sec.13 ;
		; on the file system. Now dont ask what happens when you dont ;
		; have enough res.sectors to store it, but I've never seen a  ;
		; FAT32 volume that didn't have at least 32 reserved sectors. ;
									      ;


 db 510 dup 0	; SECTOR #3
 dw 0xAA55

 db 510 dup 0	; SECTOR #4
 dw 0xAA55

 db 510 dup 0	; SECTOR #5
 dw 0xAA55

 db 510 dup 0	; SECTOR #6 Must have a Copy of BootRecord.
 dw 0xAA55

 db 510 dup 0	; SECTOR #7 Must have a Copy of FSInfo_sector
 dw 0xAA55

 db 512 dup 0	; SECTOR #8
 db 512 dup 0	; SECTOR #9
 db 512 dup 0	; SECTOR #10
 db 512 dup 0	; SECTOR #11
 db 512 dup 0	; SECTOR #12
						    ;
; Sector #13:								      ;
; Win2k second bootsector (copy of 2'nd bootsector):			      ;
Bootloader_Stage2_w2k: ; as FAT32 official say's... ;			      ;
db	  512 dup 0							      ;
									      ;
; Sectors paddeded to be this block == 32 sectors (before FAT): 	      ;
db	  18*512 dup 0							      ;
;---------------;-------------------------------------------------------------;
;-----------------------------------------------------------------------------;

; first_data_sector =
;     reserved_sector_count + (table_count * fat_size) + root_dir_sectors;
; first_fat_sector = reserved_sector_count;
; first_sector_of_cluster =
;     ((cluster - 2) * sectors_per_cluster) + first_data_sector;

;if(total_clusters < 4085)
;{
;   fat_type = FAT12;
;}
;else if(total_clusters < 65525)
;{
;   fat_type = FAT16;
;}
;else if (total_clusters < 268435445
;{
;   fat_type = FAT32;
;}
;else
;{
;   fat_type = ExFAT;
;}


; FAT [1'st copy:]:
; If "table_value" is greater than or equal to (>=) 0x0FFFFFF8 then there
; are no more clusters in the chain. This means that the whole file has
; been read. If "table_value" equals (==) 0x0FFFFFF7 then this cluster has
; been marked as "bad". "Bad" clusters are prone to errors and should be
; avoided. If "table_value" is not one of the above cases then it is the
; cluster number of the next cluster in the file.
FAT_ORIGINAL:
; 00000000 = Free
; FFFFFFF6 - FFFFFFF7 - One or more bad sectors in cluster
; FFFFFFFF - EndOfFile
  dd 0x0FFFFFF8        ; endOfFile // cluster#0
  dd 0x0FFFFFFF        ; endOfFile // cluster#1
  dd 0x0FFFFFFF        ; endOfFile // cluster#2
  dd 0x0FFFFFFF        ; endOfFile // cluster#3
  dd 0x0FFFFFFF        ; endOfFile // cluster#4
  dd 0x0FFFFFFF        ; endOfFile // cluster#5
  dd 0x0FFFFFFF        ; endOfFile // cluster#6
  dd 0x0FFFFFFF        ; endOfFile // cluster#7
  dd 0x00000009        ; endOfFile // cluster#8
  dd 0x0FFFFFFF        ; endOfFile // cluster#9
  dd ((08*512)/4)-10  dup 0x00000000


FAT_COPY:
  dd 0x0FFFFFF8        ; endOfFile // cluster#0
  dd 0x0FFFFFFF        ; endOfFile // cluster#1
  dd 0x0FFFFFFF        ; endOfFile // cluster#2
  dd 0x0FFFFFFF        ; endOfFile // cluster#3
  dd 0x0FFFFFFF        ; endOfFile // cluster#4
  dd 0x0FFFFFFF        ; endOfFile // cluster#5
  dd 0x0FFFFFFF        ; endOfFile // cluster#6
  dd 0x0FFFFFFF        ; endOfFile // cluster#7
  dd 0x00000009        ; endOfFile // cluster#8
  dd 0x0FFFFFFF        ; endOfFile // cluster#9
  dd ((08*512)/4)-10  dup 0x00000000



;----[ FAT ]----;---------[ CLUSTER #2 ]--------------------------------------;
       ; FileEntry:
       db 'SYSTEM     ' 	; 11b
       ; DIR_Name[0] == 0xE5 (or 0x05) => the directory entry is free
       ; DIR_Name[0] == 0x00 => This and NEXT enries are free
       ; not valid chars:
       ;   - Values less than 0x20 (except 0x05 in DIR_Name[0])
       ;   - 0x22, 0x2A, 0x2B, 0x2C, 0x2E, 0x2F, 0x3A, 0x3B, 0x3C,
       ;     0x3D, 0x3E, 0x3F, 0x5B, 0x5C, 0x5D, 0x7C.
       db 0x08			; ATTR == ATTR_VOLUME_ID
       ;File attributes:
       ;     ATTR_READ_ONLY  0x01
       ;     ATTR_HIDDEN     0x02
       ;     ATTR_SYSTEM     0x04
       ;     ATTR_VOLUME_ID  0x08
       ;     ATTR_DIRECTORY  0x10
       ;     ATTR_ARCHIVE    0x20
       ;     ATTR_LONG_NAME =
       ;	 ATTR_READ_ONLY | ATTR_HIDDEN | ATTR_SYSTEM | ATTR_VOLUME_ID
       ; The upper two bits of the attribute byte are reserved and should
       ; always be set to 0 when a file is created and never modified or
       ; looked at after that
       db 8 dup 0		; unused by FAT32
       dw 0x00			; ClusterHi	 ; 0000
       dw 0x00			; Modified Time  ; 8a5a
       dw 0x00			; Modified Date  ; 470a
       dw 0x00			; ClusterLo	 ; 0000
       dd 0x00			; FileSize	 ; 00000000
       ;
       db 'README  TXT'
       db 0x20			; ATTR_ARCHIVE
       db 8 dup 0		; unused by FAT32
       dw 0x0000		; cluster_hi
       dw 0x0000		; modif.time
       dw 0x0000		; modif.date
       dw 0x0004		; cluster_lo
       dd 0x00000020		; fileSize
       ;
       db 'LICENSE TXT' 	;
       db 0x20			; ATTR_ARCHIVE
       db 8 dup 0		; unused by FAT32
       dw 0x0000		; cluster_hi
       dw 0x0000		; modif.time
       dw 0x0000		; modif.date
       dw 0x0008		; cluster_lo
       dd 959			; fileSize
       ;
       db 'WINDOWS    '
       db 0x10			; ATTR_DIRECTORY
       db 8 dup 0		; unused by FAT32
       dw 0x0000		; cluster_hi
       dw 0x0000		; modif.time
       dw 0x0000		; modif.date
       dw 0x0005		; cluster_lo
       dd 0x00000000		; fileSize
       ;
       db 512 - (32*4) DUP 0

; CLUSTER #3
       db 512	       DUP 0


; CLUSTER #4
    cl_03:
       file  'readme.txt':0, 32
       DB    (512 - 32) DUP 0


; CLUSTER #5
       db '.          '
       db 0x10			; ATTR_DIRECTORY
       db 8 dup 0		; unused by FAT32
       dw 0x0000		; cluster_hi
       dw 0x0000		; modif.time
       dw 0x0000		; modif.date
       dw 0x0005		; cluster_lo
       dd 0x00000000		; fileSize

       db '..         '
       db 0x10			; ATTR_DIRECTORY
       db 8 dup 0		; unused by FAT32
       dw 0x0000		; cluster_hi
       dw 0x0000		; modif.time
       dw 0x0000		; modif.date
       dw 0x0000		; cluster_lo
       dd 0x00000000		; fileSize

       db 'SYSTEM32   '
       db 0x10			; ATTR_DIRECTORY
       db 8 dup 0		; unused by FAT32
       dw 0x0000		; cluster_hi
       dw 0x0000		; modif.time
       dw 0x0000		; modif.date
       dw 0x0006		; cluster_lo
       dd 0x00000000		; fileSize

       db 512 - (32*3) DUP 0


; CLUSTER #6
    cl_06:
       db '.          '
       db 0x10			; ATTR_DIRECTORY
       db 8 dup 0		; unused by FAT32
       dw 0x0000		; cluster_hi
       dw 0x0000		; modif.time
       dw 0x0000		; modif.date
       dw 0x0006		; cluster_lo
       dd 0x00000000		; fileSize

       db '..         '
       db 0x10			; ATTR_DIRECTORY
       db 8 dup 0		; unused by FAT32
       dw 0x0000		; cluster_hi
       dw 0x0000		; modif.time
       dw 0x0000		; modif.date
       dw 0x0005		; cluster_lo
       dd 0x00000000		; fileSize

       db 'KERNEL32SYS'
       db 0x20			; ATTR_ARCHIVE+ATTR_SYSTEM
       db 8 dup 0		; unused by FAT32
       dw 0x0000		; cluster_hi +20
       dw 0x0000		; modif.time +22
       dw 0x0000		; modif.date +24
       dw 0x0007		; cluster_lo +26
       dd 0x00000020		; fileSize

       db 512 - (32*3) DUP 0



; CLUSTER #7
    cl_07:
       file  'readme.txt':0, 32
       DB    (512 - 32) DUP 0

; CLUSTER #8
    cl_08:
       file  'license.txt':0, 512
      ; DB    (512 - 32) DUP 0

; CLUSTER #9
    cl_09:
       file  'license.txt':512, 447
       DB    (512 - 112) DUP 0



_end:
db ((1024*1024) - _end) dup 0

