//Temp - humidity version Test for git changes2
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <ArduinoJson.h>
#include <TimeLib.h>
#include <base64.h>
//For Display
#include <GxEPD.h>
// select the display class to use, only one
//#include <GxGDEP015OC1/GxGDEP015OC1.cpp>    // 1.54" b/w
#include <GxGDEW0154Z04/GxGDEW0154Z04.cpp>  // 1.54" b/w/r
#include GxEPD_BitmapExamples

// FreeFonts from Adafruit_GFX
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>
// FreeFonts from Adafruit_GFX
#include <Fonts/FreeSans18pt7b.h>

#include <GxIO/GxIO_SPI/GxIO_SPI.cpp>
#include <GxIO/GxIO.cpp>

#if defined(_GxGDEW0154Z04_H_)
#define HAS_RED_COLOR
#endif

#if defined(ESP8266)

// generic/common.h
//static const uint8_t SS    = 15;
//static const uint8_t MOSI  = 13;
//static const uint8_t MISO  = 12;
//static const uint8_t SCK   = 14;
// pins_arduino.h
//static const uint8_t D8   = 15;
//static const uint8_t D7   = 13;
//static const uint8_t D6   = 12;
//static const uint8_t D5   = 14;

// GxIO_SPI(SPIClass& spi, int8_t cs, int8_t dc, int8_t rst = -1, int8_t bl = -1);
GxIO_Class io(SPI, SS, 0, 2); // arbitrary selection of D3(=0), D4(=2), selected for default of GxEPD_Class
// GxGDEP015OC1(GxIO& io, uint8_t rst = 2, uint8_t busy = 4);
GxEPD_Class display(io); // default selection of D4(=2), D2(=4)

#elif defined(ESP32)

// pins_arduino.h, e.g. LOLIN32
//static const uint8_t SS    = 5;
//static const uint8_t MOSI  = 23;
//static const uint8_t MISO  = 19;
//static const uint8_t SCK   = 18;

// GxIO_SPI(SPIClass& spi, int8_t cs, int8_t dc, int8_t rst = -1, int8_t bl = -1);
GxIO_Class io(SPI, SS, 17, 16); // arbitrary selection of 17, 16
// GxGDEP015OC1(GxIO& io, uint8_t rst = D4, uint8_t busy = D2);
GxEPD_Class display(io, 16, 4); // arbitrary selection of (16), 4

#elif defined(ARDUINO_ARCH_SAMD)

// variant.h of MKR1000
//#define PIN_SPI_MISO  (10u)
//#define PIN_SPI_MOSI  (8u)
//#define PIN_SPI_SCK   (9u)
//#define PIN_SPI_SS    (24u) // should be 4?
// variant.h of MKRZERO
//#define PIN_SPI_MISO  (10u)
//#define PIN_SPI_MOSI  (8u)
//#define PIN_SPI_SCK   (9u)
//#define PIN_SPI_SS    (4u)

GxIO_Class io(SPI, 4, 7, 6);
GxEPD_Class display(io, 6, 5);

#elif defined(_BOARD_GENERIC_STM32F103C_H_)

// STM32 Boards (STM32duino.com)
// Generic STM32F103C series
// aka BluePill
// board.h
//#define BOARD_SPI1_NSS_PIN        PA4
//#define BOARD_SPI1_MOSI_PIN       PA7
//#define BOARD_SPI1_MISO_PIN       PA6
//#define BOARD_SPI1_SCK_PIN        PA5
//enum {
//    PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7, PA8, PA9, PA10, PA11, PA12, PA13,PA14,PA15,
//  PB0, PB1, PB2, PB3, PB4, PB5, PB6, PB7, PB8, PB9, PB10, PB11, PB12, PB13,PB14,PB15,
//  PC13, PC14,PC15
//};
// variant.h
//static const uint8_t SS   = BOARD_SPI1_NSS_PIN;
//static const uint8_t SS1  = BOARD_SPI2_NSS_PIN;
//static const uint8_t MOSI = BOARD_SPI1_MOSI_PIN;
//static const uint8_t MISO = BOARD_SPI1_MISO_PIN;
//static const uint8_t SCK  = BOARD_SPI1_SCK_PIN;

// original mapping suggestion for STM32F1, e.g. STM32F103C8T6 "BluePill"
// BUSY -> A3, RST -> A9, DC -> A8, CS-> A4, CLK -> A5, DIN -> A7

// GxIO_SPI(SPIClass& spi, int8_t cs, int8_t dc, int8_t rst = -1, int8_t bl = -1);
//GxIO_Class io(SPI, SS, 8, 9);
// GxGDEP015OC1(GxIO& io, uint8_t rst = 9, uint8_t busy = 7);
//GxEPD_Class display(io, 9, 3);

