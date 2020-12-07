

#include <Wire.h>
#define ledPin 13

String values;


//For STEPPER TURNING
const int DirPin1 = 47;    // this pin defines direction CW or CCW
const int StepPin1 = 46;   // pulse this pin to move one step
const int DirPin2 = 43;    // this pin defines direction CW or CCW
const int StepPin2 = 42;   // pulse this pin to move one step
const int SPR = 0;    // Steps per revolution
int a;
int x;
int right_stepper_position;
int left_stepper_position;
int right_position_degree;
int left_position_degree;
int max_stepper_position_positive = 1000;
int max_stepper_position_negative = -1000;
//END Stepper turning
int motor_go_status;

//Motor 1
int RM_RPWM = 3;
int RM_LPWM = 11;
int RM_L_EN = 7;
int RM_R_EN = 8;
//Motor 2
int LM_RPWM = 2;
int LM_LPWM = 12;
int LM_L_EN = 5;
int LM_R_EN = 6;

void setPWMfrequency(int freq) {
  TCCR1B = TCCR2B & 0b11111000 | freq ;
  TCCR3B = TCCR2B & 0b11111000 | freq ;
}

void MotorActiveStatus1(char Side, boolean s) {
  boolean state = s;
  if (Side == 'R') {
    digitalWrite(LM_R_EN, s);
  }
  if (Side == 'L') {
    digitalWrite(LM_L_EN, s);
  }
}

void MotorActiveStatus2 (char Side, boolean s) {
  if (Side == 'R') {
    digitalWrite(RM_R_EN, s);
  }
  if (Side == 'L') {
    digitalWrite(RM_L_EN, s);
  }
}

void setMotor1(char side, byte pwm) {
  if (side == 'R') {
    analogWrite(LM_RPWM, pwm);
  }
  if (side == 'L') {
    analogWrite(LM_LPWM, pwm);
  }
}

void setMotor2(char side, byte pwm) {
  if (side == 'R') {
    analogWrite(RM_RPWM, pwm);
  }
  if (side == 'L') {
    analogWrite(RM_LPWM, pwm);
  }
}

void closeMotor1(char side) {
  if (side == 'R') {
    digitalWrite(LM_RPWM, LOW);
  }
  if (side == 'L') {
    digitalWrite(LM_LPWM, LOW);
  }
}

void closeMotor2(char side) {
  if (side == 'R') {
    digitalWrite(RM_RPWM, LOW);
  }
  if (side == 'L') {
    digitalWrite(RM_LPWM, LOW);
  }
}








void setup() {
  setPWMfrequency(0x02);// timer 2 , 3.92KHz
  Wire.begin(8);                /* join i2c bus with address 9 */
  Wire.onReceive(receiveString); /* register receive event  */
  Serial.begin(9600);           /* start serial comm. */
  Serial.println("BACK | I am I2C Slave 0X08");
  Wire.setClock(3400000); //Fast comunication

  // LED
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // STEPPER
  pinMode(StepPin1, OUTPUT);
  pinMode(DirPin1, OUTPUT);
  pinMode(StepPin2, OUTPUT);
  pinMode(DirPin2, OUTPUT);

  // MOTORS
  for (int i = 5; i < 9; i++) {
    pinMode(i, OUTPUT);
  }
  for (int i = 5; i < 9; i++) {
    digitalWrite(i, LOW);
  }
  delay(1000);

  MotorActiveStatus1('R', true);
  MotorActiveStatus1('L', true);
  MotorActiveStatus2('R', true);
  MotorActiveStatus2('L', true);
}





void loop() {
}









void receiveString(int bytes) { //Recive data from MASTER
  String text = "";
  while (Wire.available()) {
    char c = Wire.read(); // receive a byte as character
    text += c;
  }
  Serial.print("Printing: ");
  Serial.println(text);
  commands(text);
  values = text;
  
}


