#pragma once

#include "crow.h"

#include <curl/curl.h>
#include <string>

#include "../models/quote.h"

// Recebe a resposta do curl
inline size_t WriteCallback(
    void* contents,
    size_t size,
    size_t nmemb,
    void* userp)
{
    size_t totalSize = size * nmemb;

    std::string* response =
        static_cast<std::string*>(userp);

    response->append(
        static_cast<char*>(contents),
        totalSize
    );

    return totalSize;
}

// Busca os dados da ação
inline Quote getQuoteFromBrapi(
    const std::string& ticker)
{
    Quote quote;

    quote.ticker = ticker;

    // Inicia o curl
    CURL* curl = curl_easy_init();

    if (!curl)
    {
        return quote;
    }

    // Adiciona .SA para ações da B3
    std::string yahooTicker =
        ticker + ".SA";

    // Monta a URL
    std::string url =
        "https://query1.finance.yahoo.com/v8/finance/chart/" +
        yahooTicker +
        "?range=1d&interval=1m";

    std::string response;

    curl_easy_setopt(
        curl,
        CURLOPT_URL,
        url.c_str()
    );

    curl_easy_setopt(
        curl,
        CURLOPT_WRITEFUNCTION,
        WriteCallback
    );

    curl_easy_setopt(
        curl,
        CURLOPT_WRITEDATA,
        &response
    );

    curl_easy_setopt(
        curl,
        CURLOPT_FOLLOWLOCATION,
        1L
    );

    curl_easy_setopt(
        curl,
        CURLOPT_CONNECTTIMEOUT,
        10L
    );

    curl_easy_setopt(
        curl,
        CURLOPT_TIMEOUT,
        20L
    );

    curl_easy_setopt(
        curl,
        CURLOPT_SSL_VERIFYPEER,
        1L
    );

    curl_easy_setopt(
        curl,
        CURLOPT_SSL_VERIFYHOST,
        2L
    );

    curl_easy_setopt(
        curl,
        CURLOPT_USERAGENT,
        "Mozilla/5.0"
    );

    // Faz a requisição
    CURLcode result =
        curl_easy_perform(curl);

    long httpCode = 0;

    curl_easy_getinfo(
        curl,
        CURLINFO_RESPONSE_CODE,
        &httpCode
    );

    curl_easy_cleanup(curl);

    // Se deu erro, retorna vazio
    if (result != CURLE_OK)
    {
        return quote;
    }

    if (httpCode != 200)
    {
        return quote;
    }

    // Lê o JSON
    auto json =
        crow::json::load(response);

    if (!json)
    {
        return quote;
    }

    if (!json.has("chart"))
    {
        return quote;
    }

    auto chart =
        json["chart"];

    if (!chart.has("result"))
    {
        return quote;
    }

    auto results =
        chart["result"];

    if (results.size() == 0)
    {
        return quote;
    }

    auto data =
        results[0];

    // Pega os dados principais
    if (data.has("meta"))
    {
        auto meta =
            data["meta"];

        if (meta.has("currency"))
        {
            try
            {
                quote.currency =
                    meta["currency"].s();
            }
            catch (...)
            {
            }
        }

        if (meta.has("regularMarketPrice"))
        {
            try
            {
                quote.price =
                    meta["regularMarketPrice"].d();
            }
            catch (...)
            {
            }
        }

        if (meta.has("chartPreviousClose"))
        {
            try
            {
                quote.previousClose =
                    meta["chartPreviousClose"].d();
            }
            catch (...)
            {
            }
        }

        if (meta.has("regularMarketOpen"))
        {
            try
            {
                quote.open =
                    meta["regularMarketOpen"].d();
            }
            catch (...)
            {
            }
        }

        if (meta.has("regularMarketDayHigh"))
        {
            try
            {
                quote.dayHigh =
                    meta["regularMarketDayHigh"].d();
            }
            catch (...)
            {
            }
        }

        if (meta.has("regularMarketDayLow"))
        {
            try
            {
                quote.dayLow =
                    meta["regularMarketDayLow"].d();
            }
            catch (...)
            {
            }
        }

        if (meta.has("regularMarketVolume"))
        {
            try
            {
                quote.volume =
                    meta["regularMarketVolume"].i();
            }
            catch (...)
            {
            }
        }

        if (meta.has("regularMarketTime"))
        {
            try
            {
                quote.marketTime =
                    std::to_string(
                        meta["regularMarketTime"].i()
                    );
            }
            catch (...)
            {
            }
        }
    }

    // Pega os valores do gráfico
    if (data.has("indicators"))
    {
        auto indicators =
            data["indicators"];

        if (indicators.has("quote"))
        {
            auto quoteData =
                indicators["quote"];

            if (quoteData.size() > 0)
            {
                auto currentQuote =
                    quoteData[0];

                // Abertura
                if (currentQuote.has("open"))
                {
                    auto opens =
                        currentQuote["open"];

                    for (
                        int i = static_cast<int>(opens.size()) - 1;
                        i >= 0;
                        --i)
                    {
                        try
                        {
                            double value =
                                opens[i].d();

                            if (value != 0.0)
                            {
                                quote.open = value;
                                break;
                            }
                        }
                        catch (...)
                        {
                        }
                    }
                }

                // Máxima
                if (currentQuote.has("high"))
                {
                    auto highs =
                        currentQuote["high"];

                    for (
                        int i = static_cast<int>(highs.size()) - 1;
                        i >= 0;
                        --i)
                    {
                        try
                        {
                            double value =
                                highs[i].d();

                            if (value != 0.0)
                            {
                                quote.dayHigh = value;
                                break;
                            }
                        }
                        catch (...)
                        {
                        }
                    }
                }

                // Mínima
                if (currentQuote.has("low"))
                {
                    auto lows =
                        currentQuote["low"];

                    for (
                        int i = static_cast<int>(lows.size()) - 1;
                        i >= 0;
                        --i)
                    {
                        try
                        {
                            double value =
                                lows[i].d();

                            if (value != 0.0)
                            {
                                quote.dayLow = value;
                                break;
                            }
                        }
                        catch (...)
                        {
                        }
                    }
                }

                // Preço atual
                if (currentQuote.has("close"))
                {
                    auto closes =
                        currentQuote["close"];

                    for (
                        int i = static_cast<int>(closes.size()) - 1;
                        i >= 0;
                        --i)
                    {
                        try
                        {
                            double value =
                                closes[i].d();

                            if (value != 0.0)
                            {
                                quote.price = value;
                                break;
                            }
                        }
                        catch (...)
                        {
                        }
                    }
                }

                // Volume
                if (currentQuote.has("volume"))
                {
                    auto volumes =
                        currentQuote["volume"];

                    for (
                        int i = static_cast<int>(volumes.size()) - 1;
                        i >= 0;
                        --i)
                    {
                        try
                        {
                            long long value =
                                volumes[i].i();

                            if (value != 0)
                            {
                                quote.volume = value;
                                break;
                            }
                        }
                        catch (...)
                        {
                        }
                    }
                }
            }
        }
    }

    // Calcula a variação
    if (
        quote.price != 0.0 &&
        quote.previousClose != 0.0)
    {
        quote.change =
            quote.price -
            quote.previousClose;

        quote.changePercent =
            (quote.change /
             quote.previousClose) * 100.0;
    }

    return quote;
}