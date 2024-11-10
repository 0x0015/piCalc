#pragma once
#include <span>
#include "../parseUtil.hpp"
#include "../tokenize/mediumTokenize.hpp"
#include "../../mathEngine/exprs/exponent.hpp"
#include "../../mathEngine/exprs/logarithm.hpp"
#include "parseExpression.hpp"

namespace parser{
	parseRes<std::shared_ptr<mathEngine::exprs::exponent>> parseExp(std::span<const mediumToken> tokens, expressionTypeToSkip skip = None);
	parseRes<std::shared_ptr<mathEngine::exprs::logarithm>> parseLn(std::span<const mediumToken> tokens, expressionTypeToSkip skip = None);
}
