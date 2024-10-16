#include "parseExpression.hpp"
#include "parseAdd.hpp"
#include "parseMul.hpp"
#include "parseConst.hpp"
#include "../../mathEngine/exprs/variable.hpp"

//don't know where else to put this
parser::parseRes<std::shared_ptr<mathEngine::expr>> parseParens(std::span<const parser::mediumToken> tokens){
	if(tokens.empty())
		return std::nullopt;

	if(!std::holds_alternative<parser::mediumToken::tokList>(tokens.front().value))
		return std::nullopt;

	const auto& exprTry = parser::parseExpression(std::get<parser::mediumToken::tokList>(tokens.front().value).value);
	if(!exprTry)
		return std::nullopt;

	return parser::makeParseRes(exprTry->val, 1);
}

parser::parseRes<std::shared_ptr<mathEngine::expr>> parser::parseExpression(std::span<const parser::mediumToken> tokens, expressionTypeToSkip skip){
	using rt = std::shared_ptr<mathEngine::expr>;
	const auto& parens = parseParens(tokens);
	expressionTypeToSkip sk = (expressionTypeToSkip)~skip;
	if(parens)
		return makeParseRes(parens->val, parens->toksConsumed);
	if(sk & expressionTypeToSkip::Add){
		const auto& add = parseAdd(tokens, skip);
		if(add)
			return makeParseRes<rt>(add->val, add->toksConsumed);
	}
	if(sk & expressionTypeToSkip::Mul){
		const auto& mul = parseMul(tokens, skip);
		if(mul)
			return makeParseRes<rt>(mul->val, mul->toksConsumed);
	}

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