// new mapping suggestion for STM32F1, e.g. STM32F103C8T6 "BluePill"
// BUSY -> A1, RST -> A2, DC -> A3, CS-> A4, CLK -> A5, DIN -> A7

// GxIO_SPI(SPIClass& spi, int8_t cs, int8_t dc, int8_t rst = -1, int8_t bl = -1);
GxIO_Class io(SPI, SS, 3, 2);
// GxGDEP015OC1(GxIO& io, uint8_t rst = 9, uint8_t busy = 7);
GxEPD_Class display(io, 2, 1);

#else

// pins_arduino.h, e.g. AVR
//#define PIN_SPI_SS    (10)
//#define PIN_SPI_MOSI  (11)
//#define PIN_SPI_MISO  (12)
//#define PIN_SPI_SCK   (13)

GxIO_Class io(SPI, SS, 8, 9); // arbitrary selection of 8, 9 selected for default of GxEPD_Class
GxEPD_Class display(io);

#endif

// For OTA Updates
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

#define xstr(a) str(a)
#define str(a) #a

//For DHT22
#define DHTPIN            12         // D6 Pin which is connected to the DHT sensor.
// 2 D4 for unit 1
// Uncomment the type of sensor in use:
//#define DHTTYPE           DHT11     // DHT 11
#define DHTTYPE           DHT22     // DHT 22 (AM2302)
//#define DHTTYPE           DHT21     // DHT 21 (AM2301)

// See guide for details on sensor wiring and usage:
//   https://learn.adafruit.com/dht/overview

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;
float temperature;
float humidity;
int humStatus = 0;
char minuteBuffer[6];
unsigned char level;
const char* serverTime;
const char* sunriseTime;
const char* sunsetTime;
int dayMode = 1;



//Import build flags from platformio.ini
const int FW_VERSION = VERSION;
const int domoticzID = DOMOTICZ_ID;
const String BOARD_ID = xstr(UNIT_NAME);

const char* fwUrlBase = "http://192.168.0.39/fota/";
const String fwUrlBaseTest = xstr(OTA_URL);


IPAddress ip(192, 168, 0, 201);// 92 for LUX81
IPAddress gateway(192,168,0,1);
IPAddress subnet(255,255,255,0);
const char* mqtt_server = "192.168.0.39";
int timeInterval = 300; // seconds
long rssi;
unsigned char rssi_level;
int status = WL_IDLE_STATUS;
long timeStamp = 0;
int sunrise = 6;
int sunset = 18;
int theHourA;
int theHourB;
int theMinuteA;
int theMinuteB;

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
char msg2[50];

//For battery voltage
unsigned int raw=0;
float volt=0.0;
float v_reference = 7.1026;


