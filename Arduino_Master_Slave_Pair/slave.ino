//SLAVE - Player 2

#include <Wire.h>
#include <Servo.h> 

//declare all the variables as float so they use less space and still keep the accuracy.
float xSquared, x, firstConstant, secondConstant, firstRoot, secondRoot;
//declaring the simultaneous equation coefficients
float x1, y1, x2, y2;
//declaring the solution for the simultaneous equations
int correctX, correctY;
//Generates a digit from 1-9 (cannot be zero because an equation like 0x + 3 = 5 is unsolvable)
int GenerateOneDigitRandomNo()
{
  return random(1, 10);
}
//Generates a one or zero- essentially used as a "flip the coin" later on
int GenerateOneOrZero()
{
  return random(0, 2);
}
//Generates a two digit random number- so from 10-99
int GenerateTwoDigitRandomNo()
{
  return random(10, 100);
}
//Generates a number between 1 and 4, since there are always 4 options
int GenerateOneToFour()
{
  return random(1,5);
}
//function prints the equation, prints the options, then returns the correct multiple choice out of those options
int PrintLinearEquation()
{
  //x here is the coefficient of the 'x' term in an equation like 7x + 34 = 69, where x would be 7.
  x = GenerateOneDigitRandomNo();
  firstConstant = GenerateTwoDigitRandomNo();
  secondConstant = GenerateTwoDigitRandomNo();
  Serial.print(x);
  Serial.print("x + ");
  Serial.print(firstConstant);
  Serial.print(" = ");
  Serial.print(secondConstant);
  firstRoot = ((secondConstant - firstConstant) / x);
  //creates an array that has size 5 (all arrays do NOT use the 0th element due to the way the options are written)
  float floatOptions[5];
  //makes all the elements in the arary equal to 0 (important for checking)
  for (int counter = 0; counter < 5; counter++){
    floatOptions[counter] = 0;
  }
  //Generates the position of the correct answer in the options (a contains this)
  int a = GenerateOneToFour();
  //Used 'a' so that the value of "correctAnswer" can only be assigned once and is constant
  int correctAnswer = a;
  floatOptions[a] = firstRoot;
  //does this 3 times because out of 4 options, one option is filled by the correct answer
  for (int i = 0; i < 3; i++)
  {
    //while the random option hasn't already been edited with a "wrong" option
    while (floatOptions[a] != 0)
    {
      a = GenerateOneToFour();
    }
    //tosses a coin to see if the option will have the right answer have another random number added to it or subtracted by it
    floatOptions[a] = (GenerateOneOrZero()) ? firstRoot + GenerateOneDigitRandomNo(): firstRoot - GenerateOneDigitRandomNo();
  }
  //prints the options out
  Serial.println(" ");
  for (int counter = 1; counter < 5; counter++)
  {
    Serial.println("");
    Serial.print("Option ");
    Serial.print(counter);
    Serial.print(" : ");
    Serial.print(floatOptions[counter]);
  }
  Serial.println(" ");

  return correctAnswer;
}
//does the same thing as linear except for a quadratic equation
int PrintQuadraticEquation()
{
  //all variables given a 0 value
  xSquared = 0;
  x = 0;
  firstConstant = 0;
  secondConstant = 0;
  //the numbers that are generated for each coefficient have to make a determinant that is greater than 1
  //which eliminates possibility of complex roots
  while (((x * x) - 4 * xSquared * (firstConstant-secondConstant)) <= 1)
  {
    xSquared = GenerateOneDigitRandomNo();
    x = GenerateOneDigitRandomNo();
    firstConstant = GenerateTwoDigitRandomNo();
    secondConstant = GenerateTwoDigitRandomNo();
  }
  //prints out question
  Serial.println("");
  Serial.print(xSquared);
  Serial.print("x^2 + ");
  Serial.print(x);
  Serial.print("x + ");
  Serial.print(firstConstant);
  Serial.print(" = ");
  Serial.print(secondConstant);
  //finds the answer to its own question
  double determinant = sqrt((double)((x * x) - 4 * xSquared * (firstConstant - secondConstant)));
  firstRoot = (-x + (float)(determinant))/(2 * xSquared);
  secondRoot = (-x - (float)(determinant))/(2 * xSquared);
  //the 0th element of arrays never used, so actual size of array is [2][4], but because of numbering array declared as [2+1][4+1]
  //answers to quadratic equations come in pairs and so we need to implement a two dimensional array
  float floatOptions[3][5];
  //sets all values of the array to 0 for checking
  for (int counter = 0; counter < 3; counter++)
  {
    for (int counter2 = 0; counter2 < 5; counter2++)
    {
      floatOptions[counter][counter2] = 0;
    }
  }
  //slots in the correct answer into the options array at a random position
  int a = GenerateOneToFour();
  int correctAnswer = a;
  floatOptions[1][a] = firstRoot;
  floatOptions[2][a] = secondRoot;
  //puts in random (but very close answers) into random positions
  for (int i = 0; i < 3; i++)
  {
    while(floatOptions[1][a] != 0)
    {
      a = GenerateOneToFour();
    }
    //same as with linear: coin tossed to create a positive or negative difference with the correct answer
    floatOptions[1][a] = (GenerateOneOrZero()) ? firstRoot + GenerateOneDigitRandomNo(): firstRoot - GenerateOneDigitRandomNo();
    floatOptions[2][a] = (GenerateOneOrZero()) ? secondRoot + GenerateOneDigitRandomNo(): secondRoot - GenerateOneDigitRandomNo();
    a = 0;
  }
  //output all the options
  for (int i = 1; i < 5; i++)
  {
    Serial.println("");
    Serial.print("Pair ");
    Serial.print(i);
    Serial.print(": (");
    Serial.print(floatOptions[1][i]);
    Serial.print(", ");
    Serial.print(floatOptions[2][i]);
    Serial.print(")");
  }
  Serial.println("");
  return correctAnswer;
}

