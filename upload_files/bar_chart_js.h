#ifndef UNIQUE_BAR_CHART_JS
#define UNIQUE_BAR_CHART_JS

const static char bar_chart_js_str[] = R"EOF(
import { systemColors } from "./static/utils.js";

class BarChart extends HTMLElement {
    constructor() {
        super();

        const chartTemplate = document.querySelector("template#chart");

        const shadowRoot = this.attachShadow({ mode: "open" });
        shadowRoot.appendChild(chartTemplate.content.cloneNode(true));
    }

    connectedCallback()
    {
        const chartCanvas = this.shadowRoot.querySelector("canvas");
        const chartData = {
            labels: JSON.parse(this.dataset.xLabels),
            datasets: [
                {
                    label: this.dataset.yLabel,
                    data: JSON.parse(this.dataset.coordinates),
                },
            ],
        };

        const chart = new Chart(chartCanvas, {
            type: 'bar',
            data: chartData,
            options: {
                maintainAspectRatio: true,
                elements: {
                    bar: {
                        backgroundColor: systemColors.primaryColor,
                    },
                },
                scales: {
                    x: {
                        title: {
                            display: true,
                            text: "Valor dos contratos",
                            color: systemColors.text,
                        },
                        ticks: {
                            color: systemColors.text,
                        },
                        grid: { display: false },
                    },
                    y: {
                        title: {
                            display: true,
                            text: "Quantidade de contratos",
                            color: systemColors.text,
                        },
                        ticks: {
                            stepSize: 100,
                            color: systemColors.text,
                        },
                        grid: {
                            color: systemColors.text,
                        },
                    },
                },
                interaction: {
                    mode: "x",
                    intersect: true,
                },
                onClick: (e) => {
                    const canvasPosition = Chart.helpers.getRelativePosition(e, chart);
                    const labelValues =
                        labels[
                            chart.scales.x.getValueForPixel(canvasPosition.x)
                        ];

                    const selectedExpensesRange = labelValues
                        .split(" à ")
                        .map((value) => parseInt(value));

                    console.log(selectedExpensesRange);
                },
                plugins: {
                    title: {
                        display: true,
                        color: systemColors.text,
                        text: this.dataset.title,
                    },
                    legend: { display: false },
                },
            }
        });
    }
}

customElements.define("bar-chart", BarChart);
)EOF";

#endif