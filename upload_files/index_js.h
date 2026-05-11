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
        exportLineChart.dataset.xTitle = "Ano";
        exportLineChart.dataset.yTitle = "Valor";
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

const ipeaDataRegion = document.querySelector("#ipea_data_region");
const ipeaDataText = document.querySelector("#ipea_data_text");
const ipeaDataButton = document.querySelector("#ipea_data_button");
ipeaDataText.addEventListener("keydown", (event) =>
{
    if (event.key === "Enter" && ipeaDataText.value)
    {
        location.assign(`/data/${ipeaDataRegion.selectedOptions[0].value}/${ipeaDataText.value}`);
    }
});
ipeaDataButton.addEventListener("click", (event) =>
{
    if (ipeaDataText.value)
    {
        location.assign(`/data/${ipeaDataRegion.selectedOptions[0].value}/${ipeaDataText.value}`);
    }
});
)EOF";

#endif