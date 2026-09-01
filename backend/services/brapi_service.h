#pragma once

#include "../models/quote.h"
#include <string>

Quote getQuoteFromBrapi(const std::string& ticker);