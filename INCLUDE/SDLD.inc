;
; SDLD.inc (SDL Definitions)
;
; SDL 1.2.11 includes written by Raedwulf
; a.k.a. Tai Chi Minh Ralph Eastwood
;
; There's no warranty for the correctness nor completeness of this file
; However, please mail me at tcmreastwood@ntlworld.com if you spot
; any errors in here.
;
; Version Date: 07/11/2006 (dd/mm/yyyy)
;
; Note: Thanks to gunblade about the padding ;)
;
SDL_INIT_TIMER      =   00000001h
SDL_INIT_AUDIO      =   00000010h
SDL_INIT_VIDEO      =   00000020h
SDL_INIT_CDROM      =   00000100h
SDL_INIT_JOYSTICK   =   00000200h
SDL_INIT_NOPARACHUTE    =   00100000h
SDL_INIT_EVENTTHREAD    =   01000000h
SDL_INIT_EVERYTHING =   0000FFFFh
SDL_APPMOUSEFOCUS   =   01h
SDL_APPINPUTFOCUS   =   02h
SDL_APPACTIVE       =   04h

struct SDL_AudioSpec
       freq      rd      1
       format        rw      1
       channels      rb      1
       silence       rb      1
       samples       rw      1
       padding       rw      1
       size      rd      1
       callback      rd      1
       userdata      rd      1
ends

AUDIO_U8    =   0008h
AUDIO_S8    =   8008h
AUDIO_U16LSB    =   0010h
AUDIO_S16LSB    =   8010h
AUDIO_U16MSB    =   1010h
AUDIO_S16MSB    =   9010h
AUDIO_U16   =   AUDIO_U16LSB
AUDIO_S16   =   AUDIO_S16LSB
AUDIO_U16SYS    =   AUDIO_U16LSB
AUDIO_S16SYS    =   AUDIO_S16LSB


struct  SDL_AudioCVT
    needed            rd 1
    src_format        rw 1
    dst_format        rw 1
    rate_incr         rq 1
    buf           rd 1
    len           rd 1
    len_cvt           rd 1
    len_mult          rd 1
    len_ratio         rq 1
    filters           rd 10
    filter_index          rd 1
ends


SDL_MIX_MAXVOLUME   =   128
SDL_MAX_TRACKS  =   99
SDL_AUDIO_TRACK =   00h
SDL_DATA_TRACK  =   04h

CD_TRAYEMPTY    =   0
CD_STOPPED  =   1
CD_PLAYING  =   2
CD_PAUSED   =   3
CD_ERROR = -1

struct   SDL_CDtrack
    id          rb 1
    type            rb 1
    unused          rw 1
    length          rd 1
    offset          rd 1
ends

TRACK_COUNT  = (SDL_MAX_TRACKS+1)

struct   SDL_CD
    id          rd 1
    status          rd 1
    numtracks       rd 1
    cur_track       rd 1
    cur_frame       rd 1
    track           SDL_CDtrack TRACK_COUNT
ends

SDL_LIL_ENDIAN  =   1234
SDL_BIG_ENDIAN  =   4321

SDL_BYTEORDER   equ SDL_LIL_ENDIAN
SDL_ENOMEM  =   0
SDL_EFREAD  =   1
SDL_EFWRITE =   2
SDL_EFSEEK  =   3
SDL_UNSUPPORTED =   4
SDL_LASTERROR   =   5
SDL_RELEASED    =   0
SDL_PRESSED =   1

SDL_NOEVENT     =   0
SDL_ACTIVEEVENT     =   1
SDL_KEYDOWN     =   2
SDL_KEYUP       =   3
SDL_MOUSEMOTION     =   4
SDL_MOUSEBUTTONDOWN =   5
SDL_MOUSEBUTTONUP   =   6
SDL_JOYAXISMOTION   =   7
SDL_JOYBALLMOTION   =   8
SDL_JOYHATMOTION    =   9
SDL_JOYBUTTONDOWN   =   10
SDL_JOYBUTTONUP     =   11
SDL_QUIT        =   12
SDL_SYSWMEVENT      =   13
SDL_EVENT_RESERVEDA =   14
SDL_EVENT_RESERVEDB =   15
SDL_VIDEORESIZE     =   16
SDL_VIDEOEXPOSE     =   17
SDL_EVENT_RESERVED2 =   18
SDL_EVENT_RESERVED3 =   19
SDL_EVENT_RESERVED4 =   20
SDL_EVENT_RESERVED5 =   21
SDL_EVENT_RESERVED6 =   22
SDL_EVENT_RESERVED7 =   23

