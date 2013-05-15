#include "include.h"
#include "Msender.h"

Msender::Msender(string di,bool fl)
{
  sfd=-1;
  destIp=di;
  ifUdp=fl;
  ifInit=false;
  memset(&addr, 0, sizeof(addr));
}

int Msender::init()
{
  if (ifUdp)
  {
    sfd=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
  }
  else
  {
    sfd=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  }
  if (sfd<0)
  {
    printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);
    return sfd;
  }
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(destIp.c_str());  
  addr.sin_port        = htons(PORT_LISTENING);
  if (!ifUdp)
  {
    int rst;
    rst=connect(sfd,(struct sockaddr *)&addr,sizeof(addr) );
    if (rst<0)
    {
      printf("connect error: %s(errno: %d)\n",strerror(errno),errno);
      return rst;
    }
  }
  ifInit=true;
  return 0;
}

int Msender::msend(string str2send)
{
  if (!ifInit)
  {
    cout<<"You must call init() before call send()"<<endl;
    return -2;
  }
  int rth=0,slth=str2send.size(),eth=0,ith=0;
  if (ifUdp)
  {
    while (slth>0)
    {
      eth=min(slth,MAX_DATA_LENGTH);
      rth=sendto(sfd,str2send.substr(ith,eth).c_str(),eth,0,(struct sockaddr *)&addr,sizeof(addr));
      if (rth<0)
      {
        printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
        return -1;
      }
      slth-=eth;
      ith+=eth;
    }
  }
  else
  {
    while (slth>0)
    {
      eth=min(slth,MAX_DATA_LENGTH);
      rth=send(sfd,str2send.substr(ith,eth).c_str(),eth,0);
      if (rth<0)
      {
        printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
        return -1;
      }
      slth-=rth;
      ith+=rth;
    }
  }
  return 0;
}

int Msender::bye()
{
  cout<<"Closing the connection"<<endl;
  close(sfd);
  return 0;
}
  
  