int PrintSimultaneousEquations()
{
  //the return variable initialised
  int correctAnswer;
  //this function works BACKWARDS from the answers unlike the other functions, meaning that they have integer solutions
  correctX = GenerateOneDigitRandomNo();
  correctY = GenerateOneDigitRandomNo();
  //generates the constants
  firstConstant = GenerateTwoDigitRandomNo();
  secondConstant = GenerateTwoDigitRandomNo();
  x1 = 0, x2 = 0, y1 = 0, y2 = 0;
  //generates different coefficient values for the x coefficient terms
  while (x1 == x2){
    x1 = GenerateOneDigitRandomNo();
    x2 = GenerateOneDigitRandomNo();
  }
  //if the x terms are different, but the solution the same, then the y coefficients must also be different
  y1 = (firstConstant - (x1 * correctX)) / correctY;
  y2 = (secondConstant - (x2 * correctX)) / correctY;
  //prints the question
  Serial.println("");
  Serial.print(x1);
  Serial.print("x + ");
  Serial.print(y1);
  Serial.print("y = ");
  Serial.print(firstConstant);

  Serial.println("");
  Serial.print(x2);
  Serial.print("x + ");
  Serial.print(y2);
  Serial.print("y = ");
  Serial.print(secondConstant);
  //has to have the same type of array as quadratic, but this is integer since solutions are integers
  int intOptions[3][5];
  for (int counter = 0; counter < 3; counter++)
  {
    for (int counter2 = 0; counter2 < 5; counter2++)
    {
      intOptions[counter][counter2] = 0;
    }
  }
  //slots in the correct answer
  int a = GenerateOneToFour();
  correctAnswer = a;
  intOptions[1][a] = correctX;
  intOptions[2][a] = correctY;
  //as seen before, iterates for the other options and adds a difference or subtracts depending on coin flip
  for (int i = 0; i < 3; i++)
  {
    while(intOptions[1][a] != 0)
    {
      a = GenerateOneToFour();
    }
    //shorthand if statements to decide
    intOptions[1][a] = (GenerateOneOrZero()) ? correctX + GenerateOneDigitRandomNo(): correctX - GenerateOneDigitRandomNo();
    intOptions[2][a] = (GenerateOneOrZero()) ? correctY + GenerateOneDigitRandomNo(): correctY - GenerateOneDigitRandomNo();
  }

  for (int i = 1; i < 5; i++)
  {
    Serial.println("");
    Serial.print(i);
    Serial.print(". ");
    Serial.print("(");
    Serial.print(intOptions[1][i]);
    Serial.print(", ");
    Serial.print(intOptions[2][i]);
    Serial.print(")");
  }
 
  return correctAnswer;
}
//prints 4 questions, which have 4 answers in a particular order
int PrintRapidFireQuestions()
{
  int correctAnswer;
  //0th element never used so we just put our first name initial there 'V'
  //different operators have to be used so had to make an array for them
  char operation[5] = {'V', '+', '-', '*', '/'};
  //contains the "row" of answers
  float OptionRow[5];
  //containst the total options 4x4
  int Options[5][5];
  //makes every element 0
  for (int counter1 = 0; counter1 < 5; counter1++)
  {
    for (int counter2 = 0; counter2 < 5; counter2++)
    {
      Options[counter1][counter2] = 0;
    }
  }
  //Has to generate 4 questions, so we make it generate one question four times
  for (int counter = 0; counter < 4; counter++)
  {
    x1 = GenerateOneDigitRandomNo();
    y1 = GenerateOneDigitRandomNo();
    //Generates a position for the operator array to dictate which operation is implemented
    int operatorposition = GenerateOneToFour();
   
    Serial.println("");
    Serial.print(x1);
    Serial.print(" ");
    Serial.print(operation[operatorposition]);
    Serial.print(" ");
    Serial.print(y1);
    Serial.print(";");
  //Finds the answer to the calculation and adds it to the "OptionRow"
    if (operation[operatorposition] == '+')
    {
      OptionRow[counter + 1] = x1 + y1;
    }
    if (operation[operatorposition] == '-')
    {
      OptionRow[counter + 1] = x1 - y1;
    }
    if (operation[operatorposition] == '*')
    {
      OptionRow[counter + 1] = x1 * y1;
    }
    if (operation[operatorposition] == '/')
    {
      OptionRow[counter + 1] = x1 / y1;
    }
  }
  //Puts the elements from OptionRow into Options array
  int a = GenerateOneToFour();
  correctAnswer = a;
  for (int counter = 1; counter < 5; counter++)
  {
    Options[a][counter] = OptionRow[counter];
  }
  
  for (int counter = 0; counter < 3; counter++)
  {
    a = 1;
    //Finds an empty row to start on
    while (Options[a][1] != 0)
    {
      a = GenerateOneToFour();
    }
    //Fills the row with options with differences for each calculation
    for (int counter2 = 1; counter2 < 5; counter2++)
    {
      Options[a][counter2] = (GenerateOneOrZero) ? OptionRow[counter2] + GenerateOneDigitRandomNo(): OptionRow[counter2] - GenerateOneDigitRandomNo();    
    }
  }
  //prints out all of the options
  for (int counter = 1; counter < 5; counter++)
  {
    Serial.println("");
    Serial.print(counter);
    Serial.print(". ");
    for(int counter2 = 1; counter2 < 5; counter2++)
    {
      Serial.print(Options[counter][counter2]);
      Serial.print("  ");
    }
  }
  //returns the correct answer out of those
  return correctAnswer;
}
//--------------------------------------------------------------------------------------------------------------------------//


