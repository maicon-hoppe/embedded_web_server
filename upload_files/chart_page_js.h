#ifndef UNIQUE_CHART_PAGE_JS
#define UNIQUE_CHART_PAGE_JS

const static char chart_page_js_str[] = R"EOF(
const mainElement = document.querySelector("main");
const loadingIndicator = document.querySelector("#loading-indicator");
const ipeaComent = document.createElement("div");
ipeaComent.id = "comentario";

const ipeaRegion = location.pathname.split("/").at(-2).replace("_", " ");
const ipeaCode = location.pathname.split("/").at(-1);

const validRegions = ["Regiões", "Estados", "Área metropolitana"];
if (!validRegions.includes(ipeaRegion))
{ location.assign("/404"); }

fetch(`http://www.ipeadata.gov.br/api/odata4/Metadados('${ipeaCode}')`)
.then(response => response.json())
.then(async (ipeaData) =>
{
    if (ipeaData.value.length > 0)
    {
        const chartTitle = ipeaData.value[0].SERNOME.toUpperCase();
        const chartXTitle = ipeaRegion;
        const chartYTitle = ipeaData.value[0].UNINOME;

        const territories = {};
        const ipeaSeriesValues = {};

        ipeaComent.innerHTML = ipeaData.value[0].SERCOMENTARIO;
        mainElement.appendChild(ipeaComent);

        loadingIndicator.style.opacity = 1;
        const responseSeries = await fetch(`http://www.ipeadata.gov.br/api/odata4/Metadados('${ipeaCode}')/Valores`);
        const ipeaSeries = await responseSeries.json();

        for (const dataEntry of ipeaSeries.value)
        {
            if (!territories[dataEntry.NIVNOME])
            {
                territories[dataEntry.NIVNOME] = [];
            }

            if (dataEntry.NIVNOME === chartXTitle)
            {
                ipeaSeriesValues[dataEntry.TERCODIGO] = dataEntry.VALVALOR;
                let ipeaPlace = territories[dataEntry.NIVNOME].find((TERCODIGO) => TERCODIGO[dataEntry.TERCODIGO]);
                if (!ipeaPlace)
                {
                    const placeResponse = await fetch(
                        `http://www.ipeadata.gov.br/api/odata4/Territorios(TERCODIGO='${dataEntry.TERCODIGO}',NIVNOME='${dataEntry.NIVNOME}')`
                    );
                    const placeData = await placeResponse.json();
                    territories[dataEntry.NIVNOME].push({ [dataEntry.TERCODIGO]: placeData.value[0].TERNOME });
                }
            }
        }

        const requestURL = "/bar_chart";
        const BarChartsearchParams = new URLSearchParams();
        // BarChartsearchParams.append("contained", "true");
        BarChartsearchParams.append("title", chartTitle);
        BarChartsearchParams.append("x_title", chartXTitle);
        BarChartsearchParams.append("y_title", chartYTitle);
        BarChartsearchParams.append("labels",
            JSON.stringify(territories[chartXTitle].map(data => Object.values(data)[0]))
        );
        BarChartsearchParams.append("coordinate_label", chartYTitle);
        BarChartsearchParams.append("coordinates", 
            JSON.stringify(
                Object.entries(ipeaSeriesValues)
                    .map(([key, value]) => ({ x: +key, y: +value }))
            )
        );

        const requestShape = requestURL + "?" + BarChartsearchParams.toString();
        htmx.ajax('GET', requestShape, {
            target: "main",
            swap: "beforeend"
        })
        .then(() => { loadingIndicator.style.opacity = 0 });
    }
    else
    {
        ipeaComent.innerText = "Nenhum dado encontrado.";
        mainElement.appendChild(ipeaComent);
    }
});
)EOF";

#endif