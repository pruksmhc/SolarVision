/*
  Written by Yada Pruksachatkun. 
  Arduino to motors.
 */

String enable_setup_motor_x  = "PY=1 DN=X";
String enable_setup_motor_y  = "PY=1 DN=Y";
void setup() {
 
}

void loop() {
  // get any incoming bytes:
  // xDiff = right - left
  // yDiff = top - bottom
  //TODO: How to get all 4 coordinates 
  int x_right= 100; 
  int x_left = 30; 
  int y_up = 40; 
  int y_down = 40;
  int x_diff = 0; 
  int y_diff = 0; 
  // 400 - 200 
  x_diff = x_right - x_left; //The voltage on RHS vs voltage on LHS 
  y_diff = y_up - y_down; 
  int xMotorSteps = 0; 
  int yMotorSteps = 0;
  //142 steps per 1 degree
  if (x_diff > 0) {
    // If sun is to the right. 
    xMotorSteps = 142 * x_diff; 
  } 
  else if (x_diff < 0) {
    xMotorSteps = (142 * x_diff) * -1; 
  }
  if ( y_diff > 0) {
    // If sun is in the top half of the telescope
    yMotorSteps = 142 * y_diff;  
  }
  else if (y_diff < 0) {
    yMotorSteps = (142 * y_diff) * -1; 
  }

  moveMotors(xMotorSteps, yMotorSteps);
    
  }

//Move motors X and Y positions based on 
//the solar tracker. 
void moveMotors(int xSteps, int ySteps) {
 Serial.begin(9600);
 Serial.write( "PY=1 DN=X"); 
 Serial.write("R1=" + xSteps); 
 Serial.write("X LAB Su"); 
 Serial.write("X P=0");
 int currSteps = Serial.read("P "); //Get the current position of the motors
 while (currSteps < 51200) {
   //Wait for 5 miliseconds 
   Serial.write("X H 5");
   Serial.write("X MR R1");
   Serial.write("X H 100");
   Serial.write("X PR \"POSITION = \".P\"");
   Serial.write("X MA 0"); 
   Serial.write("X H"); 
   Serial.write("X PR \"move complete\""); 
   Serial.write("X H 100"); 
   currSteps = Serial.read("P"); 
 }

 Serial.write("R1=" + ySteps); 
 Serial.write( "PY=1 DN=Y"); 
 Serial.write("Y LAB Su"); 
 Serial.write("Y P=0");
 currSteps = Serial.read("P "); //Get the current position of the motors
 while (currSteps < 51200) {
   //Wait for 5 miliseconds 
   Serial.write("Y H 5");
   Serial.write("Y MR R1");
   Serial.write("Y H 100");
   Serial.write("Y PR \"POSITION = \".P\"");
   Serial.write("Y MA 0"); 
   Serial.write("Y H"); 
   Serial.write("Y PR \"move complete\""); 
   Serial.write("Y 100"); 
   currSteps = Serial.read("P"); 
 }
 Serial.write("PY=0");
}


