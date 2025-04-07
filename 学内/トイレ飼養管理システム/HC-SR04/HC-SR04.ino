//Vcc: 5V
//Gnd: GND
//Trig: 3番ピン
//Echo: 2番ピン

int TRIG = 3;
int ECHO = 2;

double duration = 0;
double distance = 0;
double speed_of_sound = 331.5 + 0.6 * 25; // 25℃の気温の想定

void setup() {
  Serial.begin( 9600 );

  pinMode(ECHO, INPUT );
  pinMode(TRIG, OUTPUT );
}

void loop() {
  digitalWrite(TRIG, LOW); 
  delayMicroseconds(2); 
  digitalWrite( TRIG, HIGH );
  delayMicroseconds( 10 ); 
  digitalWrite( TRIG, LOW );
  duration = pulseIn( ECHO, HIGH ); // 往復にかかった時間が返却される[マイクロ秒]

  if (duration > 0) {
    duration = duration / 2; // 往路にかかった時間
    distance = duration * speed_of_sound * 100 / 1000000;
    Serial.println((int)distance);
  }

  delay(200);
}
