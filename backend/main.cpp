#include "crow.h"
#include "models/quote.h"
#include "services/market_service.h"

#include <fstream>
#include <sstream>
#include <string>

std::string readFile(const std::string& path)
{
    std::ifstream file(path);

    if (!file.is_open())
    {
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

int main()
{
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")
    ([]()
    {
        std::string html = readFile("frontend/index.html");

        if (html.empty())
        {
            return crow::response(
                500,
                "Erro: frontend/index.html nao encontrado."
            );
        }

        crow::response response(html);
        response.set_header(
            "Content-Type",
            "text/html; charset=UTF-8"
        );

        return response;
    });

    CROW_ROUTE(app, "/style.css")
    ([]()
    {
        std::string css = readFile("frontend/style.css");

        if (css.empty())
        {
            return crow::response(
                404,
                "style.css nao encontrado."
            );
        }

        crow::response response(css);
        response.set_header(
            "Content-Type",
            "text/css; charset=UTF-8"
        );

        return response;
    });

    CROW_ROUTE(app, "/app.js")
    ([]()
    {
        std::string js = readFile("frontend/app.js");

        if (js.empty())
        {
            return crow::response(
                404,
                "app.js nao encontrado."
            );
        }

        crow::response response(js);
        response.set_header(
            "Content-Type",
            "application/javascript; charset=UTF-8"
        );

        return response;
    });

    CROW_ROUTE(app, "/health")
    ([]()
    {
        return crow::response("OK");
    });

    CROW_ROUTE(app, "/api")
    ([]()
    {
        crow::json::wvalue resposta;

        resposta["name"] = "BussinessHub";
        resposta["status"] = "online";
        resposta["version"] = "1.0";

        return resposta;
    });

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

        resposta["fiftyTwoWeekHigh"] =
            quote.fiftyTwoWeekHigh;

        resposta["fiftyTwoWeekLow"] =
            quote.fiftyTwoWeekLow;

        resposta["marketTime"] =
            quote.marketTime;

        return resposta;
    });

    app.port(18080)
       .multithreaded()
       .run();
}