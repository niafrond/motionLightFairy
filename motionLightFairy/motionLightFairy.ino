int gardenPins[] = {9,10};
int deerPin[] = {11};
int motionSensorPin = 2;
int rgbPin[] = {7,8,9};
int redSat = 0;
int grennSat = 0;
int blueSat = 0;
const int maxLuminosity = 200;
volatile bool hasMotion = 0;
volatile long lastMotion = 0;
const long minMotionDuration = 5000;

void setup() {
  // put your setup code here, to run once:
  pinMode(motionSensorPin,INPUT);
  // if analog input pin 0 is unconnected, random analog
  // noise will cause the call to randomSeed() to generate
  // different seed numbers each time the sketch runs.
  // randomSeed() will then shuffle the random function.
  randomSeed(analogRead(0));
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(motionSensorPin), setMotion, CHANGE);
  hasMotion = digitalRead(motionSensorPin)!=0;
}
void setMotion(){
  hasMotion = digitalRead(motionSensorPin)!=0;
  lastMotion = millis();
  Serial.println(digitalRead(motionSensorPin));
}

void sleep(int duration){
  delay(duration);
}
void loop() {
  // put your main code here, to run repeatedly
  //test();
  
  while(motion()){
    Serial.println(digitalRead(motionSensorPin));
    analogWrite(deerPin[0],255);
    lightUp(0);
    lightUp(1);
       
  }
  analogWrite(gardenPins[0],50);
  analogWrite(gardenPins[1],50);
  for(int sat=255;sat>50;sat=sat-1){
      if(motion()){
        return;
      }
      analogWrite(gardenPins[0],sat);
      analogWrite(gardenPins[1],sat);
  }
  sleep(30000);
  //On commence l'extinction
  for(int sat=50;sat>2;sat=sat-1){
      if(motion()){
        return;
      }
       //rgb(0,sat,sat);
      analogWrite(deerPin[0],sat);
      analogWrite(gardenPins[0],sat);
      analogWrite(gardenPins[1],sat);
      sleep(1000);
    }

  
  while(!motion()){
    analogWrite(deerPin[0],0);
    analogWrite(gardenPins[0],0);
    analogWrite(gardenPins[1],0);
    sleep(2000);
    
  }


}

bool motion(){
  if(hasMotion || lastMotion < millis()+minMotionDuration){
    return true;
  }else{
    return false;
  }
 
  
}


void rgb(int r,int g,int b){
    
    analogWrite(rgbPin[0], r);
    analogWrite(rgbPin[1], g);
    analogWrite(rgbPin[2], b);

}
void lightUp(int pin){
    int pinNormal = !pin;
    analogWrite(gardenPins[pinNormal],2);
    for(int sat=2;sat<maxLuminosity;sat=sat+2){

       
       //rgb(0,sat,sat);
      analogWrite(gardenPins[pin],sat);
      sleep(100);
    }
    for(int sat=maxLuminosity;sat>0;sat=sat-2){

      
       //rgb(0,sat,sat);
      analogWrite(gardenPins[pin],sat);
      sleep(100);
    }
    analogWrite(gardenPins[pin],2);
}



