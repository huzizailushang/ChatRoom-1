#include "SocketManager.h"

void SocketInfo::Init()
{
    ser.sin_family = AF_INET;
    ser.sin_addr.s_addr = INADDR_ANY;
    ser.sin_port = htons(6666);
    sListen = socket(AF_INET, SOCK_STREAM, 0);
    bind(sListen, (struct sockaddr*)&ser, sizeof(ser));
    listen(sListen, 5);
    iLen = sizeof(cli);
}

int SocketInfo::AcceptSocket()
{
    int cSid = accept(sListen, (struct sockaddr *)&cli, &iLen);
    socket_s.insert(cSid);
    return cSid; 
}