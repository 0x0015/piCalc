#include "mergeCommutativeOperators.hpp"
#include "../expr.hpp"
#include "../exprs/constant.hpp"
#include "../exprs/add.hpp"
#include "../exprs/multiply.hpp"

std::shared_ptr<mathEngine::expr> mathEngine::simplification::mergeCommutativeOperators(std::shared_ptr<expr> exp){
	//adds
	exp->propegateDFS([](std::shared_ptr<expr> exp){
		if(exp->isInstance<exprs::add>()){
			auto add = exp->getAs<exprs::add>();
			std::vector<std::shared_ptr<expr>> newTerms;
			for(const auto& term : add->terms){
				if(term->isInstance<exprs::add>()){
					auto term_add = term->getAs<exprs::add>();
					for(const auto& subterm : term_add->terms)
						newTerms.push_back(subterm);
				}else
					newTerms.push_back(term);
			}
			add->terms = newTerms;
		}
	});

	//muls
	exp->propegateDFS([](std::shared_ptr<expr> exp){
		if(exp->isInstance<exprs::multiply>()){
			auto mult = exp->getAs<exprs::multiply>();
			std::vector<std::shared_ptr<expr>> newTerms;
			for(const auto& term : mult->terms){
				if(term->isInstance<exprs::multiply>()){
					auto term_mul = term->getAs<exprs::multiply>();
					for(const auto& subterm : term_mul->terms)
						newTerms.push_back(subterm);
				}else
					newTerms.push_back(term);
			}
			mult->terms = newTerms;
		}
	});
	return exp;
}

