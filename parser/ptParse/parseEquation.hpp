#pragma once
#include <span>
#include "../parseUtil.hpp"
#include "../tokenize/mediumTokenize.hpp"
#include "../../mathEngine/equation.hpp"

namespace parser{
	parseRes<mathEngine::equation> parseEquation(std::span<const mediumToken> tokens);
}
