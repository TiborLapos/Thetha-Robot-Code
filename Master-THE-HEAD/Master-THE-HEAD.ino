//developing brench

#include   <Wire.h>



// WHEEL SENZORS FOR ROTATION CHECK
bool stuck = false;


//FRONT LEFT
const int sensor = 7;
unsigned long previousTime = 0;
byte seconds;
int state; // 0 close - 1 open wwitch
int done_circle;
int i;
//



//
String str;
int num;
String text;
String motor_values ;
int count;

//
int dir;
int number;
bool controling = false;

unsigned long StartTime; 


void setup() {
  Serial.begin(115200); /* begin serial comm. */
  Wire.begin(0xA);
  Wire.onReceive(check_for_command);

  Serial.println("I am I2C Master");
 // Wire.setClock(3400000);

  // pinModes
  pinMode(sensor, INPUT_PULLUP);


  
}
void check_for_command(){
   while (Wire.available()) { // loop through all but the last
    number = Wire.read();
    Serial.print("data received:");
    if (controling == true){
          controls(number);
      }else{
        commands(number);
        Serial.println(number);
        }

  }
}




void loop() {
  check_for_command();

}

// CHECK FOR COMMAND 
void commands(int str) {
  Serial.println(str);
  if (str == 0){
    Serial.println("test");
    }
  if (str == 5) {
    Wire.beginTransmission(9);
    Wire.write("motor_go");
    Wire.endTransmission();
    controling = true;
  }


  /*
    Serial.println(str);
    
    if (str == "turn_left") {
      Serial.print("Command: " + text);
      Serial.println("Servo turn left ....");
      turn_left();
    }

    if (str == "turn_right") {
      Serial.print("Command: " + text);
      Serial.println("Servo turn right....");
      turn_right();
    }

    if (str == "turn_default") {
      Serial.print("Command: " + text);
      Serial.println("Servo turn right....");
      turn_default();
    }

    if (str == "check_motors_front") {
      Serial.print("command: " + text);
      check_motors_front_0X08();
    }

    if (str == "check_motors_back") {
      Serial.print("Command: " + text);
      check_motors_back_0X09();
    }
    if (str == 5) {
     Serial.print("Command: " + text);
     Wire.beginTransmission(9);
     Wire.write("motor_go");
     Wire.endTransmission();
     Wire.beginTransmission(8);
     Wire.write("motor_go");
     Wire.endTransmission();
     controling = true;
    }
    if (str == 0) {
     Serial.print("Command " + text);
     controling = false;
     motor_stop();
    }
    */
}

// Controling
void controls(int mov_dir){
  while(mov_dir == 1){
    Serial.println("Front");
    go_front();
    
    }
  
    
}




void go_stop() {
  going_speed(0, 0, 0, 0, 0);
}

void go_front() {
  going_speed(1, 40, 40, 40, 40);
  
}



void go_back() {
  going_speed(-1, 40, 40, 40, 40);
}

void go_back_stuck(){
 going_speed(-1, 100, 100, 100, 100);
}


void motor_stop(){
  Wire.beginTransmission(9);
  Wire.write("motor_stop");
  Wire.endTransmission();
  Wire.beginTransmission(8);
  Wire.write("motor_stop");
  Wire.endTransmission();
}


