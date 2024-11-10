#include "evaluateDerivatives.hpp"
#include "../exprs/add.hpp"
#include "../exprs/constant.hpp"
#include "../exprs/exponent.hpp"
#include "../exprs/multiply.hpp"
#include "../exprs/variable.hpp"
#include "../exprs/derivative.hpp"
#include "../exprs/sine.hpp"
#include "../exprs/cosine.hpp"
#include "../exprs/logarithm.hpp"

std::optional<std::shared_ptr<mathEngine::expr>> getDerivativeOf(std::shared_ptr<mathEngine::exprs::add> add, std::string_view wrtVar){
	auto output = std::make_shared<mathEngine::exprs::add>();
	for(const auto& term : add->terms){
		auto termDiff = mathEngine::simplification::evaluateDerivative(term, wrtVar);
		if(!termDiff)
			return std::nullopt;
		output->terms.push_back(*termDiff);
	}
	return output;
}

std::optional<std::shared_ptr<mathEngine::expr>> getDerivativeOf(std::shared_ptr<mathEngine::exprs::constant> constant, std::string_view wrtVar){
	auto output = std::make_shared<mathEngine::exprs::constant>();
	output->value.value = rational{0, 1};
	return output;
}

std::optional<std::shared_ptr<mathEngine::expr>> getDerivativeOf(std::shared_ptr<mathEngine::exprs::exponent> exp, std::string_view wrtVar){
	//Dx (f(x)^g(x)) = f(x)^(g(x) - 1) (g(x) f'(x) + f(x) ln(f(x)) g'(x))
	
	const auto& f = exp->base;
	const auto& g = exp->exp;

	auto output = std::make_shared<mathEngine::exprs::multiply>(); //whole thing
	auto term0 = std::make_shared<mathEngine::exprs::exponent>(); //f(x)^(g(x) - 1)
	auto term0_0 = std::make_shared<mathEngine::exprs::add>(); //g(x)-1
	auto term1 = std::make_shared<mathEngine::exprs::add>(); //g(x) f'(x) + f(x) log(f(x)) g'(x)
	auto term1_0 = std::make_shared<mathEngine::exprs::multiply>(); //g(x) f'(x)
	auto term1_0_0 = std::make_shared<mathEngine::exprs::derivative>(); //f'(x)
	auto term1_1 = std::make_shared<mathEngine::exprs::multiply>(); //f(x) ln(f(x)) g'(x)
	auto term1_1_0 = std::make_shared<mathEngine::exprs::logarithm>(); //ln(f(x))
	auto term1_1_1 = std::make_shared<mathEngine::exprs::derivative>(); //g'(x)

	auto minusOneConst = std::make_shared<mathEngine::exprs::constant>();
	minusOneConst->value = mathEngine::constVal{rational{-1, 1}};
	auto eConst = std::make_shared<mathEngine::exprs::constant>();
	eConst->value = mathEngine::constVal{mathEngine::constantName::E};

	output->terms = {term0, term1};

	term0->base = f;
	term0->exp = term0_0;
	term0_0->terms = {g, minusOneConst};

	term1->terms = {term1_0, term1_1};

	term1_0->terms = {g, term1_0_0};
	term1_0_0->expression = f;
	term1_0_0->wrtVar = wrtVar;

	term1_1->terms = {f, term1_1_0, term1_1_1};
	term1_1_0->base = eConst;
	term1_1_0->inside = f;
	term1_1_1->expression = g;
	term1_1_1->wrtVar = wrtVar;

	return output;
}

