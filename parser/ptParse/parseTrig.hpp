#pragma once
#include <span>
#include "../parseUtil.hpp"
#include "../tokenize/mediumTokenize.hpp"
#include "../../mathEngine/exprs/sine.hpp"
#include "../../mathEngine/exprs/cosine.hpp"
#include "parseExpression.hpp"

namespace parser{
	parseRes<std::shared_ptr<mathEngine::exprs::sine>> parseSine(std::span<const mediumToken> tokens, expressionTypeToSkip skip = None);
	parseRes<std::shared_ptr<mathEngine::exprs::cosine>> parseCosine(std::span<const mediumToken> tokens, expressionTypeToSkip skip = None);
	parseRes<std::shared_ptr<mathEngine::expr>> parseTangent(std::span<const mediumToken> tokens, expressionTypeToSkip skip = None);
	parseRes<std::shared_ptr<mathEngine::expr>> parseTrigAny(std::span<const mediumToken> tokens, expressionTypeToSkip skip = None);
}