SDL_USEREVENT   =   24
SDL_NUMEVENTS   =   32

SDL_ACTIVEEVENTMASK =   2
SDL_KEYDOWNMASK     =   4
SDL_KEYUPMASK       =   8
SDL_KEYEVENTMASK    =   12
SDL_MOUSEMOTIONMASK =   16
SDL_MOUSEBUTTONDOWNMASK =   32
SDL_MOUSEBUTTONUPMASK   =   64
SDL_MOUSEEVENTMASK  =   102
SDL_JOYAXISMOTIONMASK   =   128
SDL_JOYBALLMOTIONMASK   =   256
SDL_JOYHATMOTIONMASK    =   512
SDL_JOYBUTTONDOWNMASK   =   1024
SDL_JOYBUTTONUPMASK =   2048
SDL_JOYEVENTMASK    =   3968
SDL_VIDEORESIZEMASK =   65536
SDL_VIDEOEXPOSEMASK =   131072
SDL_QUITMASK        =   4096
SDL_SYSWMEVENTMASK  =   8192
SDL_ALLEVENTS       =   0FFFFFFFFh

struct   SDL_keysym
    scancode          rb 1
    pad0              rb 3
    sym           rd 1
    mod           rd 1
    unicode           rw 1
ends

struct   SDL_ActiveEvent
    type          rb 1
    gain          rb 1
    state         rb 1
    pad0          rb 1
ends


struct   SDL_KeyboardEvent
    type          rb 1
    which         rb 1
    state         rb 1
    pad0          rb 1
    keysym        SDL_keysym
ends


struct   SDL_MouseMotionEvent
    type          rb 1
    which         rb 1
    state         rb 1
    pad0          rb 1
    x         rw 1
    y         rw 1
    xrel          rw 1
    yrel          rw 1
ends


struct   SDL_MouseButtonEvent
    type          rb 1
    which         rb 1
    button        rb 1
    state         rb 1
    x         rw 1
    y         rw 1
ends


struct   SDL_JoyAxisEvent
    type          rb 1
    which         rb 1
    axis          rb 1
    pad0          rb 1
    value         rw 1
ends


struct   SDL_JoyBallEvent
    type          rb 1
    which         rb 1
    ball          rb 1
    pad0          rb 1
    xrel          rw 1
    yrel          rw 1
ends


struct   SDL_JoyHatEvent
    type          rb 1
    which         rb 1
    hat       rb 1
    value         rb 1
ends


struct   SDL_JoyButtonEvent
    type          rb 1
    which         rb 1
    button        rb 1
    state         rb 1
ends


struct   SDL_ResizeEvent
    type          rb 1
    pad0          rb 3
    w         rd 1
    h         rd 1
ends


struct   SDL_ExposeEvent
    type          rb 1
ends


struct   SDL_QuitEvent
    type          rb 1
ends


struct   SDL_UserEvent
    type          rb 1
    pad0          rb 3
    code          rd 1
    data1         rd 1
    data2         rd 1
ends


SDL_SysWMmsg equ SDL_SysWMmsg

struct   SDL_SysWMEvent
    type          rb 1
    pad0          rb 3
    msg       rd 1
ends


