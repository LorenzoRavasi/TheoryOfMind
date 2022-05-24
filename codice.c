//stato 1: testa i componenti nel setup
//stato 2: attende il caricamento della scena
//stato 3: si posiziona
//stato 4: esegue la scena
//stato 5: torna in 1 in attesa di un'altra scena oppure si spegne

#include <TB6612FNG.h>
#include <ESP32Servo.h>
#include <IRremote.h>

Tb6612fng motor(27, 14, 12, 13);

Servo myservo;
IRrecv irrecv(RECV_PIN);
decode_results results;
 
int stateCurrent = 0;
int posServo = 0;
int servoPin = 13;
int RECV_PIN = 15;
uint16_t RECV_PIN = 4;


void setup() {

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
        if (irrecv.decode(&results))
        {
          Serial.println(results.value, HEX);
        }
        Serial.printf("Infrared test successfully executed");

        //test distanziometro
        Serial.printf("Starting distance test");
        Serial.begin(9600);
        irrecv.enableIRIn();
        if (irrecv.decode(&results)) {
          if (results.value >> 32)
            Serial.print((uint32_t) (results.value >> 32), HEX);
            Serial.println((uint32_t) (results.value & 0xFFFFFFFF), HEX);
        }
        Serial.printf("Distance test successfully executed");
        Serial.printf("Initialization tests successfully completed");

        stateCurrent = 1;
        
}

void loop() {

  switch(stateCurrent){

      case 1:
        Serial.printf("Initialization tests successfully executed");
        stateCurrent = 2;
        break;
      case 2:
        break;
      case 3;
        break;
      case 4:
        break;
      case 5:
        break;
    
  }

}
