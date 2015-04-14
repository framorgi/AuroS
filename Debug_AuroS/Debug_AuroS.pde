import processing.serial.*;

Serial myPort;  
int tmp,pr=0,hum=100,old_x=0, screen_increment=width/15, old_pr=height/2,old_hum=5*height/6,old_tmp=height/6, old_y=0;     
String inString;  
int lf = 10;     
void setup() 
{

  size(displayWidth-100, 600);
  String portName = Serial.list()[0];
  println(Serial.list());
  
  myPort = new Serial(this, portName,9600);
  myPort.bufferUntil(lf);
 
 //GRAFICO ASSI
  background(0,0,0);
  stroke(255,255,255);                                                   
  strokeWeight(1);                                                 
  line(0,height/6,width,height/6);  
  line(0,height/2,width,height/2); 
  line(0,5*height/6,width,5*height/6);   
  strokeWeight(1);         
  line(width/15,0,width/15,height); 

//  TESTI FISSI

text("TMP C°", 5, 100);

text("0",width/15-15, 100);

text("50",width/15-15, 25);

text("PRES KPas", 5, 300);

text("0",width/15-15, 300);

text("50",width/15-15, 225);

text("HUM %", 5,500);

text("0",width/15-15, 500);

text("50",width/15-15, 425);

  
}//setup

void draw()
{
}

void serialEvent(Serial myPort) { 

inString = myPort.readString();
inString = trim(inString);
println(inString);
///PARSING DELLA STRINGA
String[] coppia;
coppia = split(inString,"|");
tmp=int(trim(coppia[0]));
pr=int(trim(coppia[1]));
hum=int(trim(coppia[2]));
//ARDUINO INVIA Serial.print(val1) Serial.print("|")e Serial.print(val2)
//si fa il parsing e si splitta la stringa 
//tmp = int(inString);//convert the string into a number we can use

  strokeWeight(2);
  stroke(#FF0000);
  line(old_x, old_tmp, screen_increment,100-tmp);
  stroke(#00FF00);
  line(old_x, old_pr, screen_increment,400-pr);
  stroke(#0000FF);
  line(old_x, old_hum, screen_increment,600-hum);
  
  old_x = screen_increment;
  old_tmp = 100-tmp;
  old_pr=400-pr;
  old_hum= 600-hum;
  
  screen_increment=screen_increment+8;
  
  if(screen_increment>(displayWidth-100)){
     //GRAFICO ASSI
  background(0,0,0);
  stroke(255,255,255);                                                   
  strokeWeight(1);                                                 
  line(0,height/6,width,height/6);  
  line(0,height/2,width,height/2); 
  line(0,5*height/6,width,5*height/6);   
  strokeWeight(1);         
  line(width/15,0,width/15,height); 

//  TESTI FISSI

text("TMP C°", 5, 100);

text("0",width/15-15, 100);

text("50",width/15-15, 25);

text("PRES KPas", 5, 300);

text("0",width/15-15, 300);

text("50",width/15-15, 225);

text("HUM %", 5,500);

text("0",width/15-15, 500);

text("50",width/15-15, 425);

  screen_increment=-50; 
  old_x = -50;
  old_y = 0;
  
  }
  
}//serial event
