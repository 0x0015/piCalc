#include "reduceSimpleExponents.hpp"
#include "../exprs/constant.hpp"
#include "../exprs/exponent.hpp"

int64_t ipow(int64_t base, int64_t exp){
	int result = 1;
	for(;;)
	{
		if(exp & 1)
			result *= base;
		exp >>= 1;
		if(!exp)
			break;
		base *= base;
	}

	return result;
}

std::shared_ptr<mathEngine::expr> mathEngine::simplification::reduceSimpleExponents(std::shared_ptr<expr> exp){
	auto constEvaluated = exp->propegateDFS_replace([](std::shared_ptr<expr> exp)->std::optional<std::shared_ptr<expr>>{
		if(exp->isInstance<exprs::exponent>()){
			auto expnt = exp->getAs<exprs::exponent>();
			if(expnt->base->isInstance<exprs::constant>() && expnt->exp->isInstance<exprs::constant>()){
				auto base = expnt->base->getAs<exprs::constant>();
				auto pow = expnt->exp->getAs<exprs::constant>();
				if(std::holds_alternative<rational>(base->value.value) && std::holds_alternative<rational>(pow->value.value) && std::get<rational>(pow->value.value).denom == 1){
					auto baseRat = std::get<rational>(base->value.value);
					auto powInt = std::get<rational>(pow->value.value).num;
					if(powInt < 0){
						powInt = std::abs(powInt);
						std::swap(baseRat.denom, baseRat.num);
					}
					baseRat.num = ipow(baseRat.num, powInt);
					baseRat.denom = ipow(baseRat.denom, powInt);
					baseRat.reduce();
					
					auto output = std::make_shared<exprs::constant>();
					output->value.value = baseRat;
					return output;
				}
			}
		}
		return std::nullopt;
	});
	return constEvaluated;
}

