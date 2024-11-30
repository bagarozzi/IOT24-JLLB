#ifndef __MSGSERVICE__
#define __MSGSERVICE__

#include "Arduino.h"

class MessageService {

public:
    String msgReceived;
    bool msgAvailable;
    
    void init();

    bool isMessageAvailable();
    
    String receiveMessage();

    void sendMessage(String content);
};

extern MessageService MSGService;

#endif