struc   SDL_Event
{
        virtual
        .type          rb 1
        end virtual
        virtual
        .active        SDL_ActiveEvent
        end virtual
        virtual
        .key       SDL_KeyboardEvent
        end virtual
        virtual
       .motion        SDL_MouseMotionEvent
        end virtual
        virtual
        .button        SDL_MouseButtonEvent
        end virtual
        virtual
        .jaxis         SDL_JoyAxisEvent
        end virtual
        virtual
        .jball         SDL_JoyBallEvent
        end virtual
        virtual
        .jhat          SDL_JoyHatEvent
        end virtual
        virtual
        .jbutton       SDL_JoyButtonEvent
        end virtual
        virtual
        .resize        SDL_ResizeEvent
        end virtual
        virtual
        .expose        SDL_ExposeEvent
        end virtual
        virtual
        .quit          SDL_QuitEvent
        end virtual
        virtual
        .user          SDL_UserEvent
        end virtual
        .syswm         SDL_SysWMEvent
}

SDL_ADDEVENT        =   0
SDL_PEEKEVENT       =   1
SDL_GETEVENT        =   2
SDL_QUERY       =   -1
SDL_IGNORE      =   0
SDL_DISABLE     =   0
SDL_ENABLE      =   1
SDL_HAT_CENTERED    =   00h
SDL_HAT_UP      =   01h
SDL_HAT_RIGHT       =   02h
SDL_HAT_DOWN        =   04h
SDL_HAT_LEFT        =   08h




SDL_ALL_HOTKEYS =   0FFFFFFFFh
SDL_DEFAULT_REPEAT_DELAY    =   500
SDL_DEFAULT_REPEAT_INTERVAL =   30


SDLK_UNKNOWN    =   0
SDLK_FIRST  =   0
SDLK_BACKSPACE  =   8
SDLK_TAB    =   9
SDLK_CLEAR  =   12
SDLK_RETURN =   13
SDLK_PAUSE  =   19
SDLK_ESCAPE =   27
SDLK_SPACE  =   32
SDLK_EXCLAIM    =   33
SDLK_QUOTEDBL   =   34
SDLK_HASH   =   35
SDLK_DOLLAR =   36
SDLK_AMPERSAND  =   38
SDLK_QUOTE  =   39
SDLK_LEFTPAREN  =   40
SDLK_RIGHTPAREN =   41
SDLK_ASTERISK   =   42
SDLK_PLUS   =   43
SDLK_COMMA  =   44
SDLK_MINUS  =   45
SDLK_PERIOD =   46
SDLK_SLASH  =   47
SDLK_0  =   48
SDLK_1  =   49
SDLK_2  =   50
SDLK_3  =   51
SDLK_4  =   52
SDLK_5  =   53
SDLK_6  =   54
SDLK_7  =   55
SDLK_8  =   56
SDLK_9  =   57
SDLK_COLON  =   58
SDLK_SEMICOLON  =   59
SDLK_LESS   =   60
SDLK_EQUALS =   61
SDLK_GREATER    =   62
SDLK_QUESTION   =   63
SDLK_AT =   64
SDLK_LEFTBRACKET    =   91
SDLK_BACKSLASH  =   92
SDLK_RIGHTBRACKET   =   93
SDLK_CARET  =   94
SDLK_UNDERSCORE =   95
SDLK_BACKQUOTE  =   96
SDLK_a  =   97
SDLK_b  =   98
SDLK_c  =   99
SDLK_d  =   100
SDLK_e  =   101
SDLK_f  =   102
SDLK_g  =   103
SDLK_h  =   104
SDLK_i  =   105
SDLK_j  =   106
SDLK_k  =   107
SDLK_l  =   108
SDLK_m  =   109
SDLK_n  =   110
SDLK_o  =   111
SDLK_p  =   112
SDLK_q  =   113
SDLK_r  =   114
SDLK_s  =   115
SDLK_t  =   116
SDLK_u  =   117
SDLK_v  =   118
SDLK_w  =   119
SDLK_x  =   120
SDLK_y  =   121
SDLK_z  =   122
SDLK_DELETE =   127

