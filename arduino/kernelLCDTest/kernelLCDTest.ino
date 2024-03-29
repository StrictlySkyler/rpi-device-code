// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]
// []
// []   Aquinas Kernel LCD Tester
// []   by Carlo J Quinonez 2012-07-09
// []
// []   based on i2c LCD library Display Test Demo
// []	by dale@wentztech.com 2009-04-02
// []
// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]

#include <Wire.h>
#include <inttypes.h>

//  Uncomment the two lines for the display you are using


#include <LCDi2cNHD.h>                    
LCDi2cNHD lcd = LCDi2cNHD(4,20,0x50>>1,0);

uint8_t rows = 4;
uint8_t cols = 20;

void setup() { 

  lcd.init();                          // Init the display, clears the display

  /* TWBR = ((CPU_FREQ / SCL_FREQ ) - 16) / 2  and  CPU_FREQ=16MHz  */

  TWBR=152; /* == 50kHz SCL frequency */
  
  lcd.setDelay(5,0);
 
  lcd.print("Aquinas Kernel");    
  lcd.setCursor(1,0);
  lcd.print("LCD Test and Burn-in");    
  lcd.setCursor(2,0);
  lcd.print("July 09 2012 // CJQ");    
  delay(6000);


}


void loop()
{

     lcdtest_basic();
  
}


void lcdtest_basic(){
  
  lcd.clear();
  lcd.print ("Cursor Test");
  delay(1000);
  Cursor_Type();
  
  lcd.clear();
  lcd.print("Characters Test");
  delay(1000);
  Characters();
  delay(1000);

  lcd.clear();
  lcd.print("Every Line");
  delay(1000);
  Every_Line(rows);
  delay(1000);
  
  lcd.clear();
  lcd.print("Every Position");
  delay(1000);
  Every_Pos(rows,cols);
  delay(1000);
  

  //Fancy_Clear();
  
   // Count_Numbers();
   // Fancy_Clear();


}

void Cursor_Type(){

  lcd.setCursor(0,0);
  lcd.print("Underline Cursor");
  lcd.setCursor(1,0);
  lcd.cursor_on();
  delay(1000);
  lcd.cursor_off();
  lcd.setCursor(0,0);
  lcd.print("Block Cursor    ");
  lcd.setCursor(1,0);
  lcd.blink_on();
  delay(1000);
  lcd.blink_off();
  lcd.setCursor(0,0);
  lcd.print("No Cursor      ");
  lcd.setCursor(1,0);
  delay(1000);
  }
  



void Count_Numbers(){

  lcd.clear();
  lcd.print("Count to 255");

  for (int i=0;i<255;i++){
    
    lcd.setCursor(1,0);
    
    lcd.print(i,DEC);
    
    lcd.setCursor(1,7);
    
     lcd.print(i,BIN);
    
    // delay(10);
    
  }
  
}


// Assume 16 character lcd

void Characters(){
  
int  chartoprint=48;
char a;

        lcd.clear();
 
  for(int i=0 ; i < rows ; i++){

      for(int j=0 ; j < cols ; j++){
        
          lcd.setCursor(i,j);
          a = char(chartoprint);
          lcd.print(char(chartoprint));
          chartoprint++;
          if(chartoprint == 127)
              return;
 
      }
  }
 
}


void Fancy_Clear(){
  
     for (int i=0 ; i < rows ; i++){
       
       for(int j=0 ; j < cols/2 ; j++){
       lcd.setCursor(i,j);
       lcd.print(" ");
  
       lcd.setCursor(i, cols - j);
       lcd.print(" ");
       }
       //delay(10);
       
     }
    
}

void Every_Line(int lines){

  lcd.clear();
  for(int i=0 ; i < lines ; i++){
      lcd.setCursor(i,0);
      lcd.print("Line : ");
      lcd.print(i,DEC);
      
  }  
}

void Every_Pos(int lines,int cols){
  
  lcd.clear();

  for(int i=0 ; i < lines ; i++){
    for(int j=0 ; j< cols ; j++){
      lcd.setCursor(i,j);
      lcd.print(i,DEC);
    }

  } 

}
