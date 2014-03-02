#include <Bridge.h>
#include <SpacebrewYun.h>

SpacebrewYun sb = SpacebrewYun("Demo YÚN", "SXSW Demonstration Arduino YÚN");

int led = 7;
int status_led = 13;

void setup() { 
  
  Bridge.begin();
  
  pinMode(led, OUTPUT);
  pinMode(status_led, OUTPUT);
  digitalWrite(led, LOW);
  
  sb.addSubscribe("led", "boolean");
  sb.addPublish("message", "string");
  sb.onBooleanMessage(handleBoolean);  
  sb.connect("nowir.es");
  
} 

void loop() { 
  sb.monitor();
  if ( !sb.connected() ) {
    digitalWrite(status_led, HIGH);
  }
} 

void handleBoolean (String route, boolean value) {
 
 String msg_pre = "YÚN LED (pin ";
 String msg_post_on = ") is turned ON";
 String msg_post_off = ") is turned OFF";
  
  if (value == true){
    digitalWrite(led, HIGH);
    sb.send("message", msg_pre + led + msg_post_on);
  } else {
    digitalWrite(led, LOW);
    sb.send("message", msg_pre + led + msg_post_off);
  }
  
}

