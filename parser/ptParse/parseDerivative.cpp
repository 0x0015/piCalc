#include "parseDerivative.hpp"
#include "parseExpression.hpp"

parser::parseRes<std::shared_ptr<mathEngine::exprs::derivative>> parser::parseDerivative(std::span<const parser::mediumToken> tokens, expressionTypeToSkip skip){
	if(tokens.size() < 2)
		return std::nullopt;

	unsigned int usedTokens = 0;
	const auto& diffOp = tokens.front().value;
	if(std::holds_alternative<basicToken>(diffOp) && std::get<basicToken>(diffOp).val[0] == 'D' && std::get<basicToken>(diffOp).val.size() > 1){}else
		return std::nullopt;
	const auto& varible = std::get<basicToken>(diffOp).val.substr(1);
	usedTokens ++;
	tokens = tokens.subspan(1);

	const auto& expr = parseParens(tokens);
	if(!expr)
		return std::nullopt;
	usedTokens += expr->toksConsumed;

	auto output = std::make_shared<mathEngine::exprs::derivative>();
	output->expression = expr->val;
	output->wrtVar = varible;

	return makeParseRes(output, usedTokens);
}
