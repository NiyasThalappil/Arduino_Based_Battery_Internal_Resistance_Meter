/*
 * Arduino Battery Internal Resistance Meter v1.0
 *
 * 
 * by Niyas Thalappil
 * 
 * www.youtube.com/c/NiyasThalappil
 * NiyazThalappil@gmail.com
 *
 */

#include <Wire.h>                       
#include <Adafruit_SSD1306.h>               // OLED driver Library

#define OLED_RESET  4                       // OLED reset pin
#define vIn    A0                           // raw battery voltage
#define tacSwitch   5                       // tac switch to activate load
#define xBase       9                       // xsistor base pin

Adafruit_SSD1306 display(OLED_RESET);       //create instance of OLED


float v1 = 0.000;                               // unloaded battery voltage
float v2 = 0.000;                               // loaded battery voltage
float i1 = 0.000;                               // unloaded current
float i2 = 0.000;                               // loaded current
float intRes = 0.000;                           // battery internal resistance in Ohms               
float vCal = 4.640;                             // calculated vRef (measure between 5V & Gnd)
int load = 5;                                   // load resistance in Ohms
bool j = 1;                                     // is switch triggered


void setup()   {                

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);// start OLED @ Hex address )x3C  
  display.display();                        // show whats in the buffer
  display.clearDisplay();                   // clear buffer & display 
  pinMode(vIn, INPUT);                      // set A0 as input
  pinMode(tacSwitch, INPUT_PULLUP);         // set D5 as input with internal pullup
  pinMode(xBase, OUTPUT);                   // set D9 as output
  v1 = (analogRead(vIn)*vCal)/1023;         // get unloaded voltage
  i1 = v1/979;                              // calculate unloaded current

}


void loop() {

  
  
  j = digitalRead(tacSwitch);               // is switch triggered? 1 = no 0 = yes
  if(j == 0){
    ircalc();
  }

  display.setTextSize(1);                   
  display.setTextColor(WHITE);              
  display.setCursor(0,0);                  
  display.print("V1  :");                   
  display.println(v1,3);                    
  display.print("I1  :");                   
  display.println(i1,3);                    
  display.print("V2  :");                   
  display.println(v2,3);                    
  display.print("I2  :");                   
  display.println(i2,3);                    
  display.println(" ");                     
  display.setTextColor(BLACK,WHITE);        
  display.print("Int. Res. :");             
  display.print(intRes,3);                  
  display.println(" R");                    
  display.display();                        
  delay(500);                              
  display.clearDisplay();                   


}

void ircalc(){                          // resistance calc function
  digitalWrite(xBase, HIGH);            // switch in the load
  v2 = (analogRead(vIn)*vCal)/1023;     // get loaded battery voltage
  i2 = v2/load;                         // calculate loaded current
  digitalWrite(xBase, LOW);            // switch out the load
  float vCalc = (v1-v2);
  float iCalc = (i2-i1);
  intRes = (vCalc/iCalc)*-1;
  
/*
 *               Formula to Calculate Internal Resistance
 *                         Using Voltage Drop
 *                         
 *                               V1 - V2
 *                         Ri =  -------
 *                               I2 - I1
 */

  
}

