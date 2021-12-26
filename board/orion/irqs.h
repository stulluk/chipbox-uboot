#ifndef __ORION_IRQS_H__
#define __ORION_IRQS_H__


typedef int (* irq_handle)(unsigned int, void *);


#define PA_VIC_BASE		    0x10140000
#define VIC_IRQ_STATUS	0x28	/*mask status*/
#define VIC_IRQ_ENABLE	0
#define VIC_IRQ_MASK	8	

#define INT_VICSOURCE21                 21      
#define INT_VICSOURCE22                 22     
#define INT_VICSOURCE23                 23     
#define INT_VICSOURCE24                 24      
#define INT_VICSOURCE25                 25    
#define INT_VICSOURCE26                 26      
#define INT_VICSOURCE27                 27      
#define INT_VICSOURCE28                 28      
#define INT_VICSOURCE29                 29        /* Ethernet */
#define INT_VICSOURCE30                 30      
#define INT_VICSOURCE31                 31    


#define IRQ_VIC_START		0
#define IRQ_WDOGINT		(IRQ_VIC_START + INT_WDOGINT)
#define IRQ_SOFTINT		(IRQ_VIC_START + INT_SOFTINT)
#define IRQ_COMMRx		(IRQ_VIC_START + INT_COMMRx)
#define IRQ_COMMTx		(IRQ_VIC_START + INT_COMMTx)
#define IRQ_TIMERINT0_1		(IRQ_VIC_START + INT_TIMERINT0_1)
#define IRQ_TIMERINT2_3		(IRQ_VIC_START + INT_TIMERINT2_3)
#define IRQ_GPIOINT0		(IRQ_VIC_START + INT_GPIOINT0)
#define IRQ_GPIOINT1		(IRQ_VIC_START + INT_GPIOINT1)
#define IRQ_GPIOINT2		(IRQ_VIC_START + INT_GPIOINT2)
#define IRQ_GPIOINT3		(IRQ_VIC_START + INT_GPIOINT3)
#define IRQ_RTCINT		(IRQ_VIC_START + INT_RTCINT)
#define IRQ_SSPINT		(IRQ_VIC_START + INT_SSPINT)
#define IRQ_UARTINT0		(IRQ_VIC_START + INT_UARTINT0)
#define IRQ_UARTINT1		(IRQ_VIC_START + INT_UARTINT1)
#define IRQ_UARTINT2		(IRQ_VIC_START + INT_UARTINT2)
#define IRQ_SCIINT		(IRQ_VIC_START + INT_SCIINT)
#define IRQ_CLCDINT		(IRQ_VIC_START + INT_CLCDINT)
#define IRQ_DMAINT		(IRQ_VIC_START + INT_DMAINT)
#define IRQ_PWRFAILINT 		(IRQ_VIC_START + INT_PWRFAILINT)
#define IRQ_MBXINT		(IRQ_VIC_START + INT_MBXINT)
#define IRQ_GNDINT		(IRQ_VIC_START + INT_GNDINT)
#define IRQ_VICSOURCE21		(IRQ_VIC_START + INT_VICSOURCE21)
#define IRQ_VICSOURCE22		(IRQ_VIC_START + INT_VICSOURCE22)
#define IRQ_VICSOURCE23		(IRQ_VIC_START + INT_VICSOURCE23)
#define IRQ_VICSOURCE24		(IRQ_VIC_START + INT_VICSOURCE24)
#define IRQ_VICSOURCE25		(IRQ_VIC_START + INT_VICSOURCE25)
#define IRQ_VICSOURCE26		(IRQ_VIC_START + INT_VICSOURCE26)
#define IRQ_VICSOURCE27		(IRQ_VIC_START + INT_VICSOURCE27)
#define IRQ_VICSOURCE28		(IRQ_VIC_START + INT_VICSOURCE28)
#define IRQ_VICSOURCE29		(IRQ_VIC_START + INT_VICSOURCE29)
#define IRQ_VICSOURCE30		(IRQ_VIC_START + INT_VICSOURCE30)
#define IRQ_VICSOURCE31		(IRQ_VIC_START + INT_VICSOURCE31)
#define IRQ_VIC_END		    (IRQ_VIC_START + 31)

#define IRQMASK_WDOGINT		INTMASK_WDOGINT
#define IRQMASK_SOFTINT		INTMASK_SOFTINT
#define IRQMASK_COMMRx 		INTMASK_COMMRx
#define IRQMASK_COMMTx 		INTMASK_COMMTx
#define IRQMASK_TIMERINT0_1	INTMASK_TIMERINT0_1
#define IRQMASK_TIMERINT2_3	INTMASK_TIMERINT2_3
#define IRQMASK_GPIOINT0	INTMASK_GPIOINT0
#define IRQMASK_GPIOINT1	INTMASK_GPIOINT1
#define IRQMASK_GPIOINT2	INTMASK_GPIOINT2
#define IRQMASK_GPIOINT3	INTMASK_GPIOINT3
#define IRQMASK_RTCINT 		INTMASK_RTCINT
#define IRQMASK_SSPINT 		INTMASK_SSPINT
#define IRQMASK_UARTINT0	INTMASK_UARTINT0
#define IRQMASK_UARTINT1	INTMASK_UARTINT1
#define IRQMASK_UARTINT2	INTMASK_UARTINT2
#define IRQMASK_SCIINT 		INTMASK_SCIINT
#define IRQMASK_CLCDINT		INTMASK_CLCDINT
#define IRQMASK_DMAINT 		INTMASK_DMAINT
#define IRQMASK_PWRFAILINT	INTMASK_PWRFAILINT
#define IRQMASK_MBXINT 		INTMASK_MBXINT
#define IRQMASK_GNDINT 		INTMASK_GNDINT
#define IRQMASK_VICSOURCE21	INTMASK_VICSOURCE21
#define IRQMASK_VICSOURCE22	INTMASK_VICSOURCE22
#define IRQMASK_VICSOURCE23	INTMASK_VICSOURCE23
#define IRQMASK_VICSOURCE24	INTMASK_VICSOURCE24
#define IRQMASK_VICSOURCE25	INTMASK_VICSOURCE25
#define IRQMASK_VICSOURCE26	INTMASK_VICSOURCE26
#define IRQMASK_VICSOURCE27	INTMASK_VICSOURCE27
#define IRQMASK_VICSOURCE28	INTMASK_VICSOURCE28
#define IRQMASK_VICSOURCE29	INTMASK_VICSOURCE29
#define IRQMASK_VICSOURCE30	INTMASK_VICSOURCE30
#define IRQMASK_VICSOURCE31	INTMASK_VICSOURCE31

