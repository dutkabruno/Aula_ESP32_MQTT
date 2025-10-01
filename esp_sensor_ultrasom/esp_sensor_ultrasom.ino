//trig - 20
//echo - 11
const byte echo = 11;
const byte trig = 20;
const byte g = 15;
const byte y = 7;
const byte r = 1;

void setup() {
  Serial.begin(115200);
  pinMode(echo, INPUT);   //emite som
  pinMode(trig, OUTPUT);  //recebe som
}

void loop() {
  int distance = distancia();
  if(distance <= 30){
    analogWrite(r, 255);
    analogWrite(g, 0);
    analogWrite(y, 0);
  }
  else if(distance > 30  && distance <= 50){
    analogWrite(r, 0);
    analogWrite(g, 0);
    analogWrite(y, 255);
  }
  else if(distance > 50){
    analogWrite(r, 0);
    analogWrite(g, 255);
    analogWrite(y, 0);
  }
  Serial.print("Distância:");
  Serial.println(distance);
  delay(100);
}


int distancia(){
  digitalWrite(trig, LOW);
  delayMicroseconds(10);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  //calcula o tempo da ida + o tempo da volta
  unsigned long duration = pulseIn(echo, HIGH);
  unsigned long real_time = duration / 2;
  int distance = ((real_time * 340) / 10000);
  return distance;
}