void getServerTime() {
  Serial.println("getting Server Time");
  HTTPClient httpTime;
  httpTime.begin("http://192.168.0.39:8084/json.htm?type=command&param=getSunRiseSet");//username=c3RlcGhlbg==&password=Um9ja2V0Mw==&
  httpTime.addHeader("Content-Type", "application/json");
  httpTime.addHeader("Authorization", "Basic c3RlcGhlbjpSb2NrZXQz");
  int httpReturn = httpTime.GET();
  Serial.println(httpReturn);
  if( httpReturn > 0 ) {
    const size_t bufferSize1 = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + 140;
    DynamicJsonBuffer jsonBuffer(bufferSize1);
    JsonObject& root = jsonBuffer.parseObject(httpTime.getString());
    if (!root.success()) {
      Serial.println("parseObject() failed");
      return;
    }
    serverTime = root["ServerTime"];
    sunriseTime = root["Sunrise"];
    sunsetTime = root["Sunset"];

    // Serial.println(serverTime);
    // Serial.println(sunriseTime);
    // Serial.println(sunsetTime);
    //Server time in minutes
    theHourA = serverTime[11] - '0';
    theHourB = serverTime[12] - '0';
    theMinuteA = serverTime[14] - '0';
    theMinuteB = serverTime[15] - '0';
    int dayMinute = (theHourA * 600) + (theHourB * 60) + (theMinuteA * 10) + theMinuteB;
    Serial.println(dayMinute);
    //SunriseTime in Minutes
    int theRiseHourA = sunriseTime[0] - '0';
    int theRiseHourB = sunriseTime[1] - '0';
    int theRiseMinuteA = sunriseTime[2] - '0';
    int theRiseMinuteB = sunriseTime[3] - '0';
    int riseMinute = (theRiseHourA * 600) + (theRiseHourB * 60) + (theRiseMinuteA * 10) + theRiseMinuteB;
    Serial.println(riseMinute);
    //Sunset time in minutes
    int theSetHourA = sunsetTime[0] - '0';
    int theSetHourB = sunsetTime[1] - '0';
    int theSetMinuteA = sunsetTime[2] - '0';
    int theSetMinuteB = sunsetTime[3] - '0';
    int setMinute = (theSetHourA * 600) + (theSetHourB * 60) + (theSetMinuteA * 10) + theSetMinuteB;
    Serial.println(setMinute);
    if (dayMinute < riseMinute || dayMinute > setMinute ) {
        timeInterval = timeInterval * 3;
        dayMode = 0;

  }
  httpTime.end();

  }
}

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void checkForUpdates() {
  String fwURL = String( fwUrlBase );
  fwURL.concat( BOARD_ID );
  String fwVersionURL = fwURL;
  fwVersionURL.concat( ".version" );

  Serial.println( "Checking for firmware updates." );
  Serial.println(fwUrlBaseTest);
  Serial.print( "Firmware version URL: " );
  Serial.println( fwVersionURL );

  HTTPClient httpClient;
  httpClient.begin( fwVersionURL );
  int httpCode = httpClient.GET();
  if( httpCode == 200 ) {
    String newFWVersion = httpClient.getString();

    Serial.print( "Current firmware version: " );
    Serial.println( FW_VERSION );

    String version = getValue(newFWVersion, ',', 0);
    String reference = getValue(newFWVersion, ',', 1);

    Serial.print( "Available firmware version: " );
    Serial.println( version );


    int newVersion = version.toInt();
    v_reference = reference.toFloat();

    Serial.print( "Reference set to : " );
    Serial.println(v_reference);

    if( newVersion > FW_VERSION ) {
      Serial.println( "Preparing to update" );

      //update Display
      display.setRotation(2);
      display.fillScreen(GxEPD_WHITE);
      display.setFont(&FreeMonoBold9pt7b);
      display.setTextColor(GxEPD_BLACK);
      display.setCursor(4, 16);
      display.print("Current   FW :");
      display.print(FW_VERSION);
      display.setCursor(4, 32);
      display.print("Available FW :");
      display.print(version);
      display.setCursor(4, 60);
      display.print("Updating!");
      display.update();

      String fwImageURL = fwURL;
      String fwImageURLEnd = "_";
      fwImageURLEnd.concat(version);
      fwImageURLEnd.concat( ".bin" );
      fwImageURL.concat(fwImageURLEnd);

      Serial.print("Downloading new Firmware : ");
      Serial.println(fwImageURL);

      t_httpUpdate_return ret = ESPhttpUpdate.update( fwImageURL );

      switch(ret) {
        case HTTP_UPDATE_FAILED:
          Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
          break;

        case HTTP_UPDATE_NO_UPDATES:
          Serial.println("HTTP_UPDATE_NO_UPDATES");
          break;
      }
    }
    else {
      Serial.println( "Already on latest version" );
    }
  }
  else {
    Serial.print( "Firmware version check failed, got HTTP response code " );
    Serial.println( httpCode );
  }
  httpClient.end();
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  //Serial.println(ssid);
  Serial.println(xstr(SSID_NAME));
  Serial.print("From MAC address : ");
  Serial.println(WiFi.macAddress());

  // For DHCP IP
  WiFi.begin(xstr(SSID_NAME), xstr(SSID_PASSWORD));

  // For static IP
  //WiFi.config(ip, gateway, subnet);
  while (WiFi.status() != WL_CONNECTED) {
    // For Static IP
    //status = WiFi.begin(xstr(SSID_NAME), xstr(SSID_PASSWORD));

    delay(500);
    Serial.print(".");
  }
  rssi = WiFi.RSSI();
  rssi_level = map((rssi * 10), -1000, -500, 0, 10);
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("RSSI: ");
  Serial.println(rssi);
  Serial.println(rssi_level);
}

