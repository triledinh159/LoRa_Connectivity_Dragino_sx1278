
#include <SPI.h>              // include libraries
#include <LoRa.h>
#include <AESLib.h>
uint8_t key[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
byte MasterNode = 0xFF;     
byte Node1 = 0xBB;
byte Node2 = 0xCC; 
byte Node3 = 0xDD; 


String SenderNode = "";
String outgoing;              // outgoing message

byte msgCount = 0;            // count of outgoing messages

// Tracks the time since last event fired
unsigned long previousMillis=0;
unsigned long int previoussecs = 0; 
unsigned long int currentsecs = 0; 
unsigned long currentMillis = 0;
int interval= 1 ; // updated every 1 second
int Secs = 0; 


void setup() {
  Serial.begin(9600);                   // initialize serial
  if (!LoRa.begin(915E6)) {             // initialize ratio at 915 MHz
    Serial.println("LoRa init failed. Check your connections.");
    while (true);                       // if failed, do nothing
  }

 Serial.println("LoRa init succeeded.");
}


String cmd;
String on2;
String on3;
String on1 ="on1";

void loop() {
currentMillis = millis();
if(Serial.available()){
  cmd = Serial.readStringUntil('\n');
  cmd.trim();
  // if (cmd.equals("on1")) on1=cmd; add later
  // else if (cmd.equals("off1")) on1 = "";

  if (cmd.equals("on2")) on2=cmd;
  else if (cmd.equals("off2")) on2 = "";

  if (cmd.equals("on3")) on3=cmd;
  else if (cmd.equals("off3")) on3 = "";

}

if (currentMillis % 300 == 0 && on1.equals("on1") == 1)
  {
  // Serial.println("OK11111");
  char message_0[] = "lora1"; 
  sendMessage(message_0,MasterNode, Node1);
  }
if (on2.equals("on2") == 1){
  if (currentMillis % 350 == 0 )
  {

  char message_1[] = "lora2"; 
  sendMessage(message_1,MasterNode, Node2);
  }
}

if (currentMillis % 400 == 0 && on3.equals("on3") == 1)
  {

  char message_2 = "lora3"; 
  sendMessage(message_2,MasterNode, Node3);
  }

onReceive(LoRa.parsePacket());
cmd = ""; 
}

void sendMessage(char outgoing[16], byte MasterNode, byte otherNode) {

  LoRa.beginPacket();                   // start packet
  LoRa.write(otherNode);              // add destination address
  LoRa.write(MasterNode);             // add sender address
  LoRa.write(msgCount);                 // add message ID
  
  LoRa.write(strlen(outgoing));        // add payload length
  
  int lensuit = strlen(outgoing);
  // Serial.println(lensuit);
  while (lensuit < 16) 
  {
    strcat(outgoing, " ");
    lensuit++;
  }
  // outgoing[16] ='\0';
  aes128_enc_single(key, outgoing);
  LoRa.print(outgoing);                 // add payload
  LoRa.endPacket();                     // finish packet and send it
  msgCount++;                           // increment message ID
}

void onReceive(int packetSize) {
  if (packetSize == 0) return;          // if there's no packet, return

  // read packet header bytes:
  int recipient = LoRa.read();          // recipient address
  byte sender = LoRa.read();            // sender address
  //if( sender == 0XBB )
 //SenderNode = "Node1:";
  //if( sender == 0XCC )
  //SenderNode = "Node2:";
  //if( sender == 0XDD )
  //SenderNode = "Node3:";
  byte incomingMsgId = LoRa.read();     // incoming msg ID
  byte incomingLength = LoRa.read();    // incoming msg length

  char incomingCh[16];
  char incomingCh2[16];
  int i =0;
  while (LoRa.available() && i <16) {
    incomingCh[i] = (char)LoRa.read();
    i++;
    // Serial.println(incomingStr);
    // if (i==14) incomingCh[i] = '\0';
  }

  for (int j=0;j<16;j++){
    incomingCh2[j] = incomingCh[j];
  }
  // incomingCh2[16] = '\0';
  aes128_dec_single(key, incomingCh2);
  String incoming(incomingCh2);
  incoming.trim();
  // Serial.println(incoming);
  // Serial.println(incoming.length());
  // Serial.println(incomingLength);
  if (incoming.length() > incomingLength){
    incoming = incoming.substring(0,incomingLength);
  }
  Serial.println(incoming);

  // if the recipient isn't this device or broadcast,
  if (recipient != Node1 && recipient != MasterNode) {
   // Serial.println("This message is not for me.");
    ;
    return;                             // skip rest of function
  }
    if (recipient != Node2 && recipient != MasterNode) {
    ;
    return;                            
  }
    if (recipient != Node3 && recipient != MasterNode) {
    ;
    return;                             
  }

 if( sender == 0XBB )
 { 
  delay(200);
  Serial.println("LoRa 1: "+incoming);
 }

  if( sender == 0XCC )
 { 
    delay(200);
   Serial.println("LoRa 2: "+incoming);
 }

  if( sender == 0XDD )
 { 
    delay(200);
   Serial.println("LoRa 3: "+incoming);
 }



}