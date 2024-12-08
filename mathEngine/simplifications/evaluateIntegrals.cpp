#include "evaluateIntegrals.hpp"
#include "../exprs/integral.hpp"
#include "../exprs/add.hpp"
#include "../exprs/constant.hpp"
#include "../exprs/exponent.hpp"
#include "../exprs/multiply.hpp"
#include "../exprs/variable.hpp"
#include "../exprs/derivative.hpp"
#include "../exprs/sine.hpp"
#include "../exprs/cosine.hpp"
#include "../exprs/logarithm.hpp"
#include "../exprs/absoluteValue.hpp"

std::shared_ptr<mathEngine::exprs::integral> quickConstructIntegral(std::shared_ptr<mathEngine::expr> exp, std::string_view wrtVar){
	auto output = std::make_shared<mathEngine::exprs::integral>();
	output->wrtVar = wrtVar;
	output->expression = exp;
	return output;
};

std::shared_ptr<mathEngine::exprs::variable> quickConstructVarFromWrtvar(std::string_view wrtVar){
	auto output = std::make_shared<mathEngine::exprs::variable>();
	output->name = wrtVar;
	return output;
}

std::optional<std::shared_ptr<mathEngine::expr>> getIntegralOf(std::shared_ptr<mathEngine::exprs::add> add, std::string_view wrtVar){
	if(add->terms.size() == 1)
		return mathEngine::simplification::evaluateIntegral(add->terms.front(), wrtVar);

	auto output = std::make_shared<mathEngine::exprs::add>();
	for(const auto& term : add->terms){
		output->terms.push_back(quickConstructIntegral(term, wrtVar));
	}
	return output;
}

std::optional<std::shared_ptr<mathEngine::expr>> getIntegralOf(std::shared_ptr<mathEngine::exprs::constant> constant, std::string_view wrtVar){
	auto output = std::make_shared<mathEngine::exprs::multiply>();
	output->terms = {constant, quickConstructVarFromWrtvar(wrtVar)};
	return output;
}

//the below have been left out for now due to complications with how integration is like impossible.
//special cases that do work (for example sin(x) works while sin(x^3) doesn't really) will be added later
std::optional<std::shared_ptr<mathEngine::expr>> getIntegralOf(std::shared_ptr<mathEngine::exprs::exponent> exp, std::string_view wrtVar){
	if(exp->base->isInstance<mathEngine::exprs::variable>() && exp->exp->isInstance<mathEngine::exprs::constant>()){
		//ouch.  This is a mess
		auto oneConst = std::make_shared<mathEngine::exprs::constant>();
		oneConst->value = mathEngine::constVal{rational{1, 1}};
		auto newExpVal = std::make_shared<mathEngine::exprs::add>();
		newExpVal->terms = {oneConst, exp->exp};
		auto coeff = std::make_shared<mathEngine::exprs::exponent>();
		auto minusOneConst = std::make_shared<mathEngine::exprs::constant>();
		minusOneConst->value = mathEngine::constVal{rational{-1, 1}};
		coeff->exp = minusOneConst;
		coeff->base = newExpVal->clone();
		auto newExp = std::make_shared<mathEngine::exprs::exponent>();
		newExp->base = exp->base;
		newExp->exp = newExpVal;
		auto output = std::make_shared<mathEngine::exprs::multiply>();
		output->terms = {coeff, newExp};
		return output;
	}
	return std::nullopt;
}

std::optional<std::shared_ptr<mathEngine::expr>> getIntegralOf(std::shared_ptr<mathEngine::exprs::logarithm> log, std::string_view wrtVar){
	return std::nullopt;
}

std::optional<std::shared_ptr<mathEngine::expr>> getIntegralOf(std::shared_ptr<mathEngine::exprs::multiply> mul, std::string_view wrtVar){
	return std::nullopt;
}

std::optional<std::shared_ptr<mathEngine::expr>> getIntegralOf(std::shared_ptr<mathEngine::exprs::sine> sine, std::string_view wrtVar){
	return std::nullopt;
}

