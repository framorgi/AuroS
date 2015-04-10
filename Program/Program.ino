#include <LiquidCrystal.h>


#define TMP 0
#define PR  1
#define HUM 2
#define BTIME 104

const int analogInPin = A1;  // Analog input pin that the potentiometer is attached to
const int analogOutPin = 2; // Analog output pin that the LED is attached to
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int TemperaturaMap;
int UmiditaMap;
float Temperatura=0;
int Umidita=1;

char data=255;             //inizializzazione del pacchetto dati a 1111 1111  (il segnale in uscita è attivo-basso)

void setup() {
  // 9600 bps:
  Serial.begin(9600); 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.setCursor(0,0);
  lcd.print("Temp ");
  
  lcd.setCursor(10,0);
  lcd.print("Hum ");
  
  lcd.setCursor(0,1);
  lcd.print("Pres ");
  digitalWrite(analogOutPin,1);  //attivo basso non trasmette
}

//prototipi
void pacchetto_dati(int sensorValue, int type);
void send_data();

//LOOP

void loop() {
// Lettura sensori:

Umidita=4*(Umidita);
if(Umidita>100){Umidita=1;}
Temperatura= ((float)analogRead(analogInPin))*500/1023;  //conversione del valore del livello al valore della temperatura
//seriale  
// pacchetto_dati(Temperatura,TMP);
// send_data();
      
 TemperaturaMap = map((int)Temperatura, 0,500, 0,200);
 UmiditaMap=map(Umidita, 0,100, 0,200);
 
    Serial.print(TemperaturaMap);
    Serial.print("|");
    Serial.print(Temperatura);
    Serial.print("|");
    Serial.print(analogRead(analogInPin));
    Serial.println();
   
  lcd.setCursor(5,0);
  lcd.print((int)Temperatura);
  lcd.setCursor(7,0);
  lcd.print((char)223);
  lcd.setCursor(8,0);
  // controllo unità misura
  lcd.print("C");
  lcd.setCursor(14,0);
  lcd.print(Umidita);
  
  //outputValue = map(sensorValue, 0, 1023, 0, 255);  
  delay(1500);                   
}


//scrittura pacchetto
void pacchetto_dati(int sensorValue, int type)
{

    for(int i=0;i<6;i++){
     if( bitRead(sensorValue,i)==1)   //imposta i valori del pacchetto data
     {
	    bitClear(data, i);
     }
    }	 
	 for(int i=0;i<2;i++){
     if( bitRead(type,i)==1)   //imposta i valori del pacchetto data
     {
	    bitClear(data, i+6);
     }	 
}
  
}
 
 
 //invio
void send_data()
{
     digitalWrite(analogOutPin,0 );   //start bit
        delayMicroseconds(BTIME);
     for(int i=0;i<6;i++)
	 {
	   if( bitRead(data,i)==1) digitalWrite(analogOutPin,0 );
	   else digitalWrite(analogOutPin,1 );
	   delayMicroseconds(BTIME);
     }	 

  digitalWrite(analogOutPin,0 ); //stopbit
     delayMicroseconds(BTIME);
     digitalWrite(analogOutPin,1);  //attivo basso non trasmette
}

