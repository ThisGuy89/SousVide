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
#include <NexaCtrl.h>

#define TX_PIN 12
#define RX_PIN 13
// Your remote id which can be it can be "sniffed" with the 
// reciever example at http://playground.arduino.cc/Code/HomeEasy
const static unsigned long controller_id = 21137734;
unsigned int device = 1;
NexaCtrl nexaCtrl(TX_PIN, RX_PIN);
void setup()
{
  pinMode(RX_PIN, INPUT);
 Serial.begin(9600);
 Serial.println("start");
  nexaCtrl.DeviceOn(controller_id, device);
  nexaCtrl.DeviceOn(controller_id, device);
  delay(3000);
  nexaCtrl.DeviceOff(controller_id, device);
  nexaCtrl.DeviceOff(controller_id, device);
  //nexaCtrl.DeviceOff(controller_id, device);
  Serial.println("stop"); 
}
void loop()
{
}

//
//void sniff(){
//      
//      int i = 0;
//      unsigned long t = 0;
//      byte prevBit = 0;
//      byte bit = 0;
//      
//      unsigned long sender = 0;
//      bool group = false;
//      bool on = false;
//      unsigned int recipient = 0;
//      
//      // latch 1
//      // Latch timing has been loosened to acommodate varieties in the Nexa transmitters.
//      
//      while ((t < 8000 || t > 13000)){ 
//        t = pulseIn(rxPin, LOW, 1000000);
//        t1 = t; // Save latch timing for debugging purposes
//      }
//      
//      // Next line can be used to debug latch timing. Please note that this affects timing and that recieving the following data bits may fail.
//      //Serial.println (t);
//      
//      // latch 2
//      // Latch timing has been loosened to acommodate varieties in the Nexa transmitters.
//      
//      while (t < 2200 || t > 2900){ 
//        t = pulseIn(rxPin, LOW, 1000000);
//      }
//      
//      t2 = t; // Save latch timing for debugging purposes
//      
//      // Next line can be used to debug latch timing. Please note that this affects timing and that recieving the following data bits may fail.
//      //Serial.println (t);
//      
//      // data collection from reciever circuit
//      while (i < 64){
//        t = pulseIn(rxPin, LOW, 1000000);
//        if (t > 200 && t < 400){ 
//          bit = 0;
//        }
//        else if (t > 1100 && t < 1560){ 
//          bit = 1;
//        }
//        else{ 
//          i = 0;
//          break;
//        }
//        
//        if (i % 2 == 1){
//          if ((prevBit ^ bit) == 0){ // must be either 01 or 10, cannot be 00 or 11
//            i = 0;
//            break;
//          }  
//          if (i < 53){ // first 26 data bits
//            sender <<= 1;
//            sender |= prevBit;
//          }
//          else if (i == 53){ // 26th data bit
//            group = prevBit;
//          }
//          else if (i == 55){ // 27th data bit
//            on = prevBit;
//        }
//        else{ // last 4 data bits
//          recipient <<= 1;
//          recipient |= prevBit;
//        }
//      }
//      prevBit = bit;
//      ++i;
//    }
//      
//    
//    // interpret message
//      if (i > 0){ 
//        printResult(sender, group, on, recipient); // Print the result on Serial Monitor. Use this to identify your transmitter code.
//        if (Sendercode = sender) // This is the check for the correct transimtter code. If code is incorrect then go back to look for new code.
//        if (group) // Group command affects all relays, Either all on, or all off.
//      }
//      else{
//        switch (recipient){ // Check which channel should be activated
//          break;
//        }
//      }
//}
//
//
//
//
//
//
//
//
//void printResult(unsigned long sender, bool group, bool on, unsigned int recipient){
//  
//  Serial.print("SenderID ");
//  Serial.println(sender);
//  
//  if (group){ 
//    Serial.println("group command");
//  }
//  else{ 
//    Serial.println("no group");
//  }
//  
//  if (on){
//    Serial.println("on");
//  }
//  
//  else{ 
//    Serial.println("off");
//  }
//  
//  Serial.print("ButtonID ");
//  Serial.println(recipient);
//  Serial.println(t1); // Timing for latch 1
//  Serial.println(t2); // Timing for latch 2
//}

