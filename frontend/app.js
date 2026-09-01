const stocks = [
    "PETR4",
    "VALE3",
    "ITUB4",
    "BBDC4"
];

let updating = false;
let stockData = {};

let priceChart = null;
let distributionChart = null;


async function loadStock(ticker) {

    try {

        const response =
            await fetch(`/api/quote/${ticker}`);

        if (!response.ok) {
            throw new Error(`HTTP ${response.status}`);
        }

        return await response.json();

    }
    catch (error) {

        console.error(
            `Erro ao carregar ${ticker}:`,
            error
        );

        return null;
    }
}


async function updateStocks() {

    if (updating) {
        return;
    }

    updating = true;

    try {

        for (const ticker of stocks) {

            const data =
                await loadStock(ticker);

            if (!data) {
                continue;
            }

            stockData[ticker] = data;

            updateStockCard(
                ticker,
                data
            );
        }

        updateMarketStatus();

        updatePriceChart();

        updateDistributionChart();

    }
    finally {

        updating = false;

    }
}


function formatMoney(value) {

    if (
        typeof value !== "number" ||
        !Number.isFinite(value)
    ) {
        return "--";
    }

    return value.toLocaleString(
        "pt-BR",
        {
            style: "currency",
            currency: "BRL"
        }
    );
}


function formatNumber(value) {

    if (
        typeof value !== "number" ||
        !Number.isFinite(value)
    ) {
        return "--";
    }

    return value.toLocaleString(
        "pt-BR"
    );
}


function formatPercent(value) {

    if (
        typeof value !== "number" ||
        !Number.isFinite(value)
    ) {
        return "0,00%";
    }

    return `${value
        .toFixed(2)
        .replace(".", ",")}%`;
}


function updateStockCard(
    ticker,
    data
) {

    const card =
        document.querySelector(
            `.stock[data-ticker="${ticker}"]`
        );

    if (!card) {
        return;
    }


    const name =
        card.querySelector(
            ".stock-name"
        );

    const price =
        card.querySelector(
            ".stock-price"
        );

    const change =
        card.querySelector(
            ".stock-change"
        );

    const open =
        card.querySelector(
            ".stock-open"
        );

    const high =
        card.querySelector(
            ".stock-high"
        );

    const low =
        card.querySelector(
            ".stock-low"
        );

    const volume =
        card.querySelector(
            ".stock-volume"
        );


    if (name) {

        name.textContent =
            data.name || ticker;

    }


    if (price) {

        price.textContent =
            formatMoney(
                data.price
            );

    }


    if (change) {

        const variation =
            typeof data.change === "number"
                ? data.change
                : 0;

        const percentage =
            typeof data.changePercent === "number"
                ? data.changePercent
                : 0;

        const signal =
            variation >= 0
                ? "+"
                : "";

        change.textContent =
            `${signal}${formatMoney(variation)} (${formatPercent(percentage)})`;

    }


    if (open) {

        open.textContent =
            `Abertura: ${formatMoney(data.open)}`;

    }


    if (high) {

        high.textContent =
            `Máxima: ${formatMoney(data.dayHigh)}`;

    }


    if (low) {

        low.textContent =
            `Mínima: ${formatMoney(data.dayLow)}`;

    }


    if (volume) {

        volume.textContent =
            `Volume: ${formatNumber(data.volume)}`;

    }

}


function showStockDetails(ticker) {

    const data =
        stockData[ticker];

    if (!data) {
        return;
    }


    const container =
        document.getElementById(
            "stock-details-content"
        );

    if (!container) {
        return;
    }


    const variation =
        typeof data.change === "number"
            ? data.change
            : 0;

    const percentage =
        typeof data.changePercent === "number"
            ? data.changePercent
            : 0;

    const signal =
        variation >= 0
            ? "+"
            : "";


    container.innerHTML = `

        <div class="details-header">

            <div>

                <span class="details-ticker">
                    ${data.ticker || ticker}
                </span>

                <h3>
                    ${data.name || ticker}
                </h3>

            </div>


            <div class="details-price">

                <strong>
                    ${formatMoney(data.price)}
                </strong>

                <span>
                    ${signal}${formatMoney(variation)}
                    (${formatPercent(percentage)})
                </span>

            </div>

        </div>


        <div class="details-grid">

            <div class="detail-item">

                <span>
                    Abertura
                </span>

                <strong>
                    ${formatMoney(data.open)}
                </strong>

            </div>


            <div class="detail-item">

                <span>
                    Máxima
                </span>

                <strong>
                    ${formatMoney(data.dayHigh)}
                </strong>

            </div>


            <div class="detail-item">

                <span>
                    Mínima
                </span>

                <strong>
                    ${formatMoney(data.dayLow)}
                </strong>

            </div>


            <div class="detail-item">

                <span>
                    Fechamento anterior
                </span>

                <strong>
                    ${formatMoney(data.previousClose)}
                </strong>

            </div>


            <div class="detail-item">

                <span>
                    Volume
                </span>

                <strong>
                    ${formatNumber(data.volume)}
                </strong>

            </div>


            <div class="detail-item">

                <span>
                    Moeda
                </span>

                <strong>
                    ${data.currency || "BRL"}
                </strong>

            </div>


            <div class="detail-item">

                <span>
                    Máxima 52 semanas
                </span>

                <strong>
                    ${formatMoney(data.fiftyTwoWeekHigh)}
                </strong>

            </div>


            <div class="detail-item">

                <span>
                    Mínima 52 semanas
                </span>

                <strong>
                    ${formatMoney(data.fiftyTwoWeekLow)}
                </strong>

            </div>

        </div>
    `;
}