//these variables keep track of whether each player has already entered an answer - important to know since each player can only input one answer.
bool player2Answered = false;
bool player1Answered = false;

//this variable will store the input value sent from the master board
int inByte;

//these variables keep track of each player's score
int player1Score = 0;
int player2Score = 0;

//these strings store the answers themselves
String firstAnswer = "null";
String secondAnswer;

//this variable keeps track of whether the program is ready to accept inputs or not
bool readyForQuestion = true;

//these are used to keep track of the correct answer to each question - the one that the player needs to input in order to score a point.
int answer;
char charAnswer;

//this array is used to store the order of the types of questions to be asked, and index is used to keep track of where we are in the array
String questions[11] = {"simultaneous", "linear", "linear", "linear", "rapidFire", "linear", "quadratic", "quadratic", "rapidFire", "quadratic", "quadratic"};
int index = 0;

//these variables are used to store the pins that the servo and the piezo buzzer connect to
int servoPin = 6;
const int piezo = 9;

//this variable stores the data that will be sent to the master board from the slave
byte p1Correct = 0;

//this line initiates the servo
Servo servo;

//this function flashes the LED for the slave board on for 1 second
void player2LedFlash() {
  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(13, LOW);
}

//this value is used to keep track of whether the master's request has been handled or not (to avoid an infinite loop later)
bool notSent = true;

