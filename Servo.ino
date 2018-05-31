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

void loop()
{
    bool feedtoggle = false;
    float current_weight; 
    int angle;
    if(current_weight < 100)
        feedtoggle = true;
    if(feedtoggle){        
        while(angle == MAX_DEGREE){

            //Check current_weight
            //current_weight = get_weight_data();
            if(current_weight < MAX_AMOUNT_OF_BOB){

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
            //current_weight = get_weight_data();
            delay(10);        
        }

        while(angle != 0)
            microServo.write(angle--);
    }
}