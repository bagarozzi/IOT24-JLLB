
#include "Logger.h"
#include <Arduino.h>
#include "MessageService.h"

void LoggerService::log(const String& msg){
    MSGService.sendMessage(msg);
}

