#ifndef UNIQUE_CHART_PAGE_JS
#define UNIQUE_CHART_PAGE_JS

const static char chart_page_js_str[] = R"EOF(
const mainElement = document.querySelector("main");
mainElement.innerText = location.pathname.split("/").at(-1);
)EOF";

#endif