
#define PLL_533M // PLL_600M // PLL_600M //PLL_648M //PLL_533M //PLL_667M

#ifdef CONFIG_ARCH_CSM1201 

#if defined(CONFIG_256M_DDR) //for ELPIDA 1116ABG 
//CSM1201 && 256M
  #if defined(CONFIG_16BIT_DDR) // 16bit mode for csm1201j
     #if defined(PLL_400M)
        #define DDR_PLL         0x00000a4a  //0x10171100
        #define DDR_TIMING      0x8526146c  //0x10110000
        #define DDR_MOD         0x923e81a5  //0x10110004
        #define DDR_DELAY_0     0x09090909  //0x10110008              
        #define DDR_DELAY_1     0x09090909  //0x1011000c
        #define DDR_MISC        0x70000465  //0x10110010              
     #endif
     #if defined(PLL_533M)
        #define DDR_PLL         0x0000084f  //0x10171100
        #define DDR_TIMING      0x973820b2  //0x10110000
        #define DDR_MOD         0x947d0245  //0x10110004
        #define DDR_DELAY_0     0x09090909  //0x10110008              
        #define DDR_DELAY_1     0x09090909  //0x1011000c
        #define DDR_MISC        0x70000465  //0x10110010              
     #endif 
   #else //32 bit nomal mode
     #if defined(PLL_667M)
        #define DDR_PLL         0x00000c94  //0x10171100
		#if 1
		    #define DDR_TIMING      0x98C820B4  //0x10110000
	        #define DDR_MOD         0x94A252B5  //0x10110004
	    #else
		    #define DDR_TIMING      0x98c820b4  //0x10110000
	        #define DDR_MOD         0x945142b5  //0x10110004
		#endif
        #define DDR_DELAY_0     0x06060606  //0x10110008              
        #define DDR_DELAY_1     0x06060606  //0x1011000c
        #define DDR_MISC        0x30100465  //0x10110010              
     #elif defined(PLL_533M)
        #define DDR_PLL         0x0000084f  //0x10171100
     #if 1
     	#define DDR_TIMING      0x88c7a0b4  //0x10110000
        #define DDR_MOD         0x944fe2a5  //0x10110004
        #define DDR_DELAY_0     0x07070707  //0x10110008              
        #define DDR_DELAY_1     0x07070707  //0x1011000c
        #define DDR_MISC        0x30100465  //0x10110010        
     #else
        #define DDR_TIMING      0x77369890  //0x10110000
        #define DDR_MOD         0x93410225  //0x10110004
        #define DDR_DELAY_0     0x08080808  //0x10110008              
        #define DDR_DELAY_1     0x08080808  //0x1011000c
        #define DDR_MISC        0x30100465  //0x10110010 
     #endif             
     #elif defined(PLL_648M)
        #define DDR_PLL         0x00000648
	    #define DDR_TIMING      0x88c7a0b4  //0x10110000
        #define DDR_MOD         0x944fe2a5  //0x10110004
        #define DDR_DELAY_0     0x06060606  //0x10110008              
        #define DDR_DELAY_1     0x06060606  //0x1011000c
        #define DDR_MISC        0x30100465  //0x10110010              
      #elif defined(PLL_600M)
        #define DDR_PLL         0x00000859  //0x10171100
	    #define DDR_TIMING      0x88c7a0b4  //0x10110000
        #define DDR_MOD         0x944fe2a5  //0x10110004
        #define DDR_DELAY_0     0x07070707  //0x10110008
        #define DDR_DELAY_1     0x07070707  //0x1011000c
        #define DDR_MISC        0x30100465  //0x10110010
    #endif
   #endif

