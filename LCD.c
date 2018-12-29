#include "LCD.h"

void PortA_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000001;     // 1) A clock
  delay = SYSCTL_RCGC2_R;           // delay   
  GPIO_PORTA_CR_R = 0x0C;           // 2)allow changes to PA2, PA3    
  GPIO_PORTA_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTA_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTA_DIR_R = 0x0C;          // 5) PA2, PA3 output 
  GPIO_PORTA_AFSEL_R = 0x00;        // 6) no alternate function
  //GPIO_PORTA_PUR_R = 0x11;                 
  GPIO_PORTA_DEN_R = 0x0C;          // 7) enable digital pins     
}

void PortB_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000002;     // 1) B clock
  delay = SYSCTL_RCGC2_R;           // delay   
  GPIO_PORTB_CR_R = 0x3C;           // allow changes to PB2-5       
  GPIO_PORTB_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTB_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTB_DIR_R = 0x3C;          // 5) PB2-5 output   
  GPIO_PORTB_AFSEL_R = 0x00;        // 6) no alternate function
  //GPIO_PORTB_PUR_R = 0x11;          // enable pullup resistors on        
  GPIO_PORTB_DEN_R = 0x3C;          // 7) enable digital pins        
}

void lcd_Init(void){  //uses datasheet flowchart pg45 translate the steps into code 
PortA_Init();
PortB_Init();
SysTick_Wait(1600000); //20ms 
	
lcdEN = 0x0;   //Init lcdEN to 0
lcdDB = 0x0;	 //Init lcdDB to 0
	
SysTick_Wait10ms(2);  // wait 20ms    

lcdRS =0x00;	
lcdDB = 0x0C;                        
lcdENPulse();
SysTick_Wait(328000); //wait 4.1ms        

lcdRS =0x00;	
lcdDB = 0x0C;                        
lcdENPulse();
SysTick_Wait1us(150); //wait 150us    

lcdRS =0x00;	
lcdDB = 0x0C;                        
lcdENPulse();
SysTick_Wait1us(150); //wait 150us

//lcdRS =0x00;
lcdDB = 0x08;
lcdENPulse();
SysTick_Wait1us(150); //wait 150us

//lcdWriteCommand(0x20);    
lcdWriteCommand(0x28);//    4bit 2line 5x8char
lcdWriteCommand(0x08); //Display off control
lcdWriteCommand(0x0F); //Dislpay on control 
lcdWriteCommand(0x01);
lcdWriteCommand(0x06); //entry mode set

/*
lcdWriteCommand(0xC);    //4 - 0001   dsp
lcdWriteCommand(0x0);    //5 - 0110    
lcdWriteCommand(0x6);
*/
}

void lcdENPulse(){
	//lcdDB = 0x00;
	//lcdRS = 0x00;
	lcdEN = 0x04;     // pulse EN to high
	SysTick_Wait(36); // pulse width of 450ns
	lcdEN = 0x00;     // pulse EN to low
	SysTick_Wait(36);
}

void lcdWriteCommand(unsigned char c){
lcdRS = 0x00;
//lcdDB = c;
	
SysTick_Wait(3200);
	
//HIGH NIBBLE
lcdDB = (c>>2)&0x3C;  
lcdENPulse();
SysTick_Wait1ms(5); //5ms

	//LOW NIBBLE 
lcdDB =  (c<<2)&0x3C;    
lcdENPulse();
SysTick_Wait1ms(5); //5ms delay
}

void lcdWriteData(char c){
lcdRS = 0x08;
//lcdDB = c;
	
SysTick_Wait1us(50);       //50us delay
//bit shift
//bit mask
lcdDB = (c>>2)&0x3C;  
lcdENPulse();
SysTick_Wait1us(200);     //200us delay
	
//bit mask
lcdDB = (c<<2)&0x3C;
lcdENPulse();
SysTick_Wait1us(1200);     //200us delay
}

void lcdClearScreen(void){
 lcdWriteCommand(0x01); //clears screen
 lcdWriteCommand(0x02); // returns to original position
}
	
//moves cursor position to a specified DDRAM address
void lcdGoto(unsigned char row, unsigned char column) { 
	
	if (row == 0) {
		lcdWriteCommand(0x80 + column);
	} else {
		lcdWriteCommand(0xC0 + column);
	}
	
}

void lcdWriteRamString(char *string) {
	while(*string) {
		lcdWriteData(*string);
		string++;
	}
}

//void setCGRAMaddr(unsigned char address) {
//	lcdWriteCommand(0x40 + address);
//}

void SysTick_Init(void){
  NVIC_ST_CTRL_R = 0;                   // disable SysTick during setup
  NVIC_ST_RELOAD_R = 0x00FFFFFF;        // maximum reload value
  NVIC_ST_CURRENT_R = 0;                // any write to current clears it             
  NVIC_ST_CTRL_R = 0x00000005;          // enable SysTick with core clock
}

void PLL_Init(void){
  // 0) Use RCC2
  SYSCTL_RCC2_R |=  0x80000000;  // USERCC2
  // 1) bypass PLL while initializing
  SYSCTL_RCC2_R |=  0x00000800;  // BYPASS2, PLL bypass
  // 2) select the crystal value and oscillator source
  SYSCTL_RCC_R = (SYSCTL_RCC_R &~0x000007C0)   // clear XTAL field, bits 10-6
                 + 0x00000540;   // 10101, configure for 16 MHz crystal
  SYSCTL_RCC2_R &= ~0x00000070;  // configure for main oscillator source
  // 3) activate PLL by clearing PWRDN
  SYSCTL_RCC2_R &= ~0x00002000;
  // 4) set the desired system divider
  SYSCTL_RCC2_R |= 0x40000000;   // use 400 MHz PLL
  SYSCTL_RCC2_R = (SYSCTL_RCC2_R&~ 0x1FC00000)  // clear system clock divider
                  + (4<<22);      // configure for 80 MHz clock 
	//*** YOU CAN CHANGE The 50 MHz Frequency above, by changing the Dividor "7" to 
	//*** any desired frequency, i.e (4<<22) makes it 80 MHz etc. ..
	
  // 5) wait for the PLL to lock by polling PLLLRIS
  while((SYSCTL_RIS_R&0x00000040)==0){};  // wait for PLLRIS bit
  // 6) enable use of PLL by clearing BYPASS
  SYSCTL_RCC2_R &= ~0x00000800;
}



// The delay parameter is in units of the 50 MHz core clock. (20 ns)
void SysTick_Wait(unsigned long delay){
  NVIC_ST_RELOAD_R = delay-1;  // number of counts to wait
  NVIC_ST_CURRENT_R = 0;       // any value written to CURRENT clears
  while((NVIC_ST_CTRL_R&0x00010000)==0){ // wait for count flag
  }
}

void SysTick_Wait450ns(unsigned long delay){
    unsigned long i;
    for(i=0; i<delay; i++)    {
       SysTick_Wait(36);                 // wait 450ns
    }
}

// 80*12.5ns => 1 us
void SysTick_Wait1us(unsigned long delay){
    unsigned long i;
    for(i=0; i<delay; i++)    {
       SysTick_Wait(80);                 // wait 1us
    }
}

// 80000*12.5ns => 1 ms
void SysTick_Wait1ms(unsigned long delay){
    unsigned long i;
    for(i=0; i<delay; i++)    {
       SysTick_Wait(80000);              // wait 1ms
    }
}

//800000*12.5ns => 10 ms
void SysTick_Wait10ms(unsigned long delay){
  unsigned long i;
  for(i=0; i<delay; i++){
    SysTick_Wait(800000);  // wait 10ms
  }
}










