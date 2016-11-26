// Libraries for the DS18B20 Temperature Sensor
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 8

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
 
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
 
// arrays to hold device address
DeviceAddress tempSensor;

double Input = 0;

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);


sensors.getAddress(tempSensor, 0);


sensors.begin();
sensors.setResolution(tempSensor, 12);
sensors.setWaitForConversion(false);  

}

void loop() {
  // put your main code here, to run repeatedly:
 if (sensors.isConversionAvailable(0))
  {
    Input = sensors.getTempC(tempSensor);
    sensors.requestTemperatures(); // prime the pump for the next one - but don't wait
  }
  Serial.println(Input);
  delay(1000);
}
