const int trigPin = 5;
const int echoPin = 18;

// Velocidade do som no ar em cm
#define SOUND_SPEED 0.0343 

long duration;
float distanceCm;

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  distanceCm = duration * SOUND_SPEED / 2.0;

  if (duration == 0) {
    Serial.println("Erro: Nenhum objeto detectado ou fora de alcance.");
  } else {
    Serial.print("Distancia: ");
    Serial.print(distanceCm);
    Serial.println(" cm");
  }

  delay(500); 
}