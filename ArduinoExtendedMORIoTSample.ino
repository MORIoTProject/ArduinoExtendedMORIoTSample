#include "MORIoTRS485.h"
#define SYSTEM_ID 128 //128以降でつける　ここがかぶると通信できなくなるので注意
#define COMMAND_TYPE_VERSION 1

#define VERSION_MAJOR 0
#define VERSION_MINOR 0
#define VERSION_REVISION 1
SoftwareSerial serial(10, 9);
MORIoTRS485 morIoTrs485;
void setup() {
  Serial.begin(115200);
  Serial.println("MORIoT Sample");
  morIoTrs485.begin(serial, 11);
}

void loop() {
  if (morIoTrs485.scan(SYSTEM_ID)) {
    uint8_t* request = morIoTrs485.getPacketData();
    uint8_t response[10];
    switch (morIoTrs485.getCommandId()) {

      case COMMAND_TYPE_VERSION://version
        response[0] = VERSION_MAJOR;
        response[1] = VERSION_MINOR;
        response[2] = VERSION_REVISION;
        morIoTrs485.send(SYSTEM_ID, COMMAND_TYPE_VERSION, 0, 3, response);
        Serial.println("Version Response");
        break;
    }
  }
}