void going_speed(int stat, int fl, int fr, int bl, int br) {
  char buffer[15];
  sprintf(buffer, "%02d%04d%04d%04d%4d", stat, fl, fr, bl, br);
  Wire.beginTransmission(9);
  Wire.write(buffer);
  Wire.endTransmission();
  Wire.beginTransmission(8);
  Wire.write(buffer);
  Wire.endTransmission();
}







  void turn_default() {
    Wire.beginTransmission(9); /* begin with device address 9 */
    Wire.write("front_turn_default");  /* sends hello string */
    Wire.endTransmission();    /* stop transmitting */
    Wire.beginTransmission(8); /* begin with device address 8 */
    Wire.write("front_turn_default");  /* sends hello string */
    Wire.endTransmission();    /* stop transmitting */
    check_motors_front_0X08();
    check_motors_back_0X09();
  }


  void turn_left() {
    Wire.beginTransmission(9); /* begin with device address 8 */
    Wire.write("front_turn_left");  /* sends hello string */
    Wire.endTransmission();    /* stop transmitting */
    Wire.beginTransmission(8); /* begin with device address 8 */
    Wire.write("front_turn_left");  /* sends hello string */
    Wire.endTransmission();    /* stop transmitting */
    check_motors_front_0X08();
    check_motors_back_0X09();
  }



  void turn_right() {
    Wire.beginTransmission(8); /* begin with device address 8 */
    Wire.write("front_turn_right");  /* sends hello string */
    Wire.endTransmission();    /* stop transmitting */
    Wire.beginTransmission(9); /* begin with device address 9 */
    Wire.write("front_turn_right");  /* sends hello string */
    Wire.endTransmission();    /* stop transmitting */
    check_motors_front_0X08();
    check_motors_back_0X09();
  }




























  void check_motors_back_0X09() {
    Serial.println("-------MOTOR BACK------");
    String motor_values = "";
    Wire.beginTransmission(9); /* begin with device address 8 */
    Wire.write("motor_status");  /* sends hello string */
    Wire.endTransmission();    /* stop transmitting */

    Wire.requestFrom(9, 16);    // request 6 bytes from slave device #8

    while (Wire.available()) { // slave may send less than requested
      char c = Wire.read(); // receive a byte as character

      motor_values = motor_values + c;
      Serial.print(c);         // print the character
    }


    Serial.println();
    Serial.print("Values in STRING: ");         // print the character
    Serial.println(motor_values);

    Serial.println();
    String firstThree = motor_values.substring(0, 4);
    int number = firstThree.toInt();
    Serial.print("RIGHT STPPER VALUE: ");         // print the character
    Serial.println(number);




    String firstThree2 = motor_values.substring(5, 8);
    int number2 = firstThree2.toInt();
    Serial.print("LEFT STEPPER VALUE: ");         // print the character
    Serial.println(number2);



    String firstThree3 = motor_values.substring(8, 12);
    int number3 = firstThree3.toInt();
    Serial.print("RIGHT STEPPER DEGREE: ");         // print the character
    Serial.println(number3);


    String firstThree4 = motor_values.substring(12, 16);
    int number4 = firstThree4.toInt();
    Serial.print("LEFT STPPER DEGREE:  ");         // print the character
    Serial.println(number4);



    if (number4 == 90) {
      Serial.println("Can not turn to RIGHT");
    } else {
      Serial.println("Can turn to RIGHT");
    }
    if (number4 == -90) {
      Serial.println("Can not turn to LEFT");
    }
    else {
      Serial.println("Can turn to LEFT");
    }

  }


  void check_motors_front_0X08() {
    Serial.println("--------MOTOR FRONT-------");
    String motor_values = "";
    Wire.beginTransmission(8); /* begin with device address 8 */
    Wire.write("motor_status");  /* sends hello string */
    Wire.endTransmission();    /* stop transmitting */

    Wire.requestFrom(8, 20);    // request 6 bytes from slave device #8

    while (Wire.available()) { // slave may send less than requested
      char c = Wire.read(); // receive a byte as character

      motor_values = motor_values + c;
      Serial.print(c);         // print the character
    }

    
    Serial.println();
    Serial.print("Values in STRING: ");         // print the character
    Serial.println(motor_values);

    Serial.println();
    String firstThree = motor_values.substring(0, 4);
    int number = firstThree.toInt();
    Serial.print("RIGHT STPPER VALUE: ");         // print the character
    Serial.println(number);



    String firstThree2 = motor_values.substring(5, 8);
    int number2 = firstThree2.toInt();
    Serial.print("LEFT STEPPER VALUE: ");         // print the character
    Serial.println(number2);



    String firstThree3 = motor_values.substring(8, 12);
    int number3 = firstThree3.toInt();
    Serial.print("RIGHT STEPPER DEGREE: ");         // print the character
    Serial.println(number3);


    String firstThree4 = motor_values.substring(12, 16);
    int number4 = firstThree4.toInt();
    Serial.print("LEFT STPPER DEGREE:  ");         // print the character
    Serial.println(number4);


    String firstThree5 = motor_values.substring(16, 20);
    int number5 = firstThree5.toInt();
    Serial.print("MOTOR SPEED FROTN:  ");         // print the character
    Serial.println(number5);

    if (number4 == 90) {
      Serial.println("Can not turn to RIGHT");
    } else {
      Serial.println("Can turn to RIGHT");
    }
    if (number4 == -90) {
      Serial.println("Can not turn to LEFT");
    }
    else {
      Serial.println("Can turn to LEFT");
    }

}
