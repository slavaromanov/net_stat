#include "root.h"

int choose_if() {
  struct ifaddrs *ifa;
  int n, t;
  if (getifaddrs(&ifa) == -1) {
    perror("bad address");
    exit(EXIT_FAILURE);
  }
  for (ifa, n = 0; ifa != NULL; ifa = ifa->ifa_next, n++) {
    if (ifa->ifa_addr == NULL)
      continue;
    if (ifa->ifa_addr->sa_family == AF_PACKET)
      printf("%d: %s\n", n+1, ifa->ifa_name);
  }
  scanf("%d", &t);
  return --t;
}

int data_to_screen() {
  printf(" _______________________________________________________\n"
          "|name:\t\t     %5s|MTU: %4d\t\t\t|\n"
          "|network ip:%15s|network mask:%15s|\n"
          "|extern ip: %15s|MAC:       %17s|\n",
                                    ifc.name, ifc.mtu, ifc.ip, ifc.mask,
                                    ifc.ext_ip, ifc.mac);
}

int ldata_to_screen() {
  int j, family;
  struct ifaddrs *ifa;
  char *tmp = malloc(1024*sizeof(char));
  if (getifaddrs(&ifa) == -1) {
    perror("bad address");
    exit(EXIT_FAILURE);
  }
  for (ifa, j = 0; ifa != NULL; ifa=ifa->ifa_next, j++) {
    if (ifa->ifa_addr == NULL)
      continue;
    family = ifa->ifa_addr->sa_family;
    if (family == AF_PACKET && ifa->ifa_data != NULL) {
      struct rtnl_link_stats *s = ifa->ifa_data;
      sprintf(tmp, "|_______________________________________________________|\n"
      "|\t        transmitted|    received\t\t|\n"
      "|_______________________________________________________|\n"
      "|   packets: %14u|%12u\t\t|\n"
      "|   bytes: %16u|%12u\t\t|\n"
      "|   errors: %15u|%12u\t\t|\n"
      "|   compressed: %11u|%12u\t\t|\n"
      "|_______________________________________________________|\n",
      s->tx_packets, s->rx_packets,
      s->tx_bytes, s->rx_bytes,
      s->tx_errors, s->rx_errors,
      s->tx_compressed, s->rx_compressed);
    }
  }
  printf("%s", tmp);
  return 1;
}
