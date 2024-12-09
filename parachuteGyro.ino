#include <Wire.h>
float RateRoll, RatePitch, RateYaw;
float AccX, AccY, AccZ;
float AngleRoll, AnglePitch;
float LoopTimer;

int buttonPin = 2;
bool buttonState = false;
bool toggle = true;

int LEDpin = 3;

void gyro_signals(void) 
{
  Wire.beginTransmission(0x68);
  Wire.write(0x1A);
  Wire.write(0x05);
  Wire.endTransmission();
  Wire.beginTransmission(0x68);
  Wire.write(0x1C);
  Wire.write(0x10);
  Wire.endTransmission();
  Wire.beginTransmission(0x68);
  Wire.write(0x3B);
  Wire.endTransmission(); 
  Wire.requestFrom(0x68,6);
  int16_t AccXLSB = Wire.read() << 8 | Wire.read();
  int16_t AccYLSB = Wire.read() << 8 | Wire.read();
  int16_t AccZLSB = Wire.read() << 8 | Wire.read();
  Wire.beginTransmission(0x68);
  Wire.write(0x1B); 
  Wire.write(0x8);
  Wire.endTransmission();                                                   
  Wire.beginTransmission(0x68);
  Wire.write(0x43);
  Wire.endTransmission();
  Wire.requestFrom(0x68,6);
  int16_t GyroX=Wire.read()<<8 | Wire.read();
  int16_t GyroY=Wire.read()<<8 | Wire.read();
  int16_t GyroZ=Wire.read()<<8 | Wire.read();
  RateRoll=(float)GyroX/65.5;
  RatePitch=(float)GyroY/65.5;
  RateYaw=(float)GyroZ/65.5;
  AccX=(float)AccXLSB/4096;
  AccY=(float)AccYLSB/4096;
  AccZ=(float)AccZLSB/4096  + .07;
  AngleRoll=atan(AccY/sqrt(AccX*AccX+AccZ*AccZ))*1/(3.142/180);
  AnglePitch=-atan(AccX/sqrt(AccY*AccY+AccZ*AccZ))*1/(3.142/180);
}

void parachute_event(void)
{ 
  if(toggle)
  {
    digitalWrite(LEDpin, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(50);                      // wait for a second
    digitalWrite(LEDpin, LOW);   // turn the LED off by making the voltage LOW
    delay(50);
  }
}

void setup() 
{

  Serial.begin(57600);

  pinMode(LEDpin,OUTPUT);
  digitalWrite(LEDpin, HIGH);

  pinMode(buttonPin, INPUT);
  Wire.setClock(400000);
  Wire.begin();
  delay(250);
  Wire.beginTransmission(0x68); 
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission();
}

void loop() 
{
  gyro_signals();
  /*
  Serial.print("Acceleration X [g]= ");
  Serial.print(AccX);
  Serial.print(" Acceleration Y [g]= ");
  Serial.print(AccY);
  */
  Serial.print(" Acceleration Z [g]= ");
  Serial.println(AccZ);
  Serial.println(toggle);

  buttonState = digitalRead(buttonPin);
  if(toggle && buttonState)
  {
    toggle = false;
  }
  else if(buttonState)
  {
    toggle = true;
  }

  if(AccZ <= 0)
  {
    parachute_event();
  }

  if(toggle)
  {
    digitalWrite(LEDpin,HIGH);
  }
  else
  {
    digitalWrite(LEDpin,LOW);
  }
  delay(200);
}
