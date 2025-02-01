#include<NewPing.h>          
#include<Servo.h>            
#include<AFMotor.h>          
#define SWITCH 9
#define SWITCH1 2
#define LEFT_L A5
#define RIGHT_L A4 
#define RIGHT A2              
#define LEFT A3             
#define TRIGGER_PIN A1  
#define ECHO_PIN A0          
#define MAX_DISTANCE 200  
#define MAX_SPEED 150 
#define MAX_SPEED_OFFSET 20

boolean goesForward=false;
unsigned int distance1 = 100;
unsigned int speedSet = 0;
unsigned int distance = 0;   
unsigned int Right_Value = 0; 
unsigned int Left_Value = 0;
unsigned int pos=0;

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
AF_DCMotor Motor1(1,MOTOR12_1KHZ);
AF_DCMotor Motor2(2,MOTOR12_1KHZ);
AF_DCMotor Motor3(3,MOTOR34_1KHZ);
AF_DCMotor Motor4(4,MOTOR34_1KHZ);
Servo myservo; 

void setup() 
{
  pinMode(SWITCH, INPUT);
  pinMode(SWITCH1, INPUT);
  if(digitalRead(SWITCH1)== HIGH)
  {
    myservo.attach(10);  
    myservo.write(90); 
    delay(2000);
    distance1 = readPing();
    delay(100);
    distance1 = readPing();
    delay(100);
    distance1 = readPing();
    delay(100);
    distance1 = readPing();
    delay(100);
  }
  else
  {
    pinMode(RIGHT_L, INPUT);
    pinMode(LEFT_L, INPUT);
    pinMode(RIGHT, INPUT); 
    pinMode(LEFT, INPUT); 
    Serial.begin(9600);
    myservo.attach(10); 
    {
      for(pos = 90; pos <= 180; pos += 1)
      {  
        myservo.write(pos);    
        delay(15);    
      }
      for(pos = 180; pos >= 0; pos-= 1) 
      {     
        myservo.write(pos);                 
        delay(15);    
      }
      for(pos = 0; pos<=90; pos += 1) 
      {     
        myservo.write(pos);                
        delay(15);         
      }
    }
  }
}
void loop()
{
  if(digitalRead(SWITCH1)== HIGH && digitalRead(SWITCH)== HIGH)
  {
    disable();
  }
  else if(digitalRead(SWITCH)== HIGH)
  {
    Serial.println(digitalRead(LEFT_L));
    Serial.println(digitalRead(RIGHT_L));  
    if(digitalRead(LEFT_L)==0 && digitalRead(RIGHT_L)==0)
    {
      forward();
    }
    else if(digitalRead(LEFT_L)==0 && !digitalRead(RIGHT_L)==0)
    {
      left();
    }
    else if(!digitalRead(LEFT_L)==0 && digitalRead(RIGHT_L)==0)
    {
      right();
    }
    else if(!digitalRead(LEFT_L)==0 && !digitalRead(RIGHT_L)==0)
    {
      stop();  
    }
  }
  else if(digitalRead(SWITCH1)== HIGH)
  {
  int distanceR = 0;
  int distanceL = 0;
  delay(40);
  
  if(distance1<=25)
  {
    moveStop();
    delay(100);
    moveBackward();
    delay(300);
    moveStop();
    delay(200);
    distanceR = lookRight();
    delay(200);
    distanceL = lookLeft();
    delay(200);  
    if(distanceR>=distanceL)
    {
      turnRight();
      moveStop();
    }   
    else  
    {
      turnLeft();
      moveStop();
    }
  }   
  else
  {
    moveForward();
  }
  distance1 = readPing();
}
  else if(digitalRead(SWITCH1)== LOW && digitalRead(SWITCH)== LOW)
  {
    delay(50);                              
    distance = sonar.ping_cm();  
    Serial.print("distance");                  
    Serial.println(distance);                  
    Right_Value = digitalRead(RIGHT);     
    Left_Value = digitalRead(LEFT);  
    Serial.print("RIGHT");                     
    Serial.println(Right_Value);                      
    Serial.println(Left_Value);                    
    if((distance > 1) && (distance < 15))
    {            
      forward();
    }
    else if((Right_Value==0) && (Left_Value==1))
    {   
      left();
    }
    else if((Right_Value==1)&&(Left_Value==0))
    {     
      right();
      delay(150);
    }
    else if(distance > 15)
    {                        
      stop();
    }
  }
}
void forward()
{
    Motor1.run(FORWARD);
    Motor1.setSpeed(150);
    Motor2.run(FORWARD);
    Motor2.setSpeed(150);
    Motor3.run(FORWARD);
    Motor3.setSpeed(150);
    Motor4.run(FORWARD);
    Motor4.setSpeed(150);
}
void left()
{
    Motor1.run(FORWARD);
    Motor1.setSpeed(200);
    Motor2.run(FORWARD);
    Motor2.setSpeed(200);
    Motor3.run(BACKWARD);
    Motor3.setSpeed(200);
    Motor4.run(BACKWARD);
    Motor4.setSpeed(200);
}
void right()
{
    Motor1.run(BACKWARD);
    Motor1.setSpeed(200);
    Motor2.run(BACKWARD);
    Motor2.setSpeed(200);
    Motor3.run(FORWARD);
    Motor3.setSpeed(200);
    Motor4.run(FORWARD);
    Motor4.setSpeed(200);
}
void stop()
{
    Motor1.run(RELEASE);
    Motor1.setSpeed(0);
    Motor2.run(RELEASE);
    Motor2.setSpeed(0);
    Motor3.run(RELEASE);
    Motor3.setSpeed(0);
    Motor4.run(RELEASE);
    Motor4.setSpeed(0);
}
int lookRight()
{
    myservo.write(50); 
    delay(500);
    int distance1 = readPing();
    delay(100);
    myservo.write(90); 
    return distance1;
}
int lookLeft()
{
    myservo.write(170); 
    delay(500);
    int distance1 = readPing();
    delay(100);
    myservo.write(90); 
    return distance1;
    delay(100);
}
int readPing() { 
  delay(100);
  int cm = sonar.ping_cm();
  if(cm==0)
  {
    cm = 250;
  }
  return cm;
}
void moveStop() {
  Motor1.run(RELEASE); 
  Motor2.run(RELEASE);
  Motor3.run(RELEASE);
  Motor4.run(RELEASE);
  } 
