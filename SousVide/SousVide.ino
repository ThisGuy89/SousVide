// Libraries for the DS18B20 Temperature Sensor
#include <OneWire.h>
#include <DallasTemperature.h>
#include <NexaCtrl.h>
#define ONE_WIRE_BUS 8
#define Wifi_RX_Pin 2
#define Wifi_TX_Pin 3
#define TX_PIN 12
#define RX_PIN 13
#include <SoftwareSerial.h>
#include <TextFinder.h>

// Remote Identification:
//SenderID 21137734
//no group
//on
//ButtonID 1
//10556
//2765
//
//SenderID 21137734
//no group
//off
//ButtonID 1
//10562
//2773

const static unsigned long controller_id = 21137734;
unsigned int device = 1;
NexaCtrl nexaCtrl(TX_PIN, RX_PIN);

#define SSID "cbn-1F1FA"  //name of wireless access point to connect to
#define PASS "YGQdTUdrF9nR"  //wifi password

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
// arrays to hold device address
DeviceAddress tempSensorAddress;

SoftwareSerial wifi(2,3);

int counter;  //page reload counter for testing
long int thistime;  //debug
double temperature;
bool espBusy = false;
TextFinder finder(wifi);  //seems to work much better than core fumctions

void setup()
{
  pinMode(RX_PIN, INPUT);
  Serial.begin(9600);
  wifi.begin(9600);
  //nexaCtrl.DeviceOff(controller_id, device);
  
  sensors.getAddress(tempSensorAddress, 0);


sensors.begin();
sensors.setResolution(tempSensorAddress, 12);
sensors.setWaitForConversion(false);

connectWiFi(SSID,PASS);
//issueCommandAwaitResponse("AT+CIFSR", "OK");


//Send_AT_command("AT+CIFSR", 1);
//Send_AT_command("AT+CIPMUX=1", 1);
startUDP();
//sendData(5.0);
//Serial.println(Send_AT_command("AT+CIPSEND=4,5",2));
//Serial.println(Send_AT_command("abcde", 2));
}
int count = 0;
int setpoint;
String command;
bool receiving = false;
bool wasReceiving = false;
int bytesReceived;
int cmdLength;

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available() >0){
    byte b = Serial.read();
//    wifi.write(b);
  }
  if(wifi.available() > 0){
 //   byte b = wifi.read();
 //   Serial.write(b);
  }
  
  readCommands2(command);
  if(receiving && !wasReceiving){
    //Serial.println("receiving now");
  }
  if(!receiving && wasReceiving){
    String cmd = command.substring(9);
    if(cmd == "RelayOn"){
      activateNexaRelay(true);
    }
    else if(cmd == "RelayOff"){
      activateNexaRelay(false);
    }
    else if(cmd == "Stop"){
      activateNexaRelay(false);
    }
    else if(cmd.substring(0,5)=="Start"){
      setpoint = (cmd.substring(6)).toInt();
      Serial.println("Got a start command with setpoint: "+(String)setpoint);
      sendData(30.0);
    }
  }
//  if(a == "+IPD,4,7:RelayOn"){
//    Serial.println("Activating Relay");
//    activateNexaRelay(true);
//  }
  wasReceiving = receiving;
  //delay(500);
  //Serial.println(Send_AT_command("AT+CIPSEND=4,5",1));
  //Serial.println(Send_AT_command((String)count,1));
//  count++;
  //sendData(5.0);
 // sendData(5.0);

  
//  char channel;        //ESP has 5 channels & we need to know which one to use
////  everysecond("wait for serial1  ");  //debug - does it bomb out in loop()?
//  if (wifi.available())
//  {
//    if( finder.find("+IPD,") ) 
//    {            
//      channel=wifi.read();
//      homepage(channel -'0');    //ascii to int
//      delay(1);
//     // wifi.clear();
//    }
//    delay(1);
//  }

}

String readCommands(){
  String retVal = "";
  while (wifi.available() > 0 ){
    char c = wifi.read();
    if(c != '\n') retVal += c;
    delay(1);
  }
  return retVal;
}



void readCommands2(String& command){
  if(wifi.available() > 0 ){
    char c = wifi.read();
    char next;
    if(c == '+'){
      next = wifi.peek();
      next = 'I';
      if(next =='I'){
        receiving = true;
        bytesReceived = 0;
        cmdLength = 8+48;
        command = "";
      }
    }
    if(receiving){
      if(c != '\n'){
        command += c;
        bytesReceived++;
      }
      if(bytesReceived == 8){
        cmdLength = (int)c+8;
      }
      if(bytesReceived == cmdLength-48+1){
        receiving = false;
        Serial.println(command);
      }
    }
  }
}

//+IPD,4,7:RelayOn
void cmdType(String cmd){
  //Serial.println("length: "+ cmd.charAt(7));
  Serial.println(cmd);
}

double readTemp(DallasTemperature sensors, DeviceAddress tempSensorAddress){
  double temp;
  if(sensors.isConversionAvailable(0)){
    temp = sensors.getTempC(tempSensorAddress);
    sensors.requestTemperatures();// prime the pump for the next one - but don't wait
  }
}

void activateNexaRelay(bool activate){
  if(activate){
    nexaCtrl.DeviceOn(controller_id, device);
  }
  else{
    nexaCtrl.DeviceOff(controller_id, device);
  }
}


// Send command to device & return response message. Some commands are slow so a 'wait' is included
// I like this way of doing it 
String Send_AT_command(String AT_Command, int wait){
  String tmpData;

  wifi.println(AT_Command);
  delay(wait);
  while (wifi.available() >0 )  {
    char c = wifi.read();
    tmpData += c;

    if ( tmpData.indexOf(AT_Command) > -1 )        
      tmpData = "";
    else;
    //tmpData.trim();       //get rid of this? 
  }
  return tmpData;
}

//================================================================================       
// neater than mine? Put this in my for loop instead?
boolean connectWiFi(String NetworkSSID,String NetworkPASS) {
  String cmd = "AT+CWJAP=\"";
  cmd += NetworkSSID;
  cmd += "\",\"";
  cmd += NetworkPASS;
  cmd += "\"";

  Serial.println(cmd); 
  Serial.println(Send_AT_command(cmd,10));
}


//Send temperature data every 3 seconds.
void sendData(double tempReading)
{
  Serial.println("Sending data" );
  Serial.println(Send_AT_command("AT+CIPSEND=4,6",1));
  delay(100);
  Serial.println(Send_AT_command("Temp"+(String)round(tempReading),1));
}
  
// AT command: AT+CIPSTART=4,"UDP","192.168.0.11",333,333,0
void startUDP(){
  String cmd = "AT+CIPSTART=4,";
  cmd += "\"UDP\",";
  cmd += "\"192.168.0.11\",";
  cmd += "333,333,0";
  Send_AT_command(cmd,5);
}

