/*
  Robot Car Motor Test
*/

#define LED LED_BUILTIN

// IR proximity sensors (active low)
#define LEFT  4
#define RIGHT 2

// L298N motor controller
#define ENA 10  // left motor
#define IN1 9
#define IN2 8
#define ENB 5 // right motor
#define IN3 7
#define IN4 6

#define FASTEST  255
#define FAST  254
#define SLOW  (FAST/2)
#define STOP  0
#define DEFAULT_SPEED SLOW

#define DIR_STOP  0
#define DIR_FORWARD  1
#define DIR_LEFT  2
#define DIR_RIGHT 3
#define DIR_BACK  4
#define DIR_CW  5
#define DIR_CCW 6

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED, OUTPUT);
  
  pinMode(LEFT, INPUT);
  pinMode(RIGHT, INPUT);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void runMotors(int dir, int spd) {
  switch (dir) {
    case DIR_BACK:
     digitalWrite(IN1, HIGH);
     digitalWrite(IN2, LOW);
     digitalWrite(IN3, HIGH);
     digitalWrite(IN4, LOW);
     break;
    case DIR_FORWARD:
     digitalWrite(IN1, LOW);
     digitalWrite(IN2, HIGH);
     digitalWrite(IN3, LOW);
     digitalWrite(IN4, HIGH);
     break;
    case DIR_CW:
     digitalWrite(IN1, LOW);
     digitalWrite(IN2, HIGH);
     digitalWrite(IN3, HIGH);
     digitalWrite(IN4, LOW);
     break;
    case DIR_RIGHT:
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
      break;
    case DIR_CCW:
     digitalWrite(IN1, HIGH);
     digitalWrite(IN2, LOW);
     digitalWrite(IN3, LOW);
     digitalWrite(IN4, HIGH);
     break;
    case DIR_LEFT:
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      break;
    case DIR_STOP:
     digitalWrite(IN1, LOW);
     digitalWrite(IN2, LOW);
     digitalWrite(IN3, LOW);
     digitalWrite(IN4, LOW);
     break;
  }
  if (spd == FASTEST) {
    digitalWrite(ENA, HIGH);
    digitalWrite(ENB, HIGH);
  }
  else {
    analogWrite(ENB, spd);
    analogWrite(ENA, spd);
  }
}

// the loop function runs over and over again forever
void loop() {
  int dir;
  
  // proximity sensors are LOW when triggered
  
  bool right = !digitalRead(RIGHT);
  bool left = !digitalRead(LEFT);

  
  if (left && right) dir = DIR_BACK;  // both sensors triggered
  else if (left) dir = DIR_LEFT;
  else if (right) dir = DIR_RIGHT;
  else dir = DIR_STOP;
  runMotors(dir, DEFAULT_SPEED);
}
