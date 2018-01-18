int redButton = 3; 
int greenButton = 2;        
int redPin = 13;  
int greenPin = 12; 

int wrongButton = 5;
int rightButton = 6;
int buzzerPin = 9;   

int redState = LOW;
int greenState = LOW;     
int redReading; 
int greenReading;          
int previous = LOW;    

#include "pitches.h"
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

bool pressed = false;
void setup()
{
  pinMode(redButton, INPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenButton, INPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(wrongButton, INPUT);
  pinMode(rightButton, INPUT);
  pinMode(buzzerPin, OUTPUT);

  playTheme();
  
  Serial.begin(9600);
  Serial.println("INITIALIZE");
}

void loop()
{
  buzzerLights();
  if (redState == HIGH){
    redState = answerButtons(redState, redPin);
  }
  else if (greenState == HIGH) {
    greenState = answerButtons(greenState, greenPin);
  }
  
}

static int answerButtons(int lightState, int lightPin){
  int rightReading = digitalRead(rightButton);
  int wrongReading = digitalRead(wrongButton);
  if (rightReading == HIGH) {
     for (int i = 0; i < 3; i++){
       tone(buzzerPin, 823, 200);
       Serial.println(lightState);
       lightState = changeState(lightState);
       digitalWrite(lightPin, lightState);
       delay(500);
     }
     return LOW;
   }
  else if (wrongReading == HIGH) {
    digitalWrite(lightPin, changeState(lightState));
    tone(buzzerPin, 223, 1000);
    return LOW;
  }
  return HIGH;
}


static int changeState(int state){
  if (state == HIGH){
    return LOW;
  }
  else{
    return HIGH;
  }
}

void buzzerLights(){
  redReading = digitalRead(redButton);
  greenReading = digitalRead(greenButton);

  // if the input just went from LOW and HIGH and we've waited long enough
  // to ignore any noise on the circuit, toggle the output pin and remember
  // the time
  //Serial.println(reading);
  if (redReading == HIGH && not pressed) {
    //Serial.println("PRESSED");
    pressed = true;
    if (redState == HIGH){ // delete later
      redState = LOW;
    }
    else if (greenState == LOW){
      tone(buzzerPin, 262, 500);
      redState = changeState(redState);
    }   
  }
 

  if (greenReading == HIGH && not pressed) {
    Serial.println("PRESSED");
    pressed = true;
    if (greenState == HIGH){
      greenState = LOW;
    }
    else if (redState == LOW){
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
  for (int thisNote = 23; thisNote < 30; thisNote++) {
      int noteDuration = 1000 / noteDurations[thisNote];
      tone(9, melody[thisNote], noteDuration);
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
      noTone(8);
    }
}