std::optional<std::shared_ptr<mathEngine::expr>> getDerivativeOf(std::shared_ptr<mathEngine::exprs::logarithm> log, std::string_view wrtVar){
	auto minusOneConst = std::make_shared<mathEngine::exprs::constant>();
	minusOneConst->value = mathEngine::constVal{rational{-1, 1}};

	//this NEEDS to be checked first, otherwise the below creates an infinite derivative expansion, as the ln derivatives don't get simplified before more are made
	if(dynamic_cast<mathEngine::exprs::constant*>(log->base.get()) != nullptr){
		const auto& logBaseConst = std::dynamic_pointer_cast<mathEngine::exprs::constant>(log->base);
		if(std::holds_alternative<mathEngine::constantName>(logBaseConst->value.value) && std::get<mathEngine::constantName>(logBaseConst->value.value) == mathEngine::constantName::E){
			//it's a natural log!  Dx ln(f(x)) = (Dx f(x))/f(x)
			auto output = std::make_shared<mathEngine::exprs::multiply>();
			
			auto rhs = std::make_shared<mathEngine::exprs::derivative>();
			rhs->expression = log->inside;
			rhs->wrtVar = wrtVar;

			auto lhs = std::make_shared<mathEngine::exprs::exponent>();
			lhs->exp = minusOneConst;
			lhs->base = log->inside;

			output->terms = {rhs, lhs};

			return output;
		}
	}

	//Dx log_{g(x)} (f(x)) = Dx ln(f(x))/ln(g(x))
	auto eConst = std::make_shared<mathEngine::exprs::constant>();
	eConst->value = mathEngine::constVal{mathEngine::constantName::E};

	auto div = std::make_shared<mathEngine::exprs::multiply>(); //ln(f(x))*ln(g(x))^-1
	auto divLhsExp = std::make_shared<mathEngine::exprs::exponent>(); //ln(g(x))^-1
	auto divLhs = std::make_shared<mathEngine::exprs::logarithm>(); //ln(g(x))
	divLhs->base = eConst;
	divLhs->inside = log->base;
	divLhsExp->exp = minusOneConst;
	divLhsExp->base = divLhs;

	auto divRhs = std::make_shared<mathEngine::exprs::logarithm>(); //ln(f(x))
	divRhs->base = eConst;
	divRhs->inside = log->inside;

	div->terms = {divRhs, divLhsExp};

	auto output = std::make_shared<mathEngine::exprs::derivative>();
	output->expression = div;
	output->wrtVar = wrtVar;

	return output;
}

std::optional<std::shared_ptr<mathEngine::expr>> getDerivativeOf(std::shared_ptr<mathEngine::exprs::multiply> mul, std::string_view wrtVar){
	//f'g + g'f but repeated until done
	if(mul->terms.size() == 1)
		return mathEngine::simplification::evaluateDerivative(mul->terms.front(), wrtVar);

	auto sum = std::make_shared<mathEngine::exprs::add>();

	auto fPrimeg = std::make_shared<mathEngine::exprs::multiply>();
	auto gPrimef = std::make_shared<mathEngine::exprs::multiply>();

	const auto& firstTerm = mul->terms.front();
	auto otherTerms = std::make_shared<mathEngine::exprs::multiply>();
	for(unsigned int i=1;i<mul->terms.size();i++)
		otherTerms->terms.push_back(mul->terms[i]);

	auto fTPrime = std::make_shared<mathEngine::exprs::derivative>();
	fTPrime->expression = firstTerm;
	fTPrime->wrtVar = wrtVar;

	auto oTPrime = std::make_shared<mathEngine::exprs::derivative>();
	oTPrime->expression = otherTerms;
	oTPrime->wrtVar = wrtVar;

	fPrimeg->terms = {fTPrime, otherTerms};
	gPrimef->terms = {firstTerm, oTPrime};
	
	sum->terms = {fPrimeg, gPrimef};
	return sum;
	//this is good enough, because in the next pass, the newly generated derivatives will be evaluated
}

std::optional<std::shared_ptr<mathEngine::expr>> getDerivativeOf(std::shared_ptr<mathEngine::exprs::sine> sine, std::string_view wrtVar){
	auto output = std::make_shared<mathEngine::exprs::multiply>();
	auto cosine = std::make_shared<mathEngine::exprs::cosine>();
	cosine->inside = sine->inside;
	auto insidePrime = std::make_shared<mathEngine::exprs::derivative>();
	insidePrime->expression = sine->inside;
	insidePrime->wrtVar = wrtVar;
	output->terms = {cosine, insidePrime};
	return output;
}

