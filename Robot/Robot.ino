#include <NewPing.h> //Ultrasonic sensor lib
#include <Servo.h> //Servo lib

#define trigPin  3  //sensor triggerpin
#define echoPin  2  //sensor echopin


//motor 1 = rechtsvoor
//motor 2 = linksvoor
//motor 3 = rechtsachter
//motor 4 = linksachter
const int Motor1Pin1 = 4;
const int Motor1Pin2 = 5;
const int Motor2Pin1 = 6;
const int Motor2Pin2 = 7;

const int Motor3Pin1 = 10;
const int Motor3Pin2 = 11;
const int Motor4Pin1 = 12;
const int Motor4Pin2 = 13;
Servo stuur;  //servo object
int maxAfstand = 200; //max te meten afstand
NewPing ogen(trigPin, echoPin, maxAfstand); //sensor object


int stuursnelheid = 3;
float distance;
float fDistance; //afstand voorkant
float lDistance; //afstand links
float rDistance; //afstand rechts

int pos = 90;

int modus;

/////////SETUP/////////
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
  
  stuur.attach(9);
  stuur.write(90);
  
  Serial.begin(9600);
}
////LOOP/////////
void loop(){
  while(Serial.available()) {
    int readSerial = Serial.read() - '0';
    if(readSerial == 1){
      modus = 1;
    } else if(readSerial == 2){
      modus = 2;
    } else if (readSerial == 3){
      modus = 3;
    } else if(readSerial == 0){
      modus = 0;
    }
    Serial.print("Modus ");
    Serial.println(modus);
    
  }
    ////MODUS 1/////
    if(modus == 1){
      Serial.println("Zelfdenk modus!");
      stopDriving();
      driveForward();
      kijken();
      fDistance = distance;  
      Serial.println(distance);
      //als hij 20cm van de muur af is
      if(fDistance < 20){
        Serial.println("Ik ga om me heen kijken ");
        //stop met rijden voor 2 sec
        stopDriving();
        delay(500);
        //draai naar links voor 1 sec
        turnLeft();
        delay(1000);
        stopDriving();
        //doe meting 1
        kijken();
        lDistance = distance;
        Serial.print("lDistance = ");
        Serial.println(lDistance);
        
        //draai nnar recht voor 2 sec
        turnRight();
        delay(2000);
        stopDriving();
        //doe meting 2
        kijken();
        rDistance = distance; 
        Serial.print("rDistance = ");
        Serial.println(rDistance);   
        
        delay(500);
        //als meting 1 en 2 kleiner zijn dan 30cm draai 180 graden linksom
        if(lDistance <= 30 && rDistance <= 30){
          Serial.println("Ik ga omdraaien!");
          turnLeft();
          delay(3000);
        }
        //als meting 1 kleiner is dan meting 2 ga rechtdoor
        if(lDistance < rDistance){
          Serial.println("Ik ga rechts!");
          driveForward();
        //als meting 2 kleiner is dan meting 1 draai weer terug naar links voor 2 sec en ga rechtdoor
        } else if(rDistance < lDistance){
          Serial.println("Ik ga links!");
          turnLeft();
          delay(2000);
          driveForward();
        }
        //als de afstand tussen 20 en 40cm in zit
      } else {
        Serial.println("Ik zie niks!");
        driveForward();
      } 
      ///////MODUS 2///////
    } else if(modus == 2){
      //hier komt de code voor bestuurbaar maken via de Serial. Bluetooth of met kabel.
      stopDriving();
      
      while(Serial.available() > 0){
        
        int readDirection = Serial.read() - '0';
        //char readDirectionChar = char(readDirection);
        int realDirection = readDirection + 48;
        //Serial.print("realDirection = ");
        //Serial.println(realDirection);
        
        
        if(realDirection == 'W'){
          Serial.println("RC: Vooruit");
          driveForward();
          delay(100);
          while (Serial.available() == 0);
          
        }
        if(realDirection == 'S'){
          Serial.println("RC: Achteruit");
          driveBackward();
          delay(100); 
          while (Serial.available() == 0);
          
        }
        
        if(realDirection == 'A'){
          Serial.println("RC: Turn links"); 
          turnLeft();
          delay(100);
          
          while (Serial.available() == 0);
        }
        if(realDirection == 'D'){
          Serial.println("RC: Turn Rechts");
          turnRight();
          delay(100); 
          while (Serial.available() == 0);
        }
        if(realDirection == 'R'){
          Serial.println("RC: Stuur rechtdoor");
          stuurRechtdoor();
          while (Serial.available() == 0);
        }
        if(realDirection == 'F'){
          Serial.println("RC: Stop");
          stopDriving();
          delay(100);
          while (Serial.available() == 0);
        }
        if (realDirection >= 140 && realDirection <= 240){
          realDirection = realDirection - 100;
          Serial.print("RC stuur pos: ");
          Serial.println(realDirection);
          stuur.write(realDirection);          
          while (Serial.available() == 0);
        }
        
        if(realDirection == '0') {
          modus = 0;
        }
      }
      ///////MODUS 3///////
    } else if(modus == 3){
      // hier de code voor IR remote besturing
      stopDriving();
      ///////MODUS 0///////
    } else if(modus == 0){
      //stop alles
      stopDriving();
    }  
  
}
////////////////EINDE LOOP/////////////
///////////////FUNCTIES////////////////
void driveForward(){
  driveFrontWeelsForward();
  driveRearWeelsForward();
}
void driveBackward(){
  driveFrontWeelsBackward();
  driveRearWeelsBackward();
}

