#pragma once
#include <span>
#include "../parseUtil.hpp"
#include "../tokenize/mediumTokenize.hpp"
#include "../../mathEngine/expr.hpp"

namespace parser{
	enum expressionTypeToSkip{
		None = 0,
		Add = 0b1,
		Mul = 0b10
	};
	parseRes<std::shared_ptr<mathEngine::expr>> parseExpression(std::span<const mediumToken> tokens, expressionTypeToSkip skip = None);
}
