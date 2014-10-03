#include <Servo.h>

#define echoPin  2
#define trigPin  3

const int Motor3Pin1 = 4;
const int Motor3Pin2 = 5;
const int Motor4Pin1 = 6;
const int Motor4Pin2 = 7;

const int Motor1Pin1 = 8;
const int Motor1Pin2 = 9;
const int Motor2Pin1 = 10;
const int Motor2Pin2 = 11;
Servo stuur;
const int ledPin = 13;

int stuursnelheid = 1;

int pos = 90;

void setup(){
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(Motor1Pin1, OUTPUT);   
  pinMode(Motor1Pin2, OUTPUT);   
  pinMode(Motor2Pin1, OUTPUT);   
  pinMode(Motor2Pin2, OUTPUT); 
  pinMode(Motor3Pin1, OUTPUT);   
  pinMode(Motor3Pin2, OUTPUT);   
  pinMode(Motor4Pin1, OUTPUT);   
  pinMode(Motor4Pin2, OUTPUT); 
  stuur.attach(12);
  pinMode(ledPin, OUTPUT);
}

void loop(){

  //als hij 20cm van de muur af is
  if(getDistance() <= '20'){
    //stop met rijden voor 2 sec
    stopDriving('2000');
    //draai naar links voor 1 sec
    turnLeft('1000');
    //doe meting 1
    float meting1 = getDistance();
    //draai nnar recht voor 2 sec
    turnRight('2000');
    
    //doe meting 2
    float meting2 = getDistance();
    //als meting 1 en 2 kleiner zijn dan 30cm draai 180 graden linksom
    if(meting1 <= 30 && meting2 <= 30){
      turnLeft('5000');
    }
    //als meting 1 kleiner is dan meting 2 ga rechtdoor
    if(meting1 < meting2){
      driveForward();
    //als meting 2 kleiner is dan meting 1 draai weer terug naar links voor 2 sec en ga rechtdoor
    } else if(meting2 < meting1){
      turnLeft('2000');
      driveForward();
    }
    //als de afstand tussen 20 en 40cm in zit
  } else if(getDistance() > 20 && getDistance() < 40){
    float munt = random(0,1);
    if(munt < 1){
      stuurLinks();
    } else if(munt >= 1){
      stuurRechts();
    }
  }

  /*stuur.write(90);
  delay(5000 );
  for(pos = 90; pos < 125; pos += 1){
    stuur.write(pos);
    delay(15);
  }
  for(pos = 125; pos >= 55; pos -= 1){
    stuur.write(pos);
    delay(15);
  }
  */
}
void driveForward(){
  driveFrontWeelsForward();
  driveRearWeelsForward();
}
void driveBackward(){
  driveFrontWeelsBackward();
  driveRearWeelsBackward();
}

void stopDriving(float _delay){
  stopFrontWeels();
  stopRearWeels();
  delay(_delay);
}


void driveFrontWeelsForward(){
  digitalWrite(Motor1Pin2, LOW);
  digitalWrite(Motor1Pin1, HIGH);
  digitalWrite(Motor2Pin2, LOW);
  digitalWrite(Motor2Pin1, HIGH);
}

void driveFrontWeelsBackward(){
  digitalWrite(Motor1Pin1, LOW);
  digitalWrite(Motor1Pin2, HIGH);
  digitalWrite(Motor2Pin1, LOW);
  digitalWrite(Motor2Pin2, HIGH);
}

void driveFrontRightForward(){
  digitalWrite(Motor1Pin2, LOW);
  digitalWrite(Motor1Pin1, HIGH);
  digitalWrite(Motor2Pin2, LOW);
  digitalWrite(Motor2Pin1, LOW);
}

void driveFrontRightBackward(){
  digitalWrite(Motor1Pin2, HIGH);
  digitalWrite(Motor1Pin1, LOW);
  digitalWrite(Motor2Pin2, LOW);
  digitalWrite(Motor2Pin1, LOW);
}

void driveFrontLeftForward(){
  digitalWrite(Motor1Pin2, LOW);
  digitalWrite(Motor1Pin1, LOW);
  digitalWrite(Motor2Pin2, LOW);
  digitalWrite(Motor2Pin1, HIGH);
}

