#include "SIM900.h"
#include <SoftwareSerial.h>
#include "sms.h"
#include "gps.h"
SMSGSM sms;

int numdata;
boolean started=false;
char smsbuffer[160];
char n[20];

///////Acelerometro////////////
const int VCCPin = A0;
const int xPin   = A1;
const int yPin   = A2;
const int zPin   = A3;
const int GNDPin = A4;

int valorX = 0;
int valorY = 0;
int valorZ = 0;

int valorActualx = 0;
int valorActualy = 0;
int valorActualz = 0;
boolean primerVez = true;
//////////////////////////////

char mensaje[31]; 

/////////////////////////////GPS////////////////////////
GPSGSM gps;

char lon[15];
char lat[15];
char alt[15];
char time[20];
char vel[15];
char msg1[5];
char msg2[5];

char stat;
char inSerial[20];
int i=0;
///////////////////////////////////////////////////////

void setup()
{
    //Intentando pasar el bloqueo del pin
    //gsm.SimpleWriteln("AT+CPIN=\"0000\"");
    
     //Serial connection.
     Serial.begin(9600);
     Serial.println("GSM Shield testing.");
     
     //Start configuration of shield with baudrate.
     if (gsm.begin(2400)) {
          Serial.println("\nstatus=READY");
          started=true;
     } else Serial.println("\nstatus=IDLE");
  
      pinMode(A0, OUTPUT);
      pinMode(A4, OUTPUT);
      digitalWrite(14, HIGH);
      digitalWrite(18, LOW);


}

void loop()
{

     //serialhwread();
     //serialswread();

     if(started) {
          
            if(primerVez){

                delay(9000);
                valorX = analogRead(xPin);
                valorY = analogRead(yPin);
                valorZ = analogRead(zPin);
                primerVez = false;    
                
            }

            
            valorActualx = analogRead(xPin);
            valorActualy = analogRead(yPin);
            valorActualz = analogRead(zPin);
            
            int diferenciaX = valorX - valorActualx;
            int diferenciaY = valorY - valorActualy;          

            if( diferenciaX > 1 || diferenciaY > 1  ){
                
                  //obtenerUbicacion();
                  
                  if (sms.SendSMS("317494xxxx" ,  "SE MOVIO TU VEHICULO"))
                      Serial.println("\nSMS sent OK");  
                         
                  
                  valorX = valorActualx;
                  valorY = valorActualy;
                  
              }
          
          delay(1000);
     }
}

void obtenerUbicacion(){

     if(started) {
          //GPS attach
          if (gps.attachGPS())
               Serial.println("status=GPSREADY");
          else Serial.println("status=ERROR");

          delay(20000);  //Time for fixing
          stat=gps.getStat();
          if(stat==1)
               Serial.println("NOT FIXED");
          else if(stat==0)
               Serial.println("GPS OFF");
          else if(stat==2)
               Serial.println("2D FIXED");
          else if(stat==3)
               Serial.println("3D FIXED");
          delay(6000);
          //Get data from GPS
          gps.getPar(lon,lat,alt,time,vel);
          Serial.println("Esta son las coordenadas: ");
          Serial.println(lon);
          Serial.println(lat);
          Serial.println(alt);
          Serial.println(time);
          Serial.println(vel);
     }
  
}


/////
void serialhwread()
{
     i=0;
     if (Serial.available() > 0) {
          while (Serial.available() > 0) {
               inSerial[i]=(Serial.read());
               delay(10);
               i++;
          }

          inSerial[i]='\0';
          if(!strcmp(inSerial,"/END")) {
               Serial.println("_");
               inSerial[0]=0x1a;
               inSerial[1]='\0';
               gsm.SimpleWriteln(inSerial);
          }
          //Send a saved AT command using serial port.
          if(!strcmp(inSerial,"TEST")) {
//      Serial.println("BATTERY TEST 1");
//      gps.getBattInf(msg1,msg2);
//      Serial.println(msg1);
//      Serial.println(msg2);
//      Serial.println("BATTERY TEST 2");
//      gps.getBattTVol(msg1);
//      Serial.println(msg1);
               stat=gps.getStat();
               if(stat==1)
                    Serial.println("NOT FIXED");
               else if(stat==0)
                    Serial.println("GPS OFF");
               else if(stat==2)
                    Serial.println("2D FIXED");
               else if(stat==3)
                    Serial.println("3D FIXED");
          }
          //Read last message saved.
          if(!strcmp(inSerial,"MSG")) {
               Serial.println(msg1);
          } else {
               Serial.println(inSerial);
               gsm.SimpleWriteln(inSerial);
          }
          inSerial[0]='\0';
     }
}

void serialswread()
{
     gsm.SimpleRead();
}

char* formarMensaje(  ){

  mensaje[15] = ','; 
  
  for(int i = 0; i < 15; i++){

    mensaje[i] = lon[i];
    
    
  }

  for(int i = 16, k = 0; i < 31; i++, k++){

      mensaje[i] = lat[k]; 
  
  }

  return mensaje;
  
}

