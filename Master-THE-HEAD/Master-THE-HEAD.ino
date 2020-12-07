#include   <Wire.h>

#define joyX A0
#define joyY A1
 





String text;
String motor_values ;
void setup() {
 Serial.begin(9600); /* begin serial comm. */
 Wire.begin(); /* join i2c bus as master */
 Serial.println("I am I2C Master");
 Wire.setClock(3400000);


}


//Adat kuldés a egyes boardokra a Serialon keresztul 
void loop() {
  int x;
  String str;
   if(Serial.available() > 0)
      {
          str = Serial.readStringUntil('\n');         
          Serial.println("Command : " + str);
      }
      
  if(str.length() > 0) {   
      if (str == "turn_left"){
        Serial.print("Send: " +text);
        Serial.println("Servo turn left ....");
        turn_left();
        }
        
     if (str == "turn_right"){
        Serial.print("Send: " +text);
        Serial.println("Servo turn right....");
        turn_right();
        }
        
    if (str == "turn_default"){
      Serial.print("Send: " +text);
      Serial.println("Servo turn right....");
      turn_default();
      }
      
    if (str == "check_motors_front"){
      Serial.print("Send: " +text);
      check_motors_front_0X09();
    }
    
    if (str == "check_motors_back"){
      Serial.print("Send: " +text);
      check_motors_back_0X08();
    }
    
    if (str == "controling"){
      Serial.print("Send: " +text);
      control_moovment();
    }  
           
   }
 delay(1000);
}





void control_moovment(){
    String str;
    while (true){
    if(Serial.available() > 0){
          str = Serial.readStringUntil('\n');         
          Serial.println("Command : " + str);
      }

      if (str != "stop_control"){
      // put your main code here, to run repeatedly:
      int xValue = analogRead(joyX);
      int yValue = analogRead(joyY);
     
      //print the values with to plot or view
      Serial.print(xValue);
      Serial.print("\t");
      Serial.println(yValue);

      if (xValue >= 1000){
        go_front();
        }
     if (xValue <= 10){
      go_back();
      
      }
     }else{
      break;
      }
  }
}


  



  
void go_front(){
  Wire.beginTransmission(9); 
  Wire.write("motor_go");  
  Wire.endTransmission();  
  Wire.beginTransmission(8); 
  Wire.write("motor_go");  
  Wire.endTransmission(); 
  going_speed(1,35,35,35,35);
}

void go_back(){
  Wire.beginTransmission(9); 
  Wire.write("motor_go");  
  Wire.endTransmission();  
  Wire.beginTransmission(8); 
  Wire.write("motor_go");  
  Wire.endTransmission(); 
  going_speed(-1,35,35,35,35);
}


void going_speed(int stat, int fl, int fr, int bl, int br){  
  char buffer[15];
  sprintf(buffer,"%02d%04d%04d%04d%4d",stat, fl,fr,bl,br);
  Wire.beginTransmission(9); 
  Wire.write(buffer);  
  Wire.endTransmission();
  Wire.beginTransmission(8); 
  Wire.write(buffer);  
  Wire.endTransmission();   
  }
  
void turn_default(){
   Wire.beginTransmission(9); /* begin with device address 9 */
   Wire.write("front_turn_default");  /* sends hello string */
   Wire.endTransmission();    /* stop transmitting */
   Wire.beginTransmission(8); /* begin with device address 8 */
   Wire.write("front_turn_default");  /* sends hello string */
   Wire.endTransmission();    /* stop transmitting */
   check_motors_front_0X09();
   check_motors_back_0X08();
  }


void turn_left(){
   Wire.beginTransmission(9); /* begin with device address 8 */
   Wire.write("front_turn_left");  /* sends hello string */
   Wire.endTransmission();    /* stop transmitting */
   Wire.beginTransmission(8); /* begin with device address 8 */
   Wire.write("front_turn_left");  /* sends hello string */
   Wire.endTransmission();    /* stop transmitting */
   check_motors_front_0X09();
   check_motors_back_0X08(); 
  }


 
