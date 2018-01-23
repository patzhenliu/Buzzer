//pins
int greenButton = 2; 
int redButton = 3; 
int greenPin = 12; //LED 
int redPin = 13;  //LED

int restartButton = 4;
int wrongButton = 5;
int rightButton = 6;
int buzzerPin = 11;   

int redState = LOW;
int greenState = LOW; 
int restartReading;
int rightReading;
int wrongReading; 

int rrPin = 10;
int rgPin = 9;
int grPin = 8;
int ggPin = 7;

//gets state of LEDs
int redReading;
int greenReading;             

#include "pitches.h"

//jeapardy tune
int melody[] = {
  NOTE_C4, NOTE_F4, NOTE_C4, NOTE_F3, NOTE_C4, NOTE_F4, NOTE_C4,
  NOTE_C4, NOTE_F4, NOTE_C4, NOTE_F4, NOTE_A4, NOTE_G4, NOTE_F4, NOTE_E4, NOTE_D4, NOTE_CS4,
  NOTE_C4, NOTE_F4, NOTE_C4, NOTE_F3, NOTE_C4, NOTE_F4, NOTE_C4,
  NOTE_F4, NOTE_D4, NOTE_C4, NOTE_AS3, NOTE_A3, NOTE_G3, NOTE_F3
};

int noteDurations[] = {
  4, 4, 4, 4, 4, 4, 2,
  4, 4, 4, 4, 3, 8, 8, 8, 8,
  4, 4, 4, 4, 4, 4, 2,
  3, 8, 4, 4, 4, 4, 4  
};

bool pressed = false; //flag for state of buttons
void setup()
{
  pinMode(redButton, INPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenButton, INPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(wrongButton, INPUT);
  pinMode(rightButton, INPUT);
  pinMode(buzzerPin, OUTPUT);
  
  pinMode(rrPin, OUTPUT);
  pinMode(rgPin, OUTPUT);
  pinMode(grPin, OUTPUT);
  pinMode(ggPin, OUTPUT);

  restartReading = digitalRead(restartButton);
  rightReading = digitalRead(rightButton);
  wrongReading = digitalRead(wrongButton);

  //playTheme(); //plays tune upon uploading sketch
  
  Serial.begin(9600);
  Serial.println("INITIALIZE");
}

void loop(){
  if (redState == LOW && greenState == LOW){
    //checkMotion, if motion buzzer sound
    buzzerLights();
  }
  if (redState == HIGH){
    redState = answerButtons(rrPin, rgPin);
  }
  else if (greenState == HIGH) {
    greenState = answerButtons(grPin, ggPin);
  }
  else{
    rightReading = digitalRead(rightButton);
    wrongReading = digitalRead(wrongButton);
  }
  if (digitalRead(restartButton) != restartReading) { //dip switch state is changed
    restart();
    restartReading = digitalRead(restartButton);
  }
  
}

static int answerButtons(int rPin, int gPin){ //change to DIP SWITCH 
  if (digitalRead(rightButton) != rightReading) { //dip switch
    rightReading = digitalRead(rightButton);
    turnOn(gPin);
    int tempState = HIGH;
     for (int i = 0; i < 3; i++){
       tone(buzzerPin, 823, 200);
       tempState = changeState(tempState);
       digitalWrite(gPin, tempState);
       delay(500);
     }
     return LOW;
   }
  else if (digitalRead(wrongButton) != wrongReading) { //dip switch
    //digitalWrite(lightPin, changeState(lightState));
    wrongReading = digitalRead(wrongButton);
    turnOn(rPin);
    tone(buzzerPin, 223, 1000);
    RGBoff(rPin, gPin);
    return LOW;
  }
  return HIGH;
}


static int changeState(int state){
  //changes HIGH to LOW, LOW to HIGH
  if (state == HIGH){
    return LOW;
  }
  else{
    return HIGH;
  }
}

void buzzerLights(){
  //gets state for LEDs
  redReading = digitalRead(redButton);
  greenReading = digitalRead(greenButton);

  //checks if you press light once and changes the state
  //of the light to be HIGH
  if (redReading == HIGH && not pressed) {
    pressed = true;
    /*if (redState == HIGH){ // delete later
      redState = LOW;
    }*/
    if (greenState == LOW){ //light will turn on as long as the opponent's light is not on
      tone(buzzerPin, 262, 500); //buzzer sound
      redState = changeState(redState);
    }   
  }
 
  //same for green light as red light
  if (greenReading == HIGH && not pressed) {
    pressed = true;
    /*if (greenState == HIGH){
      greenState = LOW;
    }*/
    if (redState == LOW){
      tone(buzzerPin, 294, 500);
      greenState = changeState(greenState);
    }   
  }
  
  if (redReading == LOW && greenReading == LOW){
    pressed = false;
  }
  digitalWrite(redPin, redState);
  digitalWrite(greenPin, greenState);
}

void playTheme(){
  //plays jeapardy tune
  for (int thisNote = 0; thisNote < 30; thisNote++) {
      int noteDuration = 1000 / noteDurations[thisNote];
      tone(buzzerPin, melody[thisNote], noteDuration);
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
      noTone(8);
    }
}

void turnOn(int pin){ //change colour for RGB LED
  digitalWrite(pin, HIGH);
  delay(1000);
}

void RGBoff(int rPin, int gPin){
  digitalWrite(rPin, LOW);
  digitalWrite(gPin, LOW);
}

void restart() {
  Serial.println("RESTART");
  redState = LOW;
  greenState = LOW;
  RGBoff(rrPin, rgPin);
  RGBoff(grPin, ggPin);  
  playTheme();
  //start countdown timer
}

