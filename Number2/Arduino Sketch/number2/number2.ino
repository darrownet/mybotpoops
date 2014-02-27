#include <Servo.h> 
#include <Bridge.h>
#include <SpacebrewYun.h>

SpacebrewYun sb = SpacebrewYun("Number 2", "a pooping robot");

int right_eye = 7;
int left_eye = 6;

int motorA_dir = 12;
int motorA_mov = 3;
int motorA_brk = 9;
int motorA_spd = 0;

int motorB_dir = 13;
int motorB_mov = 11;
int motorB_brk = 8;
int motorB_spd = 0;

Servo servo_bottom;
Servo servo_top;

boolean pooping;

String eyes_behave;

void eyesOn () {
  digitalWrite(right_eye, HIGH);
  digitalWrite(left_eye, HIGH);
}
void eyesOff () {
  digitalWrite(right_eye, LOW);
  digitalWrite(left_eye, LOW);
}
void eyesFlash (int del) {
  digitalWrite(right_eye, HIGH);
  digitalWrite(left_eye, HIGH);
  delay(del);
  digitalWrite(right_eye, LOW);
  digitalWrite(left_eye, LOW);
  delay(del);
}
void eyesFlick (int del, int len) {
  int pos= 0;
  for(pos = 0; pos <= len; pos += 1){
    digitalWrite(right_eye, HIGH);
    digitalWrite(left_eye, LOW);
    delay(del);
    digitalWrite(right_eye, LOW);
    digitalWrite(left_eye, HIGH);
    delay(del);
  }
}


void makeThePoop() {
  pooping = true;
  int pos = 0;
  eyes_behave = "flash";
  delay(1000);
  sb.send("pooping-status", "raising the bottom");
  for(pos = 0; pos <= 180; pos += 1){
    servo_bottom.write(map(pos, 0, 180, 10, 160));
    delay(15);
  }
  pos = 0;
  sb.send("pooping-status", "raising the top");
  for(pos = 0; pos <= 180; pos += 1){
    servo_top.write(map(pos, 0, 180, 10, 160));
    delay(15);
  }
  pos = 0;
  holdIt();
}
void holdIt() {
  eyesFlick(125, 10);
  digitalWrite(right_eye, HIGH);
  digitalWrite(left_eye, HIGH);
  finshUp();
}
void finshUp() {
  int pos = 180;
  eyes_behave = "flash";
  delay(1000);
  sb.send("pooping-status", "lowering the top");
  for(pos = 180; pos >= 0; pos -= 1){
    servo_top.write(map(pos, 0, 180, 10, 160));
    delay(15);
  }
  pos = 180;
  sb.send("pooping-status", "lowering the bottom");
  for(pos = 180; pos >= 0; pos -= 1){
    servo_bottom.write(map(pos, 0, 180, 10, 160));
    delay(15);
  }
  resetDigestiveSystem();
}
void resetDigestiveSystem() {
  servo_bottom.write(map(0, 0, 180, 10, 160));
  servo_top.write(map(0, 0, 180, 10, 160));
  pooping = false;
  sb.send("pooping-status", "done pooping");
}



void handleBoolean (String route, boolean value) {
  // Handle Pooping Command
  if (route == "poop"){
    if (pooping) {
      return;
    } else {
      if (value) {
        makeThePoop();
      } else {
        resetDigestiveSystem();
        pooping = false;
      }
    }
  }
  
}

void handleString (String route, String value) {
  if (route == "direction"){
    if(value == "forward") {
      digitalWrite(motorA_brk, LOW);
      digitalWrite(motorB_brk, LOW);
      digitalWrite(motorA_dir, HIGH);
      digitalWrite(motorB_dir, LOW);
    } else if (value == "backward") {
      digitalWrite(motorA_brk, LOW);
      digitalWrite(motorB_brk, LOW);
      digitalWrite(motorA_dir, LOW);
      digitalWrite(motorB_dir, HIGH);
    } else {
      digitalWrite(motorA_dir, LOW);
      digitalWrite(motorB_dir, LOW);
      digitalWrite(motorA_brk, HIGH);
      digitalWrite(motorB_brk, HIGH);
    }
  }
  //
  String val_a;
  String val_b;
  int pipeIndex;
  if(route == "move motors") {
    pipeIndex = value.indexOf('|');
    val_a = value.substring(0, pipeIndex);
    val_b = value.substring(pipeIndex+1);
    motorA_spd = map(val_a.toInt(), 0, 250, 0, 255);
    motorB_spd = map(val_b.toInt(), 0, 250, 0, 255);
  }
}

void handleRange (String route, int value) {
  if (route == "motor a"){
    motorA_spd = value;
  }
  if (route == "motor b"){
    motorB_spd = value;
  }
}





void setup() { 
  
  Bridge.begin();
  
  pinMode(right_eye, OUTPUT);
  pinMode(left_eye, OUTPUT);
  
  pinMode(motorA_dir, OUTPUT);
  pinMode(motorA_brk, OUTPUT);
  pinMode(motorB_dir, OUTPUT);
  pinMode(motorB_brk, OUTPUT);
  
  digitalWrite(right_eye, LOW);
  digitalWrite(left_eye, LOW);
  
  digitalWrite(motorA_dir, HIGH);
  digitalWrite(motorA_brk, LOW);
  analogWrite(motorA_mov, motorA_spd);
  digitalWrite(motorB_dir, LOW);
  digitalWrite(motorB_brk, LOW);
  analogWrite(motorB_mov, motorB_spd);
  
  servo_bottom.attach(4);
  servo_top.attach(2);
  resetDigestiveSystem();
  
  pooping = false;
  
  sb.addSubscribe("poop", "boolean");
  sb.addSubscribe("move motors", "string");
  sb.addSubscribe("direction", "string");
  sb.addPublish("pooping-status", "string");
  sb.onBooleanMessage(handleBoolean);
  sb.onRangeMessage(handleRange); 
  sb.onStringMessage(handleString);  
  sb.connect("nowir.es");
  
} 

void loop() {
  sb.monitor();
  
  if (pooping == false) {
    eyesOn();
  }
  
  if ( !sb.connected() ) {
    eyesFlash(175);
  }
  
//  if (pooping == true) {
//    if (eyes_behave == "flick") {
//      eyesFlick(100);
//    }
//    if (eyes_behave == "flash") {
//      eyesFlash(250);
//    }
//  }
  
  analogWrite(motorA_mov, motorA_spd);
  analogWrite(motorB_mov, motorB_spd);
} 

