# ESP32_FakeAP
Esto es un proyecto escolar que tiene como objetivo el descubrimiento y el uso de las funcionalidades de la placa `ESP32`.

## Descripción
Este código sirve a crear un `punto de acceso falso` (Fake Access Point) con la placa `ESP32`.<br>
Será un _wifi_ público, que perdirá al usuario unos datos (credenciales de su cuenta `Instagram`, `Facebook`, `X`... etc.) para poder conectarse a la red.<br>
Las datos proporcionados se almacenarán en una _tarjeta SD_ o serán transferidos a un otro _servidor_ igualmente implementado con una placa `ESP32`.

> [!CAUTION]
> Esto es únicamente a fines educativos. No se hará ningún uso ilícito de este código.

### Estructura
* Se encontrará el código Arduino en la carpeta [ESP32_honeyPot](/ESP32_honeyPot)
* Archivo fuente de la página root [authentication.html](/authentication.html)
* Archivo fuente de la página submit [thanksPage.html](/thanksPage.html)
* En la caperta [tools](/tools) estarán todos los recursos externos necesarios. <br>
Como pueder ser los [iconos](/tools/icons) para las paginas web. <img height= 50 src="/tools/icons/facebook-2020-2-1.svg" width=50/> <img height= 50 src="/tools/icons/instagram-icon.svg" width=50/> <img height= 50 src="/tools/icons/X_logo_2023_original.svg" width=50/>
