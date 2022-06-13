#include <IRremote.hpp>
#include <analogWrite.h>


//pins
#define SERVO_PIN 5
#define DISTANCE_PIN1 32
#define DISTANCE_PIN2 35
#define AIN1_PIN 14
#define AIN2_PIN 12
#define BIN1_PIN 26
#define BIN2_PIN 25
#define PWMA_PIN 13
#define PWMB_PIN 33
#define STBY_PIN 27
#define IR_PIN 15

//speed
#define LOW_SPEED 60 
#define HIGH_SPEED 70 
#define EXTREME_SPEED 100
#define MEDIUM_SPEED (LOW_SPEED+HIGH_SPEED)/2

// INFRARED SENSOR
IRrecv ir_recv(IR_PIN);
decode_results results;

int stateCurrent = 0;
int movementStatus = 0;
int servoStatus = 0;
int posServo = 0;


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
  pinMode(STBY_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  Serial.begin(9600);
  ir_recv.enableIRIn();

  
}

void loop() {

    if (ir_recv.decode(&results)) {
      Serial.println("PACKET:");
      Serial.println(HEX, results.value);
      if (results.value == 0xE0E0906F) {
        Serial.println("There's someone!");
        move_straight(0,HIGH); //stop the robot
      }
      else{
        avoidObstacle();
      }
    }
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
  analogWrite(PWMB_PIN, right_speed);
  //set speed motor 2
  analogWrite(PWMA_PIN, left_speed);
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
        turn(0,LOW_SPEED,true); //turn a bit right
    }
    else if(obstacle1()==false && obstacle2()==true){
        turn(LOW_SPEED,0,true); //turn a bit left 
    }
    else{
        //reverse,da vedere
    }
}
