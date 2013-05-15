#include "include.h"
#include "Mdaemon.h"

extern bool sigExit;

Mdaemon::Mdaemon()
{
  tfd=ufd=-1;
  ifUdp=ifInit=false;
  memset(&addr, 0, sizeof(addr));
}

int Mdaemon::init(bool udpflag)
{
  ifUdp=udpflag;
  ufd=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
  if (ufd<0)
  {
    printf("create UDP socket error: %s(errno: %d)\n", strerror(errno),errno);
    return ufd;
  }
  tfd=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (tfd<0)
  {
    printf("create TCP socket error: %s(errno: %d)\n", strerror(errno),errno);
    return tfd;
  }
  addr.sin_family=AF_INET;
  addr.sin_addr.s_addr=htonl(INADDR_ANY);
  addr.sin_port=htons(PORT_LISTENING);
  int rst=0;
  rst=bind(tfd,(struct sockaddr*)&addr,sizeof(struct sockaddr_in));
  if (rst<0)
  {
     printf("bind TCP socket error: %s(errno: %d)\n",strerror(errno),errno);
     return rst;
  }
  rst=bind(ufd,(struct sockaddr*)&addr,sizeof(struct sockaddr_in));
  if (rst<0)
  {
     printf("bind UDP socket error: %s(errno: %d)\n",strerror(errno),errno);
     return rst;
  }
  rst=listen(tfd,0);
  if (rst<0)
  {
     printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);
     return rst;
  }
  ifInit=true;
  return 0;
}

int Mdaemon::run()
{
  if (!ifInit)
  {
    cout<<"You must call init() before call run()"<<endl;
    return -2;
  }
  cout<<"Daemon is running"<<endl;
  int addrlen=sizeof(struct sockaddr_in),revLen=0;
  socklen_t alen=sizeof(struct sockaddr);
  struct sockaddr_in cliAddr;
  char buff[MAX_DATA_LENGTH+1];
  while (1)
  {
    if (sigExit)
    {
      return 0;
    }
    if (ifUdp)
    {
      memset(buff, 0, sizeof(buff));
      revLen=recvfrom(ufd,buff,sizeof(buff),0,(struct sockaddr *)&cliAddr,&alen);
      if (revLen<=0)
      {
        cout<<"seems UDP error"<<endl;
      }
      else
      {
        printf("UDP receive from %s :\n",inet_ntoa(cliAddr.sin_addr));
        buff[revLen]='\0';
        printf("%s",buff);
      }
    }
    else
    {
      int csfd=accept(tfd,(struct sockaddr*) &cliAddr,&alen);
      if (csfd<=0)
      {
        printf("accept TCP socket error: %s(errno: %d)\n",strerror(errno),errno);
        continue;
      }
      printf("received a connection from %s\n", inet_ntoa(cliAddr.sin_addr));
      while (1)
      {
        revLen=recv(csfd,buff,MAX_DATA_LENGTH,0);
        if (revLen<=0)
        {
          break;
        }
        else
        {
          buff[revLen]='\0';
          printf("%s",buff);
        }
      }
      if (revLen<0)
      {
        cout<<"Error during receiving"<<endl;
      }
      close(csfd);
    }
  }
}

int Mdaemon::bye()
{
  cout<<"Closing the TCP connection"<<endl;
  close(tfd);
  return 0;
}
