#ifndef MSENDER_H
#define MSENDER_H

class Msender
{
 private:
  int sfd;
  string destIp;
  bool ifUdp,ifInit;
  struct sockaddr_in addr;
 public:
  Msender(string di,bool ifu);
  int init();
  int msend(string s2s);
  int bye();
};

#endif
