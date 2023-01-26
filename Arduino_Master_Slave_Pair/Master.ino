//MASTER - Player 1

#include <Wire.h>
//defines the pin that controls the piezo
const int piezo = 9;
//prevents additional button presses from activating
bool answered = false;

void setup() {
  //all input pins defined for LEDs, piezo and servo
  Wire.begin();
  Serial.begin(9600);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(piezo, OUTPUT);
}
//flashes the led for player 1
void player1LedFlash() {
  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(13, LOW);
}
//plays a melody when a player wins
void winnerSound() {
  tone(piezo,500,200);
  delay(200);
  tone(piezo,500,200);
  delay(200);
  tone(piezo,500,200);
  delay(200);
  tone(piezo,800,150);
  delay(150);
  tone(piezo,500,500);
  delay(500);
  tone(piezo,600,1000);
}

void loop() {
  
  //this line requests data from the slave
  Wire.requestFrom(4, 2);

  //this while loop only processes data sent that is 2 bytes long
  while(Wire.available() == 2)
  { 
    //this if statement checks if the value sent from the slave board was 10 - this is an indicated that player 1 has scored a point
    if(Wire.read() == 10) {
      //the LED is flased and a tune is played from the piezo buzzer
      player1LedFlash();
      winnerSound();
    }
  }

  

  
  //if the player hasn't answered already, then the next input is considered to be the answer. A certain value is sent to the slave board to register player 1's answer
  if (answered == false){
    if (digitalRead(2) == HIGH) {
      Wire.beginTransmission(4);
      Serial.write("1a");
      Wire.write(0);
      Wire.endTransmission();
      answered = true;
    }
    else if (digitalRead(3) == HIGH) {
      Wire.beginTransmission(4);
      Serial.write("1b");
      Wire.write(1);
      Wire.endTransmission();
      answered = true;
    }
    else if (digitalRead(4) == HIGH) {
      Wire.beginTransmission(4);
      Serial.write("1c");
      Wire.write(2);
      Wire.endTransmission();
      answered = true;
    }
    else if (digitalRead(5) == HIGH) {
      Wire.beginTransmission(4);
      Serial.write("1d"); 
      Wire.write(3);
      Wire.endTransmission();
      answered = true;
    }
  }

  //this if statement resets the answered variable so that player 1 can input the answer to the next question
  if (answered == true){
    delay(1000); //
    answered = false;
  }
  
}
