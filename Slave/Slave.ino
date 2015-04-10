

#include <Wire.h>
#define ALZA_T_NO_HUM           0
#define ALZA_T_ABBASSA_HUM      1
#define ABBASSA_T_NO_HUM        2
#define ABBASSA_T_ABBASSA_HUM   3
#define NO_T_NO_HUM             4
#define NO_T_ABBASSA_HUM        5



int humidex[] {100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,85,75,65,55,45,40,30,25,20,15,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10};  //valori di umidità minimi da 0 a 49 gradi
int Temperatura,Umidita,TempDesid,HumDesid;
byte receive_array[6];
byte azione;

void setup()
{
  Wire.begin(40);                // join i2c bus with address #4
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent); // register event
  Serial.begin(9600);   
  // start serial for output
}

void loop()
{
  delay(100);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
  byte i=0;

  while( Wire.available() && i<6) 
  {
    receive_array[i] = Wire.read(); // receive byte as a character
    i++;      
  }
  Temperatura=(int)word(receive_array[1],receive_array[0]);
  TempDesid=(int)word(receive_array[3],receive_array[2]);
  Umidita=(int)word(receive_array[5],receive_array[4]);
  HumDesid=humidex[TempDesid];
  Serial.print("Temperatura -->");
  Serial.print(Temperatura);
  Serial.print("\n");
  Serial.print("TempDesid -->");
  Serial.print(TempDesid);
  Serial.print("Umidità-->");
  Serial.print(Umidita);
  Serial.print("\n");
  Serial.print("HumDesid -->");
  Serial.print(HumDesid);
  Serial.print("\n");
  if(Temperatura>TempDesid+2)
  {
    if(Umidita > HumDesid)
    {
      azione= ABBASSA_T_ABBASSA_HUM;
    }
    else
    {
       azione= ABBASSA_T_NO_HUM;
    }
  }
  if(Temperatura < TempDesid -2) 
  {
    if(Umidita > HumDesid)
    {
      azione=ALZA_T_ABBASSA_HUM;
    }
    else
    {
      azione=ALZA_T_NO_HUM;
    }
  }
  if((Temperatura>= TempDesid-2)&&(Temperatura<=TempDesid+2))
  {
    if(Umidita > HumDesid)
    {
      azione=NO_T_ABBASSA_HUM;
    }
    else
    {
      azione=NO_T_NO_HUM;
    }  
  }
}
void requestEvent()
{
  Wire.write(azione); 
                     
}
