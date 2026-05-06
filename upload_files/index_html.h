#ifndef UNIQUE_INDEX_HTML
#define UNIQUE_INDEX_HTML

const static char index_html_str[] = R"EOF(
<!DOCTYPE html>
<html lang="pt-br">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <script src="static/chart.umd.min.js"></script>
    <script src="static/htmx.min.js"></script>
    <script defer type="module" src="static/utils.js"></script>
    <script defer type="module" src="static/line_chart.js"></script>
    <script defer type="module" src="static/bar_chart.js"></script>
    <script defer type="module" src="static/index.js"></script>
    <link rel="stylesheet" href="static/global.css">
    <title>Pagina</title>
</head>
<body>
    <template id="chart">
        <div id="chart-box">
            <canvas id="graph"></canvas>
        </div>
    </template>
    <h1>Estatísticas</h1>
    <hr>

    <input id="ipea_data_text" type="text">
    <input id="ipea_data_button" class="default-button" type="button" value="Enviar">

    <p id="comentario"></p>
    <button class="default-button"
        hx-get="/line_chart?step_size=1"
        hx-trigger="click once"
        hx-target="this"
        hx-swap="outerHTML"
    >
        FOO BAR
    </button>
    <button class="default-button"
        hx-get="/bar_chart?step_size=1"
        hx-trigger="click once"
        hx-target="this"
        hx-swap="outerHTML"
    >
        COISA BARRA
    </button>
</body>
</html>)EOF";

#endif