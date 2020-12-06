#include <Arduino.h>
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecureBearSSL.h>
#include <ESP8266HTTPClient.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>
#include <ArduinoJson.h>
#include <time.h>

int pinCS = 2; // Attach CS to this pin, DIN to MOSI and CLK to SCK (cf http://arduino.cc/en/Reference/SPI )
int numberOfHorizontalDisplays = 4;
int numberOfVerticalDisplays = 1;
char time_value[20];

String dm;
String message;

const char *ssid = "<ssid>";           // SSID
const char *password = "<password>"; // Password

const char *host = "http://localhost";
const String path = "sample_message.json";
// const int httpsPort = 443;
// const char* fingerprint = "AB CD EF XX XX XX";

Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);

int wait = 50; // In milliseconds between scroll movements
int spacer = 1;
int width = 5 + spacer; // The font width is 5 pixels

void setup(void)
{
  Serial.begin(115200);

  WiFi.begin(ssid, password); // Connect to Wi-Fi

  matrix.setIntensity(2);   // Use a value between 0 and 15 for brightness
  matrix.setRotation(0, 1); // The first display is position upside down
  matrix.setRotation(1, 1); // The first display is position upside down
  matrix.setRotation(2, 1); // The first display is position upside down
  matrix.setRotation(3, 1); // The first display is position upside down

  wait = 50; // 0.05 sec
  dm = "...";
}

void loop()
{
  displayMsg(dm);
  displayMsg(dm);
  displayMsg(dm);

  getMsg();
}

void displayMsg(String message)
{
  for (int i = 0; i < width * message.length() + matrix.width() - spacer; i++)
  {
    // matrix.fillScreen(LOW);
    int letter = i / width;
    int x = (matrix.width() - 1) - i % width;
    int y = (matrix.height() - 8) / 2; // center the text vertically
    while (x + width - spacer >= 0 && letter >= 0)
    {
      if (letter < message.length())
      {
        matrix.drawChar(x, y, message[letter], HIGH, LOW, 1); // HIGH LOW means foreground ON, background OFF, reverse these to invert the display!
      }
      letter--;
      x -= width;
    }
    // Send bitmap to display
    matrix.write();
    delay(wait / 2);
  }
}

void getMsg()
{
  StaticJsonDocument<200> doc;

  // wait for Wi-Fi connection
  if ((WiFi.status() == WL_CONNECTED))
  {
    std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);

    // client->setFingerprint(fingerprint);
    client->setInsecure();

    HTTPClient https;

    Serial.print("[HTTPS] begin...\n");

    if (https.begin(*client, host + path))
    { // HTTPS

      Serial.print("[HTTPS] GET...\n");

      // start connection and send HTTP header
      int httpCode = https.GET();

      // httpCode will be negative on error
      if (httpCode > 0)
      {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTPS] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
        {
          String payload = https.getString();

          Serial.println(payload);

          DeserializationError error = deserializeJson(doc, payload);

          String message = doc["message"];
          String first_name = doc["first_name"];

          dm = message + " from " + first_name;
        }
      }
      else
      {
        Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
      }

      https.end();
    }
    else
    {
      Serial.printf("[HTTPS] Unable to connect\n");
    }
  }
}
