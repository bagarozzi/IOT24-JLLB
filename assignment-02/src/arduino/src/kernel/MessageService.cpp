#include "Arduino.h"
#include "MessageService.h"

void MessageService::init(){
    Serial.begin(9600);
    msgRecived.reserve(256);
    msgRecived = "";
    msgAvailable = NULL;
}

void serialEvent(){
    char c;
    while (Serial.available()){
        c = Serial.read(); 
        if(c != '\n'){
            MSGService.msgRecived += c;
        }
    }
    MSGService.msgAvailable = true;
}

bool MessageService::isMessageAivailable(){
    return msgAvailable;
}

String MessageService::recieveMessage(){
    msgAvailable = false;
    String app = msgRecived;
    msgRecived = "";
    return app;
}

void MessageService::sendMessage(String content){
    Serial.println(content);
}