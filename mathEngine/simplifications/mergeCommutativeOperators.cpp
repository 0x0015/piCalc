#include "mergeCommutativeOperators.hpp"
#include "../expr.hpp"
#include "../exprs/constant.hpp"
#include "../exprs/add.hpp"
#include "../exprs/multiply.hpp"

std::shared_ptr<mathEngine::expr> mathEngine::simplification::mergeCommutativeOperators(std::shared_ptr<expr> exp){
	//adds
	exp->propegateDFS([](std::shared_ptr<expr> exp){
		if(dynamic_cast<exprs::add*>(exp.get()) != nullptr){
			auto add = std::dynamic_pointer_cast<exprs::add>(exp);
			std::vector<std::shared_ptr<expr>> newTerms;
			for(const auto& term : add->terms){
				if(dynamic_cast<exprs::add*>(term.get()) != nullptr){
					auto term_add = std::dynamic_pointer_cast<exprs::add>(term);
					for(const auto& subterm : term_add->terms)
						newTerms.push_back(subterm);
				}else
					newTerms.push_back(term);
			}
			add->terms = newTerms;
		}
	}, true);

	//muls
	exp->propegateDFS([](std::shared_ptr<expr> exp){
		if(dynamic_cast<exprs::multiply*>(exp.get()) != nullptr){
			auto mult = std::dynamic_pointer_cast<exprs::multiply>(exp);
			std::vector<std::shared_ptr<expr>> newTerms;
			for(const auto& term : mult->terms){
				if(dynamic_cast<exprs::multiply*>(term.get()) != nullptr){
					auto term_mul = std::dynamic_pointer_cast<exprs::multiply>(term);
					for(const auto& subterm : term_mul->terms)
						newTerms.push_back(subterm);
				}else
					newTerms.push_back(term);
			}
			mult->terms = newTerms;
		}
	}, true);
	return exp;
}

