char serialinput  = ' ';
const int RED_PIN = 3;		// led đỏ
const int GREEN_PIN = 4;	// led xanh lá
const int BLUE_PIN = 5;	// led xanh dương
int red = 0;
int gre = 0;
int yel = 0;
void setup()
{
	pinMode(RED_PIN, OUTPUT);
	pinMode(GREEN_PIN, OUTPUT);
	pinMode(BLUE_PIN, OUTPUT);
  Serial.begin(9600);
  digitalWrite(RED_PIN, HIGH); 
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, HIGH);
  
}
void loop()
{
   if(Serial.available())
   {
      char serialinput = Serial.read();
      if (serialinput =='0') { 
        digitalWrite(RED_PIN, LOW); //red
        digitalWrite(GREEN_PIN, HIGH);
        digitalWrite(BLUE_PIN, HIGH);
        red = 1;
        gre = 0;
        yel = 0;
       }
      if (serialinput =='1') { 
          digitalWrite(RED_PIN, HIGH);//green
          digitalWrite(GREEN_PIN, LOW);
          digitalWrite(BLUE_PIN, HIGH);
          red =0;
          gre =1;
          yel =0;
       }
      if (serialinput =='2') { 
          digitalWrite(RED_PIN, LOW); //yellow
          digitalWrite(GREEN_PIN, LOW);
          digitalWrite(BLUE_PIN, HIGH);
          red =0;
          gre =0;
          yel =1;
       }

      Serial.println(serialinput);
   }
   else if (red){
    digitalWrite(RED_PIN, LOW); //red
    digitalWrite(GREEN_PIN, HIGH);
    digitalWrite(BLUE_PIN, HIGH);
    red = 1;
    gre = 0;
    yel = 0;     
   }
   else if (gre){
    digitalWrite(RED_PIN, HIGH);//green
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(BLUE_PIN, HIGH);
    red =0;
    gre =1;
    yel =0;
   }
   else if (yel){
    digitalWrite(RED_PIN, LOW); //yellow
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(BLUE_PIN, HIGH);
    red =0;
    gre =0;
    yel =1;
     
   }
}