#include "root.h"

int set_ifa(int i) {
  int j;
  struct ifaddrs *ifa;

  ifc.index = i;
  if (getifaddrs(&ifa) == -1)
    perror("bad address");
  for (ifa, j = 0; j != i; ifa=ifa->ifa_next, j++);
  sprintf(ifc.name, "%s", ifa->ifa_name);
  return 1;
}

int set_field() {
  int fd;
  struct ifreq ifr;

  if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) return (-1);
  sprintf(ifr.ifr_name, "%s", ifc.name);

  if (ioctl(fd, SIOCGIFADDR, &ifr) < 0) {
    perror("ioctl SIOCGI");
    exit(EXIT_FAILURE);
  }
  struct sockaddr_in *ip = (struct sockaddr_in *)&ifr.ifr_addr;
  sprintf(ifc.ip, "%s", inet_ntoa((struct in_addr)ip->sin_addr));

  ioctl(fd, SIOCGIFNETMASK, &ifr);
  ip = (struct sockaddr_in *)&ifr.ifr_netmask;
  sprintf(ifc.mask, "%s", inet_ntoa((struct in_addr)ip->sin_addr));

  ioctl(fd, SIOCGIFHWADDR, &ifr);
  unsigned char *tmp = calloc(6, sizeof(unsigned char));
  memcpy(tmp, ifr.ifr_hwaddr.sa_data, 6);
  sprintf(ifc.mac, "%2x:%2x:%2x:%2x:%2x:%2x", tmp[0], tmp[1], tmp[2], tmp[3], tmp[4], tmp[5]);
  free(tmp);

  ioctl(fd, SIOCGIFMTU, &ifr);
  ifc.mtu = ifr.ifr_mtu;

  return 1;
}

int set_extip() {
  struct hostent *hp;
  struct sockaddr_in addr;
  int sock, on = 1;
  const char *host = "icanhazip.com";
  in_port_t port = 80;
  if ((hp = gethostbyname(host)) == NULL) {
    herror("gethostbyname");
    exit(EXIT_FAILURE);
  }
  bcopy(hp->h_addr, &addr.sin_addr, (size_t)hp->h_length);
  addr.sin_port = htons(port);
  addr.sin_family = AF_INET;
  sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (const char *)&on, sizeof(int));
  if (sock == -1) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }
  if (connect(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1) {
    perror("connect");
    exit(EXIT_FAILURE);
  }
  char ip[IP_LEN];
  write(sock, "GET /\r\n", strlen("GET /\r\n"));
  while (read(sock, ip, IP_LEN-1) != 0) {
    memcpy(ifc.ext_ip, ip, IP_LEN-2);
  }
  shutdown(sock, SHUT_RDWR);
  close(sock);
  return 1;
}

int set_all() {
  set_ifa(choose_if());
  set_field();
  set_extip();
  return 1;
}
