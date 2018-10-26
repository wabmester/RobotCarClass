/*
  Robot Car Motor Test
*/

#define LED LED_BUILTIN

// IR proximity sensors (active low)
#define LEFT  4
#define RIGHT 2

#define FOLLOW_BLACK  0
#define FOLLOW_WHITE  1
#define FOLLOW_SAME 2
#define FOLLOW_NONE 3

#define FOLLOW_MODE FOLLOW_BLACK

#define LEFT_TRACK  13
#define RIGHT_TRACK 12

#define MAX_RETRY 4 // course corrections till we give up

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
#define SLOWER  (FAST/3)
#define STOP  0
#define DEFAULT_SPEED SLOWER

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

  pinMode(LEFT_TRACK, INPUT);
  pinMode(RIGHT_TRACK, INPUT);

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
  static int last_dir = DIR_STOP;
  static int retry = 0;
  int dir;
  
  // proximity sensors are LOW when triggered
  
  bool right = !digitalRead(RIGHT);
  bool left = !digitalRead(LEFT);
  bool left_track = !digitalRead(LEFT_TRACK);
  bool right_track = !digitalRead(RIGHT_TRACK);

  
  if (left && right) dir = DIR_BACK;  // both sensors triggered
  else if (left) dir = DIR_RIGHT;
  else if (right) dir = DIR_LEFT;
  else {
    // nothing is blocking the front, check the line
    switch (FOLLOW_MODE) {
      case FOLLOW_BLACK:
        if (left_track == right_track) {
          if (!left_track) dir = DIR_FORWARD; // neither triggered == on course
          else dir = DIR_STOP;  // both triggered, we're way off track
        }
        else if (left_track) dir = DIR_RIGHT;
        else dir = DIR_LEFT;
        break;
      case FOLLOW_WHITE:
        if (left_track == right_track) {
          if (left_track) dir = DIR_FORWARD; // both triggered == on course
          else dir = DIR_STOP;  // neither triggered, we're way off track
        }
        else if (left_track) dir = DIR_LEFT;
        else dir = DIR_RIGHT;
        break;
      case FOLLOW_SAME:
        if (left_track == right_track) dir = DIR_FORWARD; // same == on course
        else if (left_track) dir = DIR_RIGHT; // TODO: random left or right move?
        else dir = DIR_LEFT;
        break;
      case FOLLOW_NONE:
      default:
        dir = DIR_STOP;
    }    
  }
  if (dir == DIR_STOP) {
    digitalWrite(LED, HIGH); // signal that we're lost
    if (retry++ < MAX_RETRY) dir = last_dir; // let's try the previous maneuver one more time
    else retry = 0;
  }
  else digitalWrite(LED, LOW);
  runMotors(dir, DEFAULT_SPEED);
  last_dir = dir; // remember our last maneuver for next time in case we get really lost
}
