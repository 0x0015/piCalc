#include "parseIntegral.hpp"
#include "parseExpression.hpp"

parser::parseRes<std::shared_ptr<mathEngine::exprs::integral>> parser::parseIntegral(std::span<const parser::mediumToken> tokens, expressionTypeToSkip skip){
	if(tokens.size() < 2)
		return std::nullopt;

	unsigned int usedTokens = 0;
	const auto& intOp = tokens.front().value;
	if(!std::holds_alternative<basicToken>(intOp))
		return std::nullopt;
	auto basicTok = std::get<basicToken>(intOp);
	if(basicTok.val.substr(0, 4) != "int_")
		return std::nullopt;
	const auto& varible = basicTok.val.substr(4);
	usedTokens ++;
	tokens = tokens.subspan(1);

	const auto& expr = parseParens(tokens);
	if(!expr)
		return std::nullopt;
	usedTokens += expr->toksConsumed;

	auto output = std::make_shared<mathEngine::exprs::integral>();
	output->expression = expr->val;
	output->wrtVar = varible;

	return makeParseRes(output, usedTokens);
}
