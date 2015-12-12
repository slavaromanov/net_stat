// header file
#ifndef ROOT
#define ROOT
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/if_link.h>
#include <linux/if.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h> 

#define IP_LEN 16
#define MAC_LEN 20

struct ifconfig {
  char name[IP_LEN];
  char ip[IP_LEN];
  char mask[IP_LEN];
  char ext_ip[IP_LEN];
  char mac[MAC_LEN];
  int mtu;
  int index;
} ifc;

int choose_if(void);
int set_ifa(int);
int set_field(void);
int set_extip(void);
int set_all(void);
int data_to_screen(void);
int ldata_to_screen(void);

#endif
