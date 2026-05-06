#include <FFat.h>
#include <FS.h>

#include "chart_js.h"
#include "htmx.h"
#include "index_html.h"
#include "index_js.h"
#include "utils_js.h"
#include "line_chart_js.h"
#include "bar_chart_js.h"
#include "global_css.h"

void setup() {
  Serial.begin(921600);
  FFat.begin(true);

  FFat.mkdir("/static");

  fs::File index_html = FFat.open("/index.html", FILE_WRITE);
  fs::File chart_js = FFat.open("/static/chart.umd.min.js", FILE_WRITE);
  fs::File htmx = FFat.open("/static/htmx.min.js", FILE_WRITE);
  fs::File utils_js = FFat.open("/static/utils.js", FILE_WRITE);
  fs::File index_js = FFat.open("/static/index.js", FILE_WRITE);
  fs::File line_chart_js = FFat.open("/static/line_chart.js", FILE_WRITE);
  fs::File bar_chart_js = FFat.open("/static/bar_chart.js", FILE_WRITE);
  fs::File global_css = FFat.open("/static/global.css", FILE_WRITE);

  const static char FILE_WRITE_TAG[11] = "FILE_WRITE";
  if (index_html.print(index_html_str) == (sizeof(index_html_str) - 1))
  {
    ESP_LOGI(FILE_WRITE_TAG, "File written (index.html)");
  }
  else
  {
    delay(50);
    ESP_LOGE(FILE_WRITE_TAG, "Writting file failed (index.html)");
    delay(50);
  }
  if (chart_js.print(chart_js_str) == (sizeof(chart_js_str) - 1))
  {
    delay(50);
    ESP_LOGI(FILE_WRITE_TAG, "File written (chart.umd.min.js)");
    delay(50);
  }
  else
  {
    delay(50);
    ESP_LOGE(FILE_WRITE_TAG, "Writting file failed (chart.umd.min.js)");
    delay(50);
  }
  if (htmx.print(htmx_str) == (sizeof(htmx_str) - 1))
  {
    delay(50);
    ESP_LOGI(FILE_WRITE_TAG, "File written (htmx.min.js)");
    delay(50);
  }
  else
  {
    delay(50);
    ESP_LOGE(FILE_WRITE_TAG, "Writting file failed (htmx.min.js)");
    delay(50);
  }
  if (utils_js.print(utils_js_str) == (sizeof(utils_js_str) - 1))
  {
    delay(50);
    ESP_LOGI(FILE_WRITE_TAG, "File written (utils.js)");
    delay(50);
  }
  else
  {
    delay(50);
    ESP_LOGE(FILE_WRITE_TAG, "Writting file failed (utils.js)");
    delay(50);
  }
  if (index_js.print(index_js_str) == (sizeof(index_js_str) - 1))
  {
    delay(50);
    ESP_LOGI(FILE_WRITE_TAG, "File written (index.js)");
    delay(50);
  }
  else
  {
    delay(50);
    ESP_LOGE(FILE_WRITE_TAG, "Writting file failed (index.js)");
    delay(50);
  }
  if (line_chart_js.print(line_chart_js_str) == (sizeof(line_chart_js_str) - 1))
  {
    delay(50);
    ESP_LOGI(FILE_WRITE_TAG, "File written (line_chart.js)");
    delay(50);
  }
  else
  {
    delay(50);
    ESP_LOGE(FILE_WRITE_TAG, "Writting file failed (line_chart.js)");
    delay(50);
  }
  if (bar_chart_js.print(bar_chart_js_str) == (sizeof(bar_chart_js_str) - 1))
  {
    delay(50);
    ESP_LOGI(FILE_WRITE_TAG, "File written (bar_chart.js)");
    delay(50);
  }
  else
  {
    delay(50);
    ESP_LOGE(FILE_WRITE_TAG, "Writting file failed (bar_chart.js)");
    delay(50);
  }
  if (global_css.print(global_css_str) == (sizeof(global_css_str) - 1))
  {
    delay(50);
    ESP_LOGI(FILE_WRITE_TAG, "File written (global.css)");
    delay(50);
  }
  else
  {
    delay(50);
    ESP_LOGE(FILE_WRITE_TAG, "Writting file failed (global.css)");
    delay(50);
  }

  index_html.close();
  chart_js.close();
  htmx.close();
  utils_js.close();
  index_js.close();
  line_chart_js.close();
  bar_chart_js.close();
  global_css.close();
}

void loop() {}