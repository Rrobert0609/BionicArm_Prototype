#include <ezButton.h>
#include <Servo.h>

int VRX_PIN = A0; //Joystick X
int VRY_PIN = A1; //Joystick Y
int JOY_SW = 13; //Joystick Button

int MOTOR_PIN[6];

ezButton button(JOY_SW);

Servo motor[6];

int pos[6];

unsigned short deget = 0;

short xValue = 0;
short yValue = 0;

int yValueCalibrated = 0;

int button1 = 1; //Like
int button2 = 2; //Point w/ finger

int a = 3;  //Deget 1
int b = 4;  //Deget 2
int c = 5;  //Deget 3
int d = 6;  //Deget 4
int e = 7;  //Deget 5

void setup() {
  for(int i=1;i<6;i++){
    pos[i] = 0;
    MOTOR_PIN[i] = i + 7;
    motor[i].attach(MOTOR_PIN[i]);
  }
  pinMode(a, OUTPUT);  
  pinMode(b, OUTPUT);  
  pinMode(c, OUTPUT);  
  pinMode(d, OUTPUT);  
  pinMode(e, OUTPUT);  
  Serial.begin(9600);
  button.setDebounceTime(50);
  displayDeget(0);
}

void displayDeget(int digit){
  switch(deget){
    case 0:
      digitalWrite(a,HIGH);
      digitalWrite(b,HIGH);
      digitalWrite(c,HIGH);
      digitalWrite(d,HIGH);
      digitalWrite(e,HIGH);
      break;
    case 1:
      digitalWrite(a,HIGH);
      break;
    case 2:
      digitalWrite(b,HIGH);
      break;
    case 3:
      digitalWrite(c,HIGH);
      break;
    case 4:
      digitalWrite(d,HIGH);
      break;
    case 5:
      digitalWrite(e,HIGH);
      break;
  }
}

void turnOff(){
  digitalWrite(a,LOW);
  digitalWrite(b,LOW);
  digitalWrite(c,LOW);
  digitalWrite(d,LOW);
  digitalWrite(e,LOW);
}

void loop() {
  button.loop();

  xValue=analogRead(VRX_PIN);
  yValue=analogRead(VRY_PIN);
  
  yValueCalibrated = (yValue - 512) / 34;

  if(digitalRead(button1) == 1){ //Like
    for(int i=1;i<6;i++){
      pos[i] = 110;
      motor[i].write(110);
    }
    pos[5] = 0;
    motor[5].write(0);
  }

  if(digitalRead(button2) == 1){ //Point
    for(int i=1;i<6;i++){
      pos[i] = 110;
      motor[i].write(110);
    }
    //pos[4] = 0;
    //motor[4].write(0);
  }

  if(xValue > 1000){
    if(deget == 5) deget = 0;
    else deget++;
    turnOff();
    displayDeget(deget);
    delay(300);
    }
  if(xValue < 20){
    if(deget == 0) deget = 5;
    else deget--;
    turnOff();
    displayDeget(deget);
    delay(300);
  }
  if(deget != 0){
    if(yValueCalibrated > 0){
      if(pos[deget] > 98) pos[deget] = 110;
      else pos[deget]+=yValueCalibrated;
      motor[deget].write(pos[deget]);
      delay(10);
    }

    if(yValueCalibrated < 0){
      if(pos[deget] < 14) pos[deget] = 0;
      else pos[deget]+=yValueCalibrated;
      motor[deget].write(pos[deget]);
      delay(10);
    }
  }
  else{
    if(yValueCalibrated > 0){
      for(int i=1;i<6;i++){
        if(pos[i] > 98) pos[i] = 110;
        else pos[i]+=yValueCalibrated;
        motor[i].write(pos[i]);
        delay(5);
      }
      delay(10);
    }
    if(yValueCalibrated < 0){
      for(int i=1;i<6;i++){
        if(pos[i] < 14) pos[i] = 0;
        else pos[i]+=yValueCalibrated;
        motor[i].write(pos[i]);
        delay(5);
      }
      delay(10);
    }
  }
  if(digitalRead(JOY_SW) == 0){
      for(int i=1;i<6;i++){
        pos[i] = 0;
        motor[i].write(0);
      }
    }
  Serial.print("x= ");
  Serial.print(xValue);
  Serial.print(", y= ");
  Serial.print(yValue);
  Serial.print(", deget= ");
  Serial.print(deget);
  Serial.print(", pos= ");
  Serial.print(pos[deget]);
  Serial.print(", joy_sw= ");
  Serial.print(digitalRead(JOY_SW));
  Serial.print(", yCalibrated= ");
  Serial.print(yValueCalibrated);
  Serial.print(", Button1= ");
  Serial.print(digitalRead(button1));
  Serial.print(", Button2= ");
  Serial.print(digitalRead(button2));
  Serial.print(".\n");
}
