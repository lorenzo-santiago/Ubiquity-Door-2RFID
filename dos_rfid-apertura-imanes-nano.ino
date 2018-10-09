// Programa : RFID - Control de acesso / cancela
// Autor : Arduino e Cia
// Modificado: Santiago Lorenzo
// MFRC522 Library 1.3.4

#include <SPI.h>
#include <MFRC522.h>


// Definiciones de Modulo RC522 Doble
#define SS_PIN 10 // puede elegirse cualquier PIN digital
#define RST_PIN 9 // puede elegirse cualquier PIN digital
#define SS_PIN2 3 // puede e|legirse cualquier PIN digital
#define RST_PIN2 2 // puede elegirse cualquier PIN digital
MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522 mfrc522_2(SS_PIN2, RST_PIN2);

// Leds indicadores de acceso liberado o rechazado
int led_liberado = 5;
int led_negado = 6;
int porta_rele1 = 4;
int porta_rele2 = 8;
// Sensor puerta
int estadoSensor = 0;
const int sensorPin = 7;
// Alarma
int contador_alarma = 0;

char st[20];

void setup() 
{
  pinMode(led_liberado, OUTPUT);
  pinMode(led_negado, OUTPUT);
  // Inicia serial
  Serial.begin(9600);
  // Inicia  SPI bus
  SPI.begin();
  // Inicia MFRC522
  mfrc522.PCD_Init(); 
  mfrc522_2.PCD_Init(); 
  // Mensajes de inicio en el monitor serial
  Serial.println("Aproxime su Tag de acceso...");
  Serial.println();
  //Defino pin de rele
  pinMode(porta_rele1, OUTPUT);
  pinMode(porta_rele2, OUTPUT);
  //Defino pin de entrada de sensor
  pinMode(sensorPin, INPUT);
}

void loop() 
{
 // Led activo
 digitalWrite(led_negado, HIGH);
 digitalWrite(porta_rele1, HIGH);
 // Evaluo sensor de apertura
 estadoSensor = digitalRead(sensorPin);
 if (estadoSensor != 1)
  {
  for (int i= 1; i<5 ; i++)
        {
          contador_alarma ++;
          digitalWrite(led_negado, HIGH);
          delay(200);
          digitalWrite(led_negado, LOW);
          delay(200);
          Serial.println(contador_alarma);
        }
    Serial.println("Puerta Abierta");
    Serial.println();
    if (contador_alarma > 25) {
        for (int i= 1; i<5 ; i++)
          {
          Serial.println("Alarma activa");
          digitalWrite(porta_rele2, HIGH);
          delay(200);
          digitalWrite(porta_rele2, LOW);
          delay(200);
          }
        }
     else
     {
     contador_alarma ++; 
     }
  }
  else
  {
    Serial.println("Puerta Cerrada");
    contador_alarma = 0;
  // Aguarda aproximacion de tag lector 1
  lector1();
  delay(100);
  // Aguarda aproximacion de tag lector 2
  lector2();
  delay(100);
  }
}

//FUNCIONES
void AbreImanPuerta()
{
  // Abre iman y enciende led verde
  digitalWrite(led_negado, LOW);
  digitalWrite(led_liberado, HIGH);
  Serial.println(" Acesso garantizado ");
  Serial.println();
  delay(200);
  digitalWrite(porta_rele1, LOW);
  delay(3000);
  digitalWrite(porta_rele1, HIGH);
  digitalWrite(led_liberado, LOW);
  return;
}

void RechazaAcceso()
{
  Serial.println(" Acesso rechazado ");
  Serial.println();
  // Parpadea led rojo
  for (int i= 1; i<5 ; i++)
  {
    digitalWrite(led_negado, HIGH);
    delay(200);
    digitalWrite(led_negado, LOW);
    delay(200);
  }
}

//lector de adentro
void lector1()
{
    if ( ! mfrc522_2.PICC_IsNewCardPresent()) 
    {
      return;
    }
  // Seleciona un tag
  if ( ! mfrc522_2.PICC_ReadCardSerial()) 
    {
      return;
    }
  // Mostra UID en serial
  Serial.print("UID del tag :");
  String contenido= "";
  byte letra;
  for (byte i = 0; i < mfrc522_2.uid.size; i++) 
  {
     Serial.print(mfrc522_2.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522_2.uid.uidByte[i], HEX);
     contenido.concat(String(mfrc522_2.uid.uidByte[i] < 0x10 ? " 0" : " "));
     contenido.concat(String(mfrc522_2.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Mensaje : ");
  contenido.toUpperCase();
  // Verifica si el tag fue leido
  // Reemplazar las XX por los TAGS
  if (contenido.substring(1) == "XX XX XX XX" || contenido.substring(1) == "XX XX XX XX")
  AbreImanPuerta();
  else
  RechazaAcceso();
}

//lector de afuera
void lector2()
{
    if ( ! mfrc522_2.PICC_IsNewCardPresent()) 
    {
      return;
    }
  // Seleciona un tag
  if ( ! mfrc522_2.PICC_ReadCardSerial()) 
    {
      return;
    }
  // Mostra UID en serial
  Serial.print("UID del tag :");
  String contenido= "";
  byte letra;
  for (byte i = 0; i < mfrc522_2.uid.size; i++) 
  {
     Serial.print(mfrc522_2.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522_2.uid.uidByte[i], HEX);
     contenido.concat(String(mfrc522_2.uid.uidByte[i] < 0x10 ? " 0" : " "));
     contenido.concat(String(mfrc522_2.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Mensaje : ");
  contenido.toUpperCase();
  // Verifica si el tag fue leido
  // Reemplazar las XX por los TAGS
  if (contenido.substring(1) == "XX XX XX XX" || contenido.substring(1) == "XX XX XX XX")
  AbreImanPuerta();
  else
  RechazaAcceso();
}
