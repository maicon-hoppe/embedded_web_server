#ifndef UNIQUE_LINE_CHART_JS
#define UNIQUE_LINE_CHART_JS

const static char line_chart_js_str[] = R"EOF(
import { BRLCurrencyFormatter, BRLDateFormatter, systemColors } from "./static/utils.js";

const thisMonth = new Date().toLocaleDateString("pt-BR", {
    month: "long",
});

class LineChart extends HTMLElement {
    constructor()
    {
        super();

        const chartTemplate = document.querySelector("template#chart");

        const shadowRoot = this.attachShadow({"mode": "open"});
        shadowRoot.appendChild(chartTemplate.content.cloneNode(true));
    }

    connectedCallback()
    {
        const chartCanvas = this.shadowRoot.querySelector("canvas");

        console.log(this.dataset);
        console.log(`chartData -> ${this.dataset.chartData}`);

        const chart = new Chart(chartCanvas, {
            type: "line",
            data: JSON.parse(this.dataset.chartData),
            options: {
                maintainAspectRatio: true,
                elements: {
                    line: {
                        borderColor: systemColors.accent,
                    },
                    point: {
                        pointStyle: JSON.parse(this.dataset.displayPoints) ? 'circle' : false,
                        backgroundColor: systemColors.accent,
                    },
                },
                scales: {
                    x: {
                        title: {
                            display: true,
                            text: "Dia",
                            color: systemColors.text,
                        },
                        ticks: {
                            color: systemColors.text,
                            maxTicksLimit: 8,
                        },
                        grid: { display: false },
                    },
                    y: {
                        title: {
                            display: true,
                            text: "Despesas",
                            color: systemColors.text,
                        },
                        ticks: {
                            color: systemColors.text,
                            stepSize: 50_000_000,
                            callback: function (value) {
                                if (typeof value === "number") {
                                    const formatted =
                                        BRLCurrencyFormatter.formatToParts(
                                            value,
                                        )
                                            .slice(1, 3)
                                            .map(({ type, value }) => value)
                                            .join("");

                                    return value === 0
                                        ? formatted
                                        : formatted + " M";
                                }

                                return value;
                            },
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
                    const canvasPosition = getRelativePosition(e, chart);
                    const selectedDate = chart.scales.x
                        .getLabelForValue(
                            chart.scales.x.getValueForPixel(canvasPosition.x),
                        )
                        .split("/")
                        .toReversed()
                        .join("-");

                    console.log(selectedDate);
                },
                plugins: {
                    title: {
                        display: true,
                        text: this.dataset.title,
                        color: systemColors.text,
                    },
                    legend: { display: false },
                    tooltip: { display: true },
                },
            },
        });
    }
}

customElements.define("line-chart", LineChart);
)EOF";

#endif