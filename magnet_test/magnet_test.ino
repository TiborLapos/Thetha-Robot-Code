const int sensor = 4;
unsigned long previousTime = 0;
byte seconds;



int state; // 0 close - 1 open wwitch

int korte;

int alma;
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
  byte test; 

  if (seconds > 15){
       Serial.println("__________________________________________________________add speed");
      }
  if (seconds > 60){
       Serial.println("__________________________________________________________rest");
         seconds = 0;
      }

      
  state = digitalRead(sensor);
  if (state == LOW){
    if (i < 1){
      alma = alma +1;
      i =  + 1;
      Serial.println(alma);
      test = seconds;
      seconds = 0;
     
    }
  }
   if (state == HIGH){
      i = 0;
      Serial.println(alma);
    }

    Serial.print("OLD: ");
    Serial.println(test,DEC);
    Serial.print("NEW: ");
    Serial.println(seconds,DEC);


    


    

}
