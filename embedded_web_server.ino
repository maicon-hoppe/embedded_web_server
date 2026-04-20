#include <ESPmDNS.h>
#include <FFat.h>
#include <FS.h>
#include <WebServer.h>
#include <WiFi.h>

#include <HTTPClient.h>

#include "secrets.h"

using namespace application;

void handleRoot();
void handlePlace();
void handleLineChart();
void handleBarChart();
void handleData();
void handleNotFound();

WebServer server(80);

void listDir(fs::FS &fs, const char *dirname, uint8_t levels) {
  Serial.printf("Listing directory: %s\r\n", dirname);

  File root = fs.open(dirname);
  if (!root) {
    Serial.println("- failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println(" - not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels) {
        listDir(fs, file.path(), levels - 1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("\tSIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}

void setup() {
  Serial.begin(921600);
  const static char FS_TAG[12] = "FILE SYSTEM";
  while (!FFat.begin())
  {
    ESP_LOGI(FS_TAG, "File system not mounted");
    delay(100);
  }
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, wifi_password);

  pinMode(LED_BUILTIN, OUTPUT);

  static const char WIFI_TAG[12] = "WIFI_STATUS";
  while (WiFi.status() != WL_CONNECTED)
  {
    ESP_LOGI(WIFI_TAG, "Establishing connection...");
    delay(200);
  }
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);

  static const char MDNS_TAG[12] = "MDNS_STATUS";
  while (!MDNS.begin("esp32_webserver"))
  {
    ESP_LOGI(MDNS_TAG, "Initializing dns...");
    delay(200);
  }

  ESP_LOGI(WIFI_TAG, "IP Address: %s\n", WiFi.localIP().toString());

  configTzTime("America/Santarem", "pool.ntp.org");
  server.enableCORS(true);
  server.enableETag(true);

  server.serveStatic("/static", FFat, "/static");

  server.on("/", HTTP_GET, handleRoot);
  server.on("/place", HTTP_GET, handlePlace);
  server.on("/line_chart.js", HTTP_GET, handleLineChart);
  server.on("/bar_chart.js", HTTP_GET, handleBarChart);
  server.on("/data", HTTP_GET, handleData);
  server.onNotFound(handleNotFound);

  server.begin();
}

void loop() {
  server.handleClient();
  delay(3);
}

void handleRoot()
{
  fs::File index = FFat.open("/index.html", FILE_READ);
  if (index)
  {
    server.send(200, "text/html", index.readString());
  }
  index.close();
}

void handlePlace()
{
  fs::File content = FFat.open("/index.html", "r");
  listDir(FFat, "/", 2);

  server.send(200, "text/html", "<h1>Olá Maicon</h1><hr>");
  if (content)
  {
    static const char TAG[5] = "HTML";
    ESP_LOGI(TAG, "Página acessada!");
    // server.send(200, "text/html", content.readString());
  }
  content.close();
}

void handleLineChart()
{
  fs::File line_chart_js = FFat.open("/line_chart.js", FILE_READ);
  if (line_chart_js)
  {
    server.send(200, "text/javascript", line_chart_js.readString());
  }
  line_chart_js.close();
}

void handleBarChart()
{
  fs::File bar_chart_js = FFat.open("/bar_chart.js", FILE_READ);
  if (bar_chart_js)
  {
    server.send(200, "text/javascript", bar_chart_js.readString());
  }
  bar_chart_js.close();
}

void handleData()
{
  // JsonDocument coutries_data_filter;
  // coutries_data_filter[0]["series"]["serie"][0] = true;

  // SimpleAPI* IBGEDataAPI = new SimpleAPI(
    // "https://servicodados.ibge.gov.br/api/v1/paises/BR/indicadores/77849",
    // &coutries_data_filter
  // );

  HTTPClient http;
  http.begin("https://servicodados.ibge.gov.br/api/v1/paises/BR/indicadores/77849");
  // http.setAcceptEncoding("identity");
  int code = http.GET();

  String response = http.getString();
  server.sendHeader("Content-Encoding", "gzip");
  server.send(200, "application/json", response);

  http.end();
  // delete IBGEDataAPI;
}

void handleNotFound()
{
  server.send(404, "text/plain", "No content!");
}