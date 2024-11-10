#pragma once
#include <span>
#include "../parseUtil.hpp"
#include "../tokenize/mediumTokenize.hpp"
#include "../../mathEngine/exprs/multiply.hpp"
#include "parseExpression.hpp"

namespace parser{
	parseRes<std::shared_ptr<mathEngine::exprs::multiply>> parseMul(std::span<const mediumToken> tokens, expressionTypeToSkip skip = None);
	parseRes<std::shared_ptr<mathEngine::exprs::multiply>> parseDiv(std::span<const mediumToken> tokens, expressionTypeToSkip skip = None);
}
