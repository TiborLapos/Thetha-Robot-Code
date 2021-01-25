const int sensor = 5;
unsigned long previousTime = 0;
byte seconds;
int state; // 0 close - 1 open wwitch
int done_circle;
int i;



unsigned long StartTime; 

void setup() {
  pinMode(sensor, INPUT_PULLUP);
  Serial.begin(115200);
}

void loop() {
  Serial.println("test");
    motro();
     if (millis() >= (previousTime)) {
          previousTime = previousTime + 100;  // use 100000 for uS
          seconds = seconds + 1;
      }
}

void motro(){
 byte old_time; 

  if (seconds > 15){
       Serial.println("--------- ADDING SPEED ---------");
      }
  if (seconds > 60){
       Serial.println("-------- RESET -------");
         seconds = 0;
      }

      
  state = digitalRead(sensor);
  if (state == LOW){
    if (i < 1){
      done_circle = done_circle +1;
      i =  + 1;
      Serial.println(done_circle);
      old_time = seconds;
      seconds = 0;
     
    }
  }
   if (state == HIGH){
      i = 0;
      Serial.println(done_circle);
    }

    Serial.print("Old Time : ");
    Serial.println(old_time,DEC);
    Serial.print("New Time : ");
    Serial.println(seconds,DEC);


    


    

}
