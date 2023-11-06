// #include <SPI.h>
// #include <LoRa.h> 
// int pot = A0;
// int LED = 3;
// String inString = "";    // string to hold input
// int val = 0;
// void setup() {
//   Serial.begin(9600);
//   pinMode(pot,INPUT);
  
//   while (!Serial);  
//   Serial.println("LoRa Sender");
//   if (!LoRa.begin(915E6)) { // or 915E6, the MHz speed of yout module
//     Serial.println("Starting LoRa failed!");
//     while (1);
//   }
// }
 
// void loop() {
  
//   // try to parse packet
//   int packetSize = LoRa.parsePacket();
//   if (packetSize) { 
//     // read packet    
//     while (LoRa.available())
//     {
//       int inChar = LoRa.read();
//       inString += (char)inChar;
//       val = inString.toInt();       
//     }
//     inString = "";     
//     LoRa.packetRssi();    
//   }
      
//   Serial.println(val);  
//   analogWrite(LED, val);

// //trans
//   int value = map(analogRead(pot),0,1024,0,255);
//   LoRa.beginPacket();  
//   LoRa.print(value);
//   LoRa.endPacket();
//   delay(50);
 
// }



#include <string.h>
#include <SPI.h>
#include <LoRa.h> 
const int trig = 4;     // chân trig của HC-SR04
const int echo = 3;     // chân echo của HC-SR04
String outgoing;              // outgoing message

byte msgCount = 0;            // count of outgoing messages
byte MasterNode = 0xFF;     
byte Node3 = 0xDD;

void setup() {
  Serial.begin(9600);
  pinMode(trig,OUTPUT);   // chân trig sẽ phát tín hiệu
  pinMode(echo,INPUT);    // chân echo sẽ nhận tín hiệu
  while (!Serial);  
  Serial.println("LoRa Sender");
  if (!LoRa.begin(915E6)) { // or 915E6, the MHz speed of yout module
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}
 
void loop() {
    unsigned long duration; // biến đo thời gian
    int distance;           // biến lưu khoảng cách
    
    /* Phát xung từ chân trig */
    digitalWrite(trig,0);   // tắt chân trig
    delayMicroseconds(2);
    digitalWrite(trig,1);   // phát xung từ chân trig
    delayMicroseconds(5);   // xung có độ dài 5 microSeconds
    digitalWrite(trig,0);   // tắt chân trig
    
    /* Tính toán thời gian */
    // Đo độ rộng xung HIGH ở chân echo. 
    duration = pulseIn(echo,HIGH);  
    // Tính khoảng cách đến vật.
    distance = int(duration/2/29.412);
    
  onReceive(LoRa.parsePacket(), distance);
}
void sendMessage(String outgoing, byte MasterNode, byte otherNode) {
  LoRa.beginPacket();                   // start packet
  LoRa.write(MasterNode);              // add destination address
  LoRa.write(Node3);             // add sender address
  LoRa.write(msgCount);                 // add message ID
  LoRa.write(outgoing.length());        // add payload length
  LoRa.print(outgoing);                 // add payload
  LoRa.endPacket();                     // finish packet and send it
  msgCount++;                           // increment message ID
}

void onReceive(int packetSize, int distance) {
  if (packetSize == 0) return;          // if there's no packet, return

  // read packet header bytes:
  int recipient = LoRa.read();          // recipient address
  byte sender = LoRa.read();            // sender address
  byte incomingMsgId = LoRa.read();     // incoming msg ID
  byte incomingLength = LoRa.read();    // incoming msg length

  String incoming = "";

  while (LoRa.available()) {
    incoming += (char)LoRa.read();
  }

  if (incomingLength != incoming.length()) {   // check length for error
   // Serial.println("error: message length does not match length");
   ;
    return;                             // skip rest of function
  }

  // if the recipient isn't this device or broadcast,
  if (recipient != Node3 && recipient != MasterNode) {
    //Serial.println("This message is not for me.");
    ;
    return;                             // skip rest of function
  }
    Serial.println(incoming);
    Serial.println(String(distance));
    int Val = distance;
    if(Val <= 10)
    { 
    String message_2 = String(distance); 
    delay(200);
    sendMessage(message_2,MasterNode,Node3);
    delay(100);
    }
  
}


