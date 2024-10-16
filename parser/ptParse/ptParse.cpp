#include "ptParse.hpp"
#include "../tokenize/basicTokenize.hpp"
#include "../tokenize/mediumTokenize.hpp"
#include "parseEquation.hpp"
#include "parseExpression.hpp"
#include "../../mathEngine/exprs/variable.hpp"

std::optional<parser::parseResult> parser::ptParse::parse(const std::string_view str){
 	const auto& basicToks = basicTokenize(str);
	if(!basicToks)
		return std::nullopt;
	const auto& mediumToks = parseBrackets(*basicToks);
	if(!mediumToks)
		return std::nullopt;

	parseResult res;

	auto registerExprVars = [&](std::shared_ptr<mathEngine::expr> expr){
		expr->propegateDFS([&](std::shared_ptr<mathEngine::expr> expr){
			if(dynamic_cast<mathEngine::exprs::variable*>(expr.get()) != nullptr){
				auto var = std::dynamic_pointer_cast<mathEngine::exprs::variable>(expr);
				res.vars.insert(var->name);
				mathEngine::exprs::variable::varVals[var->name] = mathEngine::constVal{rational(1,1)}; //just some default value
			}
		}, false);
	};

	const auto& parsedEq = parseEquation(*mediumToks);
	if(!parsedEq){
		const auto& parsedExpr = parseExpression(*mediumToks);
		if(!parsedExpr)
			return std::nullopt;
		res.value = parsedExpr->val;
		registerExprVars(parsedExpr->val);
	}else{
		res.value = parsedEq->val;
		registerExprVars(parsedEq->val.lhs);
		registerExprVars(parsedEq->val.rhs);
	}

	return res;
}

