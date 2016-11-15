/*
  Written by Yada Pruksachatkun. 
  Arduino to IMC/Schneider Electric stepper motors.
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
  // dina_awx: Are these values just supposed to be placeholder voltages until we get the data out of our sensor? 
  int x_left = 30; 
  int y_up = 40; 
  int y_down = 40;
  int x_diff = 0; 
  int y_diff = 0; 
    // dina_awx:  okay.... not sure why you defined it as 0; I don't think that you want it to have a fixed initial value. Can't you just definite as a variable?
  
  // 400 - 200 
  x_diff = x_right - x_left; //The voltage on RHS vs voltage on LHS 
  y_diff = y_up - y_down; 
  int xMotorSteps = 0;
  int yMotorSteps = 0;
  //142 steps per 1 degree
  if (x_diff > 0) {
    // If sun is to the right. 
    xMotorSteps = 142 * x_diff; 
    // dina_awx: I would say that if the "right" side of your abscissa is positive then you want: xMotorSteps = (142 * x_diff )*-1 
    Serial.println("Xdiff more than 0, move to the right");
  } 
  else if (x_diff < 0) {
    xMotorSteps = (142 * x_diff) * -1; 
  }
     // dina_awx: Likewise, I'd say that if the "left" side of your abscissa is neg then you want "xMotorSteps = 142 *x_diff" 
  
  if ( y_diff > 0) {
    // If sun is in the top half of the telescope
    Serial.println("Xdiff more than 0, move to the up");
    yMotorSteps = 142 * y_diff;  
  } 
  //dina_awx: Same comment as for the x axis: it depends whether ordinate is positive on the "upper" part or not. 
  else if (y_diff < 0) {
    yMotorSteps = (142 * y_diff) * -1; 
  }
  // dina_awx: Same comment as for the x axis: it depends whether ordinate is positive on the "upper" part or not. 

  moveMotors(xMotorSteps, yMotorSteps);
    
  }

//Move motors X and Y positions based on 
//the solar tracker. 
void moveMotors(int xSteps, int ySteps) {
  //dina_awx : why xSteps and ySteps rather than xMotorSteps and yMotorSteps? -- I don't see you set xMotorSteps = xSteps.... 
 Serial.begin(9600); // Note: make sure that this is compatible with the Baud Rate on SEM terminal
 Serial.write( "PY=1 DN=X"); 
 Serial.write("R1=" + xSteps); //dina_awx: pick another name than R1 for the variable since r1 is a commad in Mcode
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
   Serial.write("X MR R1"); //move xstepper motor to user register??? 1 ---> in Mcode R1 stands for user register 1
   Serial.write("X H 100");
   //Print xstepper motor position
   Serial.write("X PR \"POSITION = \".P\""); 
   Serial.write("X MA 0");
   Serial.write("X H"); // dina_awx: hold..... but for how long????? 
   Serial.write("X PR \"move complete\""); 
   Serial.write("X H 100"); 
   //Wait for feedback on the position 
     if (Serial.available() > 0) {
        // read the incoming byte:
        currSteps = Serial.read();
        Serial.print("Position now"); //add a space before the quotes - it will make it easier to read
        Serial.println(currSteps, DEC);
  }
 }

 Serial.write("R1=" + ySteps); //dina_awx: same problem as mentionned before, with the R1 
 Serial.write( "PY=1 DN=Y"); 
 Serial.write("Y LAB Su"); //dina_awx: what is LAB ????
 Serial.write("Y P=0");
  // send data only when you receive data:
 while (currSteps < 51200) {
   //Wait for 5 miliseconds 
   Serial.write("Y H 5");
   Serial.write("Y MR R1"); //dina_awx: same problem as mentionned before, with the R1 
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


