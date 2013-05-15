GPP=g++ -g -pg -Wall -o
All : main.o Msender.o Mdaemon.o
	$(GPP)  MiaoW main.o Msender.o Mdaemon.o
main.o :main.cpp include.h Msender.h Mdaemon.h
	$(GPP)  main.o -c main.cpp
Msender.o:  Msender.h
	$(GPP)  Msender.o -c Msender.cpp
Mdaemon.o: Mdaemon.cpp Mdaemon.h
	$(GPP)  Mdaemon.o -c Mdaemon.cpp