SDLK_WORLD_0    =   160
SDLK_WORLD_1    =   161
SDLK_WORLD_2    =   162
SDLK_WORLD_3    =   163
SDLK_WORLD_4    =   164
SDLK_WORLD_5    =   165
SDLK_WORLD_6    =   166
SDLK_WORLD_7    =   167
SDLK_WORLD_8    =   168
SDLK_WORLD_9    =   169
SDLK_WORLD_10   =   170
SDLK_WORLD_11   =   171
SDLK_WORLD_12   =   172
SDLK_WORLD_13   =   173
SDLK_WORLD_14   =   174
SDLK_WORLD_15   =   175
SDLK_WORLD_16   =   176
SDLK_WORLD_17   =   177
SDLK_WORLD_18   =   178
SDLK_WORLD_19   =   179
SDLK_WORLD_20   =   180
SDLK_WORLD_21   =   181
SDLK_WORLD_22   =   182
SDLK_WORLD_23   =   183
SDLK_WORLD_24   =   184
SDLK_WORLD_25   =   185
SDLK_WORLD_26   =   186
SDLK_WORLD_27   =   187
SDLK_WORLD_28   =   188
SDLK_WORLD_29   =   189
SDLK_WORLD_30   =   190
SDLK_WORLD_31   =   191
SDLK_WORLD_32   =   192
SDLK_WORLD_33   =   193
SDLK_WORLD_34   =   194
SDLK_WORLD_35   =   195
SDLK_WORLD_36   =   196
SDLK_WORLD_37   =   197
SDLK_WORLD_38   =   198
SDLK_WORLD_39   =   199
SDLK_WORLD_40   =   200
SDLK_WORLD_41   =   201
SDLK_WORLD_42   =   202
SDLK_WORLD_43   =   203
SDLK_WORLD_44   =   204
SDLK_WORLD_45   =   205
SDLK_WORLD_46   =   206
SDLK_WORLD_47   =   207
SDLK_WORLD_48   =   208
SDLK_WORLD_49   =   209
SDLK_WORLD_50   =   210
SDLK_WORLD_51   =   211
SDLK_WORLD_52   =   212
SDLK_WORLD_53   =   213
SDLK_WORLD_54   =   214
SDLK_WORLD_55   =   215
SDLK_WORLD_56   =   216
SDLK_WORLD_57   =   217
SDLK_WORLD_58   =   218
SDLK_WORLD_59   =   219
SDLK_WORLD_60   =   220
SDLK_WORLD_61   =   221
SDLK_WORLD_62   =   222
SDLK_WORLD_63   =   223
SDLK_WORLD_64   =   224
SDLK_WORLD_65   =   225
SDLK_WORLD_66   =   226
SDLK_WORLD_67   =   227
SDLK_WORLD_68   =   228
SDLK_WORLD_69   =   229
SDLK_WORLD_70   =   230
SDLK_WORLD_71   =   231
SDLK_WORLD_72   =   232
SDLK_WORLD_73   =   233
SDLK_WORLD_74   =   234
SDLK_WORLD_75   =   235
SDLK_WORLD_76   =   236
SDLK_WORLD_77   =   237
SDLK_WORLD_78   =   238
SDLK_WORLD_79   =   239
SDLK_WORLD_80   =   240
SDLK_WORLD_81   =   241
SDLK_WORLD_82   =   242
SDLK_WORLD_83   =   243
SDLK_WORLD_84   =   244
SDLK_WORLD_85   =   245
SDLK_WORLD_86   =   246
SDLK_WORLD_87   =   247
SDLK_WORLD_88   =   248
SDLK_WORLD_89   =   249
SDLK_WORLD_90   =   250
SDLK_WORLD_91   =   251
SDLK_WORLD_92   =   252
SDLK_WORLD_93   =   253
SDLK_WORLD_94   =   254
SDLK_WORLD_95   =   255
SDLK_KP0    =   256
SDLK_KP1    =   257
SDLK_KP2    =   258
SDLK_KP3    =   259
SDLK_KP4    =   260
SDLK_KP5    =   261
SDLK_KP6    =   262
SDLK_KP7    =   263
SDLK_KP8    =   264
SDLK_KP9    =   265
SDLK_KP_PERIOD  =   266
SDLK_KP_DIVIDE  =   267
SDLK_KP_MULTIPLY    =   268
SDLK_KP_MINUS   =   269
SDLK_KP_PLUS    =   270
SDLK_KP_ENTER   =   271
SDLK_KP_EQUALS  =   272
SDLK_UP =   273
SDLK_DOWN   =   274
SDLK_RIGHT  =   275
SDLK_LEFT   =   276
SDLK_INSERT =   277
SDLK_HOME   =   278
SDLK_END    =   279
SDLK_PAGEUP =   280
SDLK_PAGEDOWN   =   281
SDLK_F1 =   282
SDLK_F2 =   283
SDLK_F3 =   284
SDLK_F4 =   285
SDLK_F5 =   286
SDLK_F6 =   287
SDLK_F7 =   288
SDLK_F8 =   289
SDLK_F9 =   290
SDLK_F10    =   291
SDLK_F11    =   292
SDLK_F12    =   293
SDLK_F13    =   294
SDLK_F14    =   295
SDLK_F15    =   296
SDLK_NUMLOCK    =   300
SDLK_CAPSLOCK   =   301
SDLK_SCROLLOCK  =   302
SDLK_RSHIFT =   303
SDLK_LSHIFT =   304
SDLK_RCTRL  =   305
SDLK_LCTRL  =   306
SDLK_RALT   =   307
SDLK_LALT   =   308
SDLK_RMETA  =   309
SDLK_LMETA  =   310
SDLK_LSUPER =   311
SDLK_RSUPER =   312
SDLK_MODE   =   313
SDLK_COMPOSE    =   314
SDLK_HELP   =   315
SDLK_PRINT  =   316
SDLK_SYSREQ =   317
SDLK_BREAK  =   318
SDLK_MENU   =   319
SDLK_POWER  =   320
SDLK_EURO   =   321
SDLK_UNDO   =   322
SDLK_LAST   =   324
KMOD_NONE   =   0000h
KMOD_LSHIFT =   0001h
KMOD_RSHIFT =   0002h
KMOD_LCTRL  =   0040h
KMOD_RCTRL  =   0080h
KMOD_LALT   =   0100h
KMOD_RALT   =   0200h
KMOD_LMETA  =   0400h
KMOD_RMETA  =   0800h
KMOD_NUM    =   1000h
KMOD_CAPS   =   2000h
KMOD_MODE   =   4000h
KMOD_RESERVED   =   8000h

