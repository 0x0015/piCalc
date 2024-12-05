#pragma once
#include <span>
#include "../parseUtil.hpp"
#include "../tokenize/mediumTokenize.hpp"
#include "../../mathEngine/exprs/integral.hpp"
#include "parseExpression.hpp"

namespace parser{
	parseRes<std::shared_ptr<mathEngine::exprs::integral>> parseIntegral(std::span<const mediumToken> tokens, expressionTypeToSkip skip = None);
}
