#include "root.h"


int main(int argc, char *argv[]) {
  set_all();
  while (1) {
    data_to_screen();
    ldata_to_screen();
    sleep(1);
    system("clear");
  }
}
