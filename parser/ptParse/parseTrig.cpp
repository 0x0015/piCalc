#include "parseTrig.hpp"
#include "parseExpression.hpp"
#include "../../mathEngine/exprs/multiply.hpp"
#include "../../mathEngine/exprs/exponent.hpp"
#include "../../mathEngine/exprs/constant.hpp"

parser::parseRes<std::shared_ptr<mathEngine::exprs::sine>> parser::parseSine(std::span<const parser::mediumToken> tokens, expressionTypeToSkip skip){
	if(tokens.size() < 2)
		return std::nullopt;

	unsigned int usedTokens = 0;
	const auto& diffOp = tokens.front().value;
	if(std::holds_alternative<basicToken>(diffOp) && std::get<basicToken>(diffOp).val == "sin"){}else
		return std::nullopt;
	const auto& varible = std::get<basicToken>(diffOp).val.substr(1);
	usedTokens ++;
	tokens = tokens.subspan(1);

	const auto& expr = parseParens(tokens);
	if(!expr)
		return std::nullopt;
	usedTokens += expr->toksConsumed;

	auto output = std::make_shared<mathEngine::exprs::sine>();
	output->inside = expr->val;

	return makeParseRes(output, usedTokens);
}

parser::parseRes<std::shared_ptr<mathEngine::exprs::cosine>> parser::parseCosine(std::span<const parser::mediumToken> tokens, expressionTypeToSkip skip){
	if(tokens.size() < 2)
		return std::nullopt;

	unsigned int usedTokens = 0;
	const auto& diffOp = tokens.front().value;
	if(std::holds_alternative<basicToken>(diffOp) && std::get<basicToken>(diffOp).val == "cos"){}else
		return std::nullopt;
	const auto& varible = std::get<basicToken>(diffOp).val.substr(1);
	usedTokens ++;
	tokens = tokens.subspan(1);

	const auto& expr = parseParens(tokens);
	if(!expr)
		return std::nullopt;
	usedTokens += expr->toksConsumed;

	auto output = std::make_shared<mathEngine::exprs::cosine>();
	output->inside = expr->val;

	return makeParseRes(output, usedTokens);
}

parser::parseRes<std::shared_ptr<mathEngine::expr>> parser::parseTangent(std::span<const parser::mediumToken> tokens, expressionTypeToSkip skip){
	if(tokens.size() < 2)
		return std::nullopt;

	unsigned int usedTokens = 0;
	const auto& diffOp = tokens.front().value;
	if(std::holds_alternative<basicToken>(diffOp) && std::get<basicToken>(diffOp).val == "tan"){}else
		return std::nullopt;
	const auto& varible = std::get<basicToken>(diffOp).val.substr(1);
	usedTokens ++;
	tokens = tokens.subspan(1);

	const auto& expr = parseParens(tokens);
	if(!expr)
		return std::nullopt;
	usedTokens += expr->toksConsumed;

	auto output = std::make_shared<mathEngine::exprs::multiply>();
	auto top = std::make_shared<mathEngine::exprs::sine>();
	top->inside = expr->val;

	auto bottom = std::make_shared<mathEngine::exprs::exponent>();
	auto bottomInside = std::make_shared<mathEngine::exprs::cosine>();
	bottomInside->inside = expr->val->clone();
	bottom->base = bottomInside;
	auto minus1Exp = std::make_shared<mathEngine::exprs::constant>();
	minus1Exp->value = mathEngine::constVal{rational{-1,1}};
	bottom->exp = minus1Exp;
	output->terms.push_back(top);
	output->terms.push_back(bottom);

	return makeParseRes<std::shared_ptr<mathEngine::expr>>(output, usedTokens);
}

parser::parseRes<std::shared_ptr<mathEngine::expr>> parser::parseTrigAny(std::span<const parser::mediumToken> tokens, expressionTypeToSkip skip){
	using rt = std::shared_ptr<mathEngine::expr>;
	const auto& sinTry = parseSine(tokens);
	if(sinTry)
		return makeParseRes<rt>(sinTry->val, sinTry->toksConsumed);

	const auto& cosTry = parseCosine(tokens);
	if(cosTry)
		return makeParseRes<rt>(cosTry->val, cosTry->toksConsumed);

	const auto& tanTry = parseTangent(tokens);
	if(tanTry)
		return makeParseRes<rt>(tanTry->val, tanTry->toksConsumed);

	return std::nullopt;
}
