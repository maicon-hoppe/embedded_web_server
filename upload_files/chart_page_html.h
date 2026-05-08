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
    <link rel="shortcut icon" href="/static/logo.svg" type="image/svg+xml">
    <title>Estatísticas</title>
</head>
<body>
    <template id="chart">
        <div id="chart-box">
            <canvas id="graph"></canvas>
        </div>
    </template>
    <header>
        <span>
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
            <h1>Estatísticas</h1>
        </span>
    </header>
    <hr>
    <main></main>
</body>
</html>)EOF";

#endif