#else  // CSM1201 && 128M
  #if defined(CONFIG_16BIT_DDR) // 16bit mode for csm1201j 
     #if defined(PLL_400M)  // for NANYA 32M16CG-3C
        #define DDR_PLL         0x00000a4a  //0x10171100
        #define DDR_TIMING      0x8526146c  //0x10110000
        #define DDR_MOD         0x423e8155  //0x10110004
        #define DDR_DELAY_0     0x06060606  //0x10110008              
        #define DDR_DELAY_1     0x06060606  //0x1011000c
        #define DDR_MISC        0x70000465  //0x10110010              
     #endif
     #if defined(PLL_533M)
        #define DDR_PLL         0x0000084f  //0x10171100
        #define DDR_TIMING      0x973820b2  //0x10110000
        #define DDR_MOD         0x4441e1d5  //0x10110004
        #define DDR_DELAY_0     0x06060606  //0x10110008              
        #define DDR_DELAY_1     0x06060606  //0x1011000c
        #define DDR_MISC        0x70000465  //0x10110010              
     #endif 
     #if defined(PLL_600M)
        #define DDR_PLL         0x00000859  //0x10171100
        #define DDR_TIMING      0x984820b3  //0x10110000
        #define DDR_MOD         0x44493205  //0x10110004
        #define DDR_DELAY_0     0x06060606  //0x10110008              
        #define DDR_DELAY_1     0x06060606  //0x1011000c
        #define DDR_MISC        0x70000465  //0x10110010              
     #endif 
#else  // nomal 32bit mode for csm1201
    #if defined(PLL_667M)
      #define DDR_PLL         0x000014f7  //0x10171100
      #define DDR_TIMING      0x98c820b4  //0x10110000
      #define DDR_MOD         0x44514235  //0x10110004
      #define DDR_DELAY_0     0x06060606  //0x10110008              
      #define DDR_DELAY_1     0x06060606  //0x1011000c
      #define DDR_MISC        0x30100465  //0x10110010              
    #elif defined(PLL_533M)
      #define DDR_PLL         0x0000084f  //0x10171100
      #define DDR_TIMING      0x973820b2  //0x10110000
      #define DDR_MOD         0x4441e1d5  //0x10110004
      #define DDR_DELAY_0     0x08080808  //0x10110008              
      #define DDR_DELAY_1     0x08080808  //0x1011000c
      #define DDR_MISC        0x30100465  //0x10110010              
    #elif defined(PLL_600M)
      #define DDR_PLL         0x00000859
	    #define DDR_TIMING      0x88c7a0b4  //0x10110000
        #define DDR_MOD         0x944fe2a5  //0x10110004
        #define DDR_DELAY_0     0x07070707  //0x10110008              
        #define DDR_DELAY_1     0x07070707  //0x1011000c
        #define DDR_MISC        0x30100465  //0x10110010         
    #elif defined(PLL_648M)
      #define DDR_PLL         0x00000648  //0x10171100
      #define DDR_TIMING      0x98c820b4  //0x10110000
      #define DDR_MOD         0x444f2235  //0x10110004
      #define DDR_DELAY_0     0x06060606  //0x10110008              
      #define DDR_DELAY_1     0x06060606  //0x1011000c
      #define DDR_MISC        0x30100465  //0x10110010              
    #elif defined(PLL_400M) // for elpida_32Mbx16  
      #define DDR_PLL         0x00000c59  //0x10171100
      #define DDR_TIMING      0x84a6146c  //0x10110000
      #define DDR_MOD         0x4230c155  //0x10110004
      #define DDR_DELAY_0     0x06060606  //0x10110008              
      #define DDR_DELAY_1     0x06060606  //0x1011000c
      #define DDR_MISC        0x30100465  //0x10110010              


#endif
  #endif // end 16/32bit select

#endif // CSM1201 && 128M

#elif defined(CONFIG_ARCH_CSM1200) && defined(CONFIG_16BIT_DDR) //special for 1200.1 with 16bitDDR
// only for 533MHz
     #define DDR_PLL         0x0000084f  //0x10171100
     #define DDR_TIMING      0x8eedaea3  //0x10110000
     #define DDR_MOD         0x43412293  //0x10110004
     #define DDR_DELAY_0     0x06060606  //0x10110008              
     #define DDR_DELAY_1     0x06060606  //0x1011000c
     #define DDR_MISC        0x51100000  //0x10110010              
 

#endif // CONFIG_ARCH_CSM1201
