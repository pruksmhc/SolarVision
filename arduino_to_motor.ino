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
  //The voltage on RHS vs LHS 
  int x_diff =  x_right - x_left;
  //The voltage on Upper vs lower half
  int y_diff = y_up - y_down; 

  int xMotorSteps = 0; 
  int yMotorStexps = 0;
  //142 steps per 1 degree
  if (x_diff > 0) {
    // If sun is to the right. 
    xMotorSteps = 142 * x_diff; 
    Serial.println("Xdiff more than 0, move to the right");
  } 
  else if (x_diff < 0) {
    xMotorSteps = (142 * x_diff) * -1; 
  }
  if ( y_diff > 0) {
    // If sun is in the top half of the telescope
    Serial.println("Xdiff more than 0, move to the up");
    yMotorSteps = 142 * y_diff;  
  }
  else if (y_diff < 0) {
    yMotorSteps = (142 * y_diff) * -1; 
  }

  moveMotors(xMotorSteps, yMotorSteps);
    
  }

//Move motors X and Y positions based on 
//the solar tracker. 
void moveMotors(int xMotorSteps, int yMotorSteps) {
 Serial.begin(9600);
 Serial.write( "PY=1 DN=X"); 
 Serial.write("R1=" + xMotorSteps); 
 Serial.write("X LAB Su"); 
 Serial.write("X P=0");
  while (Serial.available() < 0) {
    //wait until the Lexium MDrive sends over current locaiton. 
  }
  int currSteps = Serial.read();
  Serial.print("Position now");
  Serial.println(currSteps, DEC);
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
   //Wait for feedback on the position 
     if (Serial.available() > 0) {
        // read the incoming byte:
        currSteps = Serial.read();
        Serial.print("Position now");
        Serial.println(currSteps, DEC);
  }
 }

 Serial.write("R1=" + yMotorSteps); 
 Serial.write( "PY=1 DN=Y"); 
 Serial.write("Y LAB Su"); 
 Serial.write("Y P=0");
  // send data only when you receive data:
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
  // currSteps = Serial.read("P"); 
   if (Serial.available() > 0) {
        // read the incoming byte:
        currSteps = Serial.read();
        Serial.print("Position now");
        Serial.println(currSteps, DEC);
  }
 }
 Serial.write("PY=0");
}


