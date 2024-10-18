#include "reduceSingleTermOps.hpp"
#include "../expr.hpp"
#include "../exprs/constant.hpp"
#include "../exprs/add.hpp"
#include "../exprs/multiply.hpp"

std::shared_ptr<mathEngine::expr> mathEngine::simplification::reduceSingleTermOps(std::shared_ptr<expr> exp){
	//adds
	auto retVal = exp->propegateDFS_replace([](std::shared_ptr<expr> exp)->std::optional<std::shared_ptr<expr>>{
		if(dynamic_cast<exprs::add*>(exp.get()) != nullptr){
			auto add = std::dynamic_pointer_cast<exprs::add>(exp);
			if(add->terms.size() == 1){
				return add->terms.front();
			}
		}
		if(dynamic_cast<exprs::multiply*>(exp.get()) != nullptr){
			auto mul = std::dynamic_pointer_cast<exprs::multiply>(exp);
			if(mul->terms.size() == 1){
				return mul->terms.front();
			}
		}
		return std::nullopt;
	}, true);

	return retVal;
}

