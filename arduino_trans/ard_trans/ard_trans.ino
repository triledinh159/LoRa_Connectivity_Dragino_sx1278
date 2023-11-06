//Programa: Comunicacao Arduino e Raspberry Pi com LoRa
//Autor: Arduino e Cia

#include <SoftwareSerial.h>

//Pino do push-button
#define botao  5

//Serial Lora nos pinos 2 (TX) e 3 (RX)
SoftwareSerial loraSerial(2, 3);

int contador;

void setup()
{
  //Define o pino do botao como entrada
  pinMode(botao, INPUT_PULLUP);
  //Inicializa a serial do Arduino
  Serial.begin(9600);
  //Inicializa a serial do Lora
  loraSerial.begin(9600);
}

void loop()
{
  //Verifica se o botão foi pressionado
  if (digitalRead(botao) == 0)
  {
    //Aviso no serial monitor
    Serial.print("Botao pressionado - Contador: ");
    Serial.println(contador);

    //Envia os dados pelo modulo Lora
    loraSerial.print(contador);
    loraSerial.print("\r\n");

    while (digitalRead(botao) == 0);
    //Incrementa o contador
    contador++;
    delay(50);
  }
}