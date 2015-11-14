/*
  Based on Modbus-Arduino Library by André Sarmento Barbosa http://github.com/andresarmento/modbus-arduino
*/
#include <OneWire.h>
#include <DallasTemperature.h>
OneWire oneWire(A5); // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
DallasTemperature sensors(&oneWire); // Pass our oneWire reference to Dallas Temperature. 

DeviceAddress tempDeviceAddress;

#define  T_RESOL 12
#define  delayInMillis 750 / (1 << (12 - T_RESOL))
unsigned long lastTempRequest = 0;

#include <Modbus.h>

//#define WIZ
//#include <SPI.h>
//#include <Ethernet.h>
//#include <ModbusIP.h>

#define ENC
#include <EtherCard.h>
#include <ModbusIP_ENC28J60.h>

ModbusIP mb;

//#include <ModbusSerial.h>
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
//    mb.addIreg(DPin + 4);
    pinMode(DPin + 2, OUTPUT);
    pinMode(DPin + 6, INPUT_PULLUP);
  }
  pinMode(13, OUTPUT);

  mb.addCoil(13);
  mb.addIreg(4);
  mb.addIreg(5);

  mb.addHreg(0, -10);
  mb.addHreg(1, 10);
  mb.addHreg(2, 0xAAAA);
  mb.addHreg(3, 0xBBBB);
  mb.addHreg(4, 0xCCCC);
  mb.addHreg(5, 0xDDDD);
  mb.addHreg(6, 0xEEEE);
  mb.addHreg(7, 0xFFFF);

//  Serial.begin(115200);
 
//  sensors.begin();
//  if (!sensors.getAddress(tempDeviceAddress, 0))
//  {
//    Serial.print("No sensor");
//    while (1); 
//  }
  //Temperature sens setup
  sensors.getAddress(tempDeviceAddress, 0);
  sensors.setResolution(tempDeviceAddress, T_RESOL);
  sensors.setWaitForConversion(false);
  sensors.requestTemperatures();
  lastTempRequest = millis(); 

}

void loop() {

  mb.task();   //Call once inside loop() - all magic here

  if (millis() - lastTempRequest >= delayInMillis) // waited long enough??
  {
    int32_t rawTemp = sensors.getTemp(tempDeviceAddress); //~13ms
    sensors.requestTemperatures(); //~2ms
    lastTempRequest = millis(); 
    rawTemp = (rawTemp * 1000) / 12800;
    mb.Ireg(4, rawTemp); // contains measured temperature*10 (DP shift)

    //simple threshold type thermo-regulator 
    //Hreg(0) - On SetPoint, Hreg(1) - Off SetPoint 
    if ((int)mb.Ireg(4) < (int)mb.Hreg(0))
    {
      mb.Coil(0, true);
    }
    if ((int)mb.Ireg(4) > (int)mb.Hreg(1))
    {
      mb.Coil(0, false);
    }    

  }


  if (mb.Ireg(0) < 10)
  {
    mb.Coil(13, false);
  }
  if (mb.Ireg(0) > 1010)
  {
    mb.Coil(13, true);
  }

  for (byte DPin = 0; DPin < 4; DPin++) 
  {
    mb.Ireg(DPin, analogRead(DPin));
    mb.Ists(DPin, digitalRead(DPin + 6));
   digitalWrite(DPin + 2, mb.Coil(DPin));
  }
#ifdef WIZ
   digitalWrite(13, mb.Coil(13));
#endif
}
