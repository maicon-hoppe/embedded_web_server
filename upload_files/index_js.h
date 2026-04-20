#ifndef UNIQUE_INDEX_JS
#define UNIQUE_INDEX_JS

const static char index_js_str[] = R"EOF(
  // DBOpenRequest = indexedDB.open("br_population", 1);

  let timeSeries;
  window.onload = async () =>
  {
      const BRPopulation = await fetch("https://servicodados.ibge.gov.br/api/v1/paises/BR/indicadores/77849");
      const BRPopulationJson = await BRPopulation.json();
      timeSeries = BRPopulationJson[0].series[0].serie;

      const lineChartXLabels = [];
      const lineChartCoordinates = [];
      const barChartXLabels = [];
      const barChartCoordinates = [];
      for (const timeData of timeSeries)
      {
          const timeDataEntry = Object.entries(timeData)[0];
          if (timeDataEntry[1])
          {
              lineChartXLabels.push(timeDataEntry[0]);
              lineChartCoordinates.push({ x: timeSeries.indexOf(timeData)+1, y: +timeDataEntry[1] });
              // barChartXLabels.push(timeDataEntry[0]);
              // barChartCoordinates.push({ x: timeSeries.indexOf(timeData)+1, y: +timeDataEntry[1] });
          }
      }

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

    //   const barChart = document.createElement("bar-chart");
    //   barChart.dataset.title = "DADOS";
    //   barChart.dataset.xLabels = JSON.stringify(barChartXLabels);
    //   barChart.dataset.yLabel = "Crescimento Populacional";
    //   barChart.dataset.coordinates = JSON.stringify(barChartCoordinates);

      document.body.appendChild(lineChart);
    //   document.body.appendChild(barChart);
  }
)EOF";

#endif