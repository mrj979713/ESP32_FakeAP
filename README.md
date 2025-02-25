# ESP32_FakeAP
Esto es un proyecto escolar que tiene como objetivo el descubrimiento y el uso de las funcionalidades de la placa `ESP32`.

## Descripción
Este código sirve a crear un `punto de acceso falso` (Fake Access Point) usando la placa `ESP32`.<br>
Será un _wifi_ público, que perdirá al usuario de conectarse a su cuenta en una de sus redes (`Instagram`, `Facebook`, `X`... etc.) para poder usar la red de wifi.<br>
Las datos proporcionados serán transferidos a una segunda placa que actuará como _servidor_, igualmente implementado con una placa `ESP32`, para almacenarse en una _tarjeta SD_.

> [!CAUTION]
> Esto es únicamente a fines educativos. No se hará ningún uso ilícito de este código.

### Estructura
Para facilitar el mantenimiento, la legibilidad y la reusabilidad del código, se desarolló una librería que solo se tendrá que añadir a `Arduino IDE` y usarla en su sketch.
* Los archivos fuentes de la librería se encontrarán en la carpet [FakeAPlib](/FakeAPlib)
* Se encontrará el código Arduino en la carpeta:
    - [ESP32_honeyPot](/ESP32_honeyPot): código a ejecutar en la placa del punto de acceso
    - [ESP32_honeyPot](/ESP32_Server): código a ejecutar en la placa del servidor
* Codigo fuente de las páginas html:
    - [tools](/webpages/tools): iconos de la página [index.html](/webpages/index.html) 
    - [admin.html](/webpages/admin.html): página de inicio de sesión administrador en el servidor
    - [application.js](/webpages/application.js): lógica de la página [index.html](/webpages/index.html)
    - [index-styles.css](/webpages/index-styles.css): para el formato de la página [index.html](/webpages/index.html) 
    - [index.html](/webpages/index.html): página de inicio del punto de acceso
