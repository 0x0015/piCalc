#include "reduceBasicArithmatic.hpp"
#include "../expr.hpp"
#include "../exprs/constant.hpp"
#include "../exprs/add.hpp"
#include "../exprs/multiply.hpp"

std::shared_ptr<mathEngine::expr> mathEngine::simplification::reduceBasicArithmatic(std::shared_ptr<expr> exp){
	//adds
	exp->propegateDFS([](std::shared_ptr<expr> exp){
		if(exp->isInstance<exprs::add>()){
			auto add = exp->getAs<exprs::add>();
			std::optional<rational> rationalSum;
			std::optional<double> realSum;
			std::vector<std::shared_ptr<expr>> otherTerms;
			for(const auto& term : add->terms){
				if(term->isInstance<exprs::constant>()){
					auto term_const = term->getAs<exprs::constant>();
					if(std::holds_alternative<rational>(term_const->value.value)){
						const auto& term_rat = std::get<rational>(term_const->value.value);
						if(!rationalSum)
							rationalSum = term_rat;
						else rationalSum = *rationalSum + term_rat;
					}else if(std::holds_alternative<double>(term_const->value.value)){
						const auto& term_real = std::get<double>(term_const->value.value);
						if(!realSum)
							realSum = term_real;
						else realSum = *realSum + term_real;
					}else	
						otherTerms.push_back(term);
				}else
					otherTerms.push_back(term);
			}
			add->terms = otherTerms;
			if(rationalSum && *rationalSum != rational{0, 1}){
				auto rationalTerm = std::make_shared<exprs::constant>();
				rationalTerm->value.value = *rationalSum;
				add->terms.push_back(rationalTerm);
			}
			if(realSum && *realSum != 0.0){
				auto realTerm = std::make_shared<exprs::constant>();
				realTerm->value.value = *realSum;
				add->terms.push_back(realTerm);
			}
		}
	});

	//muls
	exp->propegateDFS([](std::shared_ptr<expr> exp){
		if(exp->isInstance<exprs::multiply>()){
			auto mul = exp->getAs<exprs::multiply>();
			std::optional<rational> rationalProduct;
			std::optional<double> realProduct;
			std::vector<std::shared_ptr<expr>> otherTerms;
			for(const auto& term : mul->terms){
				if(term->isInstance<exprs::constant>()){
					auto term_const = term->getAs<exprs::constant>();
					if(std::holds_alternative<rational>(term_const->value.value)){
						const auto& term_rat = std::get<rational>(term_const->value.value);
						if(!rationalProduct)
							rationalProduct = term_rat;
						else rationalProduct = *rationalProduct * term_rat;
					}else if(std::holds_alternative<double>(term_const->value.value)){
						const auto& term_real = std::get<double>(term_const->value.value);
						if(!realProduct)
							realProduct = term_real;
						else realProduct = *realProduct * term_real;
					}else
						otherTerms.push_back(term);
				}else
					otherTerms.push_back(term);
			}
			mul->terms = otherTerms;
			if(rationalProduct && *rationalProduct != rational{1,1}){
				auto rationalTerm = std::make_shared<exprs::constant>();
				rationalTerm->value.value = *rationalProduct;
				mul->terms.push_back(rationalTerm);
			}
			if(realProduct && *realProduct != 1.0){
				auto realTerm = std::make_shared<exprs::constant>();
				realTerm->value.value = *realProduct;
				mul->terms.push_back(realTerm);
			}
		}
	});

	//multiplications by 0
	auto retVal = exp->propegateDFS_replace([](std::shared_ptr<expr> exp)->std::optional<std::shared_ptr<expr>>{	
		if(exp->isInstance<exprs::multiply>()){
			auto mul = exp->getAs<exprs::multiply>();
			for(const auto& term : mul->terms){
				if(term->isInstance<exprs::constant>()){	
					auto term_const = term->getAs<exprs::constant>();
					if(std::holds_alternative<rational>(term_const->value.value)){
						const auto& term_rat = std::get<rational>(term_const->value.value);
						if(term_rat == rational{0, 1}){
							auto zeroConst = std::make_shared<exprs::constant>();
							zeroConst->value.value = rational{0, 1};
							return zeroConst;
						}
					}
				}
			}
		}
		return std::nullopt;
	});
	return retVal;
}

