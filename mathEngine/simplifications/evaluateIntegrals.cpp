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

template<typename T> bool isSubclass(std::shared_ptr<mathEngine::expr> exp){
	return dynamic_cast<T*>(exp.get()) != nullptr;
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
	if(isSubclass<mathEngine::exprs::variable>(exp->base) && isSubclass<mathEngine::exprs::constant>(exp->exp)){
		//ouch.  This is a mess
		auto var = std::dynamic_pointer_cast<mathEngine::exprs::variable>(exp->base);
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
	//not the most elegant, but I didn't want to put more baggage on the expr definitions themselves
	if(isSubclass<exprs::add>(der)){
		return getIntegralOf(std::dynamic_pointer_cast<exprs::add>(der), wrtVar);
	}else if(isSubclass<exprs::constant>(der)){
		return getIntegralOf(std::dynamic_pointer_cast<exprs::constant>(der), wrtVar);
	}else if(isSubclass<exprs::exponent>(der)){
		return getIntegralOf(std::dynamic_pointer_cast<exprs::exponent>(der), wrtVar);
	}else if(isSubclass<exprs::multiply>(der)){
		return getIntegralOf(std::dynamic_pointer_cast<exprs::multiply>(der), wrtVar);
	}else if(isSubclass<exprs::variable>(der)){
		return getIntegralOf(std::dynamic_pointer_cast<exprs::variable>(der), wrtVar);
	}else if(isSubclass<exprs::sine>(der)){
		return getIntegralOf(std::dynamic_pointer_cast<exprs::sine>(der), wrtVar);
	}else if(isSubclass<exprs::cosine>(der)){
		return getIntegralOf(std::dynamic_pointer_cast<exprs::cosine>(der), wrtVar);
	}else if(isSubclass<exprs::logarithm>(der)){
		return getIntegralOf(std::dynamic_pointer_cast<exprs::logarithm>(der), wrtVar);
	}else if(isSubclass<exprs::absoluteValue>(der)){
		return getIntegralOf(std::dynamic_pointer_cast<exprs::absoluteValue>(der), wrtVar);
	}else if(isSubclass<exprs::derivative>(der)){
		return getIntegralOf(std::dynamic_pointer_cast<exprs::derivative>(der), wrtVar);
	}
	return std::nullopt;
}

std::shared_ptr<mathEngine::expr> mathEngine::simplification::evaluateIntegrals(std::shared_ptr<expr> exp){
	auto retVal = exp->propegateDFS_replace([](std::shared_ptr<expr> exp)->std::optional<std::shared_ptr<expr>>{
		if(dynamic_cast<exprs::integral*>(exp.get()) != nullptr){
			auto der = std::dynamic_pointer_cast<exprs::integral>(exp);
			auto evaluated = evaluateIntegral(der->expression, der->wrtVar);
			if(evaluated){
				return *evaluated;
			}
		}
		return std::nullopt;
	});
	return retVal;
}

