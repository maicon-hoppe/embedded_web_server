#ifndef UNIQUE_INDEX_JS
#define UNIQUE_INDEX_JS

const static char index_js_str[] = R"EOF(
const lineChartXLabels = [];
const lineChartCoordinates = [];

const DBOpenRequest = indexedDB.open("br_population", 1);

DBOpenRequest.onsuccess = async (event) =>
{
    const db = DBOpenRequest.result;

    const BRPopulation = await fetch("https://servicodados.ibge.gov.br/api/v1/paises/BR/indicadores/77849");
    const BRPopulationJson = await BRPopulation.json();

    const dbPopulationTransaction = db.transaction(["br_population"], "readwrite");
    const objectStoreCountRequest = dbPopulationTransaction.objectStore("br_population").count();
    objectStoreCountRequest.onsuccess = () =>
    {
        console.log(`CONTAGEM: ${objectStoreCountRequest.result}`);
        if (objectStoreCountRequest.result === 0)
        {
            const timeSeries = BRPopulationJson[0].series[0].serie;
            console.log("SERIES -> ");
            console.log(timeSeries);

            const populationObjectStore = dbPopulationTransaction.objectStore("br_population");

            for (const timeCoordinate of timeSeries)
            {
                const [year, quantity] = Object.entries(timeCoordinate)[0];

                if (quantity)
                {
                    console.log("Entrada ->");
                    console.table({ year: +year, quantity: +quantity });

                    const insertionRequest = populationObjectStore.add({ year: year, quantity: +quantity });
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

    const objectStore = db.transaction("br_population", "readonly").objectStore("br_population");
    objectStore.openCursor().onsuccess = (event) =>
    {
        const cursor = event.target.result;
        if (!cursor)
        {
            console.log("All data retrived");

            console.log(`Chart labels: ${lineChartXLabels}`);
            console.log("Coordenadas =>");
            console.table(lineChartCoordinates);

            const lineChart = document.createElement("line-chart");
            lineChart.dataset.title = "OLÁ MUNDO";
            lineChart.dataset.displayPoints = "false";
            lineChart.dataset.chartData = JSON.stringify({
                labels: lineChartXLabels,
                datasets: [
                    {
                        label: "População",
                        data: lineChartCoordinates,
                    },
                ],
            });
            document.body.appendChild(lineChart);

            return;
        }

        const { year, quantity } = cursor.value;
        lineChartXLabels.push(year);
        lineChartCoordinates.push({ x: cursor.key, y: quantity });

        cursor.continue();
    };
};

DBOpenRequest.onupgradeneeded = async (event) =>
{
    const db = event.target.result;

    db.onerror = () => { console.log("Cannot create database"); }

    const BRPopulationObjectStore = db.createObjectStore("br_population", { autoIncrement: true });
    BRPopulationObjectStore.createIndex("year", "year", { unique: true });
    BRPopulationObjectStore.createIndex("quantity", "quantity");
};
)EOF";

#endif