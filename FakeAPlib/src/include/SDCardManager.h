#ifndef SD_CARD_MANAGER_H
#define SD_CARD_MANAGER_H

#include "include.h"


class SDCardManager
{
protected:
    fs::FS _fileSystem;
    bool _sdInitialized;

public:
    SDCardManager()
        : _fileSystem{SD_MMC}, _sdInitialized{false}
    {
    }

    fs::FS& getFileSystem()
    {
        return _fileSystem;
    }

    char *getCardType(uint8_t card)
    {
        switch (card)
        {
        case CARD_MMC:
            return "MMC";
            break;

        case CARD_SD:
            return "SD";
            break;

        case CARD_SDHC:
            return "SDHC";
            break;
        }
        return "Unknown";
    }

    inline bool logEvent(const String &event)
    {
        File logFile = SD.open(LOG_FILE, FILE_APPEND);
        if (!logFile)
        {
            if (WITH_ERROR_TYPE) Serial.printf(ERROR_FILE_OPEN, logFile);
            return 0;
        }

        logFile.println(String(millis()) + ": " + event);
        logFile.close();

        return 1;
    }

    inline bool initialize()
    {
        Serial.println("Montando la tarjeta SD....");
        SD_MMC.setPins(SD_MMC_CLK, SD_MMC_CMD, SD_MMC_D0);

        if (!SD_MMC.begin("/sdcard", true, true, SDMMC_FREQ_DEFAULT, 5))
        {
            if (WITH_ERROR_TYPE) Serial.println(ERROR_SD_CARD_READ);
            return 0;
        }

        uint8_t cardType = SD_MMC.cardType();
        if (cardType == CARD_NONE)
        {
            if (WITH_ERROR_TYPE) Serial.println(ERROR_SD_CARD_TYPE);
            return 0;
        }

        Serial.println("Tarjeta SD inicializada correctamente.");
        Serial.printf("Tipo de tarjeta: %s\n", getCardType(cardType));

        uint64_t cardSize = SD_MMC.cardSize() / (1024 * 1024);
        Serial.printf("Tamaño de la tarjeta SD_MMC: %lluMB\n", cardSize);

        listDir("/", 0);

        Serial.printf("Total espacio disponible: %lluMB\r\n", SD_MMC.totalBytes() / (1024 * 1024));

        _sdInitialized = true;

        return 1;
    }

    inline String readFile(const char *path)
    {
        File file = _fileSystem.open(path);
        if (!file)
        {
            if (WITH_ERROR_TYPE) Serial.printf(ERROR_FILE_OPEN, path);
            return "\0";
        }
        else if (!_sdInitialized)
        {
            if (WITH_ERROR_TYPE) Serial.println(ERROR_SD_NOT_INIT);
            return "\0";
        }

        String content = "";
        while (file.available())
            content += (char)file.read();

        file.close();

        return content;
    }

    inline bool writeFile(const char *path, String &content)
    {
        File file = _fileSystem.open(path, FILE_WRITE);
        if (!file)
        {
            if (WITH_ERROR_TYPE) Serial.printf(ERROR_FILE_OPEN, path);
            return 0;
        }
        else if (!file.println(content))
        {
            if (WITH_ERROR_TYPE) Serial.printf(ERROR_FILE_WRITE, path);
            return 0;
        }
        file.close();
        return 1;
    }

    inline void listDir(const char *dirname, uint8_t levels)
    {
        //Serial.printf("Listing directory: %s\n", dirname);

        File root = _fileSystem.open(dirname);
        if (!root)
        {
            if (WITH_ERROR_TYPE) Serial.printf(ERROR_FILE_OPEN, dirname);
            return;
        }
        if (!root.isDirectory())
        {
            if (WITH_ERROR_TYPE) Serial.printf(ERROR_FILE_TYPE, dirname);
            return;
        }

        File file = root.openNextFile();
        String fileList = "";
        while (file)
        {
            if (file.isDirectory())
            {
                Serial.print("  DIR : ");
                Serial.println(file.name());
                if (levels)
                    listDir(file.name(), levels - 1);
            }
            else
            {
                Serial.print("  FILE: ");
                Serial.print(file.name());
                Serial.print("  SIZE: ");
                Serial.println(file.size());
            }
            file = root.openNextFile();
        }
    }

    inline bool createDir(const char *path)
    {
        Serial.printf("Creating Dir: %s\n", path);
        if (!_fileSystem.mkdir(path))
        {
            if (WITH_ERROR_TYPE) Serial.printf(ERROR_DIR_CREATE, path);
            return 0;
        }

        return 1;
    }

    inline bool removeDir(const char *path)
    {
        Serial.printf("Removing Dir: %s\n", path);
        if (!_fileSystem.rmdir(path))
        {
            if (WITH_ERROR_TYPE) Serial.printf(ERROR_DIR_DELETE, path);
            return 0;
        }
        return 1;
    }

    inline bool appendFile(const char *path, String &content)
    {
        File file = _fileSystem.open(path, FILE_APPEND);
        if (!file)
        {
            if (WITH_ERROR_TYPE) Serial.printf(ERROR_FILE_OPEN, path);
            return 0;
        }
        content = "\n" + content;
        if (!file.print(content))
        {
            Serial.printf(ERROR_FILE_WRITE, path);
            return 0;
        }
        file.close();

        return 1;
    }

    inline bool renameFile(const char *path1, const char *path2)
    {
        if (!_fileSystem.rename(path1, path2))
        {
            if (WITH_ERROR_TYPE) Serial.printf(ERROR_FILE_RENAME, path1);
            return 0;
        }

        return 1;
    }

    inline bool deleteFile(const char *path)
    {
        Serial.printf("Deleting file: %s\n", path);
        if (!_fileSystem.remove(path))
        {
            if (WITH_ERROR_TYPE) Serial.printf(ERROR_FILE_DELETE, path);
            return 0;
        }

        return 1;
    }

    inline void testFileIO(const char *path)
    {
        File file = _fileSystem.open(path);
        static uint8_t buf[512];
        size_t len = 0;
        uint32_t start = millis();
        uint32_t end = start;

        if (file)
        {
            len = file.size();
            size_t flen = len;
            start = millis();
            while (len)
            {
                size_t toRead = len;
                if (toRead > 512)
                {
                    toRead = 512;
                }
                file.read(buf, toRead);
                len -= toRead;
            }
            end = millis() - start;
            Serial.printf("%u bytes read for %u ms\n", flen, end);
            file.close();
        }
        else
        {
            Serial.println("Failed to open file for reading");
        }

        file = _fileSystem.open(path, FILE_WRITE);
        if (!file)
        {
            Serial.println("Failed to open file for writing");
            return;
        }

        size_t i;
        start = millis();
        for (i = 0; i < 2048; i++)
            file.write(buf, 512);

        end = millis() - start;
        Serial.printf("%u bytes written for %u ms\n", 2048 * 512, end);
        file.close();
    }
};

#endif