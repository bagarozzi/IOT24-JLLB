#include "Arduino.h"
#include "MessageService.h"


MessageService MSGService;

void MessageService::init(){
    Serial.begin(9600);
    msgReceived.reserve(256);
    msgReceived = "";
    msgAvailable = NULL;
}

void serialEvent(){
    char c;
    while (Serial.available()){
        c = Serial.read(); 
        if(c != '\n'){
            MSGService.msgReceived += c;
        }
    }
    MSGService.msgAvailable = true;
}

bool MessageService::isMessageAvailable(){
    return msgAvailable;
}

String MessageService::receiveMessage(){
    msgAvailable = false;
    String app = msgReceived;
    msgReceived = "";
    return app;
}

void MessageService::sendMessage(String content){
    Serial.println(content);
}