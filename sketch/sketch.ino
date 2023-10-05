/*********
  Twofingerrightclick
  Complete project details at https://github.com/twofingerrightclick/SureShot
*********/
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESPAsyncWebSrv.h>
#include <ArduinoJson.h>
// for using the every n millis seconds functions
// need to find a libary that has just those features
#include <FastLED.h>
#include <FS.h>  // Include the SPIFFS library

#define RELAY_PIN 13  // d7
#define USE_WIFI_AP_MODE true

bool _armed = false;
bool _firing = false;

String _lastErrorMessage = "no error logged";

// Set web server port number to 80
AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);
  delay(20);
  pinMode(RELAY_PIN, OUTPUT);

  delay(20);
  resetFiring();

  // Initialize SPIFFS
  if (!SPIFFS.begin()) {
    _lastErrorMessage = "An Error has occurred while mounting SPIFFS";
    Serial.println(_lastErrorMessage);
  }


  initializeWifi();

  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());
  Serial.println(WiFi.localIP());
  //------- end wifi

  // ----- server
  // Handle CORS
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Content-Type");
  //when accessing the post end point the browser will do a preflight check with an HTTP OPTIONS call to check for the Access-Control-Allow-Orgin headers
  server.on("/post", HTTP_OPTIONS, [](AsyncWebServerRequest *request) {
    request->send(200);
  });

  server.on("/hello", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("hello request");
    request->send(200, "text/plain", "Hello World");
  });

  server.on("/error", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", _lastErrorMessage);
  });

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    String path = "/index.html";
    if (SPIFFS.exists(path)) {
      request->send(SPIFFS, "/index.html", String(), false);
    } else {
      request->send(200, "text/plain", "error - index.html not found");
    }
  });

  server.on("/arm", HTTP_GET, [](AsyncWebServerRequest *request) {
    _armed = true;
    Serial.println("armed");
    request->send(200, "text/plain", "armed");
  });

  server.on("/disarm", HTTP_GET, [](AsyncWebServerRequest *request) {
    _armed = false;
    Serial.println("disarmed");
    resetFiring();
    request->send(200, "text/plain", "disarmed");
  });

  server.on("/fire", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (!_firing) {
      fire();
      Serial.println("fire request");
    } else {
      Serial.println("request already firing.");
    }
    request->send(200, "text/plain", "fire!");
  });

  server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request) {
    String formattedString = "{ \"armed\" : " + String(_armed) + " }";

    // to-do: get status as string
    request->send(200, "text/plain", formattedString);
  });

  server.begin();
  // ----end server
}

void loop() {
  checkToStopFiring();
  if (!USE_WIFI_AP_MODE) {
    checkWifiStatus();
  }
}

uint8_t _firingCount = 0;

void resetFiring() {
  _firingCount = 0;
  _firing = false;
  _armed = false;
  digitalWrite(RELAY_PIN, LOW);
  Serial.println("reset firing");
}


void fire() {
  _firing = true;
  digitalWrite(RELAY_PIN, HIGH);
  Serial.println("fire!");
}

void checkToStopFiring() {
  if (_firing) {
    EVERY_N_MILLISECONDS(100) {
      _firingCount++;
      // stop firing at around 800ms
      if (_firingCount > 8) {
        resetFiring();
      }
    }
  }
}

JsonObject loadWifiConfig() {

  File configFile = SPIFFS.open("/default.wifi.config.json", "r");
  if (SPIFFS.exists("/wifi.config.json")) {
    Serial.println("using (non-default) wifi config");
    File configFile = SPIFFS.open("/default.wifi.config.json", "r");
  } else {
    Serial.println("using default wifi config");
  }

  StaticJsonDocument<800> wifiConfigDoc;

  deserializeJson(wifiConfigDoc, configFile);
  configFile.close();
  JsonObject root = wifiConfigDoc.as<JsonObject>();
  return root;
}

void initializeWifi() {

  if (USE_WIFI_AP_MODE) {
    useWifiAccessPoint();
  } else {
    useConnectToWifi();
  }
}

void useWifiAccessPoint() {
  JsonObject root = loadWifiConfig();

  const char *ssid = root["ssid"].as<const char *>();
  const char *secret = root["ssid"].as<const char *>();
  configFile.close();

  WiFi.softAP(ssid, secret);

  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
}

uint8_t _wifiCheckInterval = 100;  // seconds
//check on wifi connection status every 100 seconds
void checkWifiStatus() {

  EVERY_N_SECONDS_I(thistimer, _wifiCheckInterval) {
    if (_WifiMulti.run() != WL_CONNECTED) {
      // try to connect more frequently
      _wifiCheckInterval = 5;  // seconds
    } else {
      _wifiCheckInterval = 100;
    }
    thistimer.setPeriod(_wifiCheckInterval);
  }
}

ESP8266WiFiMulti _WifiMulti;
// use as "station": https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/readme.html
void useConnectToWifi() {

  // Set in station mode
  WiFi.mode(WIFI_STA);


  JsonObject root = loadWifiConfig();
  uint8_t numAps = root["numAps"].as<uint8_t>();
  JsonArray aps = root["aps"].as<JsonArray>();

  for (size_t i = 0; i < numAps; i++) {
    const char *ssid = aps[i]["ssid"].as<const char *>();
    const char *secret = aps[i]["secret"].as<const char *>();

    _WifiMulti.addAP(ssid, secret);
  }

  Serial.print("Connecting to wifi");
  while (_WifiMulti.run() != WL_CONNECTED) {  // Wait for the Wi-Fi to connect
    delay(250);
    Serial.print('.');
  }
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