KMOD_CTRL   =   00C0h
KMOD_SHIFT  =   0003h
KMOD_ALT    =   0300h
KMOD_META   =   0C00h

WMcursor equ WMcursor

struct   SDL_Rect
    x       rw 1
    y       rw 1
    w       rw 1
    h       rw 1

ends

struct   SDL_Cursor
    area        SDL_Rect
    hot_x       rw 1
    hot_y       rw 1
    data        rd 1
    mask        rd 1
    save        rd 2
    wm_cursor   rd 1
ends

SDL_BUTTON_LEFT     =   1
SDL_BUTTON_MIDDLE   =   2
SDL_BUTTON_RIGHT    =   3
SDL_BUTTON_WHEELUP  =   4
SDL_BUTTON_WHEELDOWN    =   5

SDL_BUTTON_LMASK    =   1
SDL_BUTTON_MMASK    =   2
SDL_BUTTON_RMASK    =   4

SDL_MUTEX_TIMEDOUT  =   1


RW_SEEK_SET =   0
RW_SEEK_CUR =   1
RW_SEEK_END =   2


struct   SDL_RWops
    seek              rd 1
    read              rd 1
    write             rd 1
    close             rd 1
    type              rd 1
    append            rd 1
    h             rd 1
ends

SDL_MAJOR_VERSION   =   1
SDL_MINOR_VERSION   =   2
SDL_PATCHLEVEL  =   11

struct   SDL_version
    major             rb 1
    minor             rb 1
    patch             rb 1
    pad0              rb 1
ends


SDL_ALPHA_OPAQUE    =   255
SDL_ALPHA_TRANSPARENT   =   0

struct   SDL_Color
    r           rb 1
    g           rb 1
    b           rb 1
    unused          rb 1
ends

SDL_Colour  equ SDL_Color
struct   SDL_Palette
    ncolors           rd 1
    colors            rd 1
ends


