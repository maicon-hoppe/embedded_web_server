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
    --background-10: light-dark(hsl(0 0 75), hsl(0 0 25));

    --text-color: light-dark(hsl(0 0 15), white);
    --light-text-color: hsl(0 0 15);
    --dark-text-color: white;

    --heading-size: 2.5rem;
    --text-size: 1rem;

    --default-bradius: 13px;
}

* {
    padding: 0px;
    margin: 0px;

    box-sizing: border-box;
    vertical-align: middle;
}

body {
    font: var(--text-size) Arial, Helvetica, sans-serif;

    color: var(--text-color);
    background-color: var(--background);

    h1 { font-size: var(--heading-size); }
    hr { border-color: var(--text-color); }

    .light-mode { color-scheme: only light; }
    .dark-mode { color-scheme: only dark; }

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