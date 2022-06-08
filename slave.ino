//#include <IRremote.hpp>
//#include <ESP32Servo.h>
#include <analogWrite.h>


//pins
#define LED_PIN 2
#define SERVO_PIN 5
#define DISTANCE_PIN1 16
#define DISTANCE_PIN2 17
#define AIN1_PIN 14
#define AIN2_PIN 12
#define BIN1_PIN 26
#define BIN2_PIN 25
#define PWMA_PIN 13
#define PWMB_PIN 33
#define STBY_PIN 27
#define IR_PIN 4

//speed
#define LOW_SPEED 60 
#define HIGH_SPEED 70 
#define EXTREME_SPEED 100
#define MEDIUM_SPEED (LOW_SPEED+HIGH_SPEED)/2

int stateCurrent = 0;
int movementStatus = 0;
int servoStatus = 0;
int posServo = 0;
//float speed = 0.7;


void setup() {
  
  pinMode(LED_PIN, OUTPUT);
  pinMode(DISTANCE_PIN1, INPUT);
  pinMode(DISTANCE_PIN2, INPUT);
  pinMode(PWMA_PIN, OUTPUT);
  pinMode(AIN1_PIN, OUTPUT);
  pinMode(AIN2_PIN, OUTPUT);
  pinMode(PWMB_PIN, OUTPUT);
  pinMode(BIN1_PIN, OUTPUT);
  pinMode(BIN2_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  Serial.begin(9600);

  
}

void loop() {

  delay(3000);
  
  movement(movementStatus);

}

void move_straight(int speed, bool direction)
{
  digitalWrite(STBY_PIN, HIGH);
  //set direction motor 1
  digitalWrite(AIN1_PIN, direction);
  digitalWrite(AIN2_PIN, !direction);
  //set direction motor 2
  digitalWrite(BIN1_PIN, direction);
  digitalWrite(BIN2_PIN, !direction);
  //set speed motor 1
  analogWrite(PWMA_PIN, speed);
  //set speed motor 2
  analogWrite(PWMB_PIN, speed);
}

void turn(int right_speed, int left_speed, bool direction)
{
  digitalWrite(STBY_PIN, HIGH);
  //set direction motor 1
  digitalWrite(AIN1_PIN, direction);
  digitalWrite(AIN2_PIN, !direction);
  //set direction motor 2
  digitalWrite(BIN1_PIN, direction);
  digitalWrite(BIN2_PIN, !direction);
  //set speed motor 1
  analogWrite(PWMA_PIN, right_speed);
  //set speed motor 2
  analogWrite(PWMB_PIN, left_speed);
}

void spin(int speed, bool direction)  //motor A is the right one, motor B is the left one
{
  digitalWrite(STBY_PIN, HIGH);
  //set direction motor 1
  digitalWrite(AIN1_PIN, direction);
  digitalWrite(AIN2_PIN, !direction);
  //set direction motor 2
  digitalWrite(BIN1_PIN, !direction);
  digitalWrite(BIN2_PIN, direction);
  //set speed motor 1
  analogWrite(PWMA_PIN, speed);
  //set speed motor 2
  analogWrite(PWMB_PIN, speed);
}

bool obstacle1()
{
  //The sensor1 returns a 1 if an obstacle is present,0 otherwise
  return !(digitalRead(DISTANCE_PIN1));
}

bool obstacle2()
{
  //The sensor1 returns a 1 if an obstacle is present,0 otherwise
  return !(digitalRead(DISTANCE_PIN2));
}

void avoidObstacle(){
    if(obstacle2()==false && obstacle1()==false){
        move_straight(MEDIUM_SPEED,true);
    }
    else if(obstacle1()==true && obstacle2()==false){
        turn(0,LOW_SPEED,true);
    }
    else if(obstacle1()==false && obstacle2()==true){
        turn(LOW_SPEED,0,true);
    }
    else{
        //reverse,da vedere
    }
}

void movement(int movementStatus){

  //pin 12 e 14 ruota 1 da verificare se dx o sx
  //pin 26 e 25 ruota 2 da verificare se dx o sx

  switch(movementStatus)
  {
    case 0:
        //rapido movimento servo
        movementStatus=1;
        break;

    case 1:
        //qua si gira di 90 gradi a sx
        turn(0,MEDIUM_SPEED,true);
        movementStatus=2;
        break;

    case 2:
        //qua attende qualche secondo fermo
        delay(5000);
        movementStatus=3;
        break;

    case 3:
        //qua si muove in avanti e se riceve segnale si blocca!
        break;
        
    /*
    case 1:
        //movement
    while(movementStatus = 1){
        digitalwrite(27, speed/2);
        digitalwrite(12, speed/2);
        digitalwrite(14, speed);
        digitalwrite(13, speed);
        delay(500);
        digitalwrite(27, speed);
        digitalwrite(12, speed);
        digitalwrite(14, speed/2);
        digitalwrite(13, speed/2);
        delay(500);
    }
    break;
    
    case 2:
        //rotation 90 clockwise
        speed = 1;
        digitalwrite(27, 0);
        digitalwrite(12, 0);
        digitalwrite(14, speed);
        digitalwrite(13, speed);
        delay(1000);
        speed = 0.7;
        break;
        
    case 3:
        //rotation 90 clockwise
        speed = 1;
        digitalwrite(27, speed);
        digitalwrite(12, speed);
        digitalwrite(14, 0);
        digitalwrite(13, 0);
        delay(1000);
        speed = 0.7;
        break;
  }

    case 4:
       //free rotation
       break;

    case 5:
       //stop
        digitalwrite(27, speed/2);
        digitalwrite(12, speed/2);
        digitalwrite(14, speed);
        digitalwrite(13, speed);
        delay(250);
        digitalwrite(27, 0);
        digitalwrite(12, 0);
        digitalwrite(14, 0);
        digitalwrite(13, 0);
        break;
    
    case 6:
       //reverse
        speed = -0.3;
        digitalwrite(27, speed);
        digitalwrite(12, speed);
        digitalwrite(14, speed);
        digitalwrite(13, speed);
        speed = 0.7;
        break;

    case 7:
       //stall
         speed = 0;
         digitalwrite(27, speed);
         digitalwrite(12, speed);
         digitalwrite(14, speed);
         digitalwrite(13, speed);
         speed = 0.7;
         break;

    case 8:
       //lateral adjustment with right tyre
       speed = -0.7;
       digitalwrite(27, speed);
       digitalwrite(12, speed);
       delay(250);
       //movement(7);
       break;

    case 9:
       //lateral adjustment with right tyre
       speed = -0.7;
       digitalwrite(14, speed);
       digitalwrite(13, speed);
       delay(250);
       //movement(7);
       break;*/
  }
}
