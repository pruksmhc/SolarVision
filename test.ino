void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
 Serial.write( "PY=1 DN=Y"); 
 Serial.write("Y LAB Su"); 
 Serial.write("Y MR 1000"); 
 Serial.write("Y H 5000");
 Serial.write("Y ML 1000");
}
