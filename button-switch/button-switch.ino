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
      redState = HIGH;
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
      greenState = HIGH;
    }   
  }
  
  if (redReading == LOW && greenReading == LOW){
    pressed = false;
  }
  digitalWrite(redPin, redState);
  digitalWrite(greenPin, greenState);
}


