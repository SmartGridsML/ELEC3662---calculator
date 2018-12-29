#include "LCD.h"
#include "Keypad.h"
#include <stdio.h>
#include <math.h>

int Key = 0;

//Programs
void Password_Protection(void);
void Calculator_Main(void);

//Number Variables
double Value1(void);
double Value2(void);
double num1 = 0;  // first operand
double num2 = 0;  // second operand
double answer;    //result

//First number selection
double No1 = 0;

//Anwer print buffer
char buffer[50];

//Operator variable
int oper = 0;

//Operator selection variable
int Operator = 0;

int Operator_Select(void); //Select operator +,-,*,/, 
void Calculations(void); //Calulate answer
void clearscreen(void); //Clearing screen
void reset(void); //Soft reset

//Pasword Variables
int Start = 0;
double Password = 1234;
double password_input = 0;

int main (void){
	//Initialise Ports
	PortA_Init();
	PortB_Init();
	PLL_Init();
	SysTick_Init();
	
	//Initialise external hardware
	lcd_Init();
	keypad_Init();
	lcdClearScreen();
	
	//Password Procedure
	Password_Protection();
	
	//Calculator Main
	Calculator_Main();
}


//Set First value
double Value1(void){
	if(Key == 0 || Key ==1 || Key == 2 || Key == 3 || Key == 4 || Key == 5 || Key == 6 || Key == 7 || Key == 8 || Key == 9){ //If input is a number store 
		num1=(num1*10)+Key;
	}
	return num1;
}

//Set second value
double	Value2(void){
	if(Key == 0 || Key ==1 || Key == 2 || Key == 3 || Key == 4 || Key == 5 || Key == 6 || Key == 7 || Key == 8 || Key == 9){ //If input is a number store 
		num2=(num2*10)+Key;
	}
	return num2;
}

//Calulations 
void Calculations(void) {
		
	if (Key == 34 && Operator == 1){ //Checks equals and plus operator
		answer = num1 + num2; //Add numbers
		sprintf(buffer, "%f", answer); //Convert numbers to string
		lcdGoto(1,5); //Change cursor position
	  lcdWriteRamString(buffer); //Write answer to LCD
	}	
	else if(Key == 34 && Operator == 2){ //Checks equals and minus operator
		answer = num1 - num2; //Subtract numbers
		sprintf(buffer, "%f", answer); //Convert numbers to string
		lcdGoto(1,5); //Change cursor position
	  lcdWriteRamString(buffer); //Write answer to LCD
	}
	else if(Key == 34 && Operator == 3){ //Checks equals and multiply operator
		answer = num1 * num2; //Multiply numbers
		sprintf(buffer, "%f", answer); //Convert numbers to string
		lcdGoto(1,5); //Change cursor position
	  lcdWriteRamString(buffer); //Write answer to LCD
	}
	else if(Key == 34 && Operator == 4){ //Checks equals and division operator
		if (num2==0){ //Checks if denominator is 0
			lcdGoto(1,0);
			lcdWriteRamString("ERROR"); //Prints error messages
		}
		else{
		answer = num1 / num2; //Divide numbers
		sprintf(buffer, "%f", answer); //Convert numbers to string
		lcdGoto(1,5); //Change cursor position
	  lcdWriteRamString(buffer); //Write answer to LCD
		}
	
	}
	else if(Key == 34 && Operator == 5){ //Checks for sqrt and equals
		answer  = sqrt(num2); //square root value
		sprintf(buffer, "%f", answer); //Convert numbers to string
		lcdGoto(1,5); //Change cursor position
	  lcdWriteRamString(buffer); //Write answer to LCD
	}
}

//Operator Selection
int	Operator_Select(void){
	
	if(Key ==41 ){ //Column 4, Row 1
		oper = 1;  } //Add
	else if(Key == 42){ //Column 4, Row 2
		oper = 2; } //Subtract
	else if(Key == 43){ //Column 4, Row 3
		oper = 3; } //Multiply
	else if(Key == 44){ //Column 4, Row 4
		oper = 4; } //Divide
	else if(Key == 14){ //Column 1, Row 4
		oper = 5; } //square root
	else if(Key == 55){ //External button
		reset(); //Reset All variables
	}
return oper;
		
}

//Soft reset
void reset(void){
		//Clear screen and reset variables
		lcdClearScreen();
		num1 = 0;
		num2 = 0;
		Key = 0;
		Operator = 0;
		oper = 0;
		answer = 0;
		No1 = 0;
			
}

void Password_Protection(void){
		//Password Protection
	lcdWriteRamString("Password => ");
	while(Start==0){ //While exit condition
		Key = readKeypad(); //Store Keypad value
		password_input = Value1(); //Store Password input as a number
		if(Key == 55){ //If external switch is high reset screen and variables
			reset();
			lcdWriteRamString("Password => ");
		}
		if(password_input == Password){ //If correct password is entered
		lcdGoto(1,0); //Move down to second row
		lcdWriteRamString("Correct");
		SysTick_Wait(160000000);	//2sec delay	
		SysTick_Wait(160000000);
		SysTick_Wait(160000000);
		//SysTick_Wait(160000000);
		//SysTick_Wait(160000000);
		Start = 1; //Set While exit condition
		reset(); //reset screen and variables
	}
		
}
}

//Calulator
void Calculator_Main(void){
			while(1) {;
		Key = readKeypad(); //Store keypad reading as variable
		
		Operator = Operator_Select();      //Stores selected operator
		if(Operator == 1 || Operator == 2 || Operator == 3 || Operator == 4 || Operator == 5){   //If operator is selected enter value number
			No1 = 1;                                               //Set flag to 1 disabling first value input
			Value2();                                                      //stores the second value
		}
		else {
			Value1();}                        //stores the first value when flag is 0
					
		Calculations();  //Performs calculations
		
		}
}

