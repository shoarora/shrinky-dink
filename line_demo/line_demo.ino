const int line_pin1 = A2;
const int line_pin2 = A1;
const int line_pin3 = A0;


void setup() {
  pinMode(line_pin1, INPUT);
  pinMode(line_pin2, INPUT);
  pinMode(line_pin3, INPUT);
}

void loop() {
  int val = analogRead(line_pin1);
  Serial.print("#1: ");
  Serial.println(val);
  val = analogRead(line_pin2);
  Serial.print("#2: ");
  Serial.println(val);
  val = analogRead(line_pin3);
  Serial.print("#3: ");
  Serial.println(val);
  delay(500);
}

