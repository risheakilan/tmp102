/**

@file main.cpp
@brief Temperature Regulation Device
This code implements a temperature regulation device using the mbed platform. It includes libraries for mbed, TMP102 temperature sensor, N5110 LCD screen, and a Joystick. The code sets up the necessary pinouts, flags, and menu options for the device.
@date 2023-07-12
*/


#include "mbed.h" //mbed library
#include "TMP102.h" //tmp102 library
#include "N5110.h" //N5110 LCD screen library
#include "Joystick.h" //joystick library



TMP102 temperature(PTE25, PTE24, 0x90); //the pinouts for tmp102 sensor
N5110 lcd(PTC9,PTC0,PTC7,PTD2,PTD1,PTC11); // N5110 screen pinouts
Joystick joystick(PTB10,PTB11,PTC16); // joystick pinouts
DigitalOut Led1(PTA1);//pinout for led 1 
DigitalOut Led2(PTA2);//pinout for led 2
DigitalOut Led3(PTC2);//pinout for led 3 
DigitalOut Led4(PTC3);//pinout for led 4 
DigitalOut Led5(PTC4);//pinout for led 5 
DigitalOut Led6(PTD3);//pinout for led 6 
InterruptIn Abutton(PTB9);//pinout for button A
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
void Led2Flip() {
     Led2 = !Led2;
}
void AbuttonPressed() {
     AbuttonFlag = 1; //to assign a value of 1 meaning that the button is pressed
}
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
int MainMenuAD = 0;
int MainMenuBA = 0; 
int MainMenuBB = 0;
int MainMenuBC = 0;
int MainMenuBD = 0;

int SaveTemp1 = 0; //Save first temperature
int SaveTemp2 = 0;
int SaveTemp3 = 0;
int TempUp = 39;
int TempDown = 30;



int main() {
Led1 = 0;
Led2 = 0;
Led3 = 0;

float celsius, fahrenheit;

Abutton.rise(&AbuttonPressed); //this ensures that the button is pressed
Abutton.mode(PullDown); //this line ensures that the pull-down resistor is used when the button is not pressed
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
lcd.init(); //initialises the lcd screen
lcd.setContrast(0.5); //controls the visibility of the LCD by adjusting the contrast
lcd.normalMode(); //intructs the LCD to operate in its normal settings
lcd.setBrightness(0.5); //sets the LCD brightness
lcd.refresh();
wait_ms(3000); // wait for 3 seconds for the welcome screen
lcd.clear();

lcd.printString("Temp Reg Device",5,0); 




 
  
  
  
  
  
  
  
  
  while(1) {
    if (LbuttonFlag == 1) { //If the back left button is pressed, the device will be set to the original main menu
    MainMenuAA = 1; //this ensures that main menu is selected
    MainMenuAB = 0; //this ensures that this menu is not selected since the value is 0 
    MainMenuAC = 0; //this ensures that this menu is not selected since the value is 0 
    MainMenuAD = 0;
    MainMenuBA = 0;
    MainMenuBB = 0;
    MainMenuBC = 0;
    MainMenuBD = 0;
  
  //printf("Return Main Menu");
  
  wait_ms (2000); 
   
   LbuttonFlag = 1;
    }

  
 
 
 
 
 
 
 
 
 
 
 
if (MainMenuAA == 1) {
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
        MainMenuAA = 1; //Previous Menu screen is shown
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
        MainMenuAA = 1;
        MainMenuAB = 0;  
        MainMenuAC = 0;
        MainMenuAD = 0;

    //printf("SelectedMenuAA");

       wait_ms (2000);  
        }

if (JoystickDirection == S) {
        MainMenuAA = 0;
        MainMenuAB = 0;  
        MainMenuAC = 1;
        MainMenuAD = 0;

     //printf("SelectedMenuAC");

       wait_ms (2000);  
        }

if (AbuttonFlag == 1) { 
        MainMenuAB = 0;
        MainMenuBB = 1;

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
       
       if (SaveTemp1 > TempUp ) { //if the saved temperature is above the temperature threshold then LED1 will turn on
        Led1Flip();
        }
       
        
       if (SaveTemp2 > TempUp ) { //if the saved temperature is above the temperature threshold then LED1 will turn on
        Led1Flip();
        }

         
       if (SaveTemp3 > TempUp ) { //if the saved temperature is above the temperature threshold then LED1 will turn on
        Led1Flip();
        }
       
        
       if (SaveTemp1 < TempDown ) { //if the saved temperature is below the temperature threshold then LED2 will turn on
        Led2Flip();
        }

       if (SaveTemp2 < TempDown ) { //if the saved temperature is above the temperature threshold then LED2 will turn on
        Led2Flip();
        }

        if (SaveTemp3 < TempDown ) { //if the saved temperature is above the temperature threshold then LED2 will turn on
        Led2Flip();
        }
       
       
       if (BbuttonFlag == 1) { 
        MainMenuBB = 0;
        MainMenuAB = 1;

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
        MainMenuBC = 1;

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
    int length1 = sprintf(buffer7,"  %d\n C",TempDown);
    lcd.printString(buffer7,0,4);
    lcd.refresh();
}
       
       if (BbuttonFlag == 1) { 
        MainMenuBC = 0;
        MainMenuAC = 1;

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
        MainMenuBD = 1;

//printf("SelectedMenuBD");;

       wait_ms (2000);
        AbuttonFlag = 0;
}



if (MainMenuBD == 1){

lcd.printString("CelsiusandFahrenheit",0,0);
 
 celsius = temperature.read();
 fahrenheit = (celsius * 9.0 / 5.0) + 32.0;

char buffer1[17];
int length6 = printf(buffer1,"Temp = %.2f\n C",temperature.read(), celsius, fahrenheit);
}
  
  
  }

}