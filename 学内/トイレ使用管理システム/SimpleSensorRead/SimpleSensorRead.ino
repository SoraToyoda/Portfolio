// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
const int SENSOR = A0;


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void setup() {
  Serial.begin(9600);
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void loop() {
  int sensor = analogRead(SENSOR);
  Serial.println(sensor);
  delay(100);
}