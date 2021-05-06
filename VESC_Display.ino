/*
This code is used to gather data from a VESC6 and display on a colour TFT screen.
Written for TTGO-Display
Board: ESP32 Dev
Based on code by Tom Stanton https://www.youtube.com/watch?v=7jBlKI7aqZA
Who based his on VESCUART.h code written by SolidGeek.
 */


//TFT colors
#define Black 0x0000  
#define White 0xFFFF  
#define Light_Gray 0xBDF7  
#define Dark_Gray 0x7BEF  
#define Red 0xF800  
#define Yellow 0xFFE0  
#define Orange 0xFBE0  
#define Brown 0x79E0  
#define Green 0x7E0 
#define Cyan 0x7FF 
#define Blue 0x1F  
#define Pink 0xF81F  
//TFT Resolution
#define Disp_H 240
#define Disp_V 150


#include <VescUart.h>
#include <SimpleKalmanFilter.h>

#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h


/** Initiate VescUart class */
VescUart UART;

int rpm;
float voltage;
float current;
int power;
float amphour;
float tach;
float distance;
float velocity;
float watthour;
float batpercentage;

SimpleKalmanFilter Filter1(2, 2, 0.01);



void setup() {

  /** Setup Serial port to display data */
  Serial.begin(115200);

  /** Setup UART port On TTGO Display, you have to assign the pins. 25(Tx) 26(Rx) in this case */
  Serial2.begin(19200, SERIAL_8N1, 25, 26);
  /** Define which ports to use as UART */
 UART.setSerialPort(&Serial2);

  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(Orange); // Note: the new fonts do not draw the background colour

  //Splash Screen
  tft.drawCentreString("Bike belongs to", Disp_H/2, 10, 4);
  tft.drawCentreString("Simon Rafferty", Disp_H/2, 45, 4);
  tft.setTextColor(Yellow);
  tft.drawCentreString("07774 956461", Disp_H/2, 90, 4);
  
  
  
  delay(5000);
  tft.fillScreen(TFT_BLACK);

}
void loop() {
  
////////// Read values //////////  
 if ( UART.getVescValues() ) {

  rpm = (UART.data.rpm)/23;          // The '23' is the number of pole pairs in the motor. Most motors have 14 poles, therefore 7 pole pairs
  voltage = (UART.data.inpVoltage);
  current = (UART.data.avgInputCurrent);
  power = voltage*current;
  amphour = (UART.data.ampHours);
  watthour = amphour*voltage;
  tach = (UART.data.tachometerAbs)/138;          // The '138' is the number of motor poles multiplied by 3
  distance = tach*3.142*(1/1609)*0.51*(16/185);          // Motor RPM x Pi x (1 / meters in a mile or km) x Wheel diameter x (motor pulley / wheelpulley)
  velocity = rpm*3.142*(60/1609)*0.51*(16/185);          // Motor RPM x Pi x (seconds in a minute / meters in a mile) x Wheel diameter x (motor pulley / wheelpulley)
  batpercentage = ((voltage-38.4)/12)*100;          // ((Battery voltage - minimum voltage) / number of cells) x 100

////////// Filter //////////
  // calculate the estimated value with Kalman Filter
  float powerfiltered = Filter1.updateEstimate(power);
  

////////// LCD //////////
  
// First line  

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(Orange); // Note: the new fonts do not draw the background colour
  tft.drawRightString(String(int(velocity)), 80, 3, 7);
  tft.drawString("mph", 85, 3, 2);

  //Change Battery Percentage colour
  tft.setTextColor(Red); 
  if(batpercentage>30) tft.setTextColor(Orange); 
  if(batpercentage>50) tft.setTextColor(Yellow); 
  if(batpercentage>80) tft.setTextColor(Green); 

  tft.setTextColor(Yellow); // Note: the new fonts do not draw the background colour
  tft.drawRightString(String(int(batpercentage)), 220, 3, 7);
  tft.drawString("%", 223, 3, 2);

   
// Second line
  tft.setTextColor(Green); // Note: the new fonts do not draw the background colour
  tft.drawRightString(String(int(powerfiltered)), 120, 80, 7);
  tft.drawString("W", 132, 80, 2);

  tft.setTextColor(Red); // Note: the new fonts do not draw the background colour
  tft.drawRightString(String(int(voltage)), 225, 80, 7);
  tft.drawString("V", 228, 80, 2);
  }
  else
  {    
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(Orange); // Note: the new fonts do not draw the background colour
  tft.drawCentreString("No Data!", Disp_H/2, 10, 4);

  /*
  //Screen layout test data
  tft.setTextColor(Yellow); // Note: the new fonts do not draw the background colour
  tft.drawRightString("28", 80, 3, 7);
  tft.drawString("mph", 85, 3, 2);

  tft.setTextColor(Green); // Note: the new fonts do not draw the background colour
  tft.drawRightString("100", 220, 3, 7);
  tft.drawString("%", 223, 3, 2);

// Second line
  tft.setTextColor(Orange); // Note: the new fonts do not draw the background colour
  tft.drawRightString("1378", 120, 80, 7);
  tft.drawString("W", 132, 80, 2);

  tft.setTextColor(Red); // Note: the new fonts do not draw the background colour
  tft.drawRightString("54", 225, 80, 7);
  tft.drawString("V", 228, 80, 2);
  */
  }
 
    
  delay(1000);
  
}
