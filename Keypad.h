#include "LCD.h"

/*
PD0 connected to column 1 (Pin 1) 
PD1 connected to column 2 (Pin 2) 
PD2 connected to column 3 (Pin 3) 
PD3 connected to column 4 (Pin 4) 

PE0 connected to row 1 (Pin 5)
PE1 connected to row 2 (Pin 6)
PE2 connected to row 3 (Pin 7) 
PE3 connected to row 4 (Pin 8) 

C1 C2 C3 C4
[1][2][3][+] R1
[4][5][6][-] R2
[7][8][9][x] R3
[*][0][=][÷] R4

Program initialise external keypad and button.
Reads keypad saving the button press as a variable,
to be called in main.c
Each column is individually set high then low in sequence,
any button presses link the high column signal to the low
row signal seting the inputs as high.
Each combination of input has been assigned a corresponding
number value.

*/


//Port D Columns
#define GPIO_PORTD_DATA_R       (*((volatile unsigned long *)0x400073FC))
#define GPIO_PORTD_DIR_R        (*((volatile unsigned long *)0x40007400))
#define GPIO_PORTD_AFSEL_R      (*((volatile unsigned long *)0x40007420))
#define GPIO_PORTD_PUR_R        (*((volatile unsigned long *)0x40007510))
#define GPIO_PORTD_DEN_R        (*((volatile unsigned long *)0x4000751C))
#define GPIO_PORTD_AMSEL_R      (*((volatile unsigned long *)0x40007528))
#define GPIO_PORTD_PCTL_R       (*((volatile unsigned long *)0x4000752C))
#define GPIO_PORTD_CR_R         (*((volatile unsigned long *)0x40007524))

#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
	
//Port E Rows
#define GPIO_PORTE_DATA_R       (*((volatile unsigned long *)0x400243FC))
#define GPIO_PORTE_DIR_R        (*((volatile unsigned long *)0x40024400))
#define GPIO_PORTE_AFSEL_R      (*((volatile unsigned long *)0x40024420))
#define GPIO_PORTE_PUR_R        (*((volatile unsigned long *)0x40024510))
#define GPIO_PORTE_DEN_R        (*((volatile unsigned long *)0x4002451C))
#define GPIO_PORTE_AMSEL_R      (*((volatile unsigned long *)0x40024528))
#define GPIO_PORTE_PCTL_R       (*((volatile unsigned long *)0x4002452C))
#define GPIO_PORTE_CR_R         (*((volatile unsigned long *)0x40024524))
#define PE4											(*((volatile unsigned long *)0x40024040))

	
#define column                  (*((volatile unsigned long *)0x4000703C))  
#define row                     (*((volatile unsigned long *)0x4002403C))


//Initialise Keypad and external button
void keypad_Init(void) { volatile unsigned long delay;
	
	SYSCTL_RCGC2_R |= 0x08;           //Activate clock bit 3
	
	GPIO_PORTD_CR_R = 0x0F;           // Allow changes to PD3-0
	GPIO_PORTD_AMSEL_R = 0x00;        // Disable analog function
	GPIO_PORTD_PCTL_R = 0x00000000;   // GPIO clear bit PCTL  
  GPIO_PORTD_DIR_R = 0x0F;          // PD3,PD2,PD1,PD0 output
  GPIO_PORTD_AFSEL_R = 0x00;        // No alternate function
  GPIO_PORTD_PUR_R = 0x00;          // Disable pullup resistors on PD4-PD0       
  GPIO_PORTD_DEN_R = 0x0F;          // Enable digital pins PD3-PD0
	
	SYSCTL_RCGC2_R |= 0x10;           //Activate clock bit 4 
	
	GPIO_PORTE_CR_R = 0x1F;           // Allow changes to PE4-0       
  GPIO_PORTE_AMSEL_R = 0x00;        // Disable analog function
  GPIO_PORTE_PCTL_R = 0x00000000;   // GPIO clear bit PCTL  
  GPIO_PORTE_DIR_R = 0x00;          // PE4,PE3,PE2,PE1,PE0 input   
  GPIO_PORTE_AFSEL_R = 0x00;        // No alternate function
  GPIO_PORTE_PUR_R = 0x00;          // Disable pullup resistors on PE4-PE0       
  GPIO_PORTE_DEN_R = 0x1F;          // Enable digital pins PE4-PE0
	
}


//Reading Keypad
unsigned char readKeypad(void){
	int Button; //Store for button press
	int count; //Column counter
	
	column = 0x01; //Sets column 1 high
	
	for (count=0;count<=3;count++){ //creates loop to run through 4 columns
		
		if(PE4==0x10){ //External button
			Button = 55;
		}
		
		//Check column 1
		if(column==0x01){
			SysTick_Wait(160);
			if (row==0x01){ //Row 1
				Button = 1;
				lcdWriteRamString("1");
			}
			else if (row==0x02){ //Row 2
				Button = 4;
				lcdWriteRamString("4");
			}
			else if (row==0x04){ //Row 3
				Button = 7;
				lcdWriteRamString("7");
			}
			else if (row==0x08){ //Row 4
				Button = 14;//Column 1 Row 4
				lcdWriteRamString("*");
			}
		}
		
		//Check column 2
		else if (column==0x02){     
			SysTick_Wait(160);
			if (row==0x01){ //Row 1
				Button = 2;
				lcdWriteRamString("2");
			}
			else if (row==0x02){ //Row 2
				Button = 5;
				lcdWriteRamString("5");
			}
			else if (row==0x04){ //Row 3
				Button = 8;
				lcdWriteRamString("8");
			}
			else if (row==0x08){ //Row 4
				Button = 0;					
				lcdWriteRamString("0");
			}
		}
		
		//Check column 3
		else if (column==0x04){        
			SysTick_Wait(160);
			if (row==0x1){ //Row 1
				Button = 3;
				lcdWriteRamString("3");
			}
			else if (row==0x02){ //Row 2
				Button = 6;
				lcdWriteRamString("6");
			}
			else if (row==0x04){ //Row 3
				Button = 9;
				lcdWriteRamString("9");
			}
			else if (row==0x08){ //Row 4
				Button = 34;//Column 3 Row 4 
				lcdWriteRamString("=");
			}
		}
				
		//Check column 4
		else if (column==0x08){        
			SysTick_Wait(160);
			if (row==0x1){ //Row 1
				Button = 41; //Column 4 Row 1 
				lcdWriteRamString("+");
			}
			else if (row==0x02){ //Row 2
				Button = 42; //Column 4 Row 2
				lcdWriteRamString("-");
			}
			else if (row==0x04){ //Row 3
				Button = 43; //Column 4 Row 3 
				lcdWriteRamString("x");
			}
			else if (row==0x08){ //Row 4
				Button = 44; //Column 4 Row 4
				lcdWriteRamString("/");
			}
		}
	
	column=column<<1;
		if(count==3){
			column=0x01;
		}
		SysTick_Wait(40000000);
	}
return Button;
}	