/* 
 *  FIQ interrupts definitions are the same the INT definitions.
 */
#define FIQ_WDOGINT		INT_WDOGINT
#define FIQ_SOFTINT		INT_SOFTINT
#define FIQ_COMMRx		INT_COMMRx
#define FIQ_COMMTx		INT_COMMTx
#define FIQ_TIMERINT0_1		INT_TIMERINT0_1
#define FIQ_TIMERINT2_3		INT_TIMERINT2_3
#define FIQ_GPIOINT0		INT_GPIOINT0
#define FIQ_GPIOINT1		INT_GPIOINT1
#define FIQ_GPIOINT2		INT_GPIOINT2
#define FIQ_GPIOINT3		INT_GPIOINT3
#define FIQ_RTCINT		INT_RTCINT
#define FIQ_SSPINT		INT_SSPINT
#define FIQ_UARTINT0		INT_UARTINT0
#define FIQ_UARTINT1		INT_UARTINT1
#define FIQ_UARTINT2		INT_UARTINT2
#define FIQ_SCIINT		INT_SCIINT
#define FIQ_CLCDINT		INT_CLCDINT
#define FIQ_DMAINT		INT_DMAINT
#define FIQ_PWRFAILINT 		INT_PWRFAILINT
#define FIQ_MBXINT		INT_MBXINT
#define FIQ_GNDINT		INT_GNDINT
#define FIQ_VICSOURCE21		INT_VICSOURCE21
#define FIQ_VICSOURCE22		INT_VICSOURCE22
#define FIQ_VICSOURCE23		INT_VICSOURCE23
#define FIQ_VICSOURCE24		INT_VICSOURCE24
#define FIQ_VICSOURCE25		INT_VICSOURCE25
#define FIQ_VICSOURCE26		INT_VICSOURCE26
#define FIQ_VICSOURCE27		INT_VICSOURCE27
#define FIQ_VICSOURCE28		INT_VICSOURCE28
#define FIQ_VICSOURCE29		INT_VICSOURCE29
#define FIQ_VICSOURCE30		INT_VICSOURCE30
#define FIQ_VICSOURCE31		INT_VICSOURCE31


#define FIQMASK_WDOGINT		INTMASK_WDOGINT
#define FIQMASK_SOFTINT		INTMASK_SOFTINT
#define FIQMASK_COMMRx 		INTMASK_COMMRx
#define FIQMASK_COMMTx 		INTMASK_COMMTx
#define FIQMASK_TIMERINT0_1	INTMASK_TIMERINT0_1
#define FIQMASK_TIMERINT2_3	INTMASK_TIMERINT2_3
#define FIQMASK_GPIOINT0	INTMASK_GPIOINT0
#define FIQMASK_GPIOINT1	INTMASK_GPIOINT1
#define FIQMASK_GPIOINT2	INTMASK_GPIOINT2
#define FIQMASK_GPIOINT3	INTMASK_GPIOINT3
#define FIQMASK_RTCINT 		INTMASK_RTCINT
#define FIQMASK_SSPINT 		INTMASK_SSPINT
#define FIQMASK_UARTINT0	INTMASK_UARTINT0
#define FIQMASK_UARTINT1	INTMASK_UARTINT1
#define FIQMASK_UARTINT2	INTMASK_UARTINT2
#define FIQMASK_SCIINT 		INTMASK_SCIINT
#define FIQMASK_CLCDINT		INTMASK_CLCDINT
#define FIQMASK_DMAINT 		INTMASK_DMAINT
#define FIQMASK_PWRFAILINT	INTMASK_PWRFAILINT
#define FIQMASK_MBXINT 		INTMASK_MBXINT
#define FIQMASK_GNDINT 		INTMASK_GNDINT
#define FIQMASK_VICSOURCE21	INTMASK_VICSOURCE21
#define FIQMASK_VICSOURCE22	INTMASK_VICSOURCE22
#define FIQMASK_VICSOURCE23	INTMASK_VICSOURCE23
#define FIQMASK_VICSOURCE24	INTMASK_VICSOURCE24
#define FIQMASK_VICSOURCE25	INTMASK_VICSOURCE25
#define FIQMASK_VICSOURCE26	INTMASK_VICSOURCE26
#define FIQMASK_VICSOURCE27	INTMASK_VICSOURCE27
#define FIQMASK_VICSOURCE28	INTMASK_VICSOURCE28
#define FIQMASK_VICSOURCE29	INTMASK_VICSOURCE29
#define FIQMASK_VICSOURCE30	INTMASK_VICSOURCE30
#define FIQMASK_VICSOURCE31	INTMASK_VICSOURCE31

#define NR_IRQS			32	


#endif
