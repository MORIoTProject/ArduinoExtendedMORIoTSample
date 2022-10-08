//
// Created by wami on 2022/06/19
//

#ifndef MORIOT_RS485_H
#define MORIOT_RS485_H
#define PACKET_HEADER 0
#define PACKET_ID 1
#define PACKET_COMMAND 2
#define PACKET_LENGTH_H 3
#define PACKET_LENGTH_L 4
#define PACKET_DATA 5
#include "Arduino.h"
#include <SoftwareSerial.h>
class MORIoTRS485 {
public:
    bool begin(SoftwareSerial &stream,uint8_t sendEnableIO);
    void send(uint8_t id,uint8_t command,uint8_t status,uint16_t length,uint8_t*data);
    bool scan(uint8_t id,uint8_t timeoutSec = 1);
    uint16_t getLength() const{
      return packetLength;
    }
    uint8_t getCommandId() const{
      return packetCommand;
    }
    uint8_t* getPacketData(){
      return packetData;
    }
private:
    unsigned long readPrev = 0;
    void setSendEnable(bool send);
    SoftwareSerial* _serial;
    uint8_t _sendEnableIo;
    uint8_t packetMode = PACKET_HEADER;
    uint8_t packetCommand = 0;
    uint16_t packetLength = 0;
    uint16_t packetAlreadyLength = 0;
    uint8_t packetData[1024];//WiSUNで一度に送れるデータ長が10024byteが限界なので（Headerなど入れないといけないのでもう少し短くなる）
};

#endif //MORIOT_RS485_H
