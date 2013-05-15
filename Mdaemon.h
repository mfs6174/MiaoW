#ifndef MDAEMON_H
#define MDAEMON_H

class Mdaemon
{
 private:
  int tfd,ufd;
  bool ifInit,ifUdp;
  struct sockaddr_in addr;
 public:
  Mdaemon();
  int init(bool);
  int run();
  int bye();
};


#endif
