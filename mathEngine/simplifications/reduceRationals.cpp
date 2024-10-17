#include "reduceRationals.hpp"
#include "../expr.hpp"
#include "../exprs/constant.hpp"

std::shared_ptr<mathEngine::expr> mathEngine::simplification::reduceRationals(std::shared_ptr<expr> exp){
	exp->propegateDFS([](std::shared_ptr<expr> exp){
		if(dynamic_cast<exprs::constant*>(exp.get()) != nullptr){
			auto con = std::dynamic_pointer_cast<exprs::constant>(exp);
			if(std::holds_alternative<rational>(con->value.value)){
				auto& rat = std::get<rational>(con->value.value);
				int commonDenom = std::gcd(rat.num, rat.denom);
				rat.num /= commonDenom;
				rat.denom /= commonDenom;
			}
		}
	}, true);
	return exp;
}

