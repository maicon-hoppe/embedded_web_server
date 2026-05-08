#ifndef UNIQUE_INDEX_CSS
#define UNIQUE_INDEX_CSS

const static char index_css_str[] = R"EOF(
main {
    display: flex;
    flex-flow: row wrap;
    justify-content: space-evenly;
    gap: var(--text-spacing);
    height: calc(92dvh - (2 * var(--text-spacing)));
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
