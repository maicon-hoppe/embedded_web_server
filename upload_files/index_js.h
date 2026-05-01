#ifndef UNIQUE_INDEX_JS
#define UNIQUE_INDEX_JS

const static char index_js_str[] = R"EOF(
"use strict";

import { BasicIndexedDB } from "./utils.js";

const lineChartXLabels = {};
const lineChartCoordinates = {};

const objectStoreIndicators = {
    "br_export": 77825,
    "br_import": 77826,
    "br_idh": 77831,
    "br_population": 77849
};

let dataURL = "https://servicodados.ibge.gov.br/api/v1/paises/BR/indicadores/";
for (const indicator of Object.values(objectStoreIndicators)) { dataURL += `${indicator}|`; }

let IBGEDB = new BasicIndexedDB("ibge_data");
if (IBGEDB.isEmpty())
{
    fetch(dataURL.slice(0, -1))
    .then(BRStatistics => BRStatistics.json())
    .then(BRStatisticsJson =>
    {
        const objectNames = Object.keys(objectStoreIndicators);
        const timeSeries = BRStatisticsJson.map(indicator => indicator.series[0].serie);

        IBGEDB = new BasicIndexedDB("ibge_data", objectNames, timeSeries);
        IBGEDB.useData((everything) =>
        {
            lineChartXLabels.population = everything.br_population.map(({ year, quantity }) => year);
            lineChartCoordinates.population = everything.br_population.map(
                ({ year, quantity }) => ({x: everything.br_population.indexOf(year), y: quantity })
            );

            const populationLineChart = document.createElement("line-chart");
            populationLineChart.dataset.title = "POPULAÇÃO";
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
            document.body.appendChild(populationLineChart);

            lineChartXLabels.export = everything.br_export.map(({ year, quantity }) => year);
            lineChartCoordinates.export = everything.br_export.map(
                ({ year, quantity }) => ({x: everything.br_export.indexOf(year), y: quantity })
            );

            const exportLineChart = document.createElement("line-chart");
            exportLineChart.dataset.title = "EXPORTAÇÃO";
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
            document.body.appendChild(exportLineChart);

            lineChartXLabels.import = everything.br_import.map(({ year, quantity }) => year);
            lineChartCoordinates.import = everything.br_import.map(
                ({ year, quantity }) => ({x: everything.br_import.indexOf(year), y: quantity })
            );

            const importLineChart = document.createElement("line-chart");
            importLineChart.dataset.title = "IMPORTAÇÃO";
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
            document.body.appendChild(importLineChart);

            lineChartXLabels.idh = everything.br_idh.map(({ year, quantity }) => year);
            lineChartCoordinates.idh = everything.br_idh.map(
                ({ year, quantity }) => ({x: everything.br_idh.indexOf(year), y: quantity })
            );

            const idhLineChart = document.createElement("line-chart");
            idhLineChart.dataset.title = "ÍNDICE DE DESENVOLVIMENTO HUMANO";
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
            document.body.appendChild(idhLineChart);
        });
    });
}
)EOF";

#endif