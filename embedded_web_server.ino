#include <string.h>

#include <ESPmDNS.h>
#include <FFat.h>
#include <FS.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include <WiFi.h>

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
  server.on("/line_chart", HTTP_GET, handleLineChart);
  server.on("/bar_chart", HTTP_GET, handleBarChart);
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
  const static char TAG[14] = "ROOT ENDPOINT";
  ESP_LOGI(TAG, "Hit");

  fs::File index = FFat.open("/index.html", FILE_READ);
  if (index)
  {
    server.send(200, "text/html", index.readString());
  }
  index.close();
}

void handlePlace()
{
  const static char TAG[20] = "LINE CHART ENDPOINT";
  ESP_LOGI(TAG, "Hit");

  fs::File content = FFat.open("/index.html", "r");
  listDir(FFat, "/", 2);

  String buffer = WebServer::urlDecode("%5B1%2C2%2C3%5D");
  server.send(200, "text/html", buffer);
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
  const static char TAG[20] = "LINE CHART ENDPOINT";
  ESP_LOGI(TAG, "Hit");

  String chart_title = "GRÁFICO";
  if (server.hasArg("title"))
  {
    chart_title = server.arg("title");
  }

  String chart_begin_at_zero = "true";
  if (server.hasArg("begin_at_zero"))
  {
    chart_begin_at_zero = server.arg("begin_at_zero");
  }

  String chart_display_points = "false";
  if (server.hasArg("display_points"))
  {
    chart_display_points = server.arg("display_points");
  }

  String chart_labels = "[&quot;Janeiro&quot;, &quot;Fevereiro&quot;]";
  String chart_coordinate_label = "&quot;Value&quot;";
  String chart_coordinates = "[{&quot;x&quot;: 1, &quot;y&quot;: 2}, {&quot;x&quot;: 2, &quot;y&quot;: 1}]";
  String chart_data = "{&quot;labels&quot;: "
                      + chart_labels
                      + ", &quot;datasets&quot;: [{&quot;label&quot;: "
                      + chart_coordinate_label
                      + ", &quot;data&quot;: "
                      + chart_coordinates
                      + "}]}";
  if (server.hasArg("labels") && server.hasArg("coordinate_label") && server.hasArg("coordinates"))
  {
    chart_labels = WebServer::urlDecode(server.arg("labels"));
    chart_labels.replace("\"", "&quot;");

    chart_coordinate_label = server.arg("coordinate_label");

    chart_coordinates = WebServer::urlDecode(server.arg("coordinates"));
    chart_coordinates.replace("\"", "&quot;");

    chart_data = "{&quot;labels&quot;: ";
    chart_data.concat(chart_labels);
    chart_data.concat(", &quot;datasets&quot;: [{&quot;label&quot;: &quot;");
    chart_data.concat(chart_coordinate_label);
    chart_data.concat("&quot;, &quot;data&quot;: ");
    chart_data.concat(chart_coordinates);
    chart_data.concat("}]}");
  }

  size_t buffer_size = 1000;
  char line_chart_html[buffer_size];

  if (server.hasArg("step_size"))
  {
    snprintf(
      line_chart_html, buffer_size, 
      "<line-chart \
        data-title=\"%s\" \
        data-begin-at-zero=\"%s\" \
        data-display-points=\"%s\" \
        data-step-size=\"%s\" \
        data-chart-data=\"%s\" \
      ></line-chart>",
      chart_title.c_str(),
      chart_begin_at_zero.c_str(),
      chart_display_points.c_str(),
      server.arg("step_size").c_str(),
      chart_data.c_str()
    );
  }
  else
  {
    snprintf(
      line_chart_html, buffer_size, 
      "<line-chart data-title=\"%s\" data-begin-at-zero=\"%s\" data-display-points=\"%s\" data-chart-data=\"%s\"></line-chart>",
      chart_title.c_str(),
      chart_begin_at_zero.c_str(),
      chart_display_points.c_str(),
      chart_data.c_str()
    );
  }

  server.send(200, "text/html", line_chart_html);
}

void handleBarChart()
{
  const static char TAG[19] = "BAR CHART ENDPOINT";
  ESP_LOGI(TAG, "Hit");

  String chart_title = "GRÁFICO";
  if (server.hasArg("title"))
  {
    chart_title = server.arg("title");
  }

  String chart_labels = "[&quot;Janeiro&quot;, &quot;Fevereiro&quot;]";
  String chart_coordinate_label = "&quot;Value&quot;";
  String chart_coordinates = "[{&quot;x&quot;: 1, &quot;y&quot;: 2}, {&quot;x&quot;: 2, &quot;y&quot;: 1}]";
  String chart_data = "{&quot;labels&quot;: "
                      + chart_labels
                      + ", &quot;datasets&quot;: [{&quot;label&quot;: "
                      + chart_coordinate_label
                      + ", &quot;data&quot;: "
                      + chart_coordinates
                      + "}]}";
  if (server.hasArg("labels") && server.hasArg("coordinate_label") && server.hasArg("coordinates"))
  {
    chart_labels = WebServer::urlDecode(server.arg("labels"));
    chart_labels.replace("\"", "&quot;");
    
    chart_coordinate_label = server.arg("coordinate_label");
    
    chart_coordinates = WebServer::urlDecode(server.arg("coordinates"));
    chart_coordinates.replace("\"", "&quot;");

    chart_data = "{&quot;labels&quot;: ";
    chart_data.concat(chart_labels);
    chart_data.concat(", &quot;datasets&quot;: [{&quot;label&quot;: &quot;");
    chart_data.concat(chart_coordinate_label);
    chart_data.concat("&quot;, &quot;data&quot;: ");
    chart_data.concat(chart_coordinates);
    chart_data.concat("}]}");
  }

  size_t buffer_size = 1000;
  char bar_chart_html[buffer_size];
  if (server.hasArg("step_size"))
  {
    snprintf(
      bar_chart_html, buffer_size, 
      "<bar-chart data-title=\"%s\" data-step-size=\"%s\" data-chart-data=\"%s\"></bar-chart>",
      chart_title.c_str(),
      server.arg("step_size").c_str(),
      chart_data.c_str()
    );
  }
  else
  {
    snprintf(
      bar_chart_html, buffer_size, 
      "<bar-chart data-title=\"%s\" data-chart-data=\"%s\"></bar-chart>",
      chart_title.c_str(),
      chart_data.c_str()
    );
  }

  server.send(200, "text/html", bar_chart_html);
}

void handleData()
{
  const static char TAG[14] = "DATA ENDPOINT";
  ESP_LOGI(TAG, "Hit");

  HTTPClient http;
  http.begin("https://servicodados.ibge.gov.br/api/v1/paises/BR/indicadores/77849");
  int code = http.GET();

  String response = http.getString();
  server.sendHeader("Content-Encoding", "gzip");
  server.send(200, "application/json", response);

  http.end();
}

void handleNotFound()
{
  const static char TAG[19] = "NOT FOUND ENDPOINT";
  ESP_LOGI(TAG, "Hit");
  server.send(404, "text/plain", "No content!");
}