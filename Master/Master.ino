

//Sample using LiquidCrystal library
#include <LiquidCrystal.h>
#include <Wire.h>
#include "HTU21D.h"

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

#define SlaveArduino 40

#define ALZA_T_NO_HUM           0
#define ALZA_T_ABBASSA_HUM      1
#define ABBASSA_T_NO_HUM        2
#define ABBASSA_T_ABBASSA_HUM   3
#define NO_T_NO_HUM             4
#define NO_T_ABBASSA_HUM        5

// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
HTU21D myHumidity;
// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;


bool page;
int Temperatura,Umidita;
int TempDesid=20;                         //la temperatura desiderata di default è 20°C

int HumPin = 11,TempPinRaise=12,TempPinDown=13;               //pin di uscita per il pilotaggio del sistema di condizionamento

byte counter=0,seconds=0,minutes=0;       //variabili per la temporizzazione

// read the buttons
int read_LCD_buttons()
{
 adc_key_in = analogRead(0);             // read the value from the sensor 
 
 
 if (adc_key_in > 1000) return btnNONE;  // We make this the 1st option for speed reasons since it will be the most likely result

 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 200)  return btnUP; 
 if (adc_key_in < 400)  return btnDOWN; 
 if (adc_key_in < 600)  return btnLEFT; 
 if (adc_key_in < 800)  return btnSELECT;  

 return btnNONE;                         // when all others fail, return this...
}

void send_data()
{
  byte send_array[6];                    //dichiarazione dell'array da inviare

  
  
  send_array[0]=lowByte(Temperatura);    //le variabili vengono frazionate e inserite in un array in modo ordinato
  send_array[1]=highByte(Temperatura);
  send_array[2]=lowByte(TempDesid);
  send_array[3]=highByte(TempDesid);
  send_array[4]=lowByte(Umidita);
  send_array[5]=highByte(Umidita);
  
  Wire.beginTransmission(SlaveArduino); 
  Wire.write(send_array,6);              //invio dell'array allo slave
  Wire.endTransmission();                // stop transmitting
  
}

void refresh_LCD()
{
   lcd.setCursor(0,0);     
 lcd_key = read_LCD_buttons();          // read the buttons

 switch (lcd_key)                       // depending on which button was pushed, we perform an action
 {
   case btnRIGHT:                       //visualizzazione della temperatura desiderata e comandi per le opzioni disponibili
     {
          second_page:
 	  lcd.clear();
	  page=1;
	  lcd.setCursor(0, 0);
          lcd.print("Temp Desiderata");
	  lcd.setCursor(0,1);
	  lcd.print(TempDesid);
	  lcd.setCursor(2,1);
          lcd.print((char)223);
	  lcd.setCursor(4,1);
	  lcd.print("[^]/[v]/[<]");
          delay(100);
          break;
     }
   case btnLEFT:                       //visualizzazione dei valori di temperatura,pressione e umidità e dei comandi per le opzioni
     {
        first_page:
        lcd.clear();
        page=0;
        lcd.setCursor(0, 0);
        lcd.print("Temp");
        lcd.setCursor(5,0);
        lcd.print(Temperatura);
        lcd.setCursor(9, 0);
        lcd.print("Hum");
        lcd.setCursor(12,0);
        lcd.print(Umidita);
        lcd.setCursor(0, 1);
        lcd.print("Press");
        lcd.setCursor(6,1);
        lcd.print(110);
       lcd.setCursor(12,1);
       lcd.print("[>]");
     delay(100);
     break;
     }
   case btnUP:                       //aumento la temperatura di un grado,avverto tramite lcd del cambiamento dopodichè mi riposiziono a pagina 2
     { 
       	  lcd.clear();
		 
	  lcd.setCursor(0, 0);
          lcd.print("Temp Desiderata");
          
         if(TempDesid<40)
          {
            TempDesid++;
            lcd.setCursor(0, 1);
            lcd.print("Alzata di 1");
            lcd.setCursor(11,1);
            lcd.print((char)223);
          }
          else
          {	
            lcd.setCursor(0, 1);
            lcd.print("Troppo alta");  

          }
          
          delay(750);
          goto second_page;


     break;
     }
   case btnDOWN:                //abbasso la temperatura di un grado,avverto tramite lcd del cambiamento dopodichè mi riposiziono a pagina 2
     {
       	  lcd.clear();
    
	  lcd.setCursor(0, 0);
          lcd.print("Temp Desiderata");
          if(TempDesid>10) 
          {
            TempDesid--;
            lcd.setCursor(0, 1);
            lcd.print("Abbassata di 1");
            lcd.setCursor(14,1);
            lcd.print((char)223);
          }
          else
          {	
            lcd.setCursor(0, 1);
            lcd.print("Troppo bassa");  

          }
          delay(750);
          goto second_page;
          

     break;
     }
   case btnSELECT:
     {
     
     break;
     }
     case btnNONE:
     {
       delay(250);
       if(page==0) goto first_page;
       if(page==1) goto second_page;
       break;

     }
 }
}
bool temporizza()                  //funzione di temporizzazione
{
  counter++;
  if(counter==4)
  {
    counter=0;
    seconds++;
        Serial.write("\npassato un secondo   ");
        Serial.print(seconds);

  }
  if(seconds==60)
  {
    seconds=0;
    minutes++;
        Serial.write("passato un minuto\n");
  }
  if(minutes==1)
  {
    minutes=0;
    return 1;
  }
  return 0;
}
  
