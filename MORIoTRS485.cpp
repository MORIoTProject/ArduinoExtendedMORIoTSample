//
// Created by wami on 2021/10/04.
//

#include "MORIoTRS485.h"

bool MORIoTRS485::begin(SoftwareSerial &stream,uint8_t sendEnableIO) {
  _serial = &stream;
  _sendEnableIo = sendEnableIO;
  pinMode(_sendEnableIo,OUTPUT);
  setSendEnable(false);
  ((SoftwareSerial*)_serial)->begin(115200);
    return true;
}

void MORIoTRS485::send(uint8_t id, uint8_t command, uint8_t status, uint16_t length, uint8_t *data) {
  setSendEnable(true);
  delay(10);
  ((SoftwareSerial*)_serial)->write((int)0);
  ((SoftwareSerial*)_serial)->write(0xea);
  ((SoftwareSerial*)_serial)->write(id);
  ((SoftwareSerial*)_serial)->write(command);
  ((SoftwareSerial*)_serial)->write((length >> 8) & 0xff );
  ((SoftwareSerial*)_serial)->write(length& 0xff );
  ((SoftwareSerial*)_serial)->write(status);
  if(!(length == 0 && data == nullptr)){
    ((SoftwareSerial*)_serial)->write(data,length);
  }
  ((SoftwareSerial*)_serial)->flush();
  delay(10);
  setSendEnable(false);
  readPrev = millis();
}

void MORIoTRS485::setSendEnable(bool send) {
  digitalWrite(_sendEnableIo,send);
}

bool MORIoTRS485::scan(uint8_t id,uint8_t timeoutSec) {
    if (packetMode != PACKET_HEADER && millis() - readPrev > 1000 * timeoutSec) {
        Serial.println("MORIoTRS485 scan timeout");
        readPrev = millis();
        packetMode = PACKET_HEADER;
    }
    if (((SoftwareSerial *) _serial)->available()) {
        readPrev = millis();
        while (((SoftwareSerial *) _serial)->available() > 0) {
            int read = ((SoftwareSerial *) _serial)->read();
//            Serial.printf("scan:%x\n",read);
            if (packetMode == PACKET_HEADER) {
                if (read == 0xea) {
                    packetMode = PACKET_ID;
                }
            } else if (packetMode == PACKET_ID) {
                if (read == id) {
                    packetMode = PACKET_COMMAND;
                } else {
                    packetMode = PACKET_HEADER;
                }
            } else if (packetMode == PACKET_COMMAND) {
                packetCommand = read;
                packetMode = PACKET_LENGTH_H;
            } else if (packetMode == PACKET_LENGTH_H) {
                packetLength = (read << 8);
                packetAlreadyLength = 0;
                packetMode = PACKET_LENGTH_L;
            } else if (packetMode == PACKET_LENGTH_L) {
                packetLength = packetLength + read;
                if (packetLength == 0) {
                    packetMode = PACKET_HEADER;
                    return true;
                } else {
                    packetMode = PACKET_DATA;
                    packetAlreadyLength = 0;
                }
            } else if (packetMode == PACKET_DATA) {
                packetData[packetAlreadyLength] = read;
                packetAlreadyLength++;
                if (packetLength <= packetAlreadyLength) {
                    packetMode = PACKET_HEADER;
                    return true;
                }
            }
        }
    }
    return false;
}
