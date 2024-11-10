#include "parseAdd.hpp"
#include "parseExpression.hpp"
#include "../../mathEngine/exprs/multiply.hpp"
#include "../../mathEngine/exprs/constant.hpp"

parser::parseRes<std::shared_ptr<mathEngine::exprs::add>> parser::parseAdd(std::span<const parser::mediumToken> tokens, expressionTypeToSkip skip){
	if(tokens.size() < 3)
		return std::nullopt;

	unsigned int usedTokens = 0;
	const auto& lhs = parseExpression(tokens, (expressionTypeToSkip)(expressionTypeToSkip::Add | skip));
	if(!lhs)
		return std::nullopt;
	usedTokens += lhs->toksConsumed;
	tokens = tokens.subspan(lhs->toksConsumed);

	const auto& equalSignT = tokens.front().value;
	if(std::holds_alternative<basicToken>(equalSignT) && std::get<basicToken>(equalSignT).val == "+"){}else
		return std::nullopt;
	usedTokens ++;
	tokens = tokens.subspan(1);


	const auto& rhs = parseExpression(tokens);
	if(!rhs)
		return std::nullopt;
	usedTokens += rhs->toksConsumed;
	tokens = tokens.subspan(rhs->toksConsumed);

	auto output = std::make_shared<mathEngine::exprs::add>();
	output->terms = {lhs->val, rhs->val};

	return makeParseRes(output, usedTokens);
}

parser::parseRes<std::shared_ptr<mathEngine::exprs::add>> parser::parseSub(std::span<const parser::mediumToken> tokens, expressionTypeToSkip skip){
	if(tokens.size() < 3)
		return std::nullopt;

	unsigned int usedTokens = 0;
	const auto& lhs = parseExpression(tokens, (expressionTypeToSkip)(expressionTypeToSkip::Sub | skip));
	if(!lhs)
		return std::nullopt;
	usedTokens += lhs->toksConsumed;
	tokens = tokens.subspan(lhs->toksConsumed);

	const auto& equalSignT = tokens.front().value;
	if(std::holds_alternative<basicToken>(equalSignT) && std::get<basicToken>(equalSignT).val == "-"){}else
		return std::nullopt;
	usedTokens ++;
	tokens = tokens.subspan(1);


	const auto& rhs = parseExpression(tokens);
	if(!rhs)
		return std::nullopt;
	usedTokens += rhs->toksConsumed;
	tokens = tokens.subspan(rhs->toksConsumed);

	auto rhs_actual = std::make_shared<mathEngine::exprs::multiply>();
	auto minusOneConst = std::make_shared<mathEngine::exprs::constant>();
	minusOneConst->value = mathEngine::constVal{rational{-1, 1}};
	rhs_actual->terms = {minusOneConst, rhs->val};

	auto output = std::make_shared<mathEngine::exprs::add>();
	output->terms = {lhs->val, rhs_actual};

	return makeParseRes(output, usedTokens);
}
