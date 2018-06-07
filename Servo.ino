#define MAX_DEGREE 180
#define SERVO_PIN 3
#define MAX_AMOUNT_OF_BOB 600

#include <Servo.h>   
 
Servo microServo;    
const int servoPin = SERVO_PIN;    
 
void setup()
{
    Serial.begin(9600);
    //Serial.println("hello arduino!");
    microServo.attach(servoPin);   
}
void feed(){
    int angle;
    while(angle < MAX_DEGREE){

        //Check current_weight
        current_weight = scale.getGram();
        if(current_weight > MAX_AMOUNT_OF_BOB){

            //return slowly
            while(angle != 0){
                microServo.write(angle--);
                delay(10);
            }

            //return directly    
            angle = 0;
            microServo.write(angle);

            break;
        }
        microServo.write(angle++);          
        delay(10);                    
    }

    while((current_weight < MAX_AMOUNT_OF_BOB) && (angle != 0)){
        current_weight = scale.getGram();
        delay(10);        
    }

    while(angle != 0)
        microServo.write(angle--);
}
