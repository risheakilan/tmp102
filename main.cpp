/**

@file main.cpp
@brief Temperature Regulation Device
This code implements a temperature regulation device using the mbed platform. It includes libraries for mbed, TMP102 temperature sensor, N5110 LCD screen, and a Joystick. The code sets up the necessary pinouts, flags, and menu options for the device.
@date 2023-07-31
*/

/**
 * Library for the TI TMP102 temperature sensor.
 * The TMP102 is an I2C digital temperature sensor in a small SOT563 package, with a 0.0625C resolution and 0.5C accuracy.
 Copyright (c) 2010 Donatien Garnier (donatiengar [at] gmail [dot] com)
 */

/** Joystick Class
@author Dr Craig A. Evans, University of Leeds
@brief  Library for interfacing with analogue joystick
*/

/** N5110 Class
@brief Library for interfacing with Nokia 5110 LCD display (https://www.sparkfun.com/products/10168) using the hardware SPI on the mbed.
@brief The display is powered from a GPIO pin meaning it can be controlled via software.  The LED backlight is also software-controllable (via PWM pin).
@brief Can print characters and strings to the display using the included 5x7 font.
@brief The library also implements a screen buffer so that individual pixels on the display (84 x 48) can be set, cleared and read.
@brief The library can print primitive shapes (lines, circles, rectangles)
@brief Acknowledgements to Chris Yan's Nokia_5110 Library.

@brief Revision 1.3

@author Craig A. Evans
@date   7th February 2017

*/


#include "mbed.h" //mbed library
#include "TMP102.h" //tmp102 library
#include "N5110.h" //N5110 LCD screen library
#include "Joystick.h" //joystick library


//outputs
TMP102 temperature(PTE25, PTE24, 0x90); //the pinouts for tmp102 sensor
N5110 lcd(PTC9,PTC0,PTC7,PTD2,PTD1,PTC11); // N5110 screen pinouts
Joystick joystick(PTB10,PTB11,PTC16); // joystick pinouts
PwmOut buzzerPin(PTB20); //buzzer pinout
DigitalOut Led1(PTB22);//pinout for led 1 
DigitalOut Led2(PTB21);//pinout for led 2
DigitalOut Led3(PTE26);//pinout for led 3 
InterruptIn Abutton(PTB9);//pinout for button A
/**
 * @brief The interruptIn object is used to handle events whenever A button is pressed
 */

InterruptIn Bbutton(PTD0);//pinout for button B
InterruptIn Ybutton(PTC12);//pinout for button Y
InterruptIn Xbutton(PTC17);//pinout for button X
InterruptIn Lbutton(PTB18);//pinout for button L
InterruptIn Rbutton(PTB3);//pinout for button R

volatile int AbuttonFlag = 0;//the value 0 indicates that the button to this flag has not been pressed
volatile int BbuttonFlag = 0;//the value 0 indicates that the button to this flag has not been pressed
volatile int YbuttonFlag = 0;//the value 0 indicates that the button to this flag has not been pressed
volatile int XbuttonFlag = 0;//the value 0 indicates that the button to this flag has not been pressed
volatile int LbuttonFlag = 0;//the value 0 indicates that the button to this flag has not been pressed
volatile int RbuttonFlag = 0;//the value 0 indicates that the button to this flag has not been pressed


void Led1Flip() {
     Led1 = !Led1; 

}

/**
 * @brief this function will toggle the state of the Led, i.e turn it on. 
 */

void Led2Flip() {
     Led2 = !Led2;
}
void AbuttonPressed() {
     AbuttonFlag = 1; 
}

/**
 * 
@brief this function is called whenever the button A is pressed
 * It will set the AbuttonFlag to 1 showing that the button is pressed.
 */

void BbuttonPressed() {
     BbuttonFlag = 1; //to assign a value of 1 meaning that the button is pressed
}
void YbuttonPressed() {
     YbuttonFlag = 1; //to assign a value of 1 meaning that the button is pressed
}
void XbuttonPressed() {
     XbuttonFlag = 1; //to assign a value of 1 meaning that the button is pressed
}
void LbuttonPressed() {
     LbuttonFlag = 1; //to assign a value of 1 meaning that the button is pressed
}
void RbuttonPressed() {
     RbuttonFlag = 1; //to assign a value of 1 meaning that the button is pressed
}

