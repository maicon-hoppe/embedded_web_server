#ifndef UNIQUE_UTILS_JS
#define UNIQUE_UTILS_JS

const static char utils_js_str[] = R"EOF(
export const BRLCurrencyFormatter = new Intl.NumberFormat(
    "pt-BR",
    {
        style: "currency",
        currency: "BRL",
    }
);

export const BRLDateFormatter = new Intl.DateTimeFormat(
    "pt-BR",
    { dateStyle: 'long' }
);

export const systemColors = {
    secondaryColor20: "hsl(332, 38%, 65%)",
    text: false ? "hsl(0 0 90)" : "hsl(0 0 10)",
    accent: false
        ? "hsl(37, 100%, 64%)"
        : "hsl(37, 100%, 34%)",
    accentTransparent: false
        ? "hsla(37 100 64 / 0.5)"
        : "hsla(37 100 34 / 0.5)",
};
)EOF";

#endif