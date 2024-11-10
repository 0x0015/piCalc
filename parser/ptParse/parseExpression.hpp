#pragma once
#include <span>
#include "../parseUtil.hpp"
#include "../tokenize/mediumTokenize.hpp"
#include "../../mathEngine/expr.hpp"

namespace parser{
	enum expressionTypeToSkip{
		None = 0,
		Add = 0b1,
		Sub = 0b10,
		Mul = 0b100,
		Div = 0b1000,
		Exp = 0b10000
	};
	parseRes<std::shared_ptr<mathEngine::expr>> parseParens(std::span<const parser::mediumToken> tokens);
	parseRes<std::shared_ptr<mathEngine::expr>> parseExpression(std::span<const mediumToken> tokens, expressionTypeToSkip skip = None);
}
