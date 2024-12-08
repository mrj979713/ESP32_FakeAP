#include <SDFiles.h>

/*
* The pins and functions for reading and writting to a SD are already defined in the SDFiles src files.
* Note that this is only meant to be used for a ESP32 board which has an integrated SD reader.
*
*
*
*
*/

void setup() {
  Serial.begin(115200);
  delay(2000);

  SD_MMC.setPins(SD_MMC_CLK, SD_MMC_CMD, SD_MMC_D0);

  if (!SD_MMC.begin("/sdcard", true, true, SDMMC_FREQ_DEFAULT, 5))
  {
    Serial.println("Card mount failed!!!!!!!!!");
    return;
  }

  uint8_t cardType = SD_MMC.cardType();
  if(cardType==CARD_NONE)
  {
    Serial.println("No SD_MMC card attached");
    return;
  }

  Serial.printf("Card type: %s\n", getCardType(cardType));

  uint64_t cardSize = SD_MMC.cardSize() / (1024 * 1024);
  Serial.printf("SD_MMC Card Size: %lluMB\n", cardSize);

  listDir(SD_MMC, "/", 0);
 
  Serial.printf("Total space: %lluMB\r\n", SD_MMC.totalBytes() / (1024 * 1024));
}

void loop() {
  delay(100000);

}