void publishData(){
  Serial.println("Attempting to Publish Data");
  /* Get a new sensor event */
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println("Error reading temperature!");
    Serial.print("Entering deep sleep mode for ");
    Serial.print("60");
    Serial.println(" seconds");
    ESP.deepSleep(60 * 1000000); // timed deepsleep
  }
  else {
    temperature = event.temperature;
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" *C");
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);

  if (isnan(event.relative_humidity)) {
    Serial.println("Error reading humidity!");
    Serial.print("Entering deep sleep mode for ");
    Serial.print("60");
    Serial.println(" seconds");
    ESP.deepSleep(60 * 1000000); // timed deepsleep
  }
  else {
    humidity = event.relative_humidity;
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println("%");

    if (humidity < 25) {
      humStatus = 2;
    }
    if (humidity > 60) {
      humStatus = 3;
    }
    if (humidity > 40 && humidity < 50) {
      humStatus = 1;
    }
  }
  // Get battery Voltage
  raw = analogRead(A0);
  Serial.print(raw);
  volt=raw/1023.0;
  volt=volt*v_reference;
  Serial.print(" Battery Voltage: ");
  Serial.print(volt);
  Serial.print("V - ");

  level = map((volt * 1000), 3200, 4200, 0, 100);
  Serial.print(level);
  Serial.println("%");
  if (level > 100) {
    level = 100;
  }

  //snprintf (msg2, 75, "{\"idx\": %s , \"nvalue\": 0, \"svalue\": \"%.2f\", \"Battery\": %d}", domoticzID.c_str(), LUX, level);
  snprintf (msg2, 100, "{\"idx\": %d , \"nvalue\": 0, \"svalue\": \"%.2f;%.2f;%d\", \"Battery\": %d, \"RSSI\": %d}", domoticzID, temperature, humidity, humStatus, level, rssi_level);

  //String payload = "{\"idx\": " + domoticzID + ", \"nvalue\": 0, \"svalue\": \"" + event.light + "\", \"Battery\": " + level + "}";
  for (size_t i = 0; i < 1; i++) {
    client.publish("domoticz/in", msg2, true);
    Serial.print("Message sent :");
    Serial.println(i);
    Serial.println(msg2);
    delay(250);
  }
}


void showFont()
{
  display.setRotation(2);
  display.fillScreen(GxEPD_WHITE);
  display.fillRect(1, 1, display.width()-2, 52, GxEPD_BLACK);
  display.setTextColor(GxEPD_WHITE);
  display.setFont(&FreeMonoBold9pt7b);
  display.setCursor(4, 16);
  display.print(WiFi.localIP());
  display.setCursor(4, 32);
  display.print(WiFi.macAddress());
  display.setCursor(4, 48);
  display.print(volt);
  display.print("V");
  display.setCursor(70, 48);
  display.print("FW");
  display.print(FW_VERSION);
  display.setCursor(136, 48);
  display.print("ID");
  display.print(domoticzID);

  //battery
  display.setFont(&FreeMonoBold12pt7b);
  display.setTextColor(GxEPD_BLACK);
  // if (level <= 10) {
  //   display.setTextColor(GxEPD_RED);
  // }
  display.setCursor(04, 74);
  display.print(level);
  display.print("%");

  //Day / Night
  display.setCursor(90, 74);
  if (dayMode) {
    //display.setTextColor(GxEPD_RED);
    display.setTextColor(GxEPD_BLACK);
    display.print("D");
  }
  else{
    display.setTextColor(GxEPD_BLACK);
    display.print("N");
  }
  //Time
  display.setTextColor(GxEPD_BLACK);
  display.setCursor(126, 74);
  sprintf(minuteBuffer, "%d%d:%d%d", theHourA, theHourB, theMinuteA, theMinuteB);
  display.println(minuteBuffer) ;

  display.setFont(&FreeSans18pt7b);
  display.setTextSize(2);
  display.setCursor(8, 136);
  //display.setTextColor(GxEPD_RED);
  display.print(temperature, 1);
  display.setTextSize(1);
  display.println("C");

  display.setCursor(8, 196);
  //display.setTextColor(GxEPD_BLACK);
  display.setTextSize(2);
  display.print(humidity, 1);
  display.setTextSize(1);
  display.println("%");
  display.update();
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");

      char charBuf[20];
      BOARD_ID.toCharArray(charBuf, 20);

      snprintf (msg, 75, "{\"Board_ID\": \"%s\" , \"firmware\": %d}", charBuf, FW_VERSION);

      //snprintf (msg2, 75, "{\"idx\": 86 , \"nvalue\": 0, \"svalue\": \"39.00\", \"Battery\": 100}");

      client.publish("ota_firmware", msg, true);
      // Move the main publish
      /* Get a new sensor event */
      publishData();


    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
  //showFont("FreeMonoBold9pt7b", &FreeMonoBold9pt7b);
  showFont();
  Serial.print("Entering deep sleep mode for ");
  Serial.print(timeInterval);
  Serial.println(" seconds");
  ESP.deepSleep(timeInterval * 1000000); // timed deepsleep
}

/**************************************************************************/

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
  display.init();
  Serial.begin(115200);

  Serial.println("Device Started");
  Serial.println("-------------------------------------");
  Serial.println("Running Deep Sleep Firmware!");
  Serial.println("-------------------------------------");

  setup_wifi();
  client.setServer(mqtt_server, 1883);

  dht.begin();

}

void loop() {
  getServerTime();
  checkForUpdates();

  if (!client.connected()) {
    reconnect();
  }
}
