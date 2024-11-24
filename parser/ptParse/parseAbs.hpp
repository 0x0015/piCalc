#pragma once
#include <span>
#include "../parseUtil.hpp"
#include "../tokenize/mediumTokenize.hpp"
#include "../../mathEngine/exprs/absoluteValue.hpp"
#include "parseExpression.hpp"

namespace parser{
	parseRes<std::shared_ptr<mathEngine::exprs::absoluteValue>> parseAbs(std::span<const mediumToken> tokens, expressionTypeToSkip skip = None);
}
