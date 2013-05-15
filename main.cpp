#include "include.h"
#include "Mdaemon.h"
#include "Msender.h"

void showUsage()
{
  cout<<"MiaoW or MiaoW -s -d destip (-u)"<<endl;
  exit(0);
}

bool sigExit=false;

void safeExit(int sig)
{
  sigExit=true;
  exit(0);
}

int main(int argc, char *argv[])
{
  bool ifSend=false,ifIp=false,ifUdp=false;
  string destIp;
  int oc;
  while((oc=getopt(argc,argv,"d:suh"))!=-1)
  {
    switch(oc)
    {
    case 'd':
      destIp=optarg;
      ifIp=true;
      break;
    case 's':
      ifSend=true;
      break;
    case 'u':
      ifUdp=true;
      break;
    case 'h':
      showUsage();
      break;
    case '?':
      cout<<"Wrong parameters"<<endl;
      showUsage();
      break;
    }
  }
  if (ifSend)
  {
    if (!ifIp)
    {
      cout<<"Must provide destination ip address"<<endl;
      showUsage();
    }
    Msender sender(destIp,ifUdp);
    if (sender.init())
    {
      cout<<"Sender initialization fail,exiting"<<endl;
      exit(-1);
    }
    else
    {
      if (!ifUdp)
      {
        cout<<"TCP connection established"<<endl;
      }
    }
    string rdstr;
    while (getline(cin,rdstr))
    {
      int rst=sender.msend(rdstr);
      if (rst)
      {
        cout<<"Fail to send,exiting"<<endl;
        break;
      }
    }
    sender.bye();
  }
  else
  {
    signal(SIGHUP,safeExit);
    signal(SIGINT,safeExit);
    //signal(SIGKILL,tuichu);
    signal(SIGSTOP,safeExit);
    signal(SIGTERM,safeExit);
    Mdaemon daemon;
    if (daemon.init(ifUdp))
    {
      cout<<"dameon fail,exiting"<<endl;
      exit(-1);
    }
    daemon.run();
    daemon.bye();
  }
  return 0;
}

