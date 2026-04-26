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
    <script defer type="module" src="line_chart.js"></script>
    <script defer type="module" src="bar_chart.js"></script>
    <script defer type="module" src="static/index.js"></script>
    <title>Pagina</title>
</head>
<body>
    <template id="chart">
        <div id="chart-box">
            <canvas id="graph"></canvas>
        </div>
    </template>
    <h1>Página HTML</h1>
    <hr>
    <p>Lorem ipsum dolor sit amet consectetur adipisicing elit. Modi velit expedita exercitationem 
        inima obcaecati commodi soluta. Inventore quae voluptatum saepe totam libero, provident
        autem, est incidunt qui vitae dolor culpa?
    </p>
</body>
</html>)EOF";

#endif