std::optional<std::shared_ptr<mathEngine::expr>> getIntegralOf(std::shared_ptr<mathEngine::exprs::cosine> cosine, std::string_view wrtVar){
	return std::nullopt;
}

std::optional<std::shared_ptr<mathEngine::expr>> getIntegralOf(std::shared_ptr<mathEngine::exprs::absoluteValue> abs, std::string_view wrtVar){
	return std::nullopt;
}

std::optional<std::shared_ptr<mathEngine::expr>> getIntegralOf(std::shared_ptr<mathEngine::exprs::variable> var, std::string_view wrtVar){
	if(var->name == wrtVar){
		auto output = std::make_shared<mathEngine::exprs::multiply>();
		auto halfTerm = std::make_shared<mathEngine::exprs::constant>();
		halfTerm->value = mathEngine::constVal{rational{1,2}};
		auto twoTerm = std::make_shared<mathEngine::exprs::constant>();
		twoTerm->value = mathEngine::constVal{rational{2,1}};
		auto squaredTerm = std::make_shared<mathEngine::exprs::exponent>();
		squaredTerm->base = var;
		squaredTerm->exp = twoTerm;
		output->terms = {halfTerm, squaredTerm};
		return output;
	}else{
		//for now assume all other variables are constant wrt the given variable
		auto output = std::make_shared<mathEngine::exprs::multiply>();
		output->terms = {var, quickConstructVarFromWrtvar(wrtVar)};
		return output;
	}
}

std::optional<std::shared_ptr<mathEngine::expr>> getIntegralOf(std::shared_ptr<mathEngine::exprs::derivative> der, std::string_view wrtVar){
	if(der->wrtVar == wrtVar){
		return der->expression;
	}
	return std::nullopt;
}

std::optional<std::shared_ptr<mathEngine::expr>> mathEngine::simplification::evaluateIntegral(std::shared_ptr<expr> der, std::string_view wrtVar){
	switch(der->type){
		case mathEngine::exprs::add::typeID:
			return getIntegralOf(der->getAs<exprs::add>(), wrtVar);
		case mathEngine::exprs::constant::typeID:
			return getIntegralOf(der->getAs<exprs::constant>(), wrtVar);
		case mathEngine::exprs::exponent::typeID:
			return getIntegralOf(der->getAs<exprs::exponent>(), wrtVar);
		case mathEngine::exprs::multiply::typeID:
			return getIntegralOf(der->getAs<exprs::multiply>(), wrtVar);
		case mathEngine::exprs::variable::typeID:
			return getIntegralOf(der->getAs<exprs::variable>(), wrtVar);
		case mathEngine::exprs::sine::typeID:
			return getIntegralOf(der->getAs<exprs::sine>(), wrtVar);
		case mathEngine::exprs::cosine::typeID:
			return getIntegralOf(der->getAs<exprs::cosine>(), wrtVar);
		case mathEngine::exprs::logarithm::typeID:
			return getIntegralOf(der->getAs<exprs::logarithm>(), wrtVar);
		case mathEngine::exprs::absoluteValue::typeID:
			return getIntegralOf(der->getAs<exprs::absoluteValue>(), wrtVar);
		case mathEngine::exprs::derivative::typeID:
			return getIntegralOf(der->getAs<exprs::derivative>(), wrtVar);
		default:
			return std::nullopt;
	}
}

std::shared_ptr<mathEngine::expr> mathEngine::simplification::evaluateIntegrals(std::shared_ptr<expr> exp){
	auto retVal = exp->propegateDFS_replace([](std::shared_ptr<expr> exp)->std::optional<std::shared_ptr<expr>>{
		if(exp->isInstance<exprs::integral>()){
			auto der = exp->getAs<exprs::integral>();
			auto evaluated = evaluateIntegral(der->expression, der->wrtVar);
			if(evaluated){
				return *evaluated;
			}
		}
		return std::nullopt;
	});
	return retVal;
}

