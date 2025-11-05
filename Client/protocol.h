#ifndef PROTOCE
#define PROTOCE
#include <string.h>
#include <stdlib.h>

typedef unsigned int uint;

enum ENUM_MSG_TYPE{//枚举值定义消息类型
    ENUM_MSG_MIN=0,
    ENUM_MSG_TYPE_REGIST_REQUEST,
    ENUM_MSG_TYPE_REGIST_RESPOND,//注册要有两个，一个是服务器给客户端的响应，一个是客户端给服务器的请求
    ENUM_MSG_TYPE_LOGIN_REQUEST,
    ENUM_MSG_TYPE_LOGIN_RESPOND,
    ENUM_MSG_TYPE_FINDUSER_REQUEST,//查找用户
    ENUM_MSG_TYPE_FINDUSER_RESPOND,
    ENUM_MSG_TYPE_ADDFRIENT_REQUEST,//添加好友
    ENUM_MSG_TYPE_ADDFRIENT_RESPOND,
    ENUM_MSG_TYPE_AGREEADDFRIENT_REQUEST,//同意添加好友
    ENUM_MSG_TYPE_AGREEADDFRIENT_RESPOND,
    ENUM_MSG_TYPE_FLUSHFRIENT_REQUEST,
    ENUM_MSG_TYPE_FLUSHFRIENT_RESPOND,
    ENUM_MSG_TYPE_CHAT_REQUEST,//聊天
    ENUM_MSG_TYPE_CHAT_RESPOND,
    ENUM_MSG_TYPE_MKDIR_REQUEST,//创建文件
    ENUM_MSG_TYPE_MKDIR_RESPOND,
    ENUM_MSG_TYPE_FLUSHFILE_REQUEST,//刷新文件
    ENUM_MSG_TYPE_FLUSHFILE_RESPOND,
    ENUM_MSG_TYPE_MOVEFILE_REQUEST,//移动文件
    ENUM_MSG_TYPE_MOVEFILE_RESPOND,
    ENUM_MSG_TYPE_UPLOADINIT_REQUEST,//上传文件初始化
    ENUM_MSG_TYPE_UPLOADINIT_RESPOND,
    ENUM_MSG_TYPE_UPLOADDATA_REQUEST,//上传文件
    ENUM_MSG_TYPE_UPLOADDATA_RESPOND,

    ENUM_MSG_TYPE_MAX=0x00fffff,
};

struct PDU{//协议
    uint uiPDUlen;//总长度,阻止粘包半包
    uint uiMsglen;//数据长度
    uint uiMsgtype;//数据类型
    char caData[64];//固定大小，减少内存碎片
    char caMsg[];//柔性数组，用于存更长的实际数据的
};

PDU* mkPDU(uint uiMsglen = 0);

struct FileInfo{
    char caName[32];
    int iType;
};
#endif // PROTOCE

