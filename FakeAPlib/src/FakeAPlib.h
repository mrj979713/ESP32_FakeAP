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



class FakeAp : public WebServerManager
{
protected:
    //AuthenticationManager m_authManager;
    //SDCardManager         m_sdManager;
    //WebServerManager      m_server;

public:
    FakeAp()
        : WebServerManager (new SDCardManager(), new AuthenticationManager(), SERVER_PORT)
    {
    }

    FakeAp(const uint8_t& port)
        : WebServerManager (new SDCardManager(), new AuthenticationManager(), port)
    {
    }

    FakeAp(const FakeAp&) = delete;

    FakeAp operator=(const FakeAp&) = delete;

    bool initialize(const String &ssid, const String &psw = "\0");
    bool setWifi(const String& wifissid, const String& wifipsw="\0");
    void checkState();
};

#ifndef FAKEAP_NO_GLOBALS

#endif

#endif