void turn_right(){
   Wire.beginTransmission(8); /* begin with device address 8 */
   Wire.write("front_turn_right");  /* sends hello string */
   Wire.endTransmission();    /* stop transmitting */
   Wire.beginTransmission(9); /* begin with device address 9 */
   Wire.write("front_turn_right");  /* sends hello string */
   Wire.endTransmission();    /* stop transmitting */
   check_motors_front_0X09();
   check_motors_back_0X08();
  }




























void check_motors_back_0X08(){
  Serial.println("-------MOTOR BACK------");
  String motor_values = "";
  Wire.beginTransmission(8); /* begin with device address 8 */
  Wire.write("motor_status");  /* sends hello string */
  Wire.endTransmission();    /* stop transmitting */
   
  Wire.requestFrom(8, 16);    // request 6 bytes from slave device #8

  while (Wire.available()) { // slave may send less than requested
    char c = Wire.read(); // receive a byte as character

    motor_values = motor_values + c;
    Serial.print(c);         // print the character
  }

 
  Serial.println();
  Serial.print("Values in STRING: ");         // print the character
  Serial.println(motor_values); 
  
  Serial.println();
  String firstThree = motor_values.substring(0,4);
  int number = firstThree.toInt();
  Serial.print("RIGHT STPPER VALUE: ");         // print the character
  Serial.println(number); 
  
  
  
  
  String firstThree2 = motor_values.substring(5,8);
  int number2 = firstThree2.toInt();
  Serial.print("LEFT STEPPER VALUE: ");         // print the character
  Serial.println(number2); 
  
  
  
  String firstThree3 = motor_values.substring(8,12);
  int number3 = firstThree3.toInt();
  Serial.print("RIGHT STEPPER DEGREE: ");         // print the character
  Serial.println(number3); 
 
  
  String firstThree4 = motor_values.substring(12,16);
  int number4 = firstThree4.toInt();
  Serial.print("LEFT STPPER DEGREE:  ");         // print the character
  Serial.println(number4); 

   
  
  if (number4 == 90){
    Serial.println("Can not turn to RIGHT");
    } else {
      Serial.println("Can turn to RIGHT");
      }
  if (number4 == -90){
    Serial.println("Can not turn to LEFT");
    }
    else {
      Serial.println("Can turn to LEFT");
      }

  }


void check_motors_front_0X09(){
  Serial.println("--------MOTOR FRONT-------");
  String motor_values = "";
   Wire.beginTransmission(9); /* begin with device address 8 */
   Wire.write("motor_status");  /* sends hello string */
   Wire.endTransmission();    /* stop transmitting */
   
   Wire.requestFrom(9, 20);    // request 6 bytes from slave device #8

    while (Wire.available()) { // slave may send less than requested
      char c = Wire.read(); // receive a byte as character

      motor_values = motor_values + c;
      Serial.print(c);         // print the character
    }
  Serial.println();

  Serial.println();
  Serial.print("Values in STRING: ");         // print the character
  Serial.println(motor_values); 
  
  Serial.println();
  String firstThree = motor_values.substring(0,4);
  int number = firstThree.toInt();
  Serial.print("RIGHT STPPER VALUE: ");         // print the character
  Serial.println(number); 
  
  
  
  
  String firstThree2 = motor_values.substring(5,8);
  int number2 = firstThree2.toInt();
  Serial.print("LEFT STEPPER VALUE: ");         // print the character
  Serial.println(number2); 
  
  
  
  String firstThree3 = motor_values.substring(8,12);
  int number3 = firstThree3.toInt();
  Serial.print("RIGHT STEPPER DEGREE: ");         // print the character
  Serial.println(number3); 
 
  
  String firstThree4 = motor_values.substring(12,16);
  int number4 = firstThree4.toInt();
  Serial.print("LEFT STPPER DEGREE:  ");         // print the character
  Serial.println(number4); 


  String firstThree5 = motor_values.substring(16,20);
  int number5 = firstThree5.toInt();
  Serial.print("MOTOR SPEED FROTN:  ");         // print the character
  Serial.println(number5); 
  
  if (number4 == 90){
    Serial.println("Can not turn to RIGHT");
    } else {
      Serial.println("Can turn to RIGHT");
      }
  if (number4 == -90){
    Serial.println("Can not turn to LEFT");
    }
    else {
      Serial.println("Can turn to LEFT");
      }
    
  }
  




  
