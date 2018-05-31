#include <Servo.h>    //서보모터 라이브러리 include
 
Servo microServo;    //서보모터 객체 선언
const int servoPin = 3;    //서보모터 제어핀 할당
 
void setup()
{
    Serial.begin(115200);
    Serial.println("hello arduino!");
    microServo.attach(servoPin);    //서보모터 초기화
}
 
void loop()
{
    int angle;    //각도 변수 선언
 
    
    //0 -> 170도로 서보모터 움직임
    for (angle = 0; angle < 170; angle++)
    {
        microServo.write(angle);    //angle(각도)값으로 서보모터 제어
        Serial.println(angle);        //각도 값 출력
        delay(10);                    //delay로 각도의 변화 속도를 조절
    }
    //170 -> 0도로 서보모터 움직임
    for (angle = 170; angle > 0; angle--)
    {
        microServo.write(angle);    //angle(각도)값으로 서보모터 제어
        Serial.println(angle);        //각도 값 출력
        delay(10);                    //delay로 각도의 변화 속도를 조절
    }
}