//this function is used to handle any requests from the master
void request() {
  if (notSent == true) {
    p1Correct = 10;
    Wire.write(p1Correct);
    p1Correct = 0;
    notSent = false;
  }
}


//this function is used to see if any of the players input the correct answer first
void checkAnswer(char correctAnswer, String firstAnswer, String secondAnswer) {

  //this variable stores the player who answered first
  char firstPersonToAnswer = firstAnswer.charAt(0);

  //if the first person to answer gave the correct answer, then they are given 1 point. Else, the other player is given a point. 
  //This is to punish players who randomly spam inputs and hope to get lucky
  if (firstAnswer.charAt(1) == correctAnswer) {
    Serial.println("\nFirst answer was right.");
    if (firstPersonToAnswer == '1') {
      player1Score += 1;
      Serial.println("+1 point to player 1\n\n\n\n");
      notSent = true;
      Wire.onRequest(request);
       
    }
    else {
      player2Score += 1;
      Serial.println("+1 point to player 2\n\n\n\n");
      player2LedFlash();
      winnerSound();
    }
  }
  else {
    Serial.println("\nFirst answer was wrong.");
    
    if (firstPersonToAnswer == '1') {
      player2Score += 1;
      Serial.println("+1 point to player 2\n\n\n\n");
      player2LedFlash();
      winnerSound();
    }
    else {
      player1Score += 1;
      Serial.println("+1 point to player 1\n\n\n\n");
      notSent = true;
      Wire.onRequest(request);
    }
  }
}
  

void setup() {

  //these lines get the servo working
  servo.attach(servoPin); 
  servo.write(30);

  //this line declares the piezo as an output
  pinMode(piezo, OUTPUT);

  //this line is used to create a random seed based on the value from pin A0
  randomSeed(analogRead(0));
  
  //these lines allow for a master-slave communication with the other arduino
  Wire.begin(4);                
  Wire.onReceive(receiveEvent);

  //this line initiates the serial monitor
  Serial.begin(9600);

  //this line causes pin 13 to be declared as an output (this is where the LED goes)
  pinMode(13, OUTPUT);
  
  Serial.println("Ready");
}

//this function gets the piezo buzzer to play a tune. This is called when a player scores a point.
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

//this function tells the program what to do when the slave board recieves data from the master board
void receiveEvent(int bytes) {  

  //this variable stores whatever was sent as input
  int temp = Wire.read();

  //if player 1 hasn't already answered, this input will be taken in as the answer. Else, nothing happens
  if (player1Answered == false) {

    //if the first answer hasn't been entered by player 2, this answer by player 1 will be considered as the first answer. Otherwise, it will be considered as a second
    //...answer
    if (firstAnswer == "null") {
      if (temp == 0) {
        Serial.print("\n1a");
        firstAnswer = "1a";
      }
      else if (temp == 1) {
        Serial.println("\n1b");
        firstAnswer = "1b";
      }
      else if (temp == 2) {
        Serial.println("\n1c");
        firstAnswer = "1c";
      }
      else if (temp == 3) {
        Serial.println("\n1d");
        firstAnswer = "1d";
      }
    }
    else {
      if (temp == 0) {
        Serial.print("\n1a");
        secondAnswer = "1a";
      }
      else if (temp == 1) {
        Serial.println("\n1b");
        secondAnswer = "1b";
      }
      else if (temp == 2) {
        Serial.println("\n1c");
        secondAnswer = "1c";
      }
      else if (temp == 3) {
        Serial.println("\n1d");
        secondAnswer = "1d";
      }
    }
    player1Answered = true;
  }
  
   
}




