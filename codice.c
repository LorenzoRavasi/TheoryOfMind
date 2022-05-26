//stato 1: testa i componenti nel setup
//stato 2: attende il caricamento della scena
//stato 3: si posiziona
//stato 4: esegue la scena
//stato 5: torna in 1 in attesa di un'altra scena oppure si spegne

//motore sinistro 27 12 esp32
//motore destro 14 13 esp32

#include <TB6612FNG.h>
#include <ESP32Servo.h>
#include <IRremote.h>

Tb6612fng motor(27, 14, 12, 13);

Servo myservo;
IRrecv irrecv(RECV_PIN);
decode_results results1;
decode_results results2;
decode_results results3;
 
int stateCurrent = 0;
int movementStatus = 0;
int servoStatus = 0;
int sceneSelected;
int posServo = 0;
int servoPin = 13;
int RECV_PIN = 15;
float speed = 0.7;
uint16_t RECV_PIN = 4;

void test(){
        
        //inizializzazione
        Serial.printf("Welcome, this is Robot 1");
        Serial.printf("Starting initialization tests");
        
        //test componenti

        try{
        //test motori
        Serial.printf("Starting motors test");
        motor.drive(0.5, 500);
        motor.drive(-0.5, 500);
        Serial.printf("Motors test successfully executed");
        
        //test servomotore
        Serial.printf("Starting Servomotor test");
        ESP32PWM::allocateTimer(0);
        ESP32PWM::allocateTimer(1);
        ESP32PWM::allocateTimer(2);
        ESP32PWM::allocateTimer(3);
        myservo.setPeriodHertz(50);
        myservo.attach(servoPin, 500, 2400);
        for (posServo = 0; posServo <= 180; posServo += 1) {
          myservo.write(posServo);    
          delay(15);             
        }
        for (posServo = 180; posServo >= 0; posServo -= 1) {
          myservo.write(posServo);  
          delay(15);         
        }
        Serial.printf("Servomotor test successfully executed");

        //test infrarossi
        Serial.printf("Starting Infrared test");
        Serial.begin(9600);
        irrecv.enableIRIn();
        if (irrecv.decode(&results1))
        {
          Serial.println(results1.value, HEX);
        }
        Serial.printf("Infrared test successfully executed");

        //test distanziometro
        Serial.printf("Starting distance test");
        Serial.begin(9600);
        irrecv.enableIRIn();
        if (irrecv.decode(&results2)) {
          if (results2.value >> 32)
            Serial.print((uint32_t) (results2.value >> 32), HEX);
            Serial.println((uint32_t) (results2.value & 0xFFFFFFFF), HEX);
        }
        Serial.printf("Distance test successfully executed");
        Serial.printf("Initialization tests successfully completed");

        stateCurrent = 1;
}

void movement(int movementStatus){

  switch(movementStatus)
  {
    case 0:
        //movement start
        digitalwrite(27, speed);
        digitalwrite(12, speed);
        digitalwrite(14, speed/2);
        digitalwrite(13, speed/2);
        delay(250);
        movementStatus = 1;
        
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
    
    case 2:
        //rotation 90 clockwise
        speed = 1;
        digitalwrite(27, 0);
        digitalwrite(12, 0);
        digitalwrite(14, speed);
        digitalwrite(13, speed);
        delay(1000);
        speed = 0.7;
        
    case 3:
        //rotation 90 clockwise
        speed = 1;
        digitalwrite(27, speed);
        digitalwrite(12, speed);
        digitalwrite(14, 0);
        digitalwrite(13, 0);
        delay(1000);
        speed = 0.7;
  }

    case 4:
       //free rotation

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
    
    case 6:
       //reverse
        speed = -0.3;
        digitalwrite(27, speed);
        digitalwrite(12, speed);
        digitalwrite(14, speed);
        digitalwrite(13, speed);
        speed = 0.7;

    case 7:
       //stall
         speed = 0;
         digitalwrite(27, speed);
         digitalwrite(12, speed);
         digitalwrite(14, speed);
         digitalwrite(13, speed);
         speed = 0.7;

    case 8:
       //lateral adjustment with right tyre
       speed = -0.7;
       digitalwrite(27, speed);
       digitalwrite(12, speed);
       delay(250);
       movement(7);

    case 9:
       //lateral adjustment with right tyre
       speed = -0.7;
       digitalwrite(14, speed);
       digitalwrite(13, speed);
       delay(250);
       movement(7);
}

void distance(){

    irrecv.enableIRIn();
        if (irrecv.decode(&results1)) 
        {
          //if distance to other robot is more than 16cm, the speed is slighty increase to reduce the gap
          if (results2.value > 16)
          {
            speed = speed + 0.01;
            movement(1);
          }

          else if(results.value == 16)
          {
            movement(5);
            if(results.value < 16)
            {
              movement(7);
            }
          }

          else
          {
            speed = speed - 0.03;
            movement(1);
          }
        }
}

void lateralDistance(){
      
    irrecv.enableIRIn();
        if (irrecv.decode(&results2)) 
        {
          if (results2.value <= 3)
          {
            movement(7);
            while(results2.value <= 4)
            {
              movement(6);
            }
            speed = -0.7;
            movement(8);
            movement(0);
          }

          else{
            movement(2);
          }
        }

        if (irrecv.decode(&results3)) 
        {
          if (results2.value <= 3)
          {
            movement(7);
            while(results2.value <= 4)
            {
              movement(6);
            }
            speed = -0.7;
            movement(9);
            movement(0);
          }

          else{
            movement(2);
          }
        }
}

void servoMotorMovement(int servoStauts){

        ESP32PWM::allocateTimer(0);
        ESP32PWM::allocateTimer(1);
        ESP32PWM::allocateTimer(2);
        ESP32PWM::allocateTimer(3);
        myservo.setPeriodHertz(50);
        myservo.attach(servoPin, 500, 2400);

        switch(servoStatus){
          
          case 0:
          for (posServo = 0; posServo <= 180; posServo += 1) {
            myservo.write(posServo);    
            delay(15);             
          }
          servoStauts = 1;

          case 1:
          for (posServo = 180; posServo >= -180; posServo -= 1) {
            myservo.write(posServo);  
            delay(15);         
          }
          for (posServo = -180; posServo >= 180; posServo += 1) {
            myservo.write(posServo);  
            delay(15);         
          }
        }
}


void setup() {

    test();        
        
}

void loop() {

    movement(movementStatus);
    servoMotorMovement(servoStauts);
    distance();
    laterladistance();

}
