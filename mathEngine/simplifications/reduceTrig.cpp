#include "reduceTrig.hpp"
#include "../exprs/add.hpp"
#include "../exprs/constant.hpp"
#include "../exprs/exponent.hpp"
#include "../exprs/multiply.hpp"
#include "../exprs/variable.hpp"
#include "../exprs/sine.hpp"
#include "../exprs/cosine.hpp"


std::optional<std::shared_ptr<mathEngine::expr>> mathEngine::simplification::simplifySin(std::shared_ptr<expr> exp){
	if(dynamic_cast<exprs::constant*>(exp.get()) != nullptr){
		const auto& constExp = std::dynamic_pointer_cast<exprs::constant>(exp);
		if(std::holds_alternative<std::shared_ptr<const expr>>(constExp->value.value)){
			const auto& cE_exp = std::get<std::shared_ptr<const expr>>(constExp->value.value);
			if(dynamic_cast<const exprs::add*>(cE_exp.get()) != nullptr){
				const auto& cE_add = std::dynamic_pointer_cast<const exprs::add>(cE_exp);
				//check if there are two terms which are exactly a rational, and pi -> a rational multiple of pi -> simplifiable
			}
		}
	}

	return std::nullopt;
}

std::optional<std::shared_ptr<mathEngine::expr>> mathEngine::simplification::simplifyCos(std::shared_ptr<expr> exp){
	return std::nullopt;
}

std::shared_ptr<mathEngine::expr> mathEngine::simplification::reduceTrig(std::shared_ptr<expr> exp){
	auto retVal = exp->propegateDFS_replace([](std::shared_ptr<expr> exp)->std::optional<std::shared_ptr<expr>>{
		if(dynamic_cast<exprs::sine*>(exp.get()) != nullptr){
			auto der = std::dynamic_pointer_cast<exprs::sine>(exp);
			auto evaluated = simplifySin(der->inside);
			if(evaluated){
				return *evaluated;
			}
		}
		if(dynamic_cast<exprs::cosine*>(exp.get()) != nullptr){
			auto der = std::dynamic_pointer_cast<exprs::cosine>(exp);
			auto evaluated = simplifyCos(der->inside);
			if(evaluated){
				return *evaluated;
			}
		}
		return std::nullopt;
	}, true);
	return retVal;
}