void commands(String cmd) { //RUN Recived code
  if (cmd == "ON") {
    Wire.onRequest(turn_on); /* register request event */
    turn_on();
  }
  else if (cmd == "OFF") {
    Wire.onRequest(turn_off); /* register request event */
    turn_off();
  }
  else if (cmd == "front_turn_left") {
    Wire.onRequest(front_turn_left); /* register request event */
    front_turn_left();
  }
  else if (cmd == "front_turn_default") {
    Wire.onRequest(front_turn_default); /* register request event */
    front_turn_default();
  }
  else if (cmd == "front_turn_right") {
    Wire.onRequest(front_turn_right); /* register request event */
    front_turn_right();
  } else if (cmd == "motor_status") {
    Wire.onRequest(motors_status); /* register request event */
    motors_status();
  } else if (cmd == "motor_go") {
    Wire.onRequest(motor_check); /* register request event */
    motor_check(values);
  }
  else {
    Serial.println("Unknown command: " + cmd);
  }
}





//MOTORS FRONT

void motor_go_front(int i) {
  Serial.println("FRONT");
  setMotor1('L', i);
  setMotor2('L', i);
}

void motor_go_right(int i) {
  Serial.print("Turning right speed:  ");
  Serial.println(i);
  setMotor1('L', i);
  setMotor2('L', i);
}

void motor_go_back(int i) {
  Serial.println("BACK");
  setMotor1('R', i);
  setMotor2('R', i);
}

void motor_go_left(int i) {
  Serial.print("Turning left speed: ");
  Serial.println(i);
  setMotor1('R', i);
  setMotor2('R', i);
}

void motor_go_stop() {
  Serial.println("STOP");
  closeMotor1('R');
  closeMotor2('R');
  closeMotor1('L');
  closeMotor2('L');
}


//STEPPER MOTORS FRONT
void motors_posotions(int left_stepper, int right_stepper, int right_position_degere, int left_position_degere, int motor_speed) {
  right_stepper_position = right_stepper_position + left_stepper;
  left_stepper_position = left_stepper_position + right_stepper;
  right_position_degree = right_position_degree + right_position_degere;
  left_position_degree = left_position_degree + left_position_degere;
  motor_go_status = motor_speed;


  Serial.print("LEFT STEP: ");
  Serial.print(left_stepper_position);
  Serial.print(" | DEGRE: ");
  Serial.print(left_position_degree);
  Serial.println("°");
  Serial.print("RIGHT STEP: ");
  Serial.print(right_stepper_position);
  Serial.print(" | DEGRE: ");
  Serial.print(right_position_degree);
  Serial.println("°");
  Serial.print("MOTOR SPEED: ");
  Serial.print(motor_go_status);
  Serial.println("");

}





void motors_status() {
  char buffer[26];
  sprintf(buffer, "%04d%04d%04d%04d%04d", right_stepper_position, left_stepper_position, right_position_degree, left_position_degree, motor_go_status);
  Serial.println(buffer);
  Wire.write(buffer);
}








void motor_check(String values) {
  Serial.println("Checking motors");
  Serial.println(values);
  Serial.println();
  Serial.print("Values in STRING: ");        
  Serial.println(values); 

  Serial.println();
  String status_way = values.substring(0,2);
  int way = status_way.toInt();
  Serial.print("What to do : ");        
  Serial.println(way); 

  Serial.println();
  String front_values1 = values.substring(2,6);
  int number_front1 = front_values1.toInt();
  Serial.print("Front Left value: ");        
  Serial.println(number_front1); 

  Serial.println();
  String front_values2 = values.substring(6,10);
  int number_front2 = front_values2.toInt();
  Serial.print("Front Right value: ");        
  Serial.println(number_front2); 

  Serial.println();
  String back_values1 = values.substring(10,14);
  int number_back1 = back_values1.toInt();
  Serial.print("Back Left value: ");        
  Serial.println(number_back1); 

  Serial.println();
  String back_values2 = values.substring(14,18);
  int number_back2 = back_values2.toInt();
  Serial.print("Back Right value: ");        
  Serial.println(number_back2); 
 Serial.println();
}









