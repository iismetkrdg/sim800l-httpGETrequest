#include <SoftwareSerial.h>

//Create software serial object to communicate with SIM800L
SoftwareSerial mySerial(2,4); //SIM800L Tx & Rx is connected to Arduino #3 & #2
int tekrar = 0;
void xyz()
{
  pinMode(13,OUTPUT);
  
  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);
  
  //Begin serial communication with Arduino and SIM800L
  mySerial.begin(9600);
  
  mySerial.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\""); 
  waituntil("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
  updateSerial();
  mySerial.println("AT+CSTT=\"3gprs\",\"3gprs\",\"3gprs\"");
  waituntil("AT+CSTT=\"3gprs\",\"3gprs\",\"3gprs\"");
  updateSerial();
  mySerial.println("AT+SAPBR=1,1");
  waituntil("AT+SAPBR=1,1");
  updateSerial();
  tekrar = 1;
}
void connecttoData()
{
  delay(2000);
  mySerial.println("AT+HTTPINIT﻿﻿");
  waituntil("AT+HTTPINIT﻿﻿");
  updateSerial();
  mySerial.println("﻿AT+HTTPPARA=\"CID\",1"); 
  waituntil("﻿AT+HTTPPARA=\"CID\",1");
  updateSerial();
  mySerial.println("﻿AT+HTTPPARA=\"URL\",\"http://ergast.com/api/f1/2004/1/results.json\"﻿"); 
  waituntil("﻿AT+HTTPPARA=\"URL\",\"http://ergast.com/api/f1/2004/1/results.json\"﻿");
  updateSerial();
  mySerial.println("AT+HTTPACTION=0﻿﻿"); 
  waituntil("AT+HTTPACTION=0﻿﻿");
  updateSerial();
  mySerial.println("AT+HTTPREAD﻿");
  waituntilREAD();
  updateSerial();
  mySerial.println("﻿AT+HTTPTERM");
  updateSerial();
}
void setup()
{     
  xyz();
  
}

void loop()
{
  if(tekrar != 1)
  {
    xyz();
  }
  connecttoData();
}

void updateSerial()
{
  delay(1000);
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(mySerial.available()) 
  {
    
    String gelen = mySerial.readString();
    Serial.println(gelen);
    if(gelen.indexOf("AC")!=-1){
        digitalWrite(13,HIGH);
        
    }
    if(gelen.indexOf("KAPA")!=-1){
        digitalWrite(13,LOW);
    }
  }
}
void waituntil(String key)
{
  int i=0;
  String gelen = mySerial.readString();
  Serial.println(gelen);
  bool cont = true;
  while(gelen.indexOf("OK")==-1 && cont==true)
  {
    gelen = mySerial.readString();
    Serial.println(gelen);
    if(gelen.indexOf("ERROR")!=-1 || gelen.indexOf("OK")!=-1){
      cont=false; 
     }
    delay(1000);
    i = i+1;
    if(i>4){
      mySerial.println(key);
      i=0;
    }
  } 
}
void waituntilREAD(){
  while(mySerial.readString().indexOf("+HTTPREAD:")==-1)
  {
    delay(1000);
  } 
}
