#ifndef UNIQUE_CHART_PAGE_HTML
#define UNIQUE_CHART_PAGE_HTML

const static char chart_page_html_str[] = R"EOF(
<!DOCTYPE html>
<html lang="pt-br">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="stylesheet" href="/static/css/global.css">
    <script src="/static/libs/chart.umd.min.js"></script>
    <script src="/static/libs/htmx.min.js"></script>
    <script defer type="module" src="/static/js/line_chart.js"></script>
    <script defer type="module" src="/static/js/bar_chart.js"></script>
    <script defer type="module" src="/static/js/chart_page.js"></script>
    <style>
        @keyframes spin {
            from {
                transform: rotate(0deg);
            }
            to {
                transform: rotate(360deg);
            }
        }

        main {
            height: calc(92dvh - 2px);

            #comentario {
                width: 70%;
                padding: var(--text-spacing);
                margin: auto;
            }

            bar-chart {
                height: 70%;
                width: 66%;
                margin: auto;
            }

            #loading-indicator {
                position: absolute;
                top: calc(50% - 16px);
                left: calc(50% - 16px);

                opacity: 0;
                animation: spin 1s ease infinite;
            }
        }
    </style>
    <link rel="shortcut icon" href="/static/logo.svg" type="image/svg+xml">
    <title>Estatísticas</title>
</head>
<body>
    <template id="chart">
        <div id="chart-box">
            <canvas id="graph"></canvas>
        </div>
        <style>
            :host { display: block; }
            #chart-box { height: 100%; }
        </style>
    </template>
    <header>
        <a href="/" title="Home">
            <svg id="logo"
                xmlns="http://www.w3.org/2000/svg"
                viewBox="0 0 24 24"
                stroke-width="2"
                stroke-linecap="round"
                stroke-linejoin="round"
            >
                <path d="M3 3v16a2 2 0 0 0 2 2h16"/>
                <path d="m19 9-5 5-4-4-3 3"/>
            </svg>
        </a>
    </header>
    <hr>
    <main>
        <svg id="loading-indicator"
            xmlns="http://www.w3.org/2000/svg"
            viewBox="0 0 24 24"
            stroke-width="2"
            stroke-linecap="round"
            stroke-linejoin="round"
        >
            <path d="M21 12a9 9 0 1 1-6.219-8.56"/>
        </svg>
    </main>
</body>
</html>)EOF";

#endif