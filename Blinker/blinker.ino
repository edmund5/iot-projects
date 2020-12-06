int w = 1;
int r = 2;

void setup() {
  pinMode(w, OUTPUT);
  pinMode(r, OUTPUT);
}

void loop() {
  digitalWrite(w, HIGH);
  delay(100);
  digitalWrite(w, LOW);
  delay(100);
  digitalWrite(w, HIGH);
  delay(100);
  digitalWrite(w, LOW);
  delay(1000);
  digitalWrite(r, HIGH);
  delay(100);
  digitalWrite(r, LOW);
  delay(1000);
}
