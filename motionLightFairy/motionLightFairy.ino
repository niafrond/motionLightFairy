int gardenPins[] = {9,10};

int motionSensorPin = 2;
int rgbPin[] = {3,11,13};
const int maxLuminosity = 250;
const int minLuminosity=maxLuminosity/12;
volatile bool hasMotion = 0;

const long minMotionDuration = 4000;
volatile long lastMotion = 0;
String serialString = "";
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
void debug(String message){
  Serial.println(message);
}
void setMotion(){
  volatile bool inWindow = false;
  
  serialString="setMotion : inwindow =>";
  
  hasMotion = digitalRead(motionSensorPin)!=0;
  int tmpLastMotion = millis();
  if(lastMotion < tmpLastMotion - minMotionDuration){//Si le mouvement a été fait aprés la fenetre d'allumage minimum
    lastMotion = millis();
    inWindow=true;
  }
  
  serialString.concat(inWindow);
  debug(serialString);

  
}

void sleep(long duration){
  delay(duration);
}
void loop() {
  // put your main code here, to run repeatedly
  //test();
  
  debug("loop : Debut de captation");
  while(motion()){
     debug("loop : Full light");
  //Full light for 5sec
    analogWrite(gardenPins[0],maxLuminosity);
    analogWrite(gardenPins[1],maxLuminosity);
    bonfire(minMotionDuration);
    
       
  }
  
  debug("loop:big wait");
  bonfire(10000);
   debug("loop:On commence l'extinction");
  for(int sat=maxLuminosity;sat>2;sat=sat-1){
      if(motion()){
        return;
      }
      analogWrite(gardenPins[0],sat);
      analogWrite(gardenPins[1],sat);
      bonfire(1000);
    }

  debug("loop:Pas de mouvement");
  while(!motion()){
    rgb(0,0,0);
    analogWrite(gardenPins[0],0);
    analogWrite(gardenPins[1],0);
    
  }


}

bool motion(){

  if(hasMotion || lastMotion > millis()-minMotionDuration){
    serialString="yes motion => ";
    if(hasMotion){
        serialString.concat("True motion");
    }
    else{
      serialString.concat("Timer");
    }
    debug(serialString);
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
    
    //Disable the interrupt while the light is changing
    detachInterrupt(digitalPinToInterrupt(motionSensorPin));
    Serial.println("lightUp: On commence par baisser les lumiéres");
    for(int sat=maxLuminosity;sat>minLuminosity;sat=sat-5){
      analogWrite(gardenPins[pin],sat);
      analogWrite(gardenPins[pinNormal],sat);
      bonfire(100);
    }
    analogWrite(gardenPins[pinNormal],minLuminosity);
    Serial.println("lightUp : Degrade UP");
    for(int sat=minLuminosity;sat<maxLuminosity;sat=sat+2){

      analogWrite(gardenPins[pin],sat);
      bonfire(100);
    }
    Serial.println("lightUp : Degrade Down");
    for(int sat=maxLuminosity;sat>minLuminosity;sat=sat-2){
      analogWrite(gardenPins[pin],sat);
      bonfire(100);
    }
    analogWrite(gardenPins[pin],minLuminosity);
    //reenable the interrupt
    attachInterrupt(digitalPinToInterrupt(motionSensorPin), setMotion, CHANGE);
    bonfire(1000);
}
//We maintain a bonfire while sleeping
void bonfire(unsigned long sleepDuration){
  serialString="bonfire start for ";
  unsigned long startTime = millis();
  serialString.concat(sleepDuration);
  //debug(serialString);

  while(startTime > (millis()-sleepDuration)){
   

    int green = constrain(random(25),1,25);
    int red = constrain(green*5,0,255);//To make orange, red need o be 5 times greater
    rgb(red,green,0);
    sleep(random(100));
  }
}




