#include <signal.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdio.h>
#include <stdlib.h>
#include<sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <termios.h> 
#include "DataDef.h"
#include <unistd.h>

#pragma pack(1)
extern "C" {
//free_the_set, which is used widely to free struct {_set_of(Type)}
void a_set_empty(void *a_set_of_x);

int vh_GetGNSS (GNSSInfo_s* gn);
int pm_MsgBroadcast (char* buf, int length);
int pm_StartRecv (void* (*operate) (char* buf, struct sockaddr* sendAddr, int nlen));
}
