#ifndef UNIQUE_INDEX_JS
#define UNIQUE_INDEX_JS

const static char index_js_str[] = R"EOF(
"use strict";

import { BasicIndexedDB } from "./utils.js";

const mainElement = document.querySelector("main");

const objectStoreIndicators = {
    "br_export": 77825,
    "br_import": 77826,
    "br_idh": 77831,
    "br_population": 77849
};

let dataURL = "https://servicodados.ibge.gov.br/api/v1/paises/BR/indicadores/";
for (const indicator of Object.values(objectStoreIndicators)) { dataURL += `${indicator}|`; }

let IBGEDB = new BasicIndexedDB("ibge_data");
IBGEDB.promiseIsEmpty().then((dbIsEmpty) =>
{
    const lineChartXLabels = {};
    const lineChartCoordinates = {};

    const hydrateMain = function(everything)
    {
        lineChartXLabels.population = everything.br_population.map(({ year, quantity }) => year);
        lineChartCoordinates.population = everything.br_population.map(
            ({ year, quantity }) => ({x: everything.br_population.indexOf(year), y: quantity })
        );

        const chartContainer = document.createElement("div");
        chartContainer.classList.add("chart-container");

        const populationLineChart = document.createElement("line-chart");
        populationLineChart.dataset.title = "POPULAÇÃO";
        populationLineChart.dataset.xTitle = "Ano";
        populationLineChart.dataset.yTitle = "População";
        populationLineChart.dataset.beginAtZero = "true";
        populationLineChart.dataset.displayPoints = "false";
        populationLineChart.dataset.chartData = JSON.stringify({
            labels: lineChartXLabels.population,
            datasets: [
                {
                    label: "População",
                    data: lineChartCoordinates.population,
                },
            ],
        });
        mainElement.appendChild(
            chartContainer
                .cloneNode()
                .appendChild(populationLineChart)
                .parentElement
        );

        lineChartXLabels.export = everything.br_export.map(({ year, quantity }) => year);
        lineChartCoordinates.export = everything.br_export.map(
            ({ year, quantity }) => ({x: everything.br_export.indexOf(year), y: quantity })
        );

        const exportLineChart = document.createElement("line-chart");
        exportLineChart.dataset.title = "EXPORTAÇÃO (US$)";
        exportLineChart.dataset.xTitle = "Valor";
        exportLineChart.dataset.yTitle = "Ano";
        exportLineChart.dataset.beginAtZero = "true";
        exportLineChart.dataset.displayPoints = "false";
        exportLineChart.dataset.chartData = JSON.stringify({
            labels: lineChartXLabels.export,
            datasets: [
                {
                    label: "Exportação",
                    data: lineChartCoordinates.export,
                },
            ],
        });
        mainElement.appendChild(
            chartContainer
                .cloneNode()
                .appendChild(exportLineChart)
                .parentElement
        );

        lineChartXLabels.import = everything.br_import.map(({ year, quantity }) => year);
        lineChartCoordinates.import = everything.br_import.map(
            ({ year, quantity }) => ({x: everything.br_import.indexOf(year), y: quantity })
        );

        const importLineChart = document.createElement("line-chart");
        importLineChart.dataset.title = "IMPORTAÇÃO (US$)";
        importLineChart.dataset.xTitle = "Ano";
        importLineChart.dataset.yTitle = "Valor";
        importLineChart.dataset.beginAtZero = "true";
        importLineChart.dataset.displayPoints = "false";
        importLineChart.dataset.chartData = JSON.stringify({
            labels: lineChartXLabels.import,
            datasets: [
                {
                    label: "Importação",
                    data: lineChartCoordinates.import,
                },
            ],
        });
        mainElement.appendChild(
            chartContainer
                .cloneNode()
                .appendChild(importLineChart)
                .parentElement
        );

        lineChartXLabels.idh = everything.br_idh.map(({ year, quantity }) => year);
        lineChartCoordinates.idh = everything.br_idh.map(
            ({ year, quantity }) => ({x: everything.br_idh.indexOf(year), y: quantity })
        );

        const idhLineChart = document.createElement("line-chart");
        idhLineChart.dataset.title = "ÍNDICE DE DESENVOLVIMENTO HUMANO";
        idhLineChart.dataset.xTitle = "Ano";
        idhLineChart.dataset.yTitle = "IDH";
        idhLineChart.dataset.beginAtZero = "false";
        idhLineChart.dataset.displayPoints = "false";
        idhLineChart.dataset.chartData = JSON.stringify({
            labels: lineChartXLabels.idh,
            datasets: [
                {
                    label: "IDH",
                    data: lineChartCoordinates.idh,
                },
            ],
        });
        mainElement.appendChild(
            chartContainer
                .cloneNode()
                .appendChild(idhLineChart)
                .parentElement
        );
    };

    if (dbIsEmpty)
    {
        fetch(dataURL.slice(0, -1))
        .then(BRStatistics => BRStatistics.json())
        .then(BRStatisticsJson =>
        {
            const objectNames = Object.keys(objectStoreIndicators);
            const timeSeries = BRStatisticsJson.map(indicator => indicator.series[0].serie);
            IBGEDB = new BasicIndexedDB("ibge_data", objectNames, timeSeries);

            return IBGEDB.promiseAllData();
        })
        .then(hydrateMain);
    }
    else
    {
        IBGEDB.promiseAllData().then(hydrateMain);
    }
});

const ipeaDataText = document.querySelector("#ipea_data_text");
const ipeaDataButton = document.querySelector("#ipea_data_button");
const ipeaComent = document.createElement("p");
ipeaComent.id = "comentario";

const territories = {};
const ipeaSeriesValues = {};
let chartTitle;
let chartXTitle;
let chartYTitle;

const getData = async function(event)
{
    location.assign(`/data/${ipeaDataText.value}`);

    /* const response = await fetch(`http://www.ipeadata.gov.br/api/odata4/Metadados('${ipeaDataText.value}')`);
    const ipeaData = await response.json();

    chartTitle = ipeaData.value[0].SERNOME.toUpperCase();
    chartYTitle = ipeaData.value[0].UNINOME;

    if (ipeaData.value.length > 0)
    {
        ipeaComent.innerHTML = ipeaData.value[0].SERCOMENTARIO;
        mainElement.replaceChildren(ipeaComent);

        const responseSeries = await fetch(`http://www.ipeadata.gov.br/api/odata4/Metadados('${ipeaDataText.value}')/Valores`);
        const ipeaSeries = await responseSeries.json();

        for (const dataEntry of ipeaSeries.value)
        {
            if (!territories[dataEntry.NIVNOME])
            {
                territories[dataEntry.NIVNOME] = [];
            }

            chartXTitle = "Estados";
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
        BarChartsearchParams.append("contained", "true");
        BarChartsearchParams.append("title", chartTitle);
        BarChartsearchParams.append("x_title", chartXTitle);
        BarChartsearchParams.append("y_title", chartYTitle);
        BarChartsearchParams.append("labels",
            JSON.stringify(territories[chartXTitle].map(data => Object.values(data)[0]))
        );
        BarChartsearchParams.append("coordinate_label", "Porcentagem");
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
        });
    }
    else
    {
        ipeaComent.innerText = "Nenhum dado encontrado.";
        mainElement.replaceChildren(ipeaComent);
    } */
}

ipeaDataText.addEventListener("keydown", (event) => { if (event.key === "Enter") { getData() } });
ipeaDataButton.addEventListener("click", getData);
)EOF";

#endif