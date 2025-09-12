#include "common.h"
#include "freertos/task.h"
codex/enable-task-watchdog-and-add-handlers
#include "esp_task_wdt.h"

#include "SD_MMC.h"
#include <string.h>
#include <time.h>

static const uint64_t MIN_FREE_SPACE = 10ULL * 1024 * 1024; // 10 MB

static File dataFile;
static File indexFile;
static char currentDate[9] = "";

static void deleteDirRecursive(const char *path) {
  File dir = SD_MMC.open(path);
  if (!dir) return;
  File entry = dir.openNextFile();
  while (entry) {
    char entryPath[128];
    snprintf(entryPath, sizeof(entryPath), "%s/%s", path, entry.name());
    if (entry.isDirectory()) {
      deleteDirRecursive(entryPath);
      SD_MMC.rmdir(entryPath);
    } else {
      SD_MMC.remove(entryPath);
    }
    entry.close();
    entry = dir.openNextFile();
  }
  dir.close();
  SD_MMC.rmdir(path);
}

static void rotateLogs() {
  uint64_t freeBytes = SD_MMC.totalBytes() - SD_MMC.usedBytes();
  while (freeBytes < MIN_FREE_SPACE) {
    File root = SD_MMC.open("/sdcard/LOG");
    if (!root) return;
    char oldest[64] = "";
    File dir = root.openNextFile();
    while (dir) {
      if (dir.isDirectory()) {
        const char *name = dir.name();
        if (strcmp(name, ".") != 0 && strcmp(name, "..") != 0) {
          if (oldest[0] == '\0' || strcmp(name, oldest) < 0) {
            strncpy(oldest, name, sizeof(oldest) - 1);
          }
        }
      }
      dir.close();
      dir = root.openNextFile();
    }
    root.close();
    if (oldest[0] == '\0') break;
    char path[128];
    snprintf(path, sizeof(path), "/sdcard/LOG/%s", oldest);
    deleteDirRecursive(path);
    freeBytes = SD_MMC.totalBytes() - SD_MMC.usedBytes();
  }
}

static void ensureLogFiles() {
  char date[9];
  time_t now = time(NULL);
  struct tm tm;
  localtime_r(&now, &tm);
  snprintf(date, sizeof(date), "%04d%02d%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

  if (strcmp(currentDate, date) != 0) {
    if (dataFile) dataFile.close();
    if (indexFile) indexFile.close();
    strncpy(currentDate, date, sizeof(currentDate));

    char dir[64];
    snprintf(dir, sizeof(dir), "/sdcard/LOG/%s", currentDate);
    SD_MMC.mkdir("/sdcard/LOG");
    SD_MMC.mkdir(dir);

    char dataPath[80];
    snprintf(dataPath, sizeof(dataPath), "%s/log.bin", dir);
    char indexPath[80];
    snprintf(indexPath, sizeof(indexPath), "%s/index.bin", dir);

    dataFile = SD_MMC.open(dataPath, FILE_APPEND);
    indexFile = SD_MMC.open(indexPath, FILE_APPEND);
  }
}
main

void Task_Logger(void *pvParameters) {
  log_blob_t blob;
  esp_task_wdt_add(NULL);
  for (;;) {
codex/enable-task-watchdog-and-add-handlers
    esp_task_wdt_reset();
    if (xQueueReceive(q_log, &blob, pdMS_TO_TICKS(1000))) {
      // TODO: write blob to SD card

    if (xQueueReceive(q_log, &blob, portMAX_DELAY)) {
      ensureLogFiles();
      if (!dataFile || !indexFile) continue;

      uint32_t offset = dataFile.size();
      uint16_t len = blob.len;
      dataFile.write((uint8_t *)&len, sizeof(len));
      dataFile.write(blob.data, len);
      dataFile.flush();

      indexFile.write((uint8_t *)&offset, sizeof(offset));
      indexFile.flush();

      rotateLogs();
main
    }
  }
}
