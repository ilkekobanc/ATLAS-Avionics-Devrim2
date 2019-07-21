#include <SoftwareSerial.h>

SoftwareSerial xbee(10 ,11); // (RX, TX)

bool started = false;
bool ended = false;
char incomingByte;
byte index;
byte packageIndex = 0;
char msg[20];
double package[6];

void setup() {
  Serial.begin(57600);
  xbee.begin(9600);
}


void loop() {
  while (xbee.available()) {
    incomingByte = xbee.read();
    if(incomingByte == '<') {
      index = 0;
      started = true;
      msg[index] = '\0';
    } else if (incomingByte == '>') {
      ended = true;
      break;
    } else {
      if(index < 20) {
        msg[index] = incomingByte;
        index++;
        msg[index] = '\0';
      }
    }
  }
  if(started && ended) {
      double val = atof(msg);
      package[packageIndex] = val;
      packageIndex++;
      if(packageIndex == 6) {
        packageIndex = 0;
        for(int i = 0; i < 6; i++) {
          if(i == 5) {
            Serial.println(package[i]);
          } else {
            Serial.print(package[i]);
            Serial.print(",");
          }
        }
      }
      started = false;
      ended = false;
      index = 0;
      msg[index] = '\0';
  }
}
