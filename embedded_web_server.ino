#include <string.h>

#include <ESPmDNS.h>
#include <FFat.h>
#include <FS.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include <WiFi.h>
#include <uri/UriBraces.h>

#include <NetworkClientSecure.h>

#include "secrets.h"

using namespace application;

void handleRoot();
void handlePlace();
void handleLineChart();
void handleBarChart();
void handleData();
void handleMeteo();
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

  Serial.print("\n");
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
  server.on(UriBraces("/data/{}/{}"), HTTP_GET, handleData);
  server.on(UriBraces("/meteo/{}"), HTTP_GET, handleMeteo);
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

  String chart_x_title = "Mês";
  if (server.hasArg("x_title"))
  {
    chart_title = server.arg("x_title");
  }

  String chart_y_title = "Quantidade";
  if (server.hasArg("y_title"))
  {
    chart_title = server.arg("y_title");
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

  bool chartShouldHaveContainer = (server.hasArg("contained") && server.arg("contained") == "true");

  size_t buffer_size = 2048;
  char line_chart_template[buffer_size];
  char line_chart_html[buffer_size];
  if (server.hasArg("step_size"))
  {
    if (chartShouldHaveContainer)
    {
      snprintf(
        line_chart_template, buffer_size,
        "%s %s %s",
        "<div class=\"chart-container\">",
        "<line-chart \
          data-title=\"%s\" \
          data-x-title=\"%s\" \
          data-y-title=\"%s\" \
          data-begin-at-zero=\"%s\" \
          data-display-points=\"%s\" \
          data-step-size=\"%s\" \
          data-chart-data=\"%s\" \
        ></line-chart>",
        "</div>"
      );
    }
    else
    {
      strncpy(
        line_chart_template,
        "<line-chart \
          data-title=\"%s\" \
          data-x-title=\"%s\" \
          data-y-title=\"%s\" \
          data-begin-at-zero=\"%s\" \
          data-display-points=\"%s\" \
          data-step-size=\"%s\" \
          data-chart-data=\"%s\" \
        ></line-chart>",
        247
      );
    }

    snprintf(
      line_chart_html, buffer_size, 
      line_chart_template,
      chart_title.c_str(),
      chart_x_title.c_str(),
      chart_y_title.c_str(),
      chart_begin_at_zero.c_str(),
      chart_display_points.c_str(),
      server.arg("step_size").c_str(),
      chart_data.c_str()
    );
  }
  else
  {
    if (chartShouldHaveContainer)
    {
      snprintf(
        line_chart_template, buffer_size,
        "%s %s %s",
        "<div class=\"chart-container\">",
        "<line-chart \
          data-title=\"%s\" \
          data-x-title=\"%s\" \
          data-y-title=\"%s\" \
          data-begin-at-zero=\"%s\" \
          data-display-points=\"%s\" \
          data-chart-data=\"%s\" \
        ></line-chart>",
        "</div>"
      );
    }
    else
    {
      strncpy(
        line_chart_template,
        "<line-chart \
          data-title=\"%s\" \
          data-x-title=\"%s\" \
          data-y-title=\"%s\" \
          data-begin-at-zero=\"%s\" \
          data-display-points=\"%s\" \
          data-chart-data=\"%s\" \
        ></line-chart>",
        216
      );
    }

    snprintf(
      line_chart_html, buffer_size, 
      line_chart_template,
      chart_title.c_str(),
      chart_x_title.c_str(),
      chart_y_title.c_str(),
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

  String chart_x_title = "Mês";
  if (server.hasArg("x_title"))
  {
    chart_x_title = server.arg("x_title");
  }

  String chart_y_title = "Quantidade";
  if (server.hasArg("y_title"))
  {
    chart_y_title = server.arg("y_title");
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

  bool chartShouldHaveContainer = (server.hasArg("contained") && server.arg("contained") == "true");

  size_t buffer_size = 2048;
  char bar_chart_template[buffer_size];
  char bar_chart_html[buffer_size];
  if (server.hasArg("step_size"))
  {
    if (chartShouldHaveContainer)
    {
      snprintf(
        bar_chart_template, buffer_size,
        "%s %s %s",
        "<div class=\"chart-container\">",
        "<bar-chart data-title=\"%s\" data-x-title=\"%s\" data-y-title=\"%s\" data-step-size=\"%s\" data-chart-data=\"%s\"></bar-chart>",
        "</div>"
      );
    }
    else
    {
      strncpy(
        bar_chart_template,
        "<bar-chart data-title=\"%s\" data-x-title=\"%s\" data-y-title=\"%s\" data-step-size=\"%s\" data-chart-data=\"%s\"></bar-chart>",
        117
      );
    }

    snprintf(
      bar_chart_html, buffer_size, 
      bar_chart_template,
      chart_title.c_str(),
      chart_x_title.c_str(),
      chart_y_title.c_str(),
      server.arg("step_size").c_str(),
      chart_data.c_str()
    );
  }
  else
  {
    if (chartShouldHaveContainer)
    {
      snprintf(
        bar_chart_template, buffer_size,
        "%s %s %s",
        "<div class=\"chart-container\">",
        "<bar-chart data-title=\"%s\" data-x-title=\"%s\" data-y-title=\"%s\" data-chart-data=\"%s\"></bar-chart>",
        "</div>"
      );
    }
    else
    {
      strncpy(
        bar_chart_template,
        "<bar-chart data-title=\"%s\" data-x-title=\"%s\" data-y-title=\"%s\" data-chart-data=\"%s\"></bar-chart>",
        97
      );
    }

    snprintf(
      bar_chart_html, buffer_size, 
      bar_chart_template,
      chart_title.c_str(),
      chart_x_title.c_str(),
      chart_y_title.c_str(),
      chart_data.c_str()
    );
  }

  server.send(200, "text/html", bar_chart_html);
}

void handleData()
{
  const static char TAG[14] = "DATA ENDPOINT";
  ESP_LOGI(TAG, "Hit");

  fs::File chart_page = FFat.open("/chart_page.html", FILE_READ);
  if (chart_page)
  {
    server.send(200, "text/html", chart_page.readString());
  }
  chart_page.close(); 
}

const char ca_cert[] = R"is_a_string(
-----BEGIN CERTIFICATE-----
MIIDXzCCAkegAwIBAgILBAAAAAABIVhTCKIwDQYJKoZIhvcNAQELBQAwTDEgMB4G
A1UECxMXR2xvYmFsU2lnbiBSb290IENBIC0gUjMxEzARBgNVBAoTCkdsb2JhbFNp
Z24xEzARBgNVBAMTCkdsb2JhbFNpZ24wHhcNMDkwMzE4MTAwMDAwWhcNMjkwMzE4
MTAwMDAwWjBMMSAwHgYDVQQLExdHbG9iYWxTaWduIFJvb3QgQ0EgLSBSMzETMBEG
A1UEChMKR2xvYmFsU2lnbjETMBEGA1UEAxMKR2xvYmFsU2lnbjCCASIwDQYJKoZI
hvcNAQEBBQADggEPADCCAQoCggEBAMwldpB5BngiFvXAg7aEyiie/QV2EcWtiHL8
RgJDx7KKnQRfJMsuS+FggkbhUqsMgUdwbN1k0ev1LKMPgj0MK66X17YUhhB5uzsT
gHeMCOFJ0mpiLx9e+pZo34knlTifBtc+ycsmWQ1z3rDI6SYOgxXG71uL0gRgykmm
KPZpO/bLyCiR5Z2KYVc3rHQU3HTgOu5yLy6c+9C7v/U9AOEGM+iCK65TpjoWc4zd
QQ4gOsC0p6Hpsk+QLjJg6VfLuQSSaGjlOCZgdbKfd/+RFO+uIEn8rUAVSNECMWEZ
XriX7613t2Saer9fwRPvm2L7DWzgVGkWqQPabumDk3F2xmmFghcCAwEAAaNCMEAw
DgYDVR0PAQH/BAQDAgEGMA8GA1UdEwEB/wQFMAMBAf8wHQYDVR0OBBYEFI/wS3+o
LkUkrk1Q+mOai97i3Ru8MA0GCSqGSIb3DQEBCwUAA4IBAQBLQNvAUKr+yAzv95ZU
RUm7lgAJQayzE4aGKAczymvmdLm6AC2upArT9fHxD4q/c2dKg8dEe3jgr25sbwMp
jjM5RcOO5LlXbKr8EpbsU8Yt5CRsuZRj+9xTaGdWPoO4zzUhw8lo/s7awlOqzJCK
6fBdRoyV3XpYKBovHd7NADdBj+1EbddTKJd+82cEHhXXipa0095MJ6RMG3NzdvQX
mcIfeg7jLQitChws/zyrVQ4PkX4268NXSb7hLi18YIvDQVETI53O9zJrlAGomecs
Mx86OyXShkDOOyyGeMlhLxS67ttVb9+E7gUJTb0o2HLO02JQZR7rkpeDMdmztcpH
WD9f
-----END CERTIFICATE-----
)is_a_string";

void handleMeteo()
{
  const static char TAG[15] = "METEO ENDPOINT";
  ESP_LOGI(TAG, "Hit");

  /* NetworkClientSecure* client = new NetworkClientSecure;
  client->setCACert(ca_cert);
  // client->setInsecure();

  HTTPClient http;
  http.begin(*client, // O problema é o cors?
    "https://panorama.sipam.gov.br/api/meteorologia/v1/previsao/tempo?token=vGJ6ESuSPniKednMPaFKUgkYimcULHpgJDJ5JDEyJFI2UEpCSGtJeVBZL0E0RnJWSy9scWUzMzNmRTZURDNUUGZhL1IxLzguNnNBb2lLaWtpLjdD&data=10%2F05%2F2026&hora=1&municipio_id=1503606"
  );
  http.addHeader("Content-Type", "application/json");
  // http.addHeader("X-CSRF-TOKEN", "kADLcqbnN1rRmQwTuz2pKkc3va3W12IdD0ehwwED");
  int8_t http_code = http.GET();
  if (http_code > 0)
  {
    if (http_code == HTTP_CODE_OK)
    {
      Serial.println("FOI ");
      Serial.println(http.getString());
    }
  }
  else
  {
    Serial.print("ERRO ");
    Serial.println(http_code);
    Serial.println(http.errorToString(http_code));
  }
  http.end(); */
  fs::File meteo_page = FFat.open("/meteo_page.html", FILE_READ);
  if (meteo_page)
  {
    server.send(200, "text/html", meteo_page.readString());
  }
  meteo_page.close();
}

void handleNotFound()
{
  const static char TAG[19] = "NOT FOUND ENDPOINT";
  ESP_LOGI(TAG, "Hit");
  server.send(404, "text/plain", "No content!");
}