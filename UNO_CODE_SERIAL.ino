#include <Servo.h>

Servo s1;
const int trigPin = 7;
const int echoPin = 8;
const int doorTrigPin = 11;
const int doorEchoPin = 12;
const int MAX_CAPACITY_CM = 22;
const int DOOR_DISTANCE_THRESHOLD = 10; // Adjust this threshold as needed
const int SERVO_OPEN_ANGLE = 90; // Angle to open the door
const int SERVO_CLOSE_ANGLE = 0; // Angle to close the door
const int SERVO_STEP_DELAY = 10; // Delay between each step in servo rotation
const int SERVO_STEP_ANGLE = 1; // Angle change in each step

bool doorOpen = false; // Flag to track the state of the door

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(doorTrigPin, OUTPUT);
  pinMode(doorEchoPin, INPUT);
  s1.attach(10);
}

void loop() {
  long duration, cm;
  long doorDuration, doorCM;

  // Measure distance using the main ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  cm = microsecondsToCentimeters(duration);
  cm = max(0L, min(cm, MAX_CAPACITY_CM));

  // Calculate the percentage of garbage
  float garbagePercentage = (1.0 - float(cm) / MAX_CAPACITY_CM) * 100;
  garbagePercentage = max(0.0f, min(garbagePercentage, 100.0f));

  Serial.println(garbagePercentage);

  // Check distance using the door ultrasonic sensor
  digitalWrite(doorTrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(doorTrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(doorTrigPin, LOW);
  doorDuration = pulseIn(doorEchoPin, HIGH);
  doorCM = microsecondsToCentimeters(doorDuration);

  // If something is detected by the door ultrasonic sensor and the door is not already open, open the door
  if (doorCM < DOOR_DISTANCE_THRESHOLD && !doorOpen) {
    for (int angle = SERVO_CLOSE_ANGLE; angle <= SERVO_OPEN_ANGLE; angle += SERVO_STEP_ANGLE) {
      s1.write(angle);
      delay(SERVO_STEP_DELAY);
    }
    doorOpen = true; // Set the door state flag to open
    delay(10000);
  } 
  // If nothing is detected and the door is open, close the door
  else if (doorCM >= DOOR_DISTANCE_THRESHOLD && doorOpen) {
    s1.write(SERVO_CLOSE_ANGLE);
    doorOpen = false; // Set the door state flag to closed
  }

  delay(100);
}

long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}
