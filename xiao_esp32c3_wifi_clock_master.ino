//**********************************************************************************************************
//*    clock_wifi --  Clock which time refer NTP using wifi network.                                                          *
//**********************************************************************************************************
//  Optionally, sleep by esp_light_sleep_start() API. Internal states are preserved.
//
//  2023/2/1 created by asmaro

//#include "WiFi.h"
#include "WiFiMulti.h"
#include "Wire.h"
#include <Adafruit_GFX.h>       // install using lib tool
#include <Adafruit_SSD1306.h>   // install using lib tool
#include <esp_sntp.h>           // esp lib
#include <TimeLib.h>            // https://github.com/PaulStoffregen/Time

#define I2C_SDA      4          // I2C DATA
#define I2C_SCK      5          // I2C CLOCK
#define OLED_I2C_ADDRESS 0x3C   // Check the I2C bus of your OLED device
#define SCREEN_WIDTH 128        // OLED display width, in pixels
#define SCREEN_HEIGHT 64        // OLED display height, in pixels
#define OLED_RESET -1           // Reset pin # (or -1 if sharing Arduino reset pin)

WiFiMulti wifiMulti;
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

String ssid =     "wifi1xxxxxxxx";      // WiFi 1
String password = "wifi1passxxxxx";
String ssid2 =     "wifi2xxxxxxxx";   // WiFi 2
String password2 = "wifi2passxxxxx";

struct tm *tm;
int d_mon ;
int d_mday ;
int d_hour ;
int d_min ;
int d_sec ;
int d_wday ;
static const char *weekStr[7] = {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"}; //3文字の英字

const char* ntpServer = "ntp.nict.jp";
const long  gmtOffset_sec = 32400;
const int   daylightOffset_sec = 0;

void setup()
{
    Serial.begin(115200);
    Serial.println("start");
    Wire.begin(); //
    oled.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDRESS);
    oled.clearDisplay();
    oled.setTextColor(SSD1306_WHITE);

    WiFi.mode(WIFI_STA);
    wifiMulti.addAP(ssid.c_str(), password.c_str());  
    wifiMulti.addAP(ssid2.c_str(), password2.c_str());
    wifiMulti.run();   // It may be connected to strong one
    
    while (true) {
      if(WiFi.status() == WL_CONNECTED){ break; }  // WiFi connect OK then next step
      Serial.println("WiFi Err");
      oled.setTextSize(2); // Draw 2X-scale text
      oled.setCursor(0, 0);
      oled.print("WiFi Err");
      oled.display();
      WiFi.disconnect(true);
      delay(5000);
      wifiMulti.run();
      delay(1000*300);  // Wait for Wifi ready
    }
    wifisyncjst(); // refer time and day
    splash();
    WiFi.disconnect(true);  // Connection is no longer needed
    Serial.println("Setup done");
}
void splash()
{
  oled.setTextSize(2); // Draw 2X-scale text
  oled.setCursor(0, 0);
  oled.print("Clock");
  oled.setCursor(0, 15);
  oled.print("Radio");
  oled.display();
  delay(500);
  oled.setCursor(0, 30);
  oled.print("OLED");
  oled.setCursor(0, 45);
  oled.print("V0.5.0");
  oled.display();
  delay(1000);
}

void wifisyncjst() {
  //---------内蔵時計のJST同期--------
  // NTPサーバからJST取得
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  delay(500);
  // 内蔵時計の時刻がNTP時刻に合うまで待機
  while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET) {
    delay(500);
  }
}

void loop()
{
  //---------内蔵時計の表示--------
  char ts[80];
  time_t t = time(NULL);
  tm = localtime(&t);
  d_mon  = tm->tm_mon+1;
  d_mday = tm->tm_mday;
  d_hour = tm->tm_hour;
  d_min  = tm->tm_min;
  d_sec  = tm->tm_sec;
  d_wday = tm->tm_wday;
  //Serial.print("time ");
  sprintf(ts, "%02d-%02d %s", d_mon, d_mday, weekStr[d_wday]);
  oled.setTextSize(2); // Draw 2X-scale text
  oled.clearDisplay();
  oled.setCursor(0, 0);
  oled.print(ts);
  //Serial.println(ts);
  sprintf(ts,"%02d:%02d:%02d",d_hour,d_min,d_sec);
  oled.setCursor(0, 15);
  oled.print(ts);
  //Serial.println(ts);
  oled.display();
  delay(1000);   // Comment out when you use sleep API below 
  /* esp_sleep_enable_timer_wakeup(1000 * 1000); // wake after 1 sec 
  esp_light_sleep_start();   // sleep and wake up here */
}
