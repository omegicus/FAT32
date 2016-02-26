format PE GUI 4.0
entry start
include 'INCLUDE/win32ax.inc'
TARGETOS EQU 'NT'

section '.data' readable writeable
align 4
	fhand dd 0
	paths db 'debig.bin', 0

	align 4
	malloc dd buff

	align 4
	pathd db '/usr/',0
	pathm db '/usr/files/',0
	dirn  db 'files',0
	dirrm db 'test',0
	pathf db 'license.txt',0
	pathw db 'license2.txt',0
	str_findfile db 'LICENSE TXT',0;
	handle dd 0
	buffer dd 0

	number db '-', 0

	hdimg file 'fat32.img'
	hdimg_sz = $-hdimg
	tmpbuf0 db 76959 dup '$'

	align 4
	tmpbuf db 64*1024 dup 0
	tmpstat db 64 dup 0
	bufsz	dd 0

	align 4
	buff dd 64*1024 * 16 dup 0




section '.text' code readable executable

include 'arch.inc'
include 'struct.inc'
include 'fat32.asm'
include 'os_winnt.inc'

  start:
    call   fat32_init
    cmp    eax, -1
    je	   exit
    mov    dword[handle], eax
    ;
    mov    eax, 64*1024
    call   os_malloc
    cmp    eax,  -1
    je	   exit
    mov    dword[buffer], eax
    ;
    mov    eax, dword[handle]
    mov    esi, str_findfile
    mov    edi, dword[buffer]
    call   fat32_readfile
    invoke MessageBox,HWND_DESKTOP, [buffer], str_findfile,MB_OK
    ;
    mov    eax, dword[handle]
    mov    ebx, print_string
    call   fat32_ls		      ; list ROOT dir (becouse after INIT we have / as current dir)
    ;
    ; invoke  MessageBox,HWND_DESKTOP,"Hi! I'm the example program!",invoke GetCommandLine,MB_OK

  exit:
    invoke  ExitProcess,0




align 4
print_string:
    pushad
    push    esi  ecx
    xor     ecx, ecx
@@: cmp     byte[esi], 0
    je	    @f
    cmp     ecx, 11
    jae     @f
    inc     ecx
    inc     esi
    jmp     @b
@@: mov     byte[esi], 0
    pop     ecx  esi
    invoke  MessageBox,HWND_DESKTOP,esi,esi,MB_OK
    popad
ret


section '.idata' import data readable writeable
  library kernel32,'KERNEL32.DLL',\
	  user,'USER32.DLL',\
	  msvcrt,   'msvcrt.dll'
  include 'include\api\kernel32.inc'

  import user,\
	 DialogBoxParam,'DialogBoxParamA',\
	 CheckRadioButton,'CheckRadioButton',\
	 GetDlgItemText,'GetDlgItemTextA',\
	 IsDlgButtonChecked,'IsDlgButtonChecked',\
	 MessageBox,'MessageBoxA',\
	 EndDialog,'EndDialog',\
	 wsprintfA, 'wsprintfA'
  import msvcrt,\
	 printf, 'printf'