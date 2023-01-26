#include <Servo.h> //Servo Library
// Library to attach any pin with attachInterrupt; credit: brainy-bits; link: https://www.brainy-bits.com/make-any-arduino-pin-an-interrupt-pin/
#include "PinChangeInt.h" //library so that there can be more than 2 attachInterrupts

#define NOTE_A1 55
#define NOTE_C6 1047
#define NOTE_D5  587
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_G5  784
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  176
#define NOTE_B6  1976

Servo servo; //initialising a servo object

//melodies and durations for sound
int miss_melody[] = {NOTE_A1, 0 , NOTE_A1};

int hit_melody[] = {NOTE_C6, 0 , NOTE_C6};

int noteDurations[] = {4,4,2};

//Zelda Ending Theme project; credit: Akram S0LTAN1 ; link: https://www.instructables.com/id/8bit-Songs-Using-Arduino-Zelda-Ending-Theme/
int win_melody[] = {
  NOTE_G6, NOTE_G5, NOTE_C6, NOTE_E6,
  NOTE_DS6, NOTE_G5, NOTE_B6, NOTE_B5,
  NOTE_A6, NOTE_C6, NOTE_E6,  NOTE_A6,
  NOTE_G6, NOTE_C6, NOTE_D6, NOTE_E6,

  NOTE_A6, NOTE_C6, NOTE_F6, NOTE_A6,
  NOTE_GS6, NOTE_C6, NOTE_D6, NOTE_F6,
  NOTE_E6, NOTE_G5, NOTE_C6, NOTE_E6,
  NOTE_D6, NOTE_A5, NOTE_B5, NOTE_D6,

};

int winningNoteDurations[] = {
  3, 3, 3, 3,
  3, 3, 3, 3,
  3, 3, 3, 3,
  3, 3, 3, 3,

  3, 3, 3, 3,
  3, 3, 3, 3,
  3, 3, 3, 3,
  3, 3, 3, 3,

  };

//non-user specific components
int piezoPin = A0;
int Buttons[] = {2,3,4,5}; //2 = player 1; 3 = player 2; 4 = player 3; 5 = change difficulty
int servoPin = 6;
//shift register - player 1 and 2
int clockPin = 7;
int latchPin = 8;
int dataPin = 9;
//player3
int blueP3LED = 10; //the blue LED for player 3
int yellowLED[] = {11,12,13};

//initialising variables
byte leds = 0;    // Variable to hold the pattern of which LEDs are currently turned on or off
int difficulty=0;
int delayTime;
//by default the difficulty is easy or 0 
int extraDelayTime=3000;
int defaultDelayTime=1000;
int randRedLED;
int randGreenLED;
int randYellowLED;
int p1_score;
int p2_score;
int p3_score;


bool hit;
bool miss;
bool p1_repeatInput;
bool p2_repeatInput;
bool p3_repeatInput;
/*
 * setup() - this function runs once when you turn your Arduino on
 * We initialize the serial connection with the computer
 */
void setup() 
{ 
  // Set all the pins of 74HC595 as OUTPUT
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  Serial.begin(9600);
  servo.attach(servoPin); //servo
  pinMode(piezoPin, OUTPUT); //piezo

  //interrupts
  attachInterrupt(0, player1, FALLING); //player 1 button
  attachInterrupt(1, player2, FALLING); //player 2 button
  attachPinChangeInterrupt(Buttons[2],player3,FALLING); //player 3 button
  attachPinChangeInterrupt(Buttons[3], change_difficulty,FALLING); //change difficulty button
  
  for (int i=0;i<4;i++){
    //Buttons
    pinMode(Buttons[i],INPUT);
    //yellow LED
    if (i<3){
      pinMode(yellowLED[i],OUTPUT);
    }
  }
}

/*
 * loop() - this function runs over and over again
 */
