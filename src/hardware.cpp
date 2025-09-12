#include "common.h"
#include <time.h>
#include <sys/stat.h>
#include "SD_MMC.h"

// Mount SD card and prepare log directory structure
extern "C" void hardware_init() {
  if (!SD_MMC.begin("/sdcard")) {
    printf("SD card mount failed\n");
    return;
  }

  time_t now = time(NULL);
  struct tm tm;
  localtime_r(&now, &tm);
  char dir[64];
  snprintf(dir, sizeof(dir), "/sdcard/LOG/%04d%02d%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

  SD_MMC.mkdir("/sdcard/LOG");
  SD_MMC.mkdir(dir);
}
