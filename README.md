# ESP32_FakeAP
Esto es un proyecto escolar que tiene como objetivo el descubrimiento y el uso de las funcionalidades de la placa `ESP32`.

## Descripción
Este código sirve a crear un `punto de acceso falso` (Fake Access Point) usando la placa `ESP32`.<br>
Será un _wifi_ público, que perdirá al usuario unos datos (credenciales de su cuenta `Instagram`, `Facebook`, `X`... etc.) para poder conectarse a la red.<br>
Las datos proporcionados serán transferidos a una otra placa que actuará como _servidor_, igualmente implementado con una placa `ESP32`, para almacenarse en una _tarjeta SD_.

> [!CAUTION]
> Esto es únicamente a fines educativos. No se hará ningún uso ilícito de este código.

### Estructura
* Se encontrará el código Arduino en la carpeta [ESP32_honeyPot](/ESP32_honeyPot)
* Archivo fuente de la página root [authentication.html](/authentication.html)
* Archivo fuente de la página submit [thanksPage.html](/thanksPage.html)
* En la caperta [tools](/tools) estarán todos los recursos externos necesarios. <br>
Como pueder ser los [iconos](/tools/icons) para las paginas web. <br>
<img height= 50 src="/tools/icons/facebook-2020-2-1.svg" width=50/> <img height= 50 src="/tools/icons/instagram-icon.svg" width=50/> <img height= 50 src="/tools/icons/X_logo_2023_original.svg" width=50/>

```
ESP32_FakeAP/
│
├── ESP32_honeyPot/
│   └── ESP32_honeyPot.ino
│
├── FakeAPlib/
│   ├── examples/
│   │   └── fake_access_point/
│   │       └── fake_access_point.ino
│   │
│   ├── src/
│   |   ├── include/
│   │   │   ├── Authentication.h
│   │   │   ├── include.h
│   │   │   ├── SDCardManager.h
│   │   │   └── WebServerHandler.h
│   │   │
│   │   ├── FakeAPlib.cpp
│   │   └── FakeAPlib.h
│   │
│   ├── keywords.txt
│   ├── library.properties
│   ├── //
│   └── README.md
│
├── tools/
│   ├── icons/
│   │   ├── facebook-2020-2-1.svg
│   │   ├── instagram-icon.svg
│   │   └── X_logo_2023_original.svg
│   │
│   └── login-pages/
│       ├──
|       └── 
│ 
├── webpages/
│   ├── admin.html
│   ├── index.html
│   ├── info.html
│   ├── login.html
│   ├── style.css
│   ├── success.html
│   └── upload.html
│
├── .gitignore
├── admin.html
├── authentication.html
├── index.html
├── LICENSE
├── README.md
├── style.css
└── thanksPage
```

### Componentes
#### Físicos
* Dos placas ESP32 (un punto de acceso y un servidor)
* Dos leds (una para comprobar que el punto de acceso esté activo y el otro para notificar cuando se conecta un nuevo host)
* Tarjeta SD
* Dos baterías para hacer <br>

#### Librerias personales
* FakeAPlib.h
* SDCardManager.h
* WebServerHandler.h
* Authentication.h
* include.h

#### Librerias externas
* WiFi.h
* WiFiAP.h
* WiFiSTA.h
* WebSever.h
* DNSServer.h
* HTTPClient.h
* SD_MMC.h
* FS.h
* base64.h
* String
