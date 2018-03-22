/*References:
 * https://circuitdigest.com/microcontroller-projects/arduino-calculator-using-4x4-keypad
 * https://medium.com/@18214030_IMKA/tugas-4-kalkulator-sederhana-6dc775f9811c
 * https://www.sunfounder.com/learn/Sensor-Kit-v2-0-for-Arduino/lesson-1-display-by-i2c-lcd1602\
 * -sensor-kit-v2-0-for-arduino.html
 * https://bitbucket.org/fmalpartida/new-liquidcrystal/wiki/Home
 */

#include <Key.h>
#include <Keypad.h>
#include <Wire.h>
#include <hd44780.h>                       // main hd44780 header
#include <hd44780ioClass/hd44780_I2Cexp.h> // i2c expander i/o class header

//LCD I2C Initialization
hd44780_I2Cexp lcd; // declare lcd object: auto locate & config exapander chip
// LCD geometry
const int LCD_COLS = 16;
const int LCD_ROWS = 2;

double num1,num2 ;
double total;
char operation,button;
const byte ROWS = 4; // Four rows
const byte COLS = 4; // Three columns

// Define the Keymap
char keys[ROWS][COLS] = {
  {'1','2','3','+'},
  {'4','5','6','-'},
  {'7','8','9','*'},
  {'=','0','%','/'}
};
byte rowPins[ROWS] = { 0, 1, 2, 3 };// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte colPins[COLS] = { 4, 5, 6, 7 }; // Connect keypad COL0, COL1 and COL2 to these Arduino pins.

Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS ); //  Create the Keypad

void domath() // Simple switch case to pick what operation to do, based on button pressed by user.
{
  switch(operation)
  {
    case '+': // Addition
      total = num1+num2;
      break;
    case '-': // Subtraction 
      total = num1-num2;
      break;
    case '/': // Division.
      total = num1/num2;
      break;
    case '*': // Multiplication
      total = num1*num2;
      break;
     case '%': // Modulus
      total = (int)num1%(int)num2;
      break;
  }
  // Based on case selected print our total and 
  lcd.setCursor(0,1);
  lcd.print('=');
  lcd.setCursor(1,1);
  lcd.print(total);
}

void setup() {
int status;

  // initialize LCD with number of columns and rows: 
  // hd44780 returns a status from begin() that can be used to determine if initalization failed.
  status = lcd.begin(LCD_COLS, LCD_ROWS);
  if(status) // non zero status means it was unsuccesful
  {
    status = -status; // convert negative status value to positive number

    // begin() failed so blink error code using the onboard LED if possible
    hd44780::fatalError(status); // does not return
  }

  // initalization was successful, the backlight should be on now
  
  // Print a message to the LCD
  lcd.print("Calculator");
}

void loop()
{
  // Loops are convenient for reading key press from keypad
  while(1) // First loop. Composing first number. Until operator pressed
  {
    button = kpd.getKey(); // Button read
    if (button >='0' && button <='9') // If user pressed numeric value, 1 character at a time.
    {
      lcd.clear();
      num1 = num1*10 + (button -'0'); //First operand composing
      lcd.setCursor(0,0); // Select first row on lcd
      lcd.print(num1); // Print current number1
    }
    
    //Inputting operator
    if (num1 !=0 && (button=='+' || button=='-' || button=='*' || button=='/' || button=='%'))
    {
      operation = button; // operation remembers what mathematical operation user wants on numbers
      lcd.setCursor(0,1); // set cursor to row 2
      lcd.print(operation); // print our operator
      break;
    }
  }
  
  while(1) // Second loop. Inputting second operand. Until '=' pressed
  {
    button = kpd.getKey(); // Button read
    if (button >='0' && button <='9') // Getting chars from keypad for second number
    {
      num2 = num2*10 + (button -'0'); //Second operand composing
      lcd.setCursor(1,1); // Select second row, second column on lcd
      lcd.print(num2); // Print current operand2
    }

    if (button == '=' && num2 !=0) //Do calculation if '=' is pressed
    {
      domath(); //Calls domath() subroutine
      break;
    }
  }
  
  while(1) // Clearing screen for next calculation
  {
    //Waits ‘=’ key to be pressed so it can reset program and start over.
    button = kpd.getKey();
    if (button =='=')
    {
      lcd.clear();
      lcd.setCursor(0,0);
      num1=0;
      num2=0;
      total=0;
      operation=0;
      break;
    }
  }
}
