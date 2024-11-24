#include "parseAbs.hpp"
#include "parseExpression.hpp"

parser::parseRes<std::shared_ptr<mathEngine::exprs::absoluteValue>> parser::parseAbs(std::span<const parser::mediumToken> tokens, expressionTypeToSkip skip){
	if(tokens.size() < 2)
		return std::nullopt;

	unsigned int usedTokens = 0;
	const auto& diffOp = tokens.front().value;
	if(std::holds_alternative<basicToken>(diffOp) && std::get<basicToken>(diffOp).val == "abs"){}else
		return std::nullopt;
	const auto& varible = std::get<basicToken>(diffOp).val.substr(1);
	usedTokens ++;
	tokens = tokens.subspan(1);

	const auto& expr = parseParens(tokens);
	if(!expr)
		return std::nullopt;
	usedTokens += expr->toksConsumed;

	auto output = std::make_shared<mathEngine::exprs::absoluteValue>();
	output->inside = expr->val;

	return makeParseRes(output, usedTokens);
}

