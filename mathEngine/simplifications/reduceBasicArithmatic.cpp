#include "reduceBasicArithmatic.hpp"
#include "../expr.hpp"
#include "../exprs/constant.hpp"
#include "../exprs/add.hpp"
#include "../exprs/multiply.hpp"

std::shared_ptr<mathEngine::expr> mathEngine::simplification::reduceBasicArithmatic(std::shared_ptr<expr> exp){
	//adds
	exp->propegateDFS([](std::shared_ptr<expr> exp){
		if(dynamic_cast<exprs::add*>(exp.get()) != nullptr){
			auto add = std::dynamic_pointer_cast<exprs::add>(exp);
			std::optional<rational> rationalSum;
			std::optional<double> realSum;
			std::vector<std::shared_ptr<expr>> otherTerms;
			for(const auto& term : add->terms){
				if(dynamic_cast<exprs::constant*>(term.get()) != nullptr){
					auto term_const = std::dynamic_pointer_cast<exprs::constant>(term);
					if(std::holds_alternative<rational>(term_const->value.value)){
						const auto& term_rat = std::get<rational>(term_const->value.value);
						if(!rationalSum)
							rationalSum = term_rat;
						else rationalSum = *rationalSum + term_rat;
					}
					if(std::holds_alternative<double>(term_const->value.value)){
						const auto& term_real = std::get<double>(term_const->value.value);
						if(!realSum)
							realSum = term_real;
						else realSum = *realSum + term_real;
					}
				}else
					otherTerms.push_back(term);
			}
			add->terms = otherTerms;
			if(rationalSum){
				auto rationalTerm = std::make_shared<exprs::constant>();
				rationalTerm->value.value = *rationalSum;
				add->terms.push_back(rationalTerm);
			}
			if(realSum){
				auto realTerm = std::make_shared<exprs::constant>();
				realTerm->value.value = *realSum;
				add->terms.push_back(realTerm);
			}
		}
	}, true);

	//muls
	exp->propegateDFS([](std::shared_ptr<expr> exp){
		if(dynamic_cast<exprs::multiply*>(exp.get()) != nullptr){
			auto mul = std::dynamic_pointer_cast<exprs::multiply>(exp);
			std::optional<rational> rationalProduct;
			std::optional<double> realProduct;
			std::vector<std::shared_ptr<expr>> otherTerms;
			for(const auto& term : mul->terms){
				if(dynamic_cast<exprs::constant*>(term.get()) != nullptr){
					auto term_const = std::dynamic_pointer_cast<exprs::constant>(term);
					if(std::holds_alternative<rational>(term_const->value.value)){
						const auto& term_rat = std::get<rational>(term_const->value.value);
						if(!rationalProduct)
							rationalProduct = term_rat;
						else rationalProduct = *rationalProduct * term_rat;
					}
					if(std::holds_alternative<double>(term_const->value.value)){
						const auto& term_real = std::get<double>(term_const->value.value);
						if(!realProduct)
							realProduct = term_real;
						else realProduct = *realProduct * term_real;
					}
				}else
					otherTerms.push_back(term);
			}
			mul->terms = otherTerms;
			if(rationalProduct){
				auto rationalTerm = std::make_shared<exprs::constant>();
				rationalTerm->value.value = *rationalProduct;
				mul->terms.push_back(rationalTerm);
			}
			if(realProduct){
				auto realTerm = std::make_shared<exprs::constant>();
				realTerm->value.value = *realProduct;
				mul->terms.push_back(realTerm);
			}
		}
	}, true);
	return exp;
}