void Pilotaggio()              //richiede il responso allo slave e polota il sistema di condizionamento di conseguenza
{
   byte azione;
   delay(1000);                //mi assicuro che lo slave abbia eseguito tutti i calcoli
   Wire.requestFrom(40, 1);    // request 6 bytes from slave device #2
   while(Wire.available())    // slave may send less than requested
   {
    azione = Wire.read(); // receive a byte as character
   }
  Serial.print(azione);
  Serial.print("\n");
  /* if(azione==ALZA_T_NO_HUM)
   {
     digitalWrite(HumPin, LOW);
     digitalWrite(TempPinRaise, HIGH); 
     digitalWrite(TempPinDown, LOW);     
   }
   if(azione==ALZA_T_ABBASSA_HUM)
   {
     digitalWrite(HumPin, HIGH);
     digitalWrite(TempPinRaise, HIGH); 
     digitalWrite(TempPinDown, LOW);         
   }
   if(azione==ABBASSA_T_NO_HUM)
   {
     digitalWrite(HumPin, LOW);
     digitalWrite(TempPinRaise, LOW); 
     digitalWrite(TempPinDown, HIGH);          
   }
   if(azione==ABBASSA_T_ABBASSA_HUM)
   {
     digitalWrite(HumPin, HIGH);
     digitalWrite(TempPinRaise, LOW); 
     digitalWrite(TempPinDown, HIGH);    
   }
   if(azione==NO_T_NO_HUM)
   {
     digitalWrite(HumPin, LOW);
     digitalWrite(TempPinRaise, LOW); 
     digitalWrite(TempPinDown, LOW);
   }
   if(azione==NO_T_ABBASSA_HUM)
   {
     digitalWrite(HumPin, HIGH);
     digitalWrite(TempPinRaise, LOW); 
     digitalWrite(TempPinDown, LOW);
   }   */
}

void Polling()
{
  Temperatura= (int)myHumidity.readTemperature();
  Umidita=(int)myHumidity.readHumidity();

}
void setup()
{
  Wire.begin(); 
  myHumidity.begin();
  Serial.begin(9600);
  pinMode(HumPin, OUTPUT); 
  pinMode(TempPinRaise, OUTPUT); 
  pinMode(TempPinDown, OUTPUT);   
  
  page=0;
  
  lcd.begin(16, 2);                  //inizializzo il display con la pagina 1          
  lcd.setCursor(0, 0);
  lcd.print("Temp");
  lcd.setCursor(5,0);
  lcd.print(Temperatura);
  lcd.setCursor(8,0);
  lcd.print((char)223);
  lcd.setCursor(9, 0);
  lcd.print("Hum");
  lcd.setCursor(13,0);
  lcd.print(Umidita);
  lcd.setCursor(0, 1);
  lcd.print("Pres");
  lcd.setCursor(5,1);
  lcd.print(110);
  lcd.setCursor(8,1);
  lcd.print("HPa");
  lcd.setCursor(12,1);
  lcd.print("[>]");
  
  digitalWrite(HumPin, LOW);       //inizializzo i pin di pilotaggio a valore non attivo
  digitalWrite(TempPinRaise, LOW); 
  digitalWrite(TempPinDown, LOW);  

}
 
void loop()
{
  bool trigger;
  Polling();
  refresh_LCD();
  trigger=temporizza();
//  if(trigger)        Serial.write("\n\n\nDUCE DEL FASCISMO\n\n\n\n");   // quando si inviano i dati e ci si prepara alla ricezione
  if(trigger)  {send_data();Pilotaggio();} 
  

}