std::optional<std::shared_ptr<mathEngine::expr>> getDerivativeOf(std::shared_ptr<mathEngine::exprs::cosine> cosine, std::string_view wrtVar){
	auto output = std::make_shared<mathEngine::exprs::multiply>();
	auto sine = std::make_shared<mathEngine::exprs::sine>();
	sine->inside = cosine->inside;
	auto insidePrime = std::make_shared<mathEngine::exprs::derivative>();
	insidePrime->expression = cosine->inside;
	insidePrime->wrtVar = wrtVar;
	auto minus1Const = std::make_shared<mathEngine::exprs::constant>();
	minus1Const->value = mathEngine::constVal{rational{-1, 1}};
	output->terms = {minus1Const, sine, insidePrime};
	return output;
}

std::optional<std::shared_ptr<mathEngine::expr>> getDerivativeOf(std::shared_ptr<mathEngine::exprs::variable> var, std::string_view wrtVar){
	auto output = std::make_shared<mathEngine::exprs::constant>();
	if(var->name == wrtVar)
		output->value.value = rational{1, 1};
	else
		output->value.value = rational{0, 1};//for now assume all other variables are constant wrt the given variable
	return output;
}

template<typename T> bool isSubclass(std::shared_ptr<mathEngine::expr> exp){
	return dynamic_cast<T*>(exp.get()) != nullptr;
}

std::optional<std::shared_ptr<mathEngine::expr>> mathEngine::simplification::evaluateDerivative(std::shared_ptr<expr> der, std::string_view wrtVar){
	//not the most elegant, but I didn't want to put more baggage on the expr definitions themselves
	if(isSubclass<exprs::add>(der)){
		return getDerivativeOf(std::dynamic_pointer_cast<exprs::add>(der), wrtVar);
	}else if(isSubclass<exprs::constant>(der)){
		return getDerivativeOf(std::dynamic_pointer_cast<exprs::constant>(der), wrtVar);
	}else if(isSubclass<exprs::exponent>(der)){
		return getDerivativeOf(std::dynamic_pointer_cast<exprs::exponent>(der), wrtVar);
	}else if(isSubclass<exprs::multiply>(der)){
		return getDerivativeOf(std::dynamic_pointer_cast<exprs::multiply>(der), wrtVar);
	}else if(isSubclass<exprs::variable>(der)){
		return getDerivativeOf(std::dynamic_pointer_cast<exprs::variable>(der), wrtVar);
	}else if(isSubclass<exprs::sine>(der)){
		return getDerivativeOf(std::dynamic_pointer_cast<exprs::sine>(der), wrtVar);
	}else if(isSubclass<exprs::cosine>(der)){
		return getDerivativeOf(std::dynamic_pointer_cast<exprs::cosine>(der), wrtVar);
	}else if(isSubclass<exprs::logarithm>(der)){
		return getDerivativeOf(std::dynamic_pointer_cast<exprs::logarithm>(der), wrtVar);
	}
	return std::nullopt;
}

std::shared_ptr<mathEngine::expr> mathEngine::simplification::evaluateDerivatives(std::shared_ptr<expr> exp){
	auto retVal = exp->propegateDFS_replace([](std::shared_ptr<expr> exp)->std::optional<std::shared_ptr<expr>>{
		if(dynamic_cast<exprs::derivative*>(exp.get()) != nullptr){
			auto der = std::dynamic_pointer_cast<exprs::derivative>(exp);
			auto evaluated = evaluateDerivative(der->expression, der->wrtVar);
			if(evaluated){
				return *evaluated;
			}
		}
		return std::nullopt;
	});
	return retVal;
}

