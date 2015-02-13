#define BUFF_SIZE 128

#define LED     13
#define RELAY   2
#define NOTE_C4 60

void relayOn() {
  digitalWrite(RELAY, HIGH);
  digitalWrite(LED, HIGH);
}

void relayOff() {
  digitalWrite(RELAY, LOW);
  digitalWrite(LED, LOW);
}

void setup() {
  pinMode(RELAY, OUTPUT);
  pinMode(LED, OUTPUT);
  Serial.begin(31250);
  relayOff();
}

void loop() {
  char buffer[BUFF_SIZE];
  int count = 0;
  while (Serial.available() > 0 && count < BUFF_SIZE) {
    char buff = 0;
    Serial.readBytes(&buff, 1);
    buffer[count] = buff & 0xFF;
    ++count;
    delay(1);
  }
  while (count > 0) {
    int buff0 = buffer[0] & 0xFF;
    --count;
    
    if (count > 0) {
      int buff1 = buffer[1] & 0xFF;
      --count;
      
      if (count > 0) {
        int buff2 = buffer[2] & 0xFF;
        --count;
        
        if (buff0 == 0x90 /* note on */ && buff1 == NOTE_C4 && buff2 > 0) {
          relayOn();
        }
        else if (buff0 == 0x90 && buff1 == NOTE_C4 && buff2 == 0) {
          relayOff();
        }
        else if (buff0 == 0x80 /* note off */ && buff1 == NOTE_C4) {
          relayOff();
        }
      }
    }
  }
  delay(10);
}