void driveFrontLeftBackward(){
  digitalWrite(Motor1Pin2, LOW);
  digitalWrite(Motor1Pin1, LOW);
  digitalWrite(Motor2Pin2, HIGH);
  digitalWrite(Motor2Pin1, LOW);
}
void stopFrontWeels(){
  digitalWrite(Motor1Pin2, LOW);
  digitalWrite(Motor1Pin1, LOW);
  digitalWrite(Motor2Pin1, LOW);
  digitalWrite(Motor2Pin2, LOW);
}

void driveRearWeelsForward(){
  digitalWrite(Motor3Pin2, LOW);
  digitalWrite(Motor3Pin1, HIGH);
  digitalWrite(Motor4Pin2, LOW);
  digitalWrite(Motor4Pin1, HIGH);
}

void driveRearWeelsBackward(){
  digitalWrite(Motor3Pin1, LOW);
  digitalWrite(Motor3Pin2, HIGH);
  digitalWrite(Motor4Pin1, LOW);
  digitalWrite(Motor4Pin2, HIGH);
}

void driveRearRightForward(){
  digitalWrite(Motor3Pin2, LOW);
  digitalWrite(Motor3Pin1, HIGH);
  digitalWrite(Motor4Pin2, LOW);
  digitalWrite(Motor4Pin1, LOW);
}

void driveRearRightBackward(){
  digitalWrite(Motor3Pin2, HIGH);
  digitalWrite(Motor3Pin1, LOW);
  digitalWrite(Motor4Pin2, LOW);
  digitalWrite(Motor4Pin1, LOW);
}

void driveRearLeftForward(){
  digitalWrite(Motor3Pin2, LOW);
  digitalWrite(Motor3Pin1, LOW);
  digitalWrite(Motor4Pin2, LOW);
  digitalWrite(Motor4Pin1, HIGH);
}

void driveRearLeftBackward(){
  digitalWrite(Motor3Pin2, LOW);
  digitalWrite(Motor3Pin1, LOW);
  digitalWrite(Motor4Pin2, HIGH);
  digitalWrite(Motor4Pin1, LOW);
}
void stopRearWeels(){
  digitalWrite(Motor3Pin2, LOW);
  digitalWrite(Motor3Pin1, LOW);
  digitalWrite(Motor4Pin1, LOW);
  digitalWrite(Motor4Pin2, LOW);
}

void driveLeftWeelsForward(){
  driveFrontLeftForward();
  driveRearLeftForward();
}

void driveRightWeelsForward(){
  driveFrontRightForward();
  driveRearRightForward();
}

void driveLeftWeelsBackward(){
  driveFrontLeftBackward();
  driveRearLeftBackward();
}

void driveRightWeelsBackward(){
  driveFrontRightBackward();
  driveRearRightBackward();
}


void turnLeft(float _delay){
  stuurRechtdoor();
  delay(500);
  driveRightWeelsForward();
  driveLeftWeelsBackward();
  delay(_delay);
}
void turnRight(float _delay){
  stuurRechtdoor();
  delay(500);
  driveLeftWeelsForward();
  driveRightWeelsBackward();
  delay(_delay);
}

void stuurRechts(){
  
  for(pos = 90; pos <= 125; pos + stuursnelheid){
    stuur.write(pos);
    delay(15);
  }
}

void stuurLinks(){
  
  for(pos = 90; pos <= 55; pos - stuursnelheid){
    stuur.write(pos);
    delay(15);
  }
}
void stuurRechtdoor(){
  int _pos = getPos();
  if(_pos < 90){
    for(pos = _pos; pos < 90; pos + stuursnelheid){
      stuur.write(pos);
      delay(15);
    }
  } else if (_pos > 90){
    for(pos = _pos; pos > 90; pos - stuursnelheid){
      stuur.write(pos);
      delay(15);
    }
  }
}

float getDistance(){
  float distance;
  int duration;
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(1000);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;
  return distance;
  delay(250);
}

int getPos(){
  return pos;
}

