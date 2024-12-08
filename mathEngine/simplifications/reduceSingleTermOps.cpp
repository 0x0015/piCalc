#include "reduceSingleTermOps.hpp"
#include "../expr.hpp"
#include "../exprs/constant.hpp"
#include "../exprs/add.hpp"
#include "../exprs/multiply.hpp"

std::shared_ptr<mathEngine::expr> mathEngine::simplification::reduceSingleTermOps(std::shared_ptr<expr> exp){
	//adds
	auto retVal = exp->propegateDFS_replace([](std::shared_ptr<expr> exp)->std::optional<std::shared_ptr<expr>>{
		if(exp->isInstance<exprs::add>()){
			auto add = exp->getAs<exprs::add>();
			if(add->terms.size() == 1){
				return add->terms.front();
			}
		}
		if(exp->isInstance<exprs::multiply>()){
			auto mul = exp->getAs<exprs::multiply>();
			if(mul->terms.size() == 1){
				return mul->terms.front();
			}
		}
		return std::nullopt;
	});

	return retVal;
}