void moveForward() {
 
 if(!goesForward)
  {
    goesForward=true;
    Motor1.run(FORWARD);      
    Motor2.run(FORWARD);
    Motor3.run(FORWARD); 
    Motor4.run(FORWARD);     
   for (speedSet = 0; speedSet < MAX_SPEED; speedSet +=2) 
   {
    Motor1.setSpeed(speedSet);
    Motor2.setSpeed(speedSet);
    Motor3.setSpeed(speedSet);
    Motor4.setSpeed(speedSet);
    delay(5);
   }
  }
}
void moveBackward() {
    goesForward=false;
    Motor1.run(BACKWARD);      
    Motor2.run(BACKWARD);
    Motor3.run(BACKWARD);
    Motor4.run(BACKWARD);  
  for (speedSet = 0; speedSet < MAX_SPEED; speedSet +=2) 
  {
    Motor1.setSpeed(speedSet);
    Motor2.setSpeed(speedSet);
    Motor3.setSpeed(speedSet);
    Motor4.setSpeed(speedSet);
    delay(5);
  }
}  
void turnRight() {
  Motor1.run(FORWARD);
  Motor2.run(FORWARD);
  Motor3.run(BACKWARD);
  Motor4.run(BACKWARD);     
  delay(500);
  Motor1.run(FORWARD);      
  Motor2.run(FORWARD);
  Motor3.run(FORWARD);
  Motor4.run(FORWARD);      
} 
void turnLeft() {
  Motor1.run(BACKWARD);     
  Motor2.run(BACKWARD);  
  Motor3.run(FORWARD);
  Motor4.run(FORWARD);   
  delay(500);
  Motor1.run(FORWARD);     
  Motor2.run(FORWARD);
  Motor3.run(FORWARD);
  Motor4.run(FORWARD);
}
void disable()
{
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);
}