#define MAX_DEGREE 180
#define SERVO_PIN 3
#define MAX_AMOUT_OF_BOB 600

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
        for (angle = 0; angle < MAX_DEGREE; angle++){
            microServo.write(angle);          
            delay(10);                    
        }

        do{
            //current_weight = get_weight_data();
            delay(10);        
        }while(current_weight < MAX_AMOUT_OF_BOB);

        for (angle = MAX_DEGREE; angle > 0; angle--){
            microServo.write(angle);      
            delay(10);                    
        }
    }
}