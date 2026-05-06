#ifndef UNIQUE_BAR_CHART_JS
#define UNIQUE_BAR_CHART_JS

const static char bar_chart_js_str[] = R"EOF(
"use strict";

import { systemColors } from "./utils.js";

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
        const chartData = JSON.parse(this.dataset.chartData);

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
                        beginAtZero: true,
                        title: {
                            display: true,
                            text: "Quantidade de contratos",
                            color: systemColors.text,
                        },
                        ticks: {
                            stepSize: this.dataset.stepSize ? JSON.parse(this.dataset.stepSize) : undefined,
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
                    const selectedDate = chart.scales.x
                        .getLabelForValue(
                            chart.scales.x.getValueForPixel(canvasPosition.x),
                        )

                    console.log(selectedDate);
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