struct   SDL_PixelFormat
    palette           rd 1
    BitsPerPixel          rb 1
    BytesPerPixel         rb 1
    Rloss             rb 1
    Gloss             rb 1
    Bloss             rb 1
    Aloss             rb 1
    Rshift            rb 1
    Gshift            rb 1
    Bshift            rb 1
    Ashift            rb 1
    pad0              rb 2
    Rmask             rd 1
    Gmask             rd 1
    Bmask             rd 1
    Amask             rd 1
    colorkey          rd 1
    alpha             rb 1
    pad1              rb 3
ends

struct   SDL_Surface
    flags             rd 1
    format            rd 1
    w             rd 1
    h             rd 1
    pitch             rw 1
    pad0              rb 2
    pixels            rd 1
    offset            rd 1
    hwdata            rd 1
    clip_rect         SDL_Rect
    unused1           rd 1
    locked            rd 1
    map           rd 1
    format_version        rd 1
    refcount          rd 1
ends


SDL_SWSURFACE   =   00000000h
SDL_HWSURFACE   =   00000001h
SDL_ASYNCBLIT   =   00000004h
SDL_ANYFORMAT   =   10000000h
SDL_HWPALETTE   =   20000000h
SDL_DOUBLEBUF   =   40000000h
SDL_FULLSCREEN  =   80000000h
SDL_OPENGL  =   00000002h
SDL_OPENGLBLIT  =   0000000Ah
SDL_RESIZABLE   =   00000010h
SDL_NOFRAME =   00000020h
SDL_HWACCEL =   00000100h
SDL_SRCCOLORKEY =   00001000h
SDL_RLEACCELOK  =   00002000h
SDL_RLEACCEL    =   00004000h
SDL_SRCALPHA    =   00010000h
SDL_PREALLOC    =   01000000h
SDL_YV12_OVERLAY    =   32315659h
SDL_IYUV_OVERLAY    =   56555949h
SDL_YUY2_OVERLAY    =   32595559h
SDL_UYVY_OVERLAY    =   59565955h
SDL_YVYU_OVERLAY    =   55595659h

struct   SDL_Overlay
    format            rd 1
    w             rd 1
    h             rd 1
    planes            rd 1
    pitches           rd 1
    pixels            rd 1
    hwfuncs           rd 1
    hwdata            rd 1
    hw_overlay        rd 1
    UnusedBits        rd 1
ends


SDL_GL_RED_SIZE         =   0
SDL_GL_GREEN_SIZE       =   1
SDL_GL_BLUE_SIZE        =   2
SDL_GL_ALPHA_SIZE       =   3
SDL_GL_BUFFER_SIZE      =   4
SDL_GL_DOUBLEBUFFER     =   5
SDL_GL_DEPTH_SIZE       =   6
SDL_GL_STENCIL_SIZE     =   7
SDL_GL_ACCUM_RED_SIZE       =   8
SDL_GL_ACCUM_GREEN_SIZE     =   9
SDL_GL_ACCUM_BLUE_SIZE      =   10
SDL_GL_ACCUM_ALPHA_SIZE     =   11
SDL_GL_STEREO           =   12
SDL_GL_MULTISAMPLEBUFFERS   =   13
SDL_GL_MULTISAMPLESAMPLES   =   14
SDL_GL_ACCELERATED_VISUAL   =   15
SDL_GL_SWAP_CONTROL     =   16
SDL_LOGPAL          =   01h
SDL_PHYSPAL         =   02h

SDL_GRAB_QUERY          =   -1
SDL_GRAB_OFF            =   0
SDL_GRAB_ON         =   1
SDL_GRAB_FULLSCREEN     =   2

macro SDL_MUSTLOCK reg, surfaceptr
{
    local surface

    if reg in <eax,ebx,ecx,edx,esi,edi>
       virtual at surfaceptr
           surface SDL_Surface
       end virtual
       mov  reg, [surface.flags]
       and  reg, SDL_HWSURFACE or SDL_ASYNCBLIT or SDL_RLEACCEL
       or   reg, [surface.offset]
       test reg, reg
    else
       display 'SDL_MUSTLOCK error'
       error
    end if

}
