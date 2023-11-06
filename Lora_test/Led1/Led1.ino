#include <string.h>
#include <SPI.h>
#include <LoRa.h> 
#include<AESLib.h>
uint8_t key[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
const int trig = 4;     // chân trig của HC-SR04
const int echo = 3;     // chân echo của HC-SR04
String outgoing;              // outgoing message

byte msgCount = 0;            // count of outgoing messages
byte MasterNode = 0xFF;     
byte Node1 = 0xBB;
int gre = 0;
int yel = 0;
int red = 0;
int count_packet = 0;
void setup() {
  Serial.begin(9600);
  pinMode(trig,OUTPUT);   // chân trig sẽ phát tín hiệu
  pinMode(echo,INPUT);    // chân echo sẽ nhận tín hiệu

  while (!Serial);  
  //Serial.println("LoRa Sender");
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
  delay(500);
}
void sendMessage(char outgoing[16], byte MasterNode, byte otherNode) {

  LoRa.beginPacket();                   // start packet
  LoRa.write(otherNode);              // add destination address
  LoRa.write(MasterNode);             // add sender address
  LoRa.write(msgCount);                 // add message ID
  
  LoRa.write(strlen(outgoing));        // add payload length
  int lensuit = strlen(outgoing);

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


void onReceive(int packetSize, int distance) {
  if (packetSize == 0){
    if (count_packet <1000) count_packet++;
    if (count_packet ==1000) { 
      Serial.println("X");
      Serial.println(1);
      }
    return;          // if there's no packet, return
  }
  count_packet = 0;
  // read packet header bytes:
  int recipient = LoRa.read();          // recipient address
  if (recipient != Node1) return;
  else {
  byte sender = LoRa.read();            // sender address
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
  Serial.println(incoming.length());
    if (incoming == "") {
      Serial.println("error: message length does not m=");
      return;
    }

    if (incomingLength != incoming.length() || incoming != "lora1" ) {   // check length for error
      // Serial.println("error: message length does not match length or not false" + incomingLength);
      // Serial.println("\n\n\n error: message length does not match length or not true" + incoming.length());
      
      if (red <10) red++;
      if (red ==10) {
        Serial.println("Y");
        Serial.print(1);
        gre = 0;
        yel = 0;
      }
      return;                             // skip rest of function
    }
  // if the recipient isn't this device or broadcast,
    else if (recipient != Node1 && recipient != MasterNode ) {
      //Serial.println("This message is not for me.");
      return;                             // skip rest of function
    }
  else {
      red =0;
      //Serial.println(incoming);
      //Serial.println(String(distance));
      int Val = distance;
      // Serial.println(distance);
      if(Val <= 10)
      { 
        char message[] = "1"; 
        delay(200);
        sendMessage(message,MasterNode,Node1);

        if (gre <10) gre++;
        if (gre == 10){
          Serial.println(0);
          yel = 0;
        delay(100);
        }
      }
      else {
        char message[] = "0"; 
        delay(200);
        sendMessage(message,MasterNode,Node1);
        if (yel<10) yel++;
        if (yel == 10){
          Serial.println(2);
          gre = 0;
        delay(100);
      }
    }
}
}
}