> Las carpetas con un nombre `****-login`contienen los códigos de las páginas de redes sociales. Consultar las fuentes [aquí](#fuentes)
* Archivo fuente de la página submit [thanksPage.html](/thanksPage.html) 
* En la caperta [tools](/tools) estarán todos los recursos externos utiles. <br>
Como pueden ser los [iconos](/tools/icons) para las paginas web. <br>

<img height= 50 src="/tools/icons/facebook-2020-2-1.svg" width=50/> <img height= 50 src="/tools/icons/instagram-icon.svg" width=50/> <img height= 50 src="/tools/icons/X_logo_2023_original.svg" width=50/>

> Pueden consultar el arbol del proyecto [aquí](#project-tree)

### Componentes
#### Físicos
* Dos placas ESP32 (un punto de acceso y un servidor)
> [!IMPORTANT]
> La place debe de tener un lector de tarjeta integrado.
> Utilizamos placas ESP32-WROVER-E, pero debería valer cualquiera con lector tarjeta integrado. 
* Tarjeta SD formateada previamente con el sistemas de archivos `FAT32`
> [!IMPORTANT]
> La tarjeta SD tiene no tiene que superar los 32GB para poder aplicar el formato `FAT32`.
* Dos baterías para hacer
* Dos leds (una para comprobar que el punto de acceso esté activo y el otro para notificar cuando se conecta un nuevo host) <br>

| Componentes          | Precios         |
| -------------------- | :-------------: |
| Placa ESP32-WROVER   | [Amazon: €10,99](https://www.amazon.es/ARCELI-ESP32-WROVER-DEV-Desarrollo-ESP32-CAM-Compatible/dp/B0CP7S6D9Z?__mk_es_ES=%C3%85M%C3%85%C5%BD%C3%95%C3%91&crid=2T3OTKMOJFBGH&dib=eyJ2IjoiMSJ9.oRWGfjhE8Ofw1TnGAJnW7KzVwWdYb4uecyOO5FKL7nyF3lEILCrF47EWC9PTDhDoSbcjLXssbDTvscdEyZ7Ljehc_HmHg17WvhciYllYxTRUv68sqKtOhtdCUIp6GzLb7UdsbZyXkBLwa7zgS2qd2Z_0dSKMeOS3eCAafegKQN3SDqqHwhEBkeJmC_4HUaofycBE1PlPUlALAjl1STwu5XqGBEJITYqV5RmeSUEsKMVnT4TEvVuWXLll4m1PD-fiUxt0Atcvuzk5OOsODBkhbAuCpg9XwYeddOPABz1U8kHVqHW9TQ0AQ35ezS_BCBbxPJ3vK9aadhoWCpCHLpo9d8kSzw7UCec9i9TbjMe6KBdcpZrnE2dQYCzedtE3kTMV6yaQiJ5ZVbWo4RzNSNi-Q-_9CI6RTnI6MAjZLjY2DOd7pfCplzKul6efnbR1nVB-.-zY33IEcAB-iq0MPfCVemn78hCxBiS92eia6QIytXBA&dib_tag=se&keywords=esp32+wrover-e&nsdOptOutParam=true&qid=1736031600&sprefix=esp32+wrover-e%2Caps%2C214&sr=8-10)   |
| Tarjeta SD 16GB      | [Amazon: €6,87](https://www.amazon.es/Kodak-Premiun-SDHC-16GB-Class10/dp/B01FZPDRA4?crid=3GR8LY1LWAOZY&dib=eyJ2IjoiMSJ9.0kLS3uvwbDCbwpvUcLi5-GgV6qAhXJurERZ55fT5tsRVaWdKkBz8PBrufeMOG_n89iCzgs5lU-cKcK7LdGVTP7QoN8m0KRlSND5NtChLQVSwZMEre9nVGzciLRK9ZmK5cDZY6c2R1Mefm9PcYcA3No2ft_BvC9jvgm_R95HEGPTLf1Ga0f0w3Etd15N44nR_uzluLcg1sRY95fDxVL5vhl_X9gY5ngWXZ8cF_ZhIniMLmBDcYMQRUleSTJ0FIdlldERwNbAMdsGwvOjEbhxqNFN_KyGtno4zjC4wcS7to0bJZedjdspB2a2M1C19Bo9508gKbQGnQmKvnh_0EgNqLNY6FgImuvQOkseJd_f6mJoexv_oaCA0ms5GueJ4uwuxwafy-QntUpDJGLCKwSynYxZmMNmRbsBIN1MTPhur1b9_saTEsfqMM1YuM29q1QGA.i422wxWCoWfc2CY5KTSR-cOouiKQLb98eExASJJe81o&dib_tag=se&keywords=tarjeta+sd+16gb&nsdOptOutParam=true&qid=1736031694&sprefix=tarjeta+sd+16%2Caps%2C144&sr=8-5)    |
| Tarjeta SD 32GB      | [Amazon: €7,95](https://www.amazon.es/SANDISK-Ultra-MICROSDHC-Adapter-100MB/dp/B08HVWJM8C?dib=eyJ2IjoiMSJ9.R1o3XhQ4AKSpUNWJUASxGu26T3mnxr_wq-O8EaxKUiiaENOTRmQl9wKTn6Tx3Rf5d1TF3lsJ1qa8LcuKelgDXxepsy31Xj6Cw6HuO26xTU8rJx_sOviRCI-DqNyP7nhsB2__ULdgaewxx4U5eRj6rNtz5i1ad7WtTSS2hj0AVG0m7CFLojaRciwtYk3CLJ2OvSBhF8slD8HQvtu3ITXEtq77VC0LvK_dhdy0FxkFhwHoa55MjsvPSmpLNQ-bWpyp_lOyHe2Q0VXUAZdYWJ9bixzbmMdNnFrF5TDxuTfxZulGmFdnlQVRYhnvBJjO-8TGLVP2G048iv2aijloUGEEjtf2sYHV6_T8XlrDCcUkDhW2zuu7Vn6tOMzSBTWMF8u1-aTEYwe-uMoKqHe_JdlgtCcm2_Epe08UdblcHYFSxhnPktSxD6x_yg1G9mgjMopR.3Kp6jRwcV8Ptb21pfHsxvkV3GngDWPLAE10L3KQwY58&dib_tag=se&keywords=tarjeta+sd+32gb&nsdOptOutParam=true&qid=1736030740&sr=8-25)  |
| Modulo de baterías   | [Amazon: €11,58](https://www.amazon.es/VooGenzek-Battery-Compatible-Arduino-Raspberry/dp/B09Q4ZMNLW?__mk_es_ES=%C3%85M%C3%85%C5%BD%C3%95%C3%91&crid=2FDGYN87M4GGZ&dib=eyJ2IjoiMSJ9.ka-TTEClCum9szJA1PJem8tkRPWtcgveYaemjkar68liTEUzNvzZaaFJBoZTFxvZ5d4I4fxllJ3rtWvGeDmAfFgBQirLQ0nMNEIZXmiBJDws-p5qyu33DHHpUDqmz_jLF8NaXkZ3B7E876we5I6hHyQrWGCzOKdYqT2BRIKDnonxGU0cxJfakz90fTYv8_1-0gDGKakYdrD7I596c3DSTQzTmRnUdB5ZdRwUgCcTtB6mMUometjYPSPSBWDiLhfJkFHfBMIP0p5uHSnXGEmI9t4v6IDRYAs-m5HbzeeeFLQss6SJ83SJgGlK4byhhwUO8wmthUcM_f7op4XcFtpcQPdtqA6rC8CZz0hCcbZLlUNBBLBHsCI3lQ_kGFD_plqaUhnQ6-B8cwvpl6_oG2O7bMozUT72_jqIWX_K-RxI5K_l6FOk-MowYW2pmPXCGyJg.6FIKkso1M9yuqA4GvettGPhUSGPNKXjjN-6bG243_Zg&dib_tag=se&keywords=battery%2Bshield%2Besp32%2Bwrover&nsdOptOutParam=true&qid=1736030985&sprefix=battery%2Bshield%2Besp32%2Bwrover%2Caps%2C106&sr=8-2&th=1)   |

#### Librerias personales
* FakeAPlib.h
* SDCardManager.h
* WebServerHandler.h
* WifiCaptivePortal.h
* Authentication.h
* include.h

#### Librerias externas
* WiFi.h
* WiFiAP.h
* WiFiSTA.h
* WebSever.h
* DNSServer.h
* NetworkClient.h
* SD_MMC.h
* FS.h
* base64.h
* String

## Fuentes y recursos adicionales

| Archivos                              | Fuente          |
| ------------------------------------- | --------------- |
| Páginas de inicio de sesión facebook  | [geeksforgeeks.org](https://www.geeksforgeeks.org/how-to-create-facebook-login-page-in-html-css/) |
| Páginas de inicio de sesión google    | [ziontutorial.com](https://ziontutorial.com/how-to-make-google-login-form-design-using-html-css/) |
| Páginas de inicio de sesión instagram | [Github repository](https://github.com/codingmarket07/Instagram-Login-Page) |
| Páginas de inicio de sesión twitter   | [geeksforgeeks.org](https://www.geeksforgeeks.org/how-to-create-twitter-login-page-using-html-css-javascript/) |
| Páginas de inicio de sesión facebook  |                 |

<br>

| Recursos adicionales                                  |
| ----------------------------------------------------- |
| [Proyecto similar](https://github.com/bdash9/ESP32-FileServer-HTTP-HTTPS) |
| [Proyecto similar](https://github.com/TheKevinWang/ESP32FakeAP) |
| [Ejemplo de comunicación entre dos placas por wifi](https://esp32io.com/tutorials/communication-between-two-esp32) |
| [Ejemplo de wifi como punto acceso y estación](https://randomnerdtutorials.com/esp32-useful-wi-fi-functions-arduino/) |
| [Ejemplo de servidor web](https://github.com/ESP32-Work/ESP32_Webserver/tree/main/ESP32_WebServer) |
| [Librerías ESPRESSIF arduino-esp32](https://github.com/espressif/arduino-esp32/tree/master/libraries) |
| [Documentación ESPRESSIF](https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html) |
| [Documentación Arduino](https://docs.arduino.cc/language-reference/#functions) |

<br><br>

<a name="project-tree"></a> 
````
ESP32_FakeAP/
│
├── ESP32_honeyPot/
│   └── ESP32_honeyPot.ino
│
├── ESP32_Server/
│   └── ESP32_Server.ino
│
├── FakeAPlib/
│   ├── examples/
│   │   └── fake_access_point/
│   │       └── fake_access_point.ino
│   │
│   ├── src/
│   │   ├── include/
│   │   │   ├── Authentication.h
│   │   │   ├── include.h
│   │   │   ├── SDCardManager.h
│   │   │   ├── WebServerHandler.h
│   │   │   └── WifiCaptivePortal.h
│   │   │
│   │   ├── FakeAPlib.cpp
│   │   └── FakeAPlib.h
│   │
│   ├── keywords.txt
│   ├── image.png
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
│       └── 
│ 
├── webpages/
│   ├── facebook-login/
│   │   ├── index.html
│   │   └── styles.css
│   │
│   ├── google-login/
│   │   ├── index.html
│   │   └── style.css
│   │
│   ├── icons/
│   │   ├── facebook-2020-2-1.svg
│   │   ├── instagram-icon.svg
│   │   └── X_logo_2023_original.svg
│   │
│   ├── instagram-login/
│   │   ├── appstore.png
│   │   ├── facebook.png
│   │   ├── googleplay.png
│   │   ├── index.html
│   │   ├── instagram.png
│   │   └── styles.css
│   │
│   ├── twitter-login/
│   │   ├── index.html
│   │   └── styles.css
│   │
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
├── IP routes.txt
├── LICENSE
├── README.md
├── style.css
└── thanksPage
````