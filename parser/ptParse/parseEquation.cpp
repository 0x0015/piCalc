#include "parseEquation.hpp"
#include "parseExpression.hpp"

parser::parseRes<mathEngine::equation> parser::parseEquation(std::span<const parser::mediumToken> tokens){
	if(tokens.size() < 3)
		return std::nullopt;

	unsigned int usedTokens = 0;
	const auto& lhs = parseExpression(tokens);
	if(!lhs)
		return std::nullopt;
	usedTokens += lhs->toksConsumed;
	tokens = tokens.subspan(lhs->toksConsumed);

	const auto& equalSignT = tokens.front().value;
	if(std::holds_alternative<basicToken>(equalSignT) && std::get<basicToken>(equalSignT).val == "="){}else
		return std::nullopt;
	usedTokens ++;
	tokens = tokens.subspan(1);


	const auto& rhs = parseExpression(tokens);
	if(!rhs)
		return std::nullopt;
	usedTokens += rhs->toksConsumed;
	tokens = tokens.subspan(rhs->toksConsumed);

	return makeParseRes(mathEngine::equation{lhs->val, rhs->val}, usedTokens);
}