function setupStockClicks() {

    const cards =
        document.querySelectorAll(
            ".stock"
        );


    cards.forEach(card => {

        card.addEventListener(
            "click",
            () => {

                const ticker =
                    card.dataset.ticker;

                if (!ticker) {
                    return;
                }


                cards.forEach(item => {

                    item.classList.remove(
                        "selected"
                    );

                });


                card.classList.add(
                    "selected"
                );


                showStockDetails(
                    ticker
                );


                document
                    .getElementById(
                        "stock-details"
                    )
                    ?.scrollIntoView({
                        behavior: "smooth"
                    });

            }
        );

    });

}


function setupNavigation() {

    const navItems =
        document.querySelectorAll(
            ".nav-item[data-section]"
        );


    navItems.forEach(item => {

        item.addEventListener(
            "click",
            event => {

                event.preventDefault();


                const targetId =
                    item.getAttribute(
                        "href"
                    );


                const target =
                    document.querySelector(
                        targetId
                    );


                if (!target) {
                    return;
                }


                navItems.forEach(nav => {

                    nav.classList.remove(
                        "active"
                    );

                });


                item.classList.add(
                    "active"
                );


                target.scrollIntoView({
                    behavior: "smooth",
                    block: "start"
                });

            }
        );

    });

}


function updatePriceChart() {

    const canvas =
        document.getElementById(
            "price-chart"
        );

    if (!canvas) {
        return;
    }


    const labels = [];

    const prices = [];


    stocks.forEach(ticker => {

        const data =
            stockData[ticker];

        if (!data) {
            return;
        }


        labels.push(ticker);

        prices.push(
            data.price
        );

    });


    if (prices.length === 0) {
        return;
    }


    if (priceChart) {

        priceChart.destroy();

    }


    priceChart =
        new Chart(
            canvas,
            {
                type: "bar",

                data: {

                    labels: labels,

                    datasets: [

                        {
                            label: "Preço",

                            data: prices,

                            borderWidth: 1,

                            borderRadius: 8

                        }

                    ]

                },

                options: {

                    responsive: true,

                    maintainAspectRatio: false,

                    plugins: {

                        legend: {
                            display: false
                        },

                        tooltip: {

                            callbacks: {

                                label: context => {

                                    return formatMoney(
                                        context.raw
                                    );

                                }

                            }

                        }

                    },

                    scales: {

                        y: {

                            beginAtZero: false,

                            ticks: {

                                callback: value => {

                                    return formatMoney(
                                        value
                                    );

                                }

                            }

                        }

                    }

                }

            }
        );

}


function updateDistributionChart() {

    const canvas =
        document.getElementById(
            "distribution-chart"
        );

    if (!canvas) {
        return;
    }


    const labels = [];

    const prices = [];


    stocks.forEach(ticker => {

        const data =
            stockData[ticker];

        if (!data) {
            return;
        }


        labels.push(ticker);

        prices.push(
            data.price
        );

    });


    if (prices.length === 0) {
        return;
    }


    if (distributionChart) {

        distributionChart.destroy();

    }


    distributionChart =
        new Chart(
            canvas,
            {
                type: "pie",

                data: {

                    labels: labels,

                    datasets: [

                        {
                            data: prices,

                            borderWidth: 2

                        }

                    ]

                },

                options: {

                    responsive: true,

                    maintainAspectRatio: false,

                    plugins: {

                        legend: {

                            position: "bottom"

                        },

                        tooltip: {

                            callbacks: {

                                label: context => {

                                    const total =
                                        context.dataset.data
                                            .reduce(
                                                (sum, value) =>
                                                    sum + value,
                                                0
                                            );


                                    const percentage =
                                        (context.raw / total) * 100;


                                    return `${context.label}: ${percentage
                                        .toFixed(2)
                                        .replace(".", ",")}%`;

                                }

                            }

                        }

                    }

                }

            }
        );

}


async function checkAPI() {

    const status =
        document.getElementById(
            "api-status"
        );

    if (!status) {
        return;
    }


    try {

        const response =
            await fetch(
                "/health"
            );


        if (response.ok) {

            status.textContent =
                "● API online";

        }
        else {

            status.textContent =
                "● API offline";

        }

    }
    catch (error) {

        console.error(error);

        status.textContent =
            "● API offline";

    }

}


function updateMarketStatus() {

    const status =
        document.getElementById(
            "market-status"
        );

    if (!status) {
        return;
    }


    const loaded =
        Object.keys(
            stockData
        ).length;


    if (loaded === stocks.length) {

        status.textContent =
            "Online";

    }
    else {

        status.textContent =
            `${loaded}/${stocks.length}`;

    }

}


async function startMarket() {

    setupStockClicks();

    setupNavigation();

    await checkAPI();

    await updateStocks();


    setInterval(
        async () => {

            await updateStocks();

        },
        60000
    );

}


document.addEventListener(
    "DOMContentLoaded",
    startMarket
);