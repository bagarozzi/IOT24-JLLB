#ifndef __MSGSERVICE__
#define __MSGSERVICE__

#include "Arduino.h"

class MessageService {

public:
    String msgRecived;
    bool msgAvailable;
    
    void init();

    bool isMessageAivailable();
    
    String recieveMessage();

    void sendMessage(String content);
};

extern MessageService MSGService;

#endif