void loop() {
  
  //this if statement is used to point the servo towards whoever is leading in terms of points.
  if (player1Score > player2Score) {
    servo.write(0);
  }
  else if (player1Score < player2Score) {
    servo.write(60);
  }
  else {
    servo.write(30);
  }
  
  //if the program is ready to ask for a question, then the next question (as decided in the array from before) will be asked...
  if (readyForQuestion == true) {
    if (questions[index] == "linear") {
      answer = PrintLinearEquation();
    }
    else if (questions[index] == "quadratic") {
      answer = PrintQuadraticEquation();
    }
    else if (questions[index] == "simultaneous") {
      answer = PrintSimultaneousEquations();
    }
    else if (questions[index] == "rapidFire") {
      answer = PrintRapidFireQuestions();
    }
    //...if there are no more questions to ask, then the game is over and the player with most points is declared winner
    else {
      Serial.println("GAME OVER");
      if (player1Score > player2Score) {
        Serial.println("Player 1 wins!!!");
      }
      else {
        Serial.println("Player 2 wins!!!");
      }
    }
    index += 1;
    
    //this if statement is used to convert the correct option number into a character so that it works with the checkAnswer function
    if (answer == 1) {
        charAnswer = 'a';
      }
      else if (answer == 2) {
        charAnswer = 'b';
      }
      else if (answer == 3) {
        charAnswer = 'c';
      }
      else {
        charAnswer = 'd';
      }
  }
  readyForQuestion = false;
  

   //if player2 hasn't answered yet, then whatever input is sent from the slave board is considered as player 2's final answer
  if (player2Answered == false){
    //if player 1 hasn't answered yet, then this answer is considered to be the first answer...
    if (firstAnswer == "null") {
      if (digitalRead(2) == HIGH) {
        Serial.write("2a");
        firstAnswer = "2a";
        player2Answered = true;
      }
      else if (digitalRead(3) == HIGH) {
        Serial.write("2b");
        firstAnswer = "2b";
        player2Answered = true;
      }
      else if (digitalRead(4) == HIGH) {
        Serial.write("2c");
        firstAnswer = "2c";
        player2Answered = true;
      }
      else if (digitalRead(5) == HIGH) {
        Serial.write("2d");
        firstAnswer = "2d"; 
        player2Answered = true;
      }
    }
    //... otherwise, it is considered to be the second answer
    else {
      if (digitalRead(2) == HIGH) {
        Serial.write("\n2a");
        secondAnswer = "2a";
        player2Answered = true;
      }
      else if (digitalRead(3) == HIGH) {
        Serial.write("\n2b");
        secondAnswer = "2b";
        player2Answered = true;
      }
      else if (digitalRead(4) == HIGH) {
        Serial.write("\n2c");
        secondAnswer = "2c";
        player2Answered = true;
      }
      else if (digitalRead(5) == HIGH) {
        Serial.write("\n2d");
        secondAnswer = "2d"; 
        player2Answered = true;
      }
    }
  }
    //if both players have answered the question...
    if (player1Answered == true && player2Answered == true) {

      //...check the answer and reset all variables so that the next question can be asked
      checkAnswer(charAnswer, firstAnswer, secondAnswer); 
      player1Answered = false;
      player2Answered = false;
      firstAnswer = "null";
      secondAnswer = "null";
      readyForQuestion = true;
      
      //a short 1 second delay to allow players to see the results and to allow time for the LED to flash, piezo to play a tune and servo to spin
      delay(1000);
    }
    
}
  
