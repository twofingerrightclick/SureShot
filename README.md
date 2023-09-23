# Sureshot
A simple model rocket ignition solution.

This project is intended for the ESP8266 Nodemcu 1 variant. It uses SPIFF. There is a handy [SPIFF data uploader](https://github.com/esp8266/arduino-esp8266fs-plugin) that is only available for Arduino IDE 1.X as 2.X is made with a completely different framework.

## How to get started
1. Copy the sketch.ino to a new project folder in your Arduino 1.X sketch folder. e.g. `C:/Users/[Username]/Documents/Arduino/SureshotSketch/sketch.ino`
2. Copy the data folder to the same project folder. e.g. `C:/Users/[Username]/Documents/Arduino/SureshotSketch/data`
3. Use the Spiff Data Upload Plugin to load the web page to the SPIFF
4. Compile and load the sketch to your board.
    - Install any missing libraries
5. Connect to the AP (see Wifi Config), and go to the gateway address to load the control web page.
6. Profit!

## Wifi Config
In the `data` edit the `wifi.config.json` and fill in your access point credentials.
The default wifi AP is `Sureshot`. password: `WeHaveLiftOff!`

## web dev portion
For those familiar with web dev, the webpage JavaScript is done with KnockoutJs in TypeScript. Other tooling for it is done via npm. See the web folder. 

## Useful Tutorials and Docs:
Getting aquainted with the nodemcu esp8266
- https://randomnerdtutorials.com/getting-started-with-esp8266-wifi-transceiver-review/


fastled 8266 notes:
- https://github.com/FastLED/FastLED/wiki/ESP8266-notes

fasteLed palette:
- https://github.com/FastLED/FastLED/wiki/Gradient-color-palettes

starter tutorial for asynchronous webserver
- https://techtutorialsx.com/2017/12/01/esp32-arduino-asynchronous-http-webserver/

github docs for ESPAsyncWebServer
 - https://github.com/me-no-dev/ESPAsyncWebServer#body-data-handling

Using spiffs file system on the esp8266
- https://tttapa.github.io/ESP8266/Chap11%20-%20SPIFFS.html

example of spiffs on esp32:
- https://techtutorialsx.com/2018/10/04/esp32-http-web-server-serving-external-javascript-file/
- https://microcontrollerslab.com/display-images-esp32-esp8266-web-server-arduino-ide/

esp8266 multiwifi library:
- https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/station-examples.html


Arduino programming with Vscode:
- https://learn.sparkfun.com/tutorials/efficient-arduino-programming-with-arduino-cli-and-visual-studio-code/all
