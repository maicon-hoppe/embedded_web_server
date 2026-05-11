#include <vector>
#include <FFat.h>
#include <FS.h>

#include "index_html.h"
#include "chart_page_html.h"
#include "meteo_page_html.h"
#include "chart_js.h"
#include "htmx.h"
#include "index_js.h"
#include "chart_page_js.h"
#include "meteo_page_js.h"
#include "utils_js.h"
#include "line_chart_js.h"
#include "bar_chart_js.h"
#include "global_css.h"
#include "index_css.h"
#include "logo_svg.h"

typedef struct {
  String path;
  const char* content;
  size_t size;
} FileData;

void setup() {
  Serial.begin(921600);
  FFat.begin(true);

  FFat.mkdir("/static");
  FFat.mkdir("/static/libs");
  FFat.mkdir("/static/js");
  FFat.mkdir("/static/css");

  const static char FILE_WRITE_TAG[11] = "FILE_WRITE";

  std::vector<FileData> files = {
    {"/index.html", index_html_str, (sizeof(index_html_str) - 1)},
    {"/chart_page.html", chart_page_html_str, (sizeof(chart_page_html_str) - 1)},
    {"/meteo_page.html", meteo_page_html_str, (sizeof(meteo_page_html_str) - 1)},
    {"/static/logo.svg", logo_svg_str, (sizeof(logo_svg_str) - 1)},
    {"/static/css/global.css", global_css_str, (sizeof(global_css_str) - 1)},
    {"/static/css/index.css", index_css_str, (sizeof(index_css_str) - 1)},
    {"/static/libs/chart.umd.min.js", chart_js_str, (sizeof(chart_js_str) - 1)},
    {"/static/libs/htmx.min.js", htmx_str, (sizeof(htmx_str) - 1)},
    {"/static/js/utils.js", utils_js_str, (sizeof(utils_js_str) - 1)},
    {"/static/js/index.js", index_js_str, (sizeof(index_js_str) - 1)},
    {"/static/js/chart_page.js", chart_page_js_str, (sizeof(chart_page_js_str) - 1)},
    {"/static/js/meteo_page.js", meteo_page_js_str, (sizeof(meteo_page_js_str) - 1)},
    {"/static/js/line_chart.js", line_chart_js_str, (sizeof(line_chart_js_str) - 1)},
    {"/static/js/bar_chart.js", bar_chart_js_str, (sizeof(bar_chart_js_str) - 1)},
  };

  for (FileData& file_data : files)
  {
    fs::File file = FFat.open(file_data.path, FILE_WRITE);
    if (file.print(file_data.content) == file_data.size)
    {
      ESP_LOGI(FILE_WRITE_TAG, "File written (%s)", file.path());
    }
    else
    {
      ESP_LOGE(FILE_WRITE_TAG, "Writting file failed (%s)", file.path());
    }
    file.close();
  }
}

void loop() {}