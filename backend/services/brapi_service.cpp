#include "brapi_service.h"

#include <curl/curl.h>
#include <stdexcept>
#include <string>

// Recebe os dados que o CURL baixar
static size_t WriteCallback(
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

Quote getQuoteFromBrapi(const std::string& ticker)
{
    CURL* curl = curl_easy_init();

    if (!curl)
    {
        throw std::runtime_error(
            "Não foi possível inicializar o CURL."
        );
    }

    std::string response;

    std::string url =
        "https://brapi.dev/api/quote/" + ticker;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

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

    CURLcode result = curl_easy_perform(curl);

    curl_easy_cleanup(curl);

    if (result != CURLE_OK)
    {
        throw std::runtime_error(
            "Erro ao consultar a BRAPI."
        );
    }

    // Por enquanto, vamos apenas verificar
    // se conseguimos receber a resposta.
    Quote quote;

    quote.ticker = ticker;
    quote.price = 0;
    quote.change = 0;
    quote.volume = 0;

    return quote;
}