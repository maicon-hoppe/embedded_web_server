#ifndef UNIQUE_INDEX_JS
#define UNIQUE_INDEX_JS

const static char index_js_str[] = R"EOF(
const lineChartXLabels = {};
const lineChartCoordinates = {};

const DBOpenRequest = indexedDB.open("ibge_data", 1);

DBOpenRequest.onsuccess = async (event) =>
{
    const db = DBOpenRequest.result;

    const BRStatistics = await fetch("https://servicodados.ibge.gov.br/api/v1/paises/BR/indicadores/77825|77826|77831|77849");
    const BRStatisticsJson = await BRStatistics.json();

    const dbWriteTransaction = db.transaction(["br_export", "br_import", "br_idh", "br_population"], "readwrite");
    const exportCountRequest = dbWriteTransaction.objectStore("br_export").count();
    exportCountRequest.onsuccess = () =>
    {
        console.log(`CONTAGEM EXPORTAÇÃO: ${exportCountRequest.result}`);
        if (exportCountRequest.result === 0)
        {
            const exportTimeSeries = BRStatisticsJson[0].series[0].serie;
            console.log("Export SERIES -> ");
            console.log(exportTimeSeries);

            const exportObjectStore = dbWriteTransaction.objectStore("br_export");

            for (const exportTimeCoordinate of exportTimeSeries)
            {
                const [year, quantity] = Object.entries(exportTimeCoordinate)[0];

                if (quantity)
                {
                    console.log("Entrada ->");
                    console.table({ year: +year, quantity: +quantity });

                    const insertionRequest = exportObjectStore.add({ year: +year, quantity: +quantity });
                    insertionRequest.onsuccess = (event) =>
                    {
                        console.log("Value inserted in the database");
                    };

                    insertionRequest.onerror = (event) =>
                    {
                        console.log("Unable to add value");
                        console.table({ year: +year, quantity: +quantity });
                    }
                }
            }
        }
    }

    const importCountRequest = dbWriteTransaction.objectStore("br_import").count();
    importCountRequest.onsuccess = () =>
    {
        console.log(`CONTAGEM IMPORTAÇÃO: ${importCountRequest.result}`);
        if (importCountRequest.result === 0)
        {
            const importTimeSeries = BRStatisticsJson[1].series[0].serie;
            console.log("Import SERIES -> ");
            console.log(importTimeSeries);

            const importObjectStore = dbWriteTransaction.objectStore("br_import");

            for (const importTimeCoordinate of importTimeSeries)
            {
                const [year, quantity] = Object.entries(importTimeCoordinate)[0];

                if (quantity)
                {
                    console.log("Entrada ->");
                    console.table({ year: +year, quantity: +quantity });

                    const insertionRequest = importObjectStore.add({ year: +year, quantity: +quantity });
                    insertionRequest.onsuccess = (event) =>
                    {
                        console.log("Value inserted in the database");
                    };

                    insertionRequest.onerror = (event) =>
                    {
                        console.log("Unable to add value");
                        console.table({ year: +year, quantity: +quantity });
                    }
                }
            }
        }
    }

    const idhCountRequest = dbWriteTransaction.objectStore("br_idh").count();
    idhCountRequest.onsuccess = () =>
    {
        console.log(`CONTAGEM POPULAÇÃO: ${idhCountRequest.result}`);
        if (idhCountRequest.result === 0)
        {
            const idhTimeSeries = BRStatisticsJson[2].series[0].serie;
            console.log("idh SERIES -> ");
            console.log(idhTimeSeries);

            const idhObjectStore = dbWriteTransaction.objectStore("br_idh");

            for (const idhTimeCoordinate of idhTimeSeries)
            {
                const [year, quantity] = Object.entries(idhTimeCoordinate)[0];

                if (quantity)
                {
                    console.log("Entrada ->");
                    console.table({ year: +year, quantity: +quantity });

                    const insertionRequest = idhObjectStore.add({ year: +year, quantity: +quantity });
                    insertionRequest.onsuccess = (event) =>
                    {
                        console.log("Value inserted in the database");
                    };

                    insertionRequest.onerror = (event) =>
                    {
                        console.log("Unable to add value");
                        console.table({ year: +year, quantity: +quantity });
                    }
                }
            }
        }
    }

    const populationCountRequest = dbWriteTransaction.objectStore("br_population").count();
    populationCountRequest.onsuccess = () =>
    {
        console.log(`CONTAGEM POPULAÇÃO: ${populationCountRequest.result}`);
        if (populationCountRequest.result === 0)
        {
            const populationTimeSeries = BRStatisticsJson[3].series[0].serie;
            console.log("Population SERIES -> ");
            console.log(populationTimeSeries);

            const populationObjectStore = dbWriteTransaction.objectStore("br_population");

            for (const populationTimeCoordinate of populationTimeSeries)
            {
                const [year, quantity] = Object.entries(populationTimeCoordinate)[0];

                if (quantity)
                {
                    console.log("Entrada ->");
                    console.table({ year: +year, quantity: +quantity });

                    const insertionRequest = populationObjectStore.add({ year: +year, quantity: +quantity });
                    insertionRequest.onsuccess = (event) =>
                    {
                        console.log("Value inserted in the database");
                    };

                    insertionRequest.onerror = (event) =>
                    {
                        console.log("Unable to add value");
                        console.table({ year: +year, quantity: +quantity });
                    }
                }
            }
        }
    }

    const dbReadTransaction = db.transaction(["br_export", "br_import", "br_idh", "br_population"], "readonly");
    lineChartXLabels.export = [];
    lineChartCoordinates.export = [];
    const exportObjectStore = dbReadTransaction.objectStore("br_export");
    exportObjectStore.openCursor().onsuccess = (event) =>
    {
        const cursor = event.target.result;

        if (!cursor)
        {
            console.log("All data retrived");

            console.log(`Chart labels (export): ${lineChartXLabels.export}`);
            console.log("Coordenadas de Exportação =>");
            console.table(lineChartCoordinates.export);

            const exportLineChart = document.createElement("line-chart");
            exportLineChart.dataset.title = "EXPORTAÇÂO";
            exportLineChart.dataset.displayPoints = "false";
            exportLineChart.dataset.chartData = JSON.stringify({
                labels: lineChartXLabels.export,
                datasets: [
                    {
                        label: "População",
                        data: lineChartCoordinates.export,
                    },
                ],
            });
            document.body.appendChild(exportLineChart);

            return;
        }

        const { year, quantity } = cursor.value;
        lineChartXLabels.export.push(year);
        lineChartCoordinates.export.push({ x: cursor.key, y: quantity });

        cursor.continue();
    };

    lineChartXLabels.import = [];
    lineChartCoordinates.import = [];
    const importObjectStore = dbReadTransaction.objectStore("br_import");
    importObjectStore.openCursor().onsuccess = (event) =>
    {
        const cursor = event.target.result;

        if (!cursor)
        {
            console.log("All data retrived");

            console.log(`Chart labels (import): ${lineChartXLabels.import}`);
            console.log("Coordenadas de Importação =>");
            console.table(lineChartCoordinates.import);

            const importLineChart = document.createElement("line-chart");
            importLineChart.dataset.title = "IMPORTAÇÂO";
            importLineChart.dataset.displayPoints = "false";
            importLineChart.dataset.chartData = JSON.stringify({
                labels: lineChartXLabels.import,
                datasets: [
                    {
                        label: "População",
                        data: lineChartCoordinates.import,
                    },
                ],
            });
            document.body.appendChild(importLineChart);

            return;
        }

        const { year, quantity } = cursor.value;
        lineChartXLabels.import.push(year);
        lineChartCoordinates.import.push({ x: cursor.key, y: quantity });

        cursor.continue();
    };

    lineChartXLabels.idh = [];
    lineChartCoordinates.idh = [];
    const idhObjectStore = dbReadTransaction.objectStore("br_idh");
    idhObjectStore.openCursor().onsuccess = (event) =>
    {
        const cursor = event.target.result;

        if (!cursor)
        {
            console.log("All data retrived");

            console.log(`Chart labels (idh): ${lineChartXLabels.idh}`);
            console.log("Coordenadas Populacionais =>");
            console.table(lineChartCoordinates.idh);

            const idhLineChart = document.createElement("line-chart");
            idhLineChart.dataset.title = "IDH";
            idhLineChart.dataset.displayPoints = "false";
            idhLineChart.dataset.chartData = JSON.stringify({
                labels: lineChartXLabels.idh,
                datasets: [
                    {
                        label: "Índice de Desenvolvimento Humano",
                        data: lineChartCoordinates.idh,
                    },
                ],
            });
            document.body.appendChild(idhLineChart);

            return;
        }

        const { year, quantity } = cursor.value;
        lineChartXLabels.idh.push(year);
        lineChartCoordinates.idh.push({ x: cursor.key, y: quantity });

        cursor.continue();
    };

    lineChartXLabels.population = [];
    lineChartCoordinates.population = [];
    const populationObjectStore = dbReadTransaction.objectStore("br_population");
    populationObjectStore.openCursor().onsuccess = (event) =>
    {
        const cursor = event.target.result;

        if (!cursor)
        {
            console.log("All data retrived");

            console.log(`Chart labels (population): ${lineChartXLabels.population}`);
            console.log("Coordenadas Populacionais =>");
            console.table(lineChartCoordinates.population);

            const populationLineChart = document.createElement("line-chart");
            populationLineChart.dataset.title = "POPULAÇÃO";
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

            return;
        }

        const { year, quantity } = cursor.value;
        lineChartXLabels.population.push(year);
        lineChartCoordinates.population.push({ x: cursor.key, y: quantity });

        cursor.continue();
    };
};

DBOpenRequest.onupgradeneeded = async (event) =>
{
    const db = event.target.result;

    db.onerror = () => { console.log("Cannot create database"); }

    const BRExportObjectStore = db.createObjectStore("br_export", { autoIncrement: true });
    BRExportObjectStore.createIndex("year", "year", { unique: true });

    const BRImportObjectStore = db.createObjectStore("br_import", { autoIncrement: true });
    BRImportObjectStore.createIndex("year", "year", { unique: true });

    const BRIDHObjectStore = db.createObjectStore("br_idh", { autoIncrement: true });
    BRIDHObjectStore.createIndex("year", "year", { unique: true });

    const BRPopulationObjectStore = db.createObjectStore("br_population", { autoIncrement: true });
    BRPopulationObjectStore.createIndex("year", "year", { unique: true });
};
)EOF";

#endif