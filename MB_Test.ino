/*
  Based on Modbus-Arduino Library by André Sarmento Barbosa http://github.com/andresarmento/modbus-arduino
*/

#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 2 
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

#include <Modbus.h>

//#define WIZ
//#include <SPI.h>
//#include <Ethernet.h>
//#include <ModbusIP.h>

#define ENC
#include <EtherCard.h>
#include <ModbusIP_ENC28J60.h>

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
    mb.config(mac, ip);

    // Config Modbus Serial (port, speed, byte format) 
//    mb.config(&Serial, 38400, SERIAL_8N1);
    // Set the Slave ID (1-247)
//    mb.setSlaveId(10);  


    //Set Pin mode
  for (byte DPin = 0; DPin < 4; DPin++) 
  {
    mb.addCoil(DPin);
    mb.addIsts(DPin);
    mb.addIreg(DPin);
    mb.addIreg(DPin + 4);
    pinMode(DPin + 2, OUTPUT);
    pinMode(DPin + 6, INPUT_PULLUP);
  }
    pinMode(13, OUTPUT);

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
   //Call once inside loop() - all magic here
   mb.task();

   if (mb.Ireg(0) < 10)
   {
     mb.Coil(0, false);
     mb.Coil(13, false);
   }
   if (mb.Ireg(0) > 1010)
   {
     mb.Coil(0, true);
     mb.Coil(13, true);
   }

  for (byte APin = 0; APin < 7; APin++) 
  {
    mb.Ireg(APin, analogRead(APin));
  }
  for (byte DPin = 0; DPin < 4; DPin++) 
  {
    mb.Ists(DPin, digitalRead(DPin + 6));
   digitalWrite(DPin + 2, mb.Coil(DPin));
  }
#ifdef WIZ
   digitalWrite(13, mb.Coil(13));
#endif
}
