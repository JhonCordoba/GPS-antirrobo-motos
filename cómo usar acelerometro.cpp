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

void setup() {
  // pin A0 (pin14) is VCC and pin A4 (pin18) in GND to activate the GY-61-module
  pinMode(A0, OUTPUT);
  pinMode(A4, OUTPUT);
  digitalWrite(14, HIGH);
  digitalWrite(18, LOW);
  
  // activating debugging for arduino UNO
  Serial.begin(9600);
} // end setup

void loop() {

  if(primerVez){
    
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
  
  if( diferenciaX > 1 || diferenciaY > 1){
    
      // show x, y and z-values
      Serial.print("x = ");
      Serial.print(valorActualx);
      Serial.print("  y = ");
      Serial.print(valorActualy);
      Serial.print("  z = ");
      Serial.print(valorActualz);
      // show angle
      Serial.println(constrain(map(valorActualx,349,281,0,90),0,90));
      
      valorX = valorActualx;
      valorY = valorActualy;
      
  }

  
} // end loop