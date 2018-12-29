#include "Keypad.h"

void lcdWriteCommand(unsigned char c);
void lcdWriteRamString(char *string);
void SysTick_Wait(unsigned long delay);

void keypad_Init(void) { volatile unsigned long delay;
	
	SYSCTL_RCGC2_R |= 0x08;           //Activate clock bit 3
	
	GPIO_PORTD_CR_R = 0x0F;           // Allow changes to PD3-0
	GPIO_PORTD_AMSEL_R = 0x00;        // Disable analog function
	GPIO_PORTD_PCTL_R = 0x00000000;   // GPIO clear bit PCTL  
  GPIO_PORTD_DIR_R = 0x0F;          // PD3,PD2,PD1,PD0 output
  GPIO_PORTD_AFSEL_R = 0x00;        // No alternate function
  GPIO_PORTD_PUR_R = 0x0F;          // Enable pullup resistors on PD4-PD0       
  GPIO_PORTD_DEN_R = 0x0F;          // Enable digital pins PD3-PD0
	
	SYSCTL_RCGC2_R |= 0x10;           //Activate clock bit 4 
	
	GPIO_PORTE_CR_R = 0x0F;           // Allow changes to PE3-0       
  GPIO_PORTE_AMSEL_R = 0x00;        // Disable analog function
  GPIO_PORTE_PCTL_R = 0x00000000;   // GPIO clear bit PCTL  
  GPIO_PORTE_DIR_R = 0x00;          // PE3,PE2,PE1,PE0 input   
  GPIO_PORTE_AFSEL_R = 0x00;        // No alternate function
  GPIO_PORTE_PUR_R = 0x00;          // Disable pullup resistors on PE4-PE0       
  GPIO_PORTE_DEN_R = 0x0F;          // Enable digital pins PE3-PE0
}


//Reading Keypad
unsigned char readKeypad(void){
	int Button;
	int count;
	
	column = 0x01;
	for (count=0;count<=3;count++){
		
		if (column==0x08){ //Check column 1
			if (row==0x10){ //Row 1
				Button = 1;
				lcdWriteRamString("1");
			}
			else if (column==0x04){ //Row 2
				Button = 4;
				lcdWriteRamString("4");
			}
			else if (column==0x02){ //Row 3
				Button = 7;
				lcdWriteRamString("7");
			}
			else if (column==0x01){ //Row 4
				Button = 14;					//Column 1 Row 4 = 14
				lcdWriteRamString("*");
			}
		}
		
		else if (column==0x02){ //Check column 2
			if (row==0x10){ //Row 1
				Button = 2;
				lcdWriteRamString("2");
			}
			else if (column==0x04){ //Row 2
				Button = 5;
				lcdWriteRamString("5");
			}
			else if (column==0x02){ //Row 3
				Button = 8;
				lcdWriteRamString("8");
			}
			else if (column==0x01){ //Row 4
				Button = 0;					
				lcdWriteRamString("0");
			}
		}
		
		else if (column==0x04){ //Check column 3
			if (row==0x10){ //Row 1
				Button = 3;
				lcdWriteRamString("3");
			}
			else if (column==0x04){ //Row 2
				Button = 6;
				lcdWriteRamString("6");
			}
			else if (column==0x02){ //Row 3
				Button = 9;
				lcdWriteRamString("9");
			}
			else if (column==0x01){ //Row 4
				Button = 34;					//Column 3 Row 4 = 34
				lcdWriteRamString("#");
			}
		}
				
		else if (column==0x08){ //Check column 3
			if (row==0x10){ //Row 1
				Button = 41; //Column 4 Row 1 = 41
				lcdWriteRamString("A");
			}
			else if (column==0x04){ //Row 2
				Button = 42; //Column 4 Row 2 = 42
				lcdWriteRamString("B");
			}
			else if (column==0x02){ //Row 3
				Button = 43; //Column 4 Row 3 = 43
				lcdWriteRamString("C");
			}
			else if (column==0x01){ //Row 4
				Button = 44;					//Column 4 Row 4 = 44
				lcdWriteRamString("D");
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
