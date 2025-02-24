#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <JPEGDEC.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include <XPT2046_Touchscreen.h>

const char* ssid = "<SSID>";
const char* password = "<PASSWORD>";

const String url = "http://<SERVER_IP>:3000/";

TFT_eSPI tft = TFT_eSPI();
SPIClass mySpi = SPIClass(VSPI);
XPT2046_Touchscreen ts(TOUCH_CS, TOUCH_IRQ);
JPEGDEC jpeg;
HTTPClient http;

int JPEGDraw(JPEGDRAW* pDraw) {
    if (pDraw->y >= tft.height()) return 0;
    tft.pushImage(pDraw->x, pDraw->y, pDraw->iWidth, pDraw->iHeight,
                  pDraw->pPixels);
    return 1;
}

enum class TouchAction { UP = 0, DOWN = 1, MOVE = 2 };
TouchAction lastAction = TouchAction::UP;

void requestTouchAction() {
    if (ts.tirqTouched() && ts.touched()) {
        TS_Point p = ts.getPoint();
        if (lastAction == TouchAction::UP) {
            lastAction = TouchAction::DOWN;
        } else if (lastAction == TouchAction::DOWN) {
            lastAction = TouchAction::MOVE;
        }
        const int x = (map(p.x, 450, 3900, 0, TFT_HEIGHT));
        const int y = map(p.y, 280, 3800, 0, TFT_WIDTH);
        StaticJsonDocument<200> doc;
        doc["x"] = x;
        doc["y"] = y;
        doc["action"] = static_cast<int>(lastAction);
        String json;
        serializeJson(doc, json);
        http.addHeader("Content-Type", "application/json");
        const int result = http.PUT(json);
        http.end();
    } else if (lastAction != TouchAction::UP) {
        lastAction = TouchAction::UP;
        StaticJsonDocument<200> doc;
        doc["action"] = static_cast<int>(lastAction);
        String json;
        serializeJson(doc, json);
        http.addHeader("Content-Type", "application/json");
        const int result = http.PUT(json);
        http.end();
    }
}

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);

    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.print("OK! IP=");
    Serial.println(WiFi.localIP());

    Serial.print("Fetching " + url + "... ");

    tft.init();
    tft.setRotation(3);
    tft.fillScreen(TFT_BLACK);

    mySpi.begin(TOUCH_CLK, TOUCH_MISO, TOUCH_MOSI, TOUCH_CS);
    ts.begin(mySpi);
    ts.setRotation(3);

    http.begin(url);
}

void loop() {
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
        String payload = http.getString();
        if (jpeg.openFLASH((uint8_t*)payload.c_str(), payload.length(),
                           JPEGDraw)) {
            jpeg.setPixelType(1);
            jpeg.decode(0, 0, 0);
            jpeg.close();
            http.end();
        }
    }
    requestTouchAction();
}