void loop() 
{  
  /*  _     _____ ____  ____  
     | |   | ____|  _ \/ ___| 
     | |   |  _| | | | \___ \ 
     | |___| |___| |_| |___) |
     |_____|_____|____/|____/ 
                              
  */
  if (p1_score<10&&p2_score<10&&p3_score<10){
    Serial.print("\np1_score: ");
    Serial.print(p1_score);
    Serial.print("\np2_score: ");
    Serial.print(p2_score);
    Serial.print("\np3_score: ");
    Serial.print(p3_score);
    //servo.write(30);
    delayTime = defaultDelayTime + random(extraDelayTime);
    delay(delayTime);
    //servo.write(90);
    //turns off all LEDS
    leds = 0;
    updateShiftRegister();
    randRedLED = random(4, 7); // select a random red LED
    randGreenLED = random(3); // select a random green LED
    randYellowLED = random(3); // select a random yellow LED
    bitSet(leds, randRedLED); //turn on a random red LED on 
    bitSet(leds, randGreenLED); //turn on a random red LED on 
    digitalWrite(yellowLED[randYellowLED], HIGH);  //turn on a random yellow LED on 
    updateShiftRegister();
    delay(delayTime);
    
    bitClear(leds, randRedLED); //turn off the random red LED
    bitClear(leds, randGreenLED); //turn off the random green LED
    updateShiftRegister();
    digitalWrite(yellowLED[randYellowLED], LOW);  //turn off the random yellow LED
    digitalWrite(blueP3LED, LOW);
    /*
       ____  _____ ______     _____  
      / ___|| ____|  _ \ \   / / _ \ 
      \___ \|  _| | |_) \ \ / / | | |
       ___) | |___|  _ < \ V /| |_| |
      |____/|_____|_| \_\ \_/  \___/ 
                                  
     */
    if ((p1_score>p2_score)&&(p1_score>p3_score)){
      //point towards player 1 button
      servo.write(135);
    }
    if ((p2_score>p1_score)&&(p2_score>p3_score)){
      //point towards player 2 button
      servo.write(90);
    }
    if ((p3_score>p2_score)&&(p3_score>p1_score)){
      //point towards player 3 button
      servo.write(45);
    }

    
    
  }




  
  //if any of the player's score reach 10
  else{
    //red, green and blue for both player 1 and 2, LEDs
    for(int i=0;i<8;i++){
      bitSet(leds,i);
      updateShiftRegister();
      delay(100);
      bitClear(leds,i);
      updateShiftRegister();
    }
    //yellow LEDs
    for (int i=0;i<3;i++){
      digitalWrite(yellowLED[i],HIGH);
      delay(100);
      digitalWrite(yellowLED[i],LOW);
    }
    //blue player 3 LED
    digitalWrite(blueP3LED,HIGH);
    delay(100);
    digitalWrite(blueP3LED,LOW);
    //play the winning tune
    playWon();
  }

  if (miss == true){
    playMiss();
  }
  
  if (hit == true){
    playHit();
  }
  
  p1_repeatInput = false;
  p2_repeatInput = false;
  p3_repeatInput = false;
  hit = false;
  miss = false;

  
}


void playMiss(){

  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 3; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(piezoPin, miss_melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(piezoPin);
  }
}


void playHit(){

  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 3; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(piezoPin, hit_melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(piezoPin);
  }
}

void playWon() {
    for (int thisNote = 0; thisNote < 32; thisNote++) {

      // to calculate the note duration, take one second
      // divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 1000 / winningNoteDurations[thisNote];

      tone(piezoPin, win_melody[thisNote], noteDuration);

      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);

      // stop the tone playing:
      noTone(piezoPin);
      }
}


void player1(){
  if (!p1_repeatInput){
  //checks all red LEDs to see if any are on when the button is pressed
    for (int i=4; i<7;i++){
      if (bitRead(leds,i)==1){
        delay(300);
        //turn the blue LED on
        bitSet(leds,7);
        Serial.print("player 1 scored");
        p1_score ++;
        p1_repeatInput = true;
        updateShiftRegister();
        hit = true;
      }
    }
   //if user missed
   if ((bitRead(leds,4)==0) && (bitRead(leds,5)==0) && (bitRead(leds,4)==0 )){
    miss = true;
   }
  }
  else{
    miss = true;
  }
}


void player2(){
  if (!p2_repeatInput){
  //checks all green LEDs to see if any are on when the button is pressed
    for (int i=0; i<3;i++){
      if (bitRead(leds,i)==1){
        delay(300);
        //turn the blue LED on
        Serial.print("player 2 scored");
        bitSet(leds,3);
        updateShiftRegister();
        p2_score++;
        p2_repeatInput = true;
        hit = true;
      }
    }
    //if user missed
    if ((bitRead(leds,0)==0) && (bitRead(leds,1)==0) && (bitRead(leds,2)==0 )){
       miss = true;
   }
  }
  else{
    miss=true;
  }
}


void player3(){
  if (!p3_repeatInput){
    for (int i=0; i<3; i++){
      if (digitalRead(yellowLED[i])==HIGH){
        digitalWrite(blueP3LED, HIGH);
        p3_score++;
        delay(300);
        p3_repeatInput = true;
        hit = true;
      }
    }
    //if user missed
   if ((digitalRead(yellowLED[0])==LOW) && (digitalRead(yellowLED[1])==LOW) && (digitalRead(yellowLED[2])==LOW)){
    miss = true;
   }
  }
  else{
    miss=true;
  }
}


void change_difficulty(){
  
  if (difficulty == 0){
    Serial.print("normie\n");
    defaultDelayTime = 1000;
    extraDelayTime = 3000;
  }
  
  if (difficulty == 1){
    Serial.print("noob\n");
    defaultDelayTime = 500;
    extraDelayTime = 2000;
    
  }
  if (difficulty == 2){
    Serial.print("pro-gamer \n");
    defaultDelayTime = 0;
    extraDelayTime = 1000;
  }
  if (difficulty==0 || difficulty ==1){
    difficulty++;
    }
   else if(difficulty==2){
    difficulty = 0;
   }
  }


/*
 * updateShiftRegister() - This function sets the latchPin to low, then calls the Arduino function 'shiftOut' to shift out contents of variable 'leds' in the shift register before putting the 'latchPin' high again.
 */
void updateShiftRegister()
{
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, LSBFIRST, leds);
   digitalWrite(latchPin, HIGH);
}
