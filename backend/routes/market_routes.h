#pragma once

#include "crow.h"
#include "../models/quote.h"
#include "../services/market_service.h"

void registerMarketRoutes(crow::SimpleApp& app)
{
    CROW_ROUTE(app, "/api/quote/<string>")
    ([](std::string ticker)
    {
        Quote quote = getQuoteFromBrapi(ticker);

        crow::json::wvalue resposta;

        resposta["ticker"] = quote.ticker;
        resposta["name"] = quote.name;
        resposta["currency"] = quote.currency;

        resposta["price"] = quote.price;
        resposta["change"] = quote.change;
        resposta["changePercent"] = quote.changePercent;

        resposta["open"] = quote.open;
        resposta["dayHigh"] = quote.dayHigh;
        resposta["dayLow"] = quote.dayLow;
        resposta["previousClose"] = quote.previousClose;

        resposta["volume"] = quote.volume;
        resposta["marketCap"] = quote.marketCap;

        resposta["fiftyTwoWeekHigh"] = quote.fiftyTwoWeekHigh;
        resposta["fiftyTwoWeekLow"] = quote.fiftyTwoWeekLow;

        resposta["marketTime"] = quote.marketTime;

        return resposta;
    });
}