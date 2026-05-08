#ifndef UNIQUE_GLOBAL_CSS
#define UNIQUE_GLOBAL_CSS

const static char global_css_str[] = R"EOF(
@charset "utf-8";

:root {
    color-scheme: light dark;

    --primary-color: hsl(59, 50%, 39%);
    --secondary-color: hsl(57, 20%, 21%);
    --accent-color: hsl(59, 100%, 46%);

    --primary-color-20: hsl(59, 50%, 59%);
    --secondary-color-20: hsl(57, 20%, 41%);
    --accent-color-20: hsl(59, 100%, 66%);

    --background: light-dark(hsl(0 0 90), hsl(0 0 10));
    --background-10: light-dark(hsl(0 0 85), hsl(0 0 15));

    --text-color: light-dark(hsl(0 0 15), white);
    --light-text-color: hsl(0 0 15);
    --dark-text-color: white;

    --heading-size: 2.5rem;
    --text-size: 1rem;

    --text-spacing: 8px;
    --heading-spacing: 12px;

    --default-bradius: 13px;
}

* {
    padding: 0px;
    margin: 0px;

    box-sizing: border-box;
    vertical-align: middle;
}

:where(body) {
    font: var(--text-size) Arial, Helvetica, sans-serif;

    color: var(--text-color);
    background-color: var(--background);

    header {
        display: flex;
        flex-flow: row nowrap;
        justify-content: space-between;
        height: 8dvh;

        background-image: linear-gradient(
            180deg,
            var(--primary-color),
            var(--background)
        );

        span {
            & > * { display: inline; }

            svg#logo {
                height: 32px;
                width: 32px;
                fill: none;
                stroke: var(--text-color);
            }

            h1 { font-size: var(--heading-size); }
        }
    }

    hr { border-color: var(--text-color); }
    main { height: 92dvh; }

    .light-mode { color-scheme: only light; }
    .dark-mode { color-scheme: only dark; }

    .chart-container {
        padding: var(--text-spacing);

        border-radius: var(--default-bradius);
        background-color: var(--background-10);
    }

    .default-button {
        display: inline-block;
        padding: 8px;

        cursor: pointer;
        user-select: none;

        border: none;
        border-radius: var(--default-bradius);
        background-color: var(--primary-color);

        &, & > a {
            color: var(--dark-text-color);
            font-size: var(--text-size);
            font-weight: bold;
            text-decoration: none;
        }

        &:hover {
            background-color: var(--primary-color-20);
        }

        &:active {
            background-color: var(--secondary-color-20);
        }
    }
})EOF";

#endif