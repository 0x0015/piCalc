#include "parseExpression.hpp"
#include "parseAdd.hpp"
#include "parseMul.hpp"
#include "parseConst.hpp"
#include "parseDerivative.hpp"
#include "parseIntegral.hpp"
#include "parseTrig.hpp"
#include "parseExp.hpp"
#include "parseAbs.hpp"
#include "../../mathEngine/exprs/variable.hpp"

//don't know where else to put this
parser::parseRes<std::shared_ptr<mathEngine::expr>> parser::parseParens(std::span<const parser::mediumToken> tokens){
	if(tokens.empty())
		return std::nullopt;

	if(!std::holds_alternative<parser::mediumToken::tokList>(tokens.front().value))
		return std::nullopt;

	const auto& exprTry = parser::parseExpression(std::get<parser::mediumToken::tokList>(tokens.front().value).value);
	if(!exprTry)
		return std::nullopt;

	return parser::makeParseRes(exprTry->val, 1);
}

parser::parseRes<std::shared_ptr<mathEngine::expr>> parser::parseSimpleExpression(std::span<const parser::mediumToken> tokens){
	using rt = std::shared_ptr<mathEngine::expr>;
	const auto& derivative = parseDerivative(tokens);
	if(derivative)
		return makeParseRes<rt>(derivative->val, derivative->toksConsumed);
	const auto& integral = parseIntegral(tokens);
	if(integral)
		return makeParseRes<rt>(integral->val, integral->toksConsumed);
	const auto& trig = parseTrigAny(tokens);
	if(trig)
		return makeParseRes<rt>(trig->val, trig->toksConsumed);
	const auto& ln = parseLn(tokens);
	if(ln)
		return makeParseRes<rt>(ln->val, ln->toksConsumed);
	const auto& abs = parseAbs(tokens);
	if(abs)
		return makeParseRes<rt>(abs->val, abs->toksConsumed);
	const auto& parens = parseParens(tokens);
	if(parens)
		return makeParseRes(parens->val, parens->toksConsumed);

	const auto& constant = parseConst(tokens);
	if(constant)
		return makeParseRes<rt>(constant->val, constant->toksConsumed);

	if(!tokens.empty() && std::holds_alternative<basicToken>(tokens.front().value)){//try variable if all else fails
		auto var = std::make_shared<mathEngine::exprs::variable>();
		var->name = std::get<basicToken>(tokens.front().value).val;
		return makeParseRes<rt>(var, 1);
	}

	return std::nullopt;
}

parser::parseRes<std::shared_ptr<mathEngine::expr>> parser::parseExpression(std::span<const parser::mediumToken> tokens, expressionTypeToSkip skip){
	using rt = std::shared_ptr<mathEngine::expr>;
	expressionTypeToSkip sk = (expressionTypeToSkip)~skip;
	if(sk & expressionTypeToSkip::Add){
		const auto& add = parseAdd(tokens, skip);
		if(add)
			return makeParseRes<rt>(add->val, add->toksConsumed);
	}
	if(sk & expressionTypeToSkip::Sub){
		const auto& sub = parseSub(tokens, skip);
		if(sub)
			return makeParseRes<rt>(sub->val, sub->toksConsumed);
	}
	if(sk & expressionTypeToSkip::Mul){
		const auto& mul = parseMul(tokens, skip);
		if(mul)
			return makeParseRes<rt>(mul->val, mul->toksConsumed);
	}
	if(sk & expressionTypeToSkip::Div){
		const auto& div = parseDiv(tokens, skip);
		if(div)
			return makeParseRes<rt>(div->val, div->toksConsumed);
	}
	if(sk & expressionTypeToSkip::Exp){
		const auto& exp = parseExp(tokens, skip);
		if(exp)
			return makeParseRes<rt>(exp->val, exp->toksConsumed);
	}
	const auto& derivative = parseDerivative(tokens);
	if(derivative)
		return makeParseRes<rt>(derivative->val, derivative->toksConsumed);
	const auto& integral = parseIntegral(tokens);
	if(integral)
		return makeParseRes<rt>(integral->val, integral->toksConsumed);
	const auto& trig = parseTrigAny(tokens);
	if(trig)
		return makeParseRes<rt>(trig->val, trig->toksConsumed);
	const auto& ln = parseLn(tokens);
	if(ln)
		return makeParseRes<rt>(ln->val, ln->toksConsumed);
	const auto& abs = parseAbs(tokens);
	if(abs)
		return makeParseRes<rt>(abs->val, abs->toksConsumed);
	const auto& parens = parseParens(tokens);
	if(parens)
		return makeParseRes(parens->val, parens->toksConsumed);

	const auto& constant = parseConst(tokens);
	if(constant)
		return makeParseRes<rt>(constant->val, constant->toksConsumed);

	if(!tokens.empty() && std::holds_alternative<basicToken>(tokens.front().value)){//try variable if all else fails
		auto var = std::make_shared<mathEngine::exprs::variable>();
		var->name = std::get<basicToken>(tokens.front().value).val;
		return makeParseRes<rt>(var, 1);
	}

	return std::nullopt;
}
