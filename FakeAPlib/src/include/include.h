#ifndef INCLUDE_H
#define INCLUDE_H

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiSTA.h>
#include <WebServer.h>
#include <DNSServer.h>
//#include <HTTPRequest.hpp>
//#include <HTTPResponse.hpp>
#include <SD_MMC.h>
#include <FS.h>
#include <base64.h>
#include <String>
//#include <memory>

typedef enum
{
    AUTHPAGE   = 'a',
    DATAFILE   = 'd',
    THANKSPAGE = 't',
}FileType_t;


static String getCurrentTime()
{
    unsigned long runMillis= millis();
    unsigned long allSeconds= runMillis/1000;
    int secsRemaining= allSeconds%3600;

    uint8_t hours   = allSeconds/3600;
    uint8_t minutes = secsRemaining/60;
    uint8_t seconds = secsRemaining%60;

    return String(hours) + ":" + String(minutes) + ":" + String(seconds);
}

#define TIME_OUT_LIMIT 20000
#define MAX_LOGIN_TIME 150000
#define LOGIN_LOCKOUT_TIME 300000
#define MAX_LOGIN_ATTEMPTS 3
#define LOG_FILE "/data/logs.txt"
#define ADMIN_USERNAME "admin"
#define ADMIN_PASSWORD "admin123"

#ifndef DNS_PORT
#define DNS_PORT 53
#endif

#ifndef SERVER_PORT
#define SERVER_PORT 80
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

#ifndef WITH_ERROR_TYPE
#define WITH_ERROR_TYPE true
#endif

#ifndef WITH_SUCCESS_MESSAGE
#define WITH_SUCCESS_MESSAGE true
#endif

/*
auto withError = [] (bool useError = true){
    #define WITH_ERROR_TYPE useError
}*/

#if (WITH_ERROR_TYPE)
    #define ERROR_SD_NOT_INIT  "[ERROR] La tarjeta SD no ha sido inicializada"
    #define ERROR_SD_CARD_TYPE "[ERROR] El tipo de tarjeta SD es incompatible"
    #define ERROR_SD_CARD_READ "[ERROR] error en la lectura de la tarjeta"
    #define ERROR_FILE_OPEN "[ERROR] Error al abrir el archivo: %s\n"
    #define ERROR_FILE_WRITE "[ERROR] Error al escribir en el archivo: %s\n"
    #define ERROR_FILE_RENAME "[ERROR] Error al renombrar archivo/carpeta: %s\n"
    #define ERROR_FILE_TYPE "[ERROR] Error de tipo de archivo/carpeta: %s\n"
    #define ERROR_FILE_DELETE "[ERROR] Error al borrar el archivo: %s\n"
    #define ERROR_DIR_CREATE "[ERROR] Error al crear la carpeta: %s\n"
    #define ERROR_DIR_DELETE "[ERROR] Error al borrar la carpeta: %s\n"
    #define ERROR_WIFIAP_CREATE "[ERROR] La creación del punto de acceso ha fallado"
#endif


#if (WITH_SUCCESS_MESSAGE)
    #define SUCCESS_SD_INIT "[SUCCESS] Tarjeta SD inicializada correctamente"
    #define SUCCESS_FILE_OPEN "[SUCCESS] El archivo: %s se ha leído correctamente\n"
    #define SUCCESS_FILE_WRITE "[SUCCESS] El archivo: %s se ha guardado correctamente\n"
    #define SUCCESS_FILE_RENAME "[SUCCESS] El archivo: %s se ha renombrado correctamente\n"
    #define SUCCESS_FILE_DELETE "[SUCCESS] El archivo: %s se ha borrado correctamente\n"
    #define SUCCESS_DIR_CREATE "[SUCCESS] La carpeta: %s se ha creado correctamente\n"
    #define SUCCESS_DIR_DELETE "[SUCCESS] La carpeta: %s se ha borrado correctamente\n"
    #define SUCCESS_WIFIAP_INIT "[SUCCESS] Punto de acceso iniciado correctamente"
    #define SUCCESS_WIFISTA_CONECT "[SUCCESS] Se ha conectado correctamente a la red: %s\n"
    #define SUCCESS_SERVER_INIT "[SUCCESS] Servidor iniciado correctamente"
    #define SUCCESS_LOG_EVENT "[LOGS] Evento guardado correctamente"
    //#define SUCCESS_FILE_TYPE "[SUCCESS] El archivo: %s se ha renombrado correctamente\n"
    //#define SUCCESS_SD_CARD_TYPE "[SUCCESS] El tipo de tarjeta es incompatible"
    //#define SUCCESS_SD_CARD_READ "[SUCCESS] en la lectura de la tarjeta"
#endif

#endif