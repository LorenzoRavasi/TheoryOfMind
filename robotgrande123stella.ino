#include <ESP32Servo.h>

#define PIN_SX1  14 
#define PIN_SX2  12 
#define PIN_SXEN  13 

#define PIN_DX1  26 
#define PIN_DX2  25 
#define PIN_DXEN  33 

Servo myservo;

int speed;
int stop = 0;
int movementType = 0;
int servoPin = 32;
int posServo;
int IRSensor = 23;
int IRSensor2 = 17;
int Send = 34;

void setup() {
  // initialize digital pins as outputs.
  pinMode(PIN_SX1, OUTPUT);
  pinMode(PIN_SX2, OUTPUT);
  pinMode(PIN_SXEN, OUTPUT);
  pinMode(PIN_DX1, OUTPUT);
  pinMode(PIN_DX2, OUTPUT);
  pinMode(PIN_DXEN, OUTPUT);
  pinMode(27, OUTPUT);
  digitalWrite(27, HIGH);
  Serial.begin(9600);
  pinMode (IRSensor, INPUT); // sensor pin INPUT
  pinMode(Send, OUTPUT); 
}

void servoMovement()
{
    myservo.attach(servoPin, 500, 2400);
    myservo.setPeriodHertz(150);
    
    for (posServo = 0; posServo <= 90; posServo += 1) {
       myservo.write(posServo);    
       delay(15);             
    }
    
    for (posServo = 90; posServo >= 0; posServo -= 1) {
       myservo.write(posServo);  
       delay(15);         
    }
}

void distanceLeft()
{
  int statusSensor = digitalRead (IRSensor);
  if(statusSensor==LOW){
    Serial.println("Obstacle");
    speed = 70;
    analogWrite(PIN_SXEN, speed);
    analogWrite(PIN_DXEN, 0);
    delay(90);
    speed = 160;
    analogWrite(PIN_SXEN, speed);
    analogWrite(PIN_DXEN, speed);
    delay(150);  
  }
  else{
    Serial.println("Free");
  }
  delay(50);
}

void distanceRight()
{
  int statusSensor2 = digitalRead (IRSensor2);
  if(statusSensor2==LOW){
    Serial.println("Obstacle");
    speed = 70;
    analogWrite(PIN_SXEN, 0);
    analogWrite(PIN_DXEN, speed);
    delay(90);
    speed = 160;
    analogWrite(PIN_SXEN, speed);
    analogWrite(PIN_DXEN, speed);
    delay(150);    
  }
  else{
    Serial.println("Free");
  }
  delay(50);
}

void loop() {

  delay(5000);
  //movimento in avanti
  digitalWrite(PIN_SX1, HIGH);
  digitalWrite(PIN_SX2, LOW);
  digitalWrite(PIN_DX1, HIGH);
  digitalWrite(PIN_DX2, LOW);

  speed = 120;
  servoMovement();
  analogWrite(PIN_SXEN, speed);
  analogWrite(PIN_DXEN, speed + 2);
  distanceLeft();
  distanceRight();
  delay(4500);

  speed = 248;
  analogWrite(PIN_SXEN, speed);
  analogWrite(PIN_DXEN, 0);
  delay(1800);
  analogWrite(Send, 0xFF);
  delay(50);
  analogWrite(PIN_SXEN, 0);
  analogWrite(PIN_DXEN, 0);
  delay(3000);

  speed = 238;
  analogWrite(PIN_SXEN, speed);
  analogWrite(PIN_DXEN, 0);
  delay(1650);
  analogWrite(PIN_SXEN, 0);
  analogWrite(PIN_DXEN, 0);
  servoMovement();
  delay(1000);
  
  speed = 150;
  servoMovement();
  analogWrite(PIN_SXEN, speed);
  analogWrite(PIN_DXEN, speed + 2);
  distanceLeft();
  distanceRight();
  delay(5000);
    
  speed = 248;
  analogWrite(PIN_SXEN, speed);
  analogWrite(PIN_DXEN, 0);
  delay(1800);
  analogWrite(Send, 0xFF);
  delay(50);
  analogWrite(PIN_SXEN, 0);
  analogWrite(PIN_DXEN, 0);
  delay(3000);
  
  speed = 232;
  analogWrite(PIN_SXEN, speed);
  analogWrite(PIN_DXEN, 0); 
  delay(1650);
  analogWrite(PIN_SXEN, 0);
  analogWrite(PIN_DXEN, 0);
  servoMovement();  
  delay(10000);


}
