/*
  Based on Modbus-Arduino Library by André Sarmento Barbosa http://github.com/andresarmento/modbus-arduino
*/


#include <Modbus.h>

//#define WIZ
#include <SPI.h>
//#include <Ethernet.h>
//#include <ModbusIP.h>

#define ENC
#include <EtherCard_STM.h>
#include <ModbusIP_ENC28J60.h> // correct this for STM

//#include <ModbusSerial.h>


//ModbusIP object
ModbusIP mb;
//ModbusSerial mb;

void setup() {
    // The media access control (ethernet hardware) address for the shield
    byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
    // The IP address for the shield
    byte ip[] = { 192, 168, 1, 222 };
    //Config Modbus IP
    SPI.setModule(2); // setting hardware SPI2 as default SPI module (pins 28, 29, 30 on maple mini)
    mb.config(mac, ip);

    // Config Modbus Serial (port, speed, byte format) 
//    mb.config(&Serial, 38400, SERIAL_8N1);
    // Set the Slave ID (1-247)
//    mb.setSlaveId(10);  
//  Serial.begin(115200);


    //Set Pin mode
  for (byte DPin = 0; DPin < 4; DPin++) 
  {
    mb.addCoil(DPin);
    mb.addIsts(DPin);
    mb.addIreg(DPin);
    mb.addIreg(DPin + 4);
    //pinMode(DPin + 15, OUTPUT);
    //pinMode(DPin + 19, INPUT_PULLUP);
  }
    pinMode(PC13, OUTPUT);
    pinMode(PB0, INPUT_ANALOG);
    pinMode(PA0, INPUT_ANALOG);
    pinMode(PA1, INPUT_ANALOG);
    pinMode(PA2, INPUT_ANALOG);
    pinMode(PA3, INPUT_ANALOG);
    pinMode(PA4, INPUT_ANALOG);
    pinMode(PA5, INPUT_ANALOG);
    pinMode(PA6, INPUT_ANALOG);
    pinMode(PA7, INPUT_ANALOG);

    mb.addCoil(13);

    mb.addHreg(0, 12345);
    mb.addHreg(1, 54321);
    mb.addHreg(2, 0xAAAA);
    mb.addHreg(3, 0xBBBB);
    mb.addHreg(4, 0xCCCC);
    mb.addHreg(5, 0xDDDD);
    mb.addHreg(6, 0xEEEE);
    mb.addHreg(7, 0xFFFF);

}

void loop() 
{
   //digitalWrite(2, true);
  //Call once inside loop() - all magic here
   mb.task();
   //digitalWrite(2, false);
   
   if (mb.Ireg(0) < 1000)
   {
     mb.Coil(0, false);
     mb.Coil(13, false);
   }
   if (mb.Ireg(0) > 2000)
   {
     mb.Coil(0, true);
     mb.Coil(13, true);
   }

//  for (byte APin = 0; APin < 8; APin++) 
//  {
//    unsigned long buf = 0;
//    for (byte cnt = 0; cnt < 10; cnt++)
//      {
//        buf += analogRead(APin);
//      }
//      mb.Ireg(APin, buf/10);
//  }
    mb.Ireg(0, analogRead(PA0));
    mb.Ireg(0, analogRead(PA0));
   mb.task();
    mb.Ireg(1, analogRead(PA1));
    mb.Ireg(1, analogRead(PA1));
   mb.task();
    mb.Ireg(2, analogRead(PA2));
    mb.Ireg(2, analogRead(PA2));
   mb.task();
    mb.Ireg(3, analogRead(PA3));
    mb.Ireg(3, analogRead(PA3));
   mb.task();
    mb.Ireg(4, analogRead(PA4));
    mb.Ireg(4, analogRead(PA4));
   mb.task();
    mb.Ireg(5, analogRead(PA5));
    mb.Ireg(5, analogRead(PA5));
   mb.task();
    mb.Ireg(6, analogRead(PA6));
    mb.Ireg(6, analogRead(PA6));
   mb.task();
    mb.Ireg(7, analogRead(PA7));
    mb.Ireg(7, analogRead(PA7));
   mb.task();
//  for (byte DPin = 0; DPin < 4; DPin++) 
//  {
//    mb.Ists(DPin, digitalRead(DPin + 16));
//   digitalWrite(DPin + 12, mb.Coil(DPin));
//  }
   digitalWrite(PC13, mb.Coil(13));
}
