#ifndef UNIQUE_METEO_PAGE_JS
#define UNIQUE_METEO_PAGE_JS

const static char meteo_page_js_str[] = R"EOF(
fetch(
    "https://panorama.sipam.gov.br/api/meteorologia/v1/previsao/tempo/municipio/1503606?token=...&data=10%2F05%2F2026"
)
.then(response => response.json())
.then(responseJson =>
{
    // if (responseJson === "Token Inválido")

    const mainElement = document.createElement("main");
    mainElement.append(responseJson);
    document.body.appendChild(mainElement);
});
)EOF";

#endif