void stopDriving(){
  stopFrontWeels();
  stopRearWeels();
  
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
  digitalWrite(Motor2Pin1, HIGH);
  digitalWrite(Motor2Pin2, LOW);
  digitalWrite(Motor4Pin1, HIGH);
  digitalWrite(Motor4Pin2, LOW);
}

void driveRightWeelsForward(){
  digitalWrite(Motor1Pin2, LOW);
  digitalWrite(Motor1Pin1, HIGH);
  digitalWrite(Motor3Pin2, LOW);
  digitalWrite(Motor3Pin1, HIGH);
}

void driveLeftWeelsBackward(){
  digitalWrite(Motor2Pin2, HIGH);
  digitalWrite(Motor2Pin1, LOW);
  digitalWrite(Motor4Pin2, HIGH);
  digitalWrite(Motor4Pin1, LOW);
}

void driveRightWeelsBackward(){
  digitalWrite(Motor1Pin1, LOW);
  digitalWrite(Motor1Pin2, HIGH);
  digitalWrite(Motor3Pin1, LOW);
  digitalWrite(Motor3Pin2, HIGH);
}


void turnLeft(){
  stuurRechtdoor();
  digitalWrite(Motor1Pin2, LOW);
  digitalWrite(Motor1Pin1, HIGH);
  digitalWrite(Motor3Pin2, LOW);
  digitalWrite(Motor3Pin1, HIGH);
  digitalWrite(Motor2Pin2, HIGH);
  digitalWrite(Motor2Pin1, LOW);
  digitalWrite(Motor4Pin2, HIGH);
  digitalWrite(Motor4Pin1, LOW);
}
void turnRight(){
  stuurRechtdoor();
  
  digitalWrite(Motor1Pin1, LOW);
  digitalWrite(Motor1Pin2, HIGH);
  digitalWrite(Motor3Pin1, LOW);
  digitalWrite(Motor3Pin2, HIGH);
  digitalWrite(Motor2Pin1, HIGH);
  digitalWrite(Motor2Pin2, LOW);
  digitalWrite(Motor4Pin1, HIGH);
  digitalWrite(Motor4Pin2, LOW);
}

void stuurLinks(){
  int _pos = getPos();
  
  for(pos = _pos; pos <= 140; pos = pos + stuursnelheid){
    stuur.write(pos);
    setPos(pos);
    delay(15);
  }
}

void stuurRechts(){
  int _pos = getPos();
  for(pos = _pos; pos >= 40; pos = pos - stuursnelheid){
    stuur.write(pos);
    setPos(pos);
    delay(15);
  }
}
void stuurRechtdoor(){
  int _pos = getPos();
  if(_pos < 90){
    for(pos = _pos; pos < 89; pos = pos + stuursnelheid){
      stuur.write(pos);
      setPos(pos);
      delay(15);
    }
  } else if (_pos > 90){
    for(pos = _pos; pos > 91; pos = pos - stuursnelheid){
      stuur.write(pos);
      setPos(pos);
      delay(15);
    }
  }
}

void kijken(){
  
  int duration;
  duration = ogen.ping();
  distance = duration / US_ROUNDTRIP_CM;
  if(distance == 0){
    distance = 200;
  }
  
  delay(100);
}
void setPos(int _pos){
  pos = _pos;
}

int getPos(){
  return pos;
}

