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

#include "./include/SDCardManager.h"
#include "./include/WebServerHandler.h"
#include "./include/Authentication.h"
#include "./include/include.h"

#define WITH_ERROR_TYPE true

class FakeAp : public WebServerManager
{
protected:
    AuthenticationManager m_authManager;
    SDCardManager         m_sdManager;
    WebServerManager      m_server;

public:
    FakeAp()
        :m_authManager{AuthenticationManager()}, m_sdManager{SDCardManager()}, m_server{WebServerManager(m_sdManager, m_authManager)}
    {
    }

    bool initialize(const String &ssid, const String &psw = "\0");
    bool setWifi(const String& wifissid, const String& wifipsw="\0");
};


namespace sdfiles
{
    char* getCardType (uint8_t card);
    bool sdinit();

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

    extern String _wifissid;
    extern String _wifipsw;

    extern String authPage_;
    extern String thksPage_;
    extern String adminPage_;
    extern String datafile_;

    bool apinit(const char* &ssid, const byte& dns_port, const char* psw="\0");
    bool wifiInit (const char* ssid, const char* psw="\0");
    void setPath(const char* path, FileType_t fileType);
    void handleRoot();
    void handleSubmit();
    void handleAdmin();
}

#endif