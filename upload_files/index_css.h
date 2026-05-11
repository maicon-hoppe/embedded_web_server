#ifndef UNIQUE_INDEX_CSS
#define UNIQUE_INDEX_CSS

const static char index_css_str[] = R"EOF(
header span {
    display: flex;
    flex-flow: row nowrap;
    align-items: center;
    gap: 5px;
}

main {
    display: flex;
    flex-flow: row wrap;
    justify-content: space-evenly;
    gap: var(--text-spacing);
    height: calc(92dvh - (3 * var(--text-spacing)) - 2px);
    margin: var(--text-spacing);

    .chart-container {
        height: 50%;
        /* width: 50%; */
        flex: 0.43;

        line-chart { height: 100%; }
    }
}
)EOF";

#endif
