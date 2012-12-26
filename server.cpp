#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include "SocketManager.h"

using namespace std;

#define RECV_BUF_SIZE 1024

void send_msg(int sid);
void accept_client();
void server_socket();

char    name_data[10];
char    buf_recv[RECV_BUF_SIZE];
int     fdp_max(0);

fd_set              fds;
set<int>            socket_s;
map<int,string>     name_buf;

struct timeval      timeout;
SocketInfo          sInfo;

int main(int argc, char* argv[])
{
    memset(buf_recv,0,sizeof(buf_recv));
    timeout.tv_sec = timeout.tv_usec = 0;
    sInfo.init();

    while(1)
    {
        FD_ZERO(&fds);
        FD_SET(sInfo.sListen, &fds);
        for(set<int>::iterator it = socket_s.begin(); it != socket_s.end(); it++)
        {
            FD_SET(*it, &fds);
            fdp_max = fdp_max < *it ? *it:fdp_max;
        }
        fdp_max = fdp_max < sInfo.sListen ? sInfo.sListen:fdp_max;
        switch(select(fdp_max +1, &fds, NULL, NULL, &timeout))
        {
            case -1:
                cout << "select error!" << endl;
                break;
            case 0:
                break;
            default:
                if(socket_s.size() > 0)
                {
                    for(set<int>::iterator it = socket_s.begin(); it != socket_s.end(); it++)
                    {
                        if(FD_ISSET(*it, &fds))
                        {
                            send_msg(*it);
                        }
                    }
                }
                if(FD_ISSET(sInfo.sListen, &fds))
                {
                    accept_client();
                }
        }
    }

    return 0;
}
// �߳�1:������Ϣ�������ͻ���
void send_msg(int sid)
{
    char name_c[30] = ""; 
    const char* name_m = name_buf[sid].c_str(); 
    int recv_int = recv(sid, buf_recv, sizeof(buf_recv), 0);
    if(recv_int > 0)
    {
        cout << "recv data from " << name_m << ": " << buf_recv << endl;
        if(socket_s.size() > 1)
        {
            strcat(name_c, name_m);
            strcat(name_c, ": ");
            strcat(name_c, buf_recv);

            for(set<int>::iterator it = socket_s.begin(); it != socket_s.end(); it++)
            {
                if(*it != sid)
                {
                    send(*it, name_c, sizeof(name_c),0);
                }
            }
        }
    }
    else
    {
        socket_s.erase(sid);
        cout << sid << "has removed" << endl;
    }
}
// �߳�2:�����ͻ�����������
void accept_client()
{
    int tmp = accept(sInfo.sListen, (struct sockaddr *)&sInfo.cli, &sInfo.iLen);
    // ����client�û�������name_data
    recv(tmp, name_data, sizeof(name_data),0);
    name_buf.insert(pair<int,string>(tmp, name_data)); 
    socket_s.insert(tmp);
    cout << "port:" << ntohs(sInfo.cli.sin_port) << " num:" << socket_s.size() << endl;
}
