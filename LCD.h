#define GPIO_PORTA_DATA_R			 (*((volatile unsigned long *)0x400043FC))
#define GPIO_PORTA_DIR_R			 (*((volatile unsigned long *)0x40004400))
#define GPIO_PORTA_AFSEL_R		 (*((volatile unsigned long *)0x40004420))
#define GPIO_PORTA_PUR_R 				(*((volatile unsigned long *)0x40004510))
#define GPIO_PORTA_DEN_R			   (*((volatile unsigned long *)0x4000451C))
#define GPIO_PORTA_CR_R 				(*((volatile unsigned long *)0x40004524))
#define GPIO_PORTA_AMSEL_R			 (*((volatile unsigned long *)0x40004528))
#define GPIO_PORTA_PCTL_R				 (*((volatile unsigned long *)0x4000452C))
	
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
	
#define GPIO_PORTB_DATA_R       (*((volatile unsigned long *)0x400053FC))
#define GPIO_PORTB_DIR_R        (*((volatile unsigned long *)0x40005400))
#define GPIO_PORTB_AFSEL_R      (*((volatile unsigned long *)0x40005420))
#define GPIO_PORTB_PUR_R        (*((volatile unsigned long *)0x40005510))
#define GPIO_PORTB_DEN_R        (*((volatile unsigned long *)0x4000551C))
#define GPIO_PORTB_CR_R         (*((volatile unsigned long *)0x40005524))
#define GPIO_PORTB_AMSEL_R      (*((volatile unsigned long *)0x40005528))
#define GPIO_PORTB_PCTL_R       (*((volatile unsigned long *)0x4000552C))
	

#define lcdRS    (*((volatile unsigned long *)0x40004020))  //PA3
#define lcdEN    (*((volatile unsigned long *)0x40004010))  //PA2
#define lcdDB    (*((volatile unsigned long *)0x400050F0))  //Pb2-5

extern void PortA_Init(void);

extern void PortB_Init(void);

extern void lcd_Init(void);

extern void lcdENPulse(void);

extern void lcdWriteCommand(unsigned char c);

extern void lcdClearScreen(void);

extern void lcdGoto(unsigned char row, unsigned char column);

extern void lcdWriteData(char c); 

extern void lcdWriteRamString(char *string);

extern void setCGRAMaddr(unsigned char address);

#define SYSCTL_RIS_R          (*((volatile unsigned long *)0x400FE050))	
#define SYSCTL_RCC_R          (*((volatile unsigned long *)0x400FE060))
#define SYSCTL_RCC2_R         (*((volatile unsigned long *)0x400FE070))	

//SysTick related Defines	
#define NVIC_ST_CTRL_R        (*((volatile unsigned long *)0xE000E010))
#define NVIC_ST_RELOAD_R      (*((volatile unsigned long *)0xE000E014))
#define NVIC_ST_CURRENT_R     (*((volatile unsigned long *)0xE000E018))


extern void SysTick_Init(void);

extern void PLL_Init(void);

extern void SysTick_Wait(unsigned long delay);

extern void SysTick_Wait450ns(unsigned long delay);

extern void SysTick_Wait1us(unsigned long delay);

extern void SysTick_Wait1ms(unsigned long delay);

extern void SysTick_Wait10ms(unsigned long delay);


