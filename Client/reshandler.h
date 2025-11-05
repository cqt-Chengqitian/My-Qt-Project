#ifndef RESHANDLER_H
#define RESHANDLER_H

#include "protocol.h"



class ResHandler
{
public:
    ResHandler(PDU* pdu);
    ~ResHandler();
    PDU* pdu;
    void regist();
    void login();
    void finduser();
    void addfriend();
    void addfriendresend();
    void addfriendagree();
    void flushfriend();
    void getchat();
    void mkdir();
    void flushfile();
    void movefile();
    void uploadFileInit();
    void uoloadFileData();
    void startUpload();


};

#endif // RESHANDLER_H