//   TURN THE ALL FRONT MOTOR TO DEFAULT POSITION
void front_turn_default() {
  const int SPR = 50;    // Steps per revolution
  Wire.write("Motor was turned default");  /*send string on request */
  Serial.println("Tuning default ....");

  //CHECK IFF THE MOTOR IS ON RIGHT SID OR ON LEFT
  if (right_stepper_position > 0) {
    //Steper -- Turning LEFT
    int a = right_stepper_position / SPR;
    Serial.println(a);
    digitalWrite(DirPin1, HIGH);   // <-- MOTOR POSITION TURNING
    digitalWrite(DirPin2, HIGH);   // <-- MOTOR POSITION TURNING
    motor_go_left(35);
    for (a > 0; a--;) {
      for ( x = 0; x <=  SPR; x++) {
        Serial.print(a);
        Serial.print(" | ");
        Serial.println(x);
        digitalWrite(StepPin1, HIGH);
        digitalWrite(StepPin2, HIGH);
        delayMicroseconds(5000); //5000
        digitalWrite(StepPin1, LOW);
        digitalWrite(StepPin2, LOW);
        delayMicroseconds(15000); //15000
      }

      motors_posotions(-SPR, -SPR, -18, -18, 0);
    }
  }
  motor_go_stop();
  if (left_stepper_position < 0) {
    //Steper -- Turning RIGHT
    int a = right_stepper_position / SPR;
    Serial.println(a);
    digitalWrite(DirPin1, LOW);   // <-- MOTOR POSITION TURNING
    digitalWrite(DirPin2, LOW);   // <-- MOTOR POSITION TURNING
    motor_go_right(35);
    for (a < 0; a++;) {
      for ( x = 0; x <=  SPR; x++) {
        Serial.print(a);
        Serial.print(" | ");
        Serial.println(x);
        digitalWrite(StepPin1, HIGH);
        digitalWrite(StepPin2, HIGH);
        delayMicroseconds(5000); //5000
        digitalWrite(StepPin1, LOW);
        digitalWrite(StepPin2, LOW);
        delayMicroseconds(15000); //15000
      }

      motors_posotions(+SPR, +SPR, +18, +18, 0);
    }
  }
  motor_go_stop();




}

//   TURN THE ALL FRONT MOTOR TO RIGHT
void front_turn_right() {
  const int SPR = 50;    // Steps per revolution
  Serial.println("Tuning RIGHT ....");
  if (right_stepper_position < 250 || right_position_degree < 90 && left_stepper_position < 250 || left_position_degree < 90) {
    //Steper -- Turning RIGH
    turn_on();
    digitalWrite(DirPin1, LOW);   // <-- MOTOR POSITION TURNING
    digitalWrite(DirPin2, LOW);   // <-- MOTOR POSITION TURNING
    motor_go_right(35);
    for ( x = 0; x <=  SPR; x++) {
      Serial.print(a);
      Serial.print(" | ");
      Serial.println(x);
      digitalWrite(StepPin1, HIGH);
      digitalWrite(StepPin2, HIGH);
      delayMicroseconds(5000); //5000
      digitalWrite(StepPin1, LOW);
      digitalWrite(StepPin2, LOW);
      delayMicroseconds(15000); //15000
    }
    motors_posotions(SPR, SPR, 18, 18, 0);
    delay(100);
    turn_off();
    motor_go_stop();
  } else {
    turn_off();
    Serial.println("Can not turn to RIGHT !");
  }




}
//    TURN THE ALL FRONT MOTOR TO LEFT
void front_turn_left() {

  const int SPR = 50 ;    // Steps per revolution
  Serial.println("Tuning LEFT ....");
  if (right_stepper_position > -250 || right_position_degree > -90 && left_stepper_position > -250 || left_position_degree > -90) {
    turn_on();
    //Steper -- Turning LEFT
    digitalWrite(DirPin1, HIGH);   // <-- MOTOR POSITION TURNING
    digitalWrite(DirPin2, HIGH);   // <-- MOTOR POSITION TURNING
    motor_go_left(35);
    for ( x = 0; x <=  SPR; x++) {
      Serial.print(a);
      Serial.print(" | ");
      Serial.println(x);
      digitalWrite(StepPin1, HIGH);
      digitalWrite(StepPin2, HIGH);
      delayMicroseconds(5000); //5000
      digitalWrite(StepPin1, LOW);
      digitalWrite(StepPin2, LOW);
      delayMicroseconds(15000); //15000
    }
    delay(100);
    Wire.write("Motor was turned right");  /*send string on request */
    turn_off();
    motor_go_stop();
    motors_posotions(-SPR, -SPR, -18, -18, 0);
  } else {
    turn_off();
    Serial.println("Can not turn to left");

  }
}


void turn_on() {
  digitalWrite(ledPin, HIGH);
}
void turn_off() {
  digitalWrite(ledPin, LOW);
}