int MainMenuAA = 1; //Menu Option AA "Read Temp", this will enable the sensor to read the current temperature
int MainMenuAB = 0; //Menu Option AB "SaveCurrentTemp", this will enable the option to save the current temperature
int MainMenuAC = 0; //Menu Option AC "TempThreshold", will enable the option to adjust the thershold of the temp
int MainMenuAD = 0; //Menu Option AD "View in Fahreheit" will enable the option to view the current temparture in either celsius or fahrenheit. 
int MainMenuBA = 0; //Menu Option BA will allow the user to perform a temperature scan and obtain a reading 
int MainMenuBB = 0; //Menu Option BB will allow the user to view the three saved temperatures 
int MainMenuBC = 0; //Menu Option BC will allow the user to change the limits of the fever threshold. 
int MainMenuBD = 0; //Menu Option BD will allow the user to change the units of the current temperature measured. 

int SaveTemp1 = 0; //Save first temperature
int SaveTemp2 = 0; //Save second temperature 
int SaveTemp3 = 0; //Save third temperature 
int TempUp = 39; //Preset temperature upper limit 
int TempDown = 30; //Preset tempreature lower limit 



int main() {
Led1 = 0;
Led2 = 0;
Led3 = 0;

float celsius, fahrenheit;

float frequency = 659.0; 



buzzerPin.period(1.0 / frequency);

/**
 * @brief Set the period for the buzzer.
 *
 * The period is calculated by dividing 1.0 by the frequency value.
 * The frequency value manages the frequency in Hz for the A4 note.
 */

Abutton.rise(&AbuttonPressed); //this ensures that the button is pressed

/**
 * @brief rising edge is when a button is pressed
 * When button A is pressed, the AbuttonPressed function will be called.
 */

Abutton.mode(PullDown); 

/**
 * @brief Configure button A pin mode as PullDown.
 * A pulldown resistor is used to ensure that the button is pulled to ground when not pressed.  
 * 
 */

Bbutton.rise(&BbuttonPressed);//this ensures that the button is pressed
Bbutton.mode(PullDown);//this line ensures that the pull-down resistor is used when the button is not pressed
Ybutton.rise(&YbuttonPressed);//this ensures that the button is pressed
Ybutton.mode(PullDown);//this line ensures that the pull-down resistor is used when the button is not pressed
Xbutton.rise(&XbuttonPressed);//this ensures that the button is pressed
Xbutton.mode(PullDown);//this line ensures that the pull-down resistor is used when the button is not pressed
Lbutton.rise(&LbuttonPressed);//this ensures that the button is pressed
Lbutton.mode(PullDown);//this line ensures that the pull-down resistor is used when the button is not pressed
Rbutton.rise(&RbuttonPressed);//this ensures that the button is pressed
Rbutton.mode(PullDown);//this line ensures that the pull-down resistor is used when the button is not pressed

joystick.init(); //initialises the joysitck

/**
 * @brief Initializes the joystick.
 */

lcd.init(); 

/**
 * @brief Initializes the LCD screen.
 */

lcd.setContrast(0.5); 

/**
 * @brief Controls the visibility of the LCD by adjusting the contrast.
 * The lowest contrast value is 0 (dimmest) and the highest contrast value is 1 (brightest)
 */

lcd.normalMode(); 

/**
 * @brief Instructs the LCD to operate in its normal mode.
 */

lcd.setBrightness(0.5); //sets the LCD brightness

/**
 * @brief Sets the brightness level of the LCD screen.
 * The brightness value ranges from 0 to 1, with 0 being the lowest brightness and 1 being the highest brightness.
 */


lcd.refresh();

/**
 * @brief Refreshes the content on the LCD screen.
 */


wait_ms(3000); 

/**
 * @brief Delays program execution for 3 seconds.
 * It is used here to display the welcome screen for 3 seconds before proceeding further.
 */


lcd.clear();

/**
 * @brief Clears the content on the LCD screen and resets the screen's buffer.
 */

lcd.printString("Temp Reg Device",5,0); 




 
  
  
  
  
  
  
  
  
  while(1) {
    if (LbuttonFlag == 1) { //If the back left button is pressed, the device will be set to the original main menu
    MainMenuAA = 1; //this ensures that main menu is selected
    MainMenuAB = 0; //this ensures that this menu is not selected since the value is 0 
    MainMenuAC = 0; //this ensures that this menu is not selected since the value is 0 
    MainMenuAD = 0; //this ensures that this menu is not selected since the value is 0 
    MainMenuBA = 0; //this ensures that this menu is not selected since the value is 0 
    MainMenuBB = 0; //this ensures that this menu is not selected since the value is 0 
    MainMenuBC = 0; //this ensures that this menu is not selected since the value is 0 
    MainMenuBD = 0; //this ensures that this menu is not selected since the value is 0 
  
  //printf("Return Main Menu");
  
  wait_ms (2000); 
   
   LbuttonFlag = 1;
    }

  
 
 
 
 
 
 
 
 
 
 
 
if (MainMenuAA == 1) {

    // Display sub-menu options
    lcd.clear();
    lcd.printString("1.=ViewCurrentTemp",0,0);
    lcd.printString("2.ViewTempHistory",0,1);
    lcd.printString("3.SetFeverThreshold",0,2);
    lcd.printString("4.ViewInFahrenheit",0,3);
    lcd.refresh();
    
    Direction JoystickDirection = joystick.get_direction(); //to obtain the current direction of the joystick
        if (AbuttonFlag == 1) { 
        MainMenuAA = 0;
        MainMenuBA = 1;
   
    //printf("MenuBASelected");
       
       wait_ms (2000);
        AbuttonFlag = 0;
}

if (JoystickDirection == S) {
        MainMenuAA = 0;
        MainMenuAB = 1;  
        MainMenuAC = 0;

        //printf("MenuABselected");
       
       wait_ms (2000);  
        }

 }

 
 
 
 
 
 
 
 
 
 
 
 if (MainMenuBA == 1) {
    lcd.clear(); 
    lcd.printString("TempScan",0,0);
    char buffer1[17];
    int length = sprintf(buffer1,"Temp = %.2f\n C",temperature.read()); //temp value is read
    
    //printf("Temperature: %s\n", buffer1);

    lcd.printString(buffer1,0,1);
    lcd.printString("R to save",0,3); //the button R will save the current temp
    lcd.refresh();

 }
        
        if (BbuttonFlag == 1) { //if B button is pressed, the user is returned to previous menu 
        MainMenuAA = 1; //Previous Menu screen is displayed
        MainMenuAB = 0; 
        MainMenuAC = 0;
        MainMenuAD = 0;
        MainMenuBA = 0;
        MainMenuBB = 0;
        MainMenuBC = 0;
        MainMenuBD = 0;
        
      //printf("ReturntoMenuAA");

       wait_ms (2000);
        BbuttonFlag = 0; 
        } 

        
        if (RbuttonFlag == 1) { 
        SaveTemp1 = temperature.read(); //temp is saved in "SaveTemp1"
      
        //printf("SavingTemp1: %d\n", SaveTemp1);
      
       wait_ms (2000);
        XbuttonFlag = 0; 
        }

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 if (MainMenuAB == 1) {
    lcd.clear();
    lcd.printString("1.ViewCurrentTemp",0,0);
    lcd.printString("2.=ViewTempHist",0,1);
    lcd.printString("3.SetFeverLimit",0,2);
    lcd.printString("4.ViewInFahrenheit",0,3);
    lcd.refresh();
 }
 
 Direction JoystickDirection = joystick.get_direction(); 
//printf(JoystickDirection);

//When the joystick is moved to the north 
 if (JoystickDirection == N) {
        MainMenuAA = 1; //Main Menu AA is displayed 
        MainMenuAB = 0;  
        MainMenuAC = 0;
        MainMenuAD = 0;

    //printf("SelectedMenuAA");

       wait_ms (2000);  
        }

if (JoystickDirection == S) {
        MainMenuAA = 0;
        MainMenuAB = 0;  
        MainMenuAC = 1; //Main Menu AC is displayed 
        MainMenuAD = 0;

     //printf("SelectedMenuAC");

       wait_ms (2000);  
        }

if (AbuttonFlag == 1) { 
        MainMenuAB = 0;
        MainMenuBB = 1; //Main Menu BB is displayed 

    //printf("SelectedMenuBB");

       wait_ms (2000);
        AbuttonFlag = 0;
}










if (MainMenuBB == 1){
    lcd.clear(); 
    lcd.printString("SaveTemp1 ",0,0);
    char buffer3[17];
    int length = sprintf(buffer3,"Temp = %.2d\n C",SaveTemp1);
    lcd.printString(buffer3,0,1);

    lcd.printString("SaveTemp2 ",0,2);
    char buffer4[17];
    int length2 = sprintf(buffer4,"Temp = %.2d\n C",SaveTemp2);
    lcd.printString(buffer4,0,3);

    lcd.printString("SaveTemp3 ",0,4);
    char buffer5[17];
    int length3 = sprintf(buffer5,"Temp = %.2d\n C",SaveTemp3);
    lcd.printString(buffer5,0,5);
    lcd.refresh();

}
       
       if (SaveTemp1 > TempUp || SaveTemp2 > TempUp || SaveTemp3 > TempUp) { //if the saved temperature is above the temperature threshold then LED1 will turn on
            Led1Flip(); // will toggle the state of Led1. Since it is set as 0, the Led will turn on
            buzzerPin.period(1.0 / frequency); // frequncy value is defined in main loop
            buzzerPin = 0.5;  // Set the initial duty cycle
            wait(1.0);
            buzzerPin = 0.0; // buzzer turns off 
    
        }
       
       if (SaveTemp1 < TempDown || SaveTemp2 < TempDown|| SaveTemp3 < TempDown) { //if the saved temperature is below the temperature threshold then LED2 will turn on
           Led2Flip(); // will toggle the state of Led2. Since it is set as 0, the Led will turn on
           buzzerPin.period(1.0 / frequency); // frequncy value is defined in main loop
            buzzerPin = 0.5;              //Set the initial duty cycle
            wait(1.0);
            buzzerPin = 0.0; // buzzer turns off 
        }
        

       if (BbuttonFlag == 1) { //when button B is pressed 
        MainMenuBB = 0;
        MainMenuAB = 1; //Main Menu AB is displayed 

//printf("SelectedMenuAB");

       wait_ms (2000);
        BbuttonFlag = 0; 
        }










if (MainMenuAC == 1) {
    lcd.clear();
    lcd.printString("1.ViewCurrentTemp",0,0);
    lcd.printString("2.ViewTempHist",0,1);
    lcd.printString("3.=SetFeverLimit",0,2);
    lcd.printString("4.ViewInFahrenheit",0,3);
    lcd.refresh();
 }

 if (AbuttonFlag == 1) {
        MainMenuAC = 0;
        MainMenuBC = 1; //Main Menu BC is displayed 

//printf("SelectedMenuBC");

       wait_ms (2000);
        AbuttonFlag = 0;

 }
        


if (MainMenuBC == 1){
    lcd.clear(); 
    lcd.printString("UpperTemp",0,0);
  char buffer6[17];
    int length = sprintf(buffer6,"> %d\n C",TempUp);
//printf(length);
    lcd.printString(buffer6,0,2);
    lcd.printString("LowerTemp",0,3);
    char buffer7[17];
    int length1 = sprintf(buffer7,"< %d\n C",TempDown);
    lcd.printString(buffer7,0,4);
    lcd.refresh();
}
       
       if (BbuttonFlag == 1) { 
        MainMenuBC = 0;
        MainMenuAC = 1; //Main Menu AC is displayed 

    //printf("SelectedMenuAC");

       wait_ms (2000);
        BbuttonFlag = 0; 
        }










if (MainMenuAD == 1) { 
    lcd.clear();
    lcd.printString("1.ViewCurrentTemp",0,0);
    lcd.printString("2.ViewTempHist",0,1);
    lcd.printString("3.SetFeverLimit",0,2);
    lcd.printString("4.=ViewInFahrenheit",0,3);
    lcd.refresh();
 }
 
 
if (AbuttonFlag == 1) { 
        MainMenuAD = 0;
        MainMenuBD = 1; //Main Menu BD is displayed 

//printf("SelectedMenuBD");;

       wait_ms (2000);
        AbuttonFlag = 0;
}



if (MainMenuBD == 1){

lcd.printString("CelsiusandFahrenheit",0,0);
 
 celsius = temperature.read();
 fahrenheit = (celsius * 9.0 / 5.0) + 32.0; //the conversion formula from celsius to fahrenheit

char buffer1[17];
int length6 = printf(buffer1,"Temp = %.2f\n C",temperature.read(), celsius, fahrenheit); //the temperature will be printed in both celsius and fahreheit 
}
  
  
  }

}