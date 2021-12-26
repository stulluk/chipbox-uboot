ELF Header:
  Magic:   7f 45 4c 46 01 01 01 61 00 00 00 00 00 00 00 00 
  Class:                             ELF32
  Data:                              2's complement, little endian
  Version:                           1 (current)
  OS/ABI:                            ARM
  ABI Version:                       0
  Type:                              EXEC (Executable file)
  Machine:                           ARM
  Version:                           0x1
  Entry point address:               0x6000000
  Start of program headers:          52 (bytes into file)
  Start of section headers:          44368 (bytes into file)
  Flags:                             0x202, has entry point, GNU EABI, software FP
  Size of this header:               52 (bytes)
  Size of program headers:           32 (bytes)
  Number of program headers:         2
  Size of section headers:           40 (bytes)
  Number of section headers:         17
  Section header string table index: 14

Section Headers:
  [Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al
  [ 0]                   NULL            00000000 000000 000000 00      0   0  0
  [ 1] .text             PROGBITS        06000000 008000 001138 00  AX  0   0  4
  [ 2] .rodata           PROGBITS        06001138 009138 0003e8 00   A  0   0  4
  [ 3] .data             PROGBITS        06009520 009520 000000 00  WA  0   0  1
  [ 4] .bss              NOBITS          06009520 009520 002268 00  WA  0   0  4
  [ 5] .comment          PROGBITS        00000000 009520 000024 00      0   0  1
  [ 6] .debug_aranges    PROGBITS        00000000 009544 000040 00      0   0  1
  [ 7] .debug_pubnames   PROGBITS        00000000 009584 0000df 00      0   0  1
  [ 8] .debug_info       PROGBITS        00000000 009663 000a3f 00      0   0  1
  [ 9] .debug_abbrev     PROGBITS        00000000 00a0a2 000254 00      0   0  1
  [10] .debug_line       PROGBITS        00000000 00a2f6 00034d 00      0   0  1
  [11] .debug_frame      PROGBITS        00000000 00a644 00006c 00      0   0  4
  [12] .debug_str        PROGBITS        00000000 00a6b0 0005b0 01  MS  0   0  1
  [13] .debug_ranges     PROGBITS        00000000 00ac60 000048 00      0   0  1
  [14] .shstrtab         STRTAB          00000000 00aca8 0000a8 00      0   0  1
  [15] .symtab           SYMTAB          00000000 00aff8 0007d0 10     16  33  4
  [16] .strtab           STRTAB          00000000 00b7c8 000674 00      0   0  1
Key to Flags:
  W (write), A (alloc), X (execute), M (merge), S (strings)
  I (info), L (link order), G (group), x (unknown)
  O (extra OS processing required) o (OS specific), p (processor specific)

There are no section groups in this file.

Program Headers:
  Type           Offset   VirtAddr   PhysAddr   FileSiz MemSiz  Flg Align
  LOAD           0x008000 0x06000000 0x06000000 0x01520 0x01520 R E 0x8000
  LOAD           0x009520 0x06009520 0x06009520 0x00000 0x02268 RW  0x8000

 Section to Segment mapping:
  Segment Sections...
   00     .text .rodata 
   01     .bss 

There is no dynamic section in this file.

There are no relocations in this file.

There are no unwind sections in this file.

Symbol table '.symtab' contains 125 entries:
   Num:    Value  Size Type    Bind   Vis      Ndx Name
     0: 00000000     0 NOTYPE  LOCAL  DEFAULT  UND 
     1: 06000000     0 SECTION LOCAL  DEFAULT    1 
     2: 06001138     0 SECTION LOCAL  DEFAULT    2 
     3: 06009520     0 SECTION LOCAL  DEFAULT    3 
     4: 06009520     0 SECTION LOCAL  DEFAULT    4 
     5: 00000000     0 SECTION LOCAL  DEFAULT    5 
     6: 00000000     0 SECTION LOCAL  DEFAULT    6 
     7: 00000000     0 SECTION LOCAL  DEFAULT    7 
     8: 00000000     0 SECTION LOCAL  DEFAULT    8 
     9: 00000000     0 SECTION LOCAL  DEFAULT    9 
    10: 00000000     0 SECTION LOCAL  DEFAULT   10 
    11: 00000000     0 SECTION LOCAL  DEFAULT   11 
    12: 00000000     0 SECTION LOCAL  DEFAULT   12 
    13: 00000000     0 SECTION LOCAL  DEFAULT   13 
    14: 00000000     0 SECTION LOCAL  DEFAULT   14 
    15: 00000000     0 SECTION LOCAL  DEFAULT   15 
    16: 00000000     0 SECTION LOCAL  DEFAULT   16 
    17: 00000000     0 FILE    LOCAL  DEFAULT  ABS hello_world.c
    18: 06009520     0 OBJECT  LOCAL  DEFAULT    4 $d
    19: 06001138    15 OBJECT  LOCAL  DEFAULT    2 __FUNCTION__.0
    20: 06000000     0 FUNC    LOCAL  DEFAULT    1 $a
    21: 06000dc4     0 OBJECT  LOCAL  DEFAULT    1 $d
    22: 0600a6b0     4 OBJECT  LOCAL  DEFAULT    4 mFilter
    23: 0600a6b4     4 OBJECT  LOCAL  DEFAULT    4 mMask
    24: 0600a6b8  4284 OBJECT  LOCAL  DEFAULT    4 mpBuff
    25: 0600b774     4 OBJECT  LOCAL  DEFAULT    4 mPID
    26: 0600b778     1 OBJECT  LOCAL  DEFAULT    4 mTABID
    27: 0600b77c     4 OBJECT  LOCAL  DEFAULT    4 mPos
    28: 0600b780     4 OBJECT  LOCAL  DEFAULT    4 mIsChecked
    29: 0600b784     1 OBJECT  LOCAL  DEFAULT    4 mNextCounter
    30: 00000000     0 FILE    LOCAL  DEFAULT  ABS stubs.c
    31: 06000ec0     0 FUNC    LOCAL  DEFAULT    1 $a
    32: 06001130     0 OBJECT  LOCAL  DEFAULT    1 $d
    33: 06000f80     0 NOTYPE  GLOBAL DEFAULT    1 xport_pidft_set_status
    34: 06001070     0 NOTYPE  GLOBAL DEFAULT    1 CSDEMUX_Filter_Disable
    35: 06000fb8     0 NOTYPE  GLOBAL DEFAULT    1 CSDEMUX_Init
    36: 06001030     0 NOTYPE  GLOBAL DEFAULT    1 CSDEMUX_Filter_SetFilter
    37: 06000f30     0 NOTYPE  GLOBAL DEFAULT    1 getenv
    38: 06001050     0 NOTYPE  GLOBAL DEFAULT    1 CSDEMUX_Filter_CheckDataS
    39: 06001100     0 NOTYPE  GLOBAL DEFAULT    1 fp_write_serial_in
    40: 06001080     0 NOTYPE  GLOBAL DEFAULT    1 CSTUNER_Open
    41: 06009520     1 OBJECT  GLOBAL DEFAULT    4 last_section_num
    42: 06000f90     0 NOTYPE  GLOBAL DEFAULT    1 xport_filter_set_pid
    43: 06000f78     0 NOTYPE  GLOBAL DEFAULT    1 xport_pidft_set_pid
    44: 06000f58     0 NOTYPE  GLOBAL DEFAULT    1 xport_init
    45: 060010a0     0 NOTYPE  GLOBAL DEFAULT    1 fp_set_led_display_pos
    46: 06000f70     0 NOTYPE  GLOBAL DEFAULT    1 xport_pidft_bind_chl
    47: 06000000  3776 FUNC    GLOBAL DEFAULT    1 hello_world
    48: 06000f88     0 NOTYPE  GLOBAL DEFAULT    1 xport_filter_set_filter
    49: 06000f38     0 NOTYPE  GLOBAL DEFAULT    1 setenv
    50: 06001058     0 NOTYPE  GLOBAL DEFAULT    1 CSDEMUX_Filter_GetSection
    51: 06000f00     0 NOTYPE  GLOBAL DEFAULT    1 malloc
    52: 060010f0     0 NOTYPE  GLOBAL DEFAULT    1 fp_reset_serial_in
    53: 06000fa0     0 NOTYPE  GLOBAL DEFAULT    1 xport_filter_set_status
    54: 06009528     4 OBJECT  GLOBAL DEFAULT    4 cur_pmt_parser_index
    55: 06000f50     0 NOTYPE  GLOBAL DEFAULT    1 i2c_read
    56: 06000ff0     0 NOTYPE  GLOBAL DEFAULT    1 CSDEMUX_PIDFT_Close
    57: 06001040     0 NOTYPE  GLOBAL DEFAULT    1 CSDEMUX_Filter_ResetPID
    58: 06000f68     0 NOTYPE  GLOBAL DEFAULT    1 xport_set_channel_inputmo
    59: 06001028     0 NOTYPE  GLOBAL DEFAULT    1 CSDEMUX_Filter_SetFilterT
    60: 060010b0     0 NOTYPE  GLOBAL DEFAULT    1 fp_set_led_dispaly_char
    61: 06000f28     0 NOTYPE  GLOBAL DEFAULT    1 do_reset
    62: 06009520     0 NOTYPE  GLOBAL DEFAULT    3 __data_start
    63: 0600110c    44 FUNC    GLOBAL DEFAULT    1 app_startup
    64: 060010d8     0 NOTYPE  GLOBAL DEFAULT    1 fp_get_keyscan_input
    65: 06009520     0 NOTYPE  GLOBAL DEFAULT  ABS __bss_start__
    66: 060010b8     0 NOTYPE  GLOBAL DEFAULT    1 fp_set_led_display_raw
    67: 06000f48     0 NOTYPE  GLOBAL DEFAULT    1 i2c_write
    68: 06001020     0 NOTYPE  GLOBAL DEFAULT    1 CSDEMUX_Filter_Close
    69: 06000ff8     0 NOTYPE  GLOBAL DEFAULT    1 CSDEMUX_PIDFT_SetChannel
    70: 06000ec0   588 FUNC    GLOBAL DEFAULT    1 dummy
    71: 0600a2b0  1024 OBJECT  GLOBAL DEFAULT    4 testfilter
    72: 06000f60     0 NOTYPE  GLOBAL DEFAULT    1 xport_set_channel_status
    73: 06001088     0 NOTYPE  GLOBAL DEFAULT    1 CSTUNER_SetFrequency
    74: 06000f20     0 NOTYPE  GLOBAL DEFAULT    1 vprintf
    75: 060010d0     0 NOTYPE  GLOBAL DEFAULT    1 fp_set_keyscan_map
    76: 06001038     0 NOTYPE  GLOBAL DEFAULT    1 CSDEMUX_Filter_AddPID
    77: 0600b788     0 NOTYPE  GLOBAL DEFAULT  ABS __end__
    78: 0600b788     0 NOTYPE  GLOBAL DEFAULT  ABS __bss_end__
    79: 060010e0     0 NOTYPE  GLOBAL DEFAULT    1 fp_enable_serial_in
    80: 060010c0     0 NOTYPE  GLOBAL DEFAULT    1 fp_enable_keyscan
    81: 06000fe0     0 NOTYPE  GLOBAL DEFAULT    1 CSDEMUX_CHL_Disable
    82: 06000f10     0 NOTYPE  GLOBAL DEFAULT    1 udelay
    83: 060010e8     0 NOTYPE  GLOBAL DEFAULT    1 fp_disable_serial_in
    84: 06000fb0     0 NOTYPE  GLOBAL DEFAULT    1 xport_fliter_read_data
    85: 060010a8     0 NOTYPE  GLOBAL DEFAULT    1 fp_set_led_dispaly_mode
    86: 06000fd8     0 NOTYPE  GLOBAL DEFAULT    1 CSDEMUX_CHL_Enable
    87: 06000ec0     0 NOTYPE  GLOBAL DEFAULT    1 get_version
    88: 06009520     0 NOTYPE  GLOBAL DEFAULT  ABS __bss_start
    89: 0600b788     0 NOTYPE  GLOBAL DEFAULT  ABS _bss_end__
    90: 06000ef0     0 NOTYPE  GLOBAL DEFAULT    1 install_hdlr
    91: 06001018     0 NOTYPE  GLOBAL DEFAULT    1 CSDEMUX_Filter_Open
    92: 06000fa8     0 NOTYPE  GLOBAL DEFAULT    1 xport_filter_wait_data
    93: 06000f98     0 NOTYPE  GLOBAL DEFAULT    1 xport_filter_set_type
    94: 06001008     0 NOTYPE  GLOBAL DEFAULT    1 CSDEMUX_PIDFT_Enable
    95: 06009530  1024 OBJECT  GLOBAL DEFAULT    4 section_buf
    96: 0600952c     4 OBJECT  GLOBAL DEFAULT    4 service_num
    97: 060010f8     0 NOTYPE  GLOBAL DEFAULT    1 fp_set_serial_in_attr
    98: 0600b788     0 NOTYPE  GLOBAL DEFAULT  ABS _end
    99: 06000f40     0 NOTYPE  GLOBAL DEFAULT    1 simple_strtoul
   100: 06001010     0 NOTYPE  GLOBAL DEFAULT    1 CSDEMUX_PIDFT_Disable
   101: 06000fd0     0 NOTYPE  GLOBAL DEFAULT    1 CSDEMUX_CHL_SetInputMode
   102: 06000ed0     0 NOTYPE  GLOBAL DEFAULT    1 tstc
   103: 06009930  2432 OBJECT  GLOBAL DEFAULT    4 service
   104: 06000f18     0 NOTYPE  GLOBAL DEFAULT    1 get_timer
   105: 06001098     0 NOTYPE  GLOBAL DEFAULT    1 CSTUNER_ReadStatus
   106: 06000fc8     0 NOTYPE  GLOBAL DEFAULT    1 CSDEMUX_CHL_Close
   107: 06000f08     0 NOTYPE  GLOBAL DEFAULT    1 free
   108: 06009521     1 OBJECT  GLOBAL DEFAULT    4 current_section_num
   109: 06009520     0 NOTYPE  GLOBAL DEFAULT  ABS _edata
   110: 06000ec8     0 NOTYPE  GLOBAL DEFAULT    1 getc
   111: 06000ee8     0 NOTYPE  GLOBAL DEFAULT    1 printf
   112: 06000ee0     0 NOTYPE  GLOBAL DEFAULT    1 puts
   113: 06001048     0 NOTYPE  GLOBAL DEFAULT    1 CSDEMUX_Filter_ReadWait
   114: 06001068     0 NOTYPE  GLOBAL DEFAULT    1 CSDEMUX_Filter_Enable
   115: 06000fc0     0 NOTYPE  GLOBAL DEFAULT    1 CSDEMUX_CHL_Open
   116: 06000ef8     0 NOTYPE  GLOBAL DEFAULT    1 free_hdlr
   117: 06009524     4 OBJECT  GLOBAL DEFAULT    4 sdtcomplete
   118: 06001060     0 NOTYPE  GLOBAL DEFAULT    1 CSDEMUX_Filter_ReadSectio
   119: 06001078     0 NOTYPE  GLOBAL DEFAULT    1 CSTUNER_Init
   120: 06000fe8     0 NOTYPE  GLOBAL DEFAULT    1 CSDEMUX_PIDFT_Open
   121: 06000ed8     0 NOTYPE  GLOBAL DEFAULT    1 putc
   122: 06001000     0 NOTYPE  GLOBAL DEFAULT    1 CSDEMUX_PIDFT_SetPID
   123: 06001090     0 NOTYPE  GLOBAL DEFAULT    1 CSTUNER_GetTunerInfo
   124: 060010c8     0 NOTYPE  GLOBAL DEFAULT    1 fp_disable_keyscan

No version information found in this file.
