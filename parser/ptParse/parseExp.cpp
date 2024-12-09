#include "parseExp.hpp"
#include "parseExpression.hpp"
#include "../../mathEngine/exprs/constant.hpp"

parser::parseRes<std::shared_ptr<mathEngine::exprs::exponent>> parser::parseExp(std::span<const parser::mediumToken> tokens, expressionTypeToSkip skip){
	if(tokens.size() < 3)
		return std::nullopt;

	unsigned int usedTokens = 0;
	const auto& lhs = parseSimpleExpression(tokens);
	if(!lhs)
		return std::nullopt;
	usedTokens += lhs->toksConsumed;
	tokens = tokens.subspan(lhs->toksConsumed);

	const auto& equalSignT = tokens.front().value;
	if(std::holds_alternative<basicToken>(equalSignT) && std::get<basicToken>(equalSignT).val == "^"){}else
		return std::nullopt;
	usedTokens ++;
	tokens = tokens.subspan(1);


	const auto& rhs = parseExpression(tokens);
	if(!rhs)
		return std::nullopt;
	usedTokens += rhs->toksConsumed;
	tokens = tokens.subspan(rhs->toksConsumed);

	auto output = std::make_shared<mathEngine::exprs::exponent>();
	output->base = lhs->val;
	output->exp = rhs->val;

	return makeParseRes(output, usedTokens);
}

parser::parseRes<std::shared_ptr<mathEngine::exprs::logarithm>> parser::parseLn(std::span<const parser::mediumToken> tokens, expressionTypeToSkip skip){
	if(tokens.size() < 2)
		return std::nullopt;

	unsigned int usedTokens = 0;
	const auto& diffOp = tokens.front().value;
	if(std::holds_alternative<basicToken>(diffOp) && std::get<basicToken>(diffOp).val == "ln"){}else
		return std::nullopt;
	const auto& varible = std::get<basicToken>(diffOp).val.substr(1);
	usedTokens ++;
	tokens = tokens.subspan(1);

	const auto& expr = parseParens(tokens);
	if(!expr)
		return std::nullopt;
	usedTokens += expr->toksConsumed;

	auto output = std::make_shared<mathEngine::exprs::logarithm>();
	output->inside = expr->val;
	auto eConst = std::make_shared<mathEngine::exprs::constant>();
	eConst->value = mathEngine::constVal{mathEngine::constantName::E};
	output->base = eConst;

	return makeParseRes(output, usedTokens);
}

