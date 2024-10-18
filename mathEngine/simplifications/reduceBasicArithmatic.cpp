#include "reduceBasicArithmatic.hpp"
#include "../expr.hpp"
#include "../exprs/constant.hpp"
#include "../exprs/add.hpp"
#include "../exprs/multiply.hpp"

std::shared_ptr<mathEngine::expr> mathEngine::simplification::reduceBasicArithmatic(std::shared_ptr<expr> exp){
	//adds
	auto addRes = exp->propegateDFS_replace([](std::shared_ptr<expr> exp)->std::optional<std::shared_ptr<expr>>{
		if(dynamic_cast<exprs::add*>(exp.get()) != nullptr){
			auto add = std::dynamic_pointer_cast<exprs::add>(exp);
			if(dynamic_cast<exprs::constant*>(add->lhs.get()) != nullptr && dynamic_cast<exprs::constant*>(add->rhs.get()) != nullptr){
				auto lhs_const = std::dynamic_pointer_cast<exprs::constant>(add->lhs);
				auto rhs_const = std::dynamic_pointer_cast<exprs::constant>(add->rhs);
				if(std::holds_alternative<rational>(lhs_const->value.value) && std::holds_alternative<rational>(rhs_const->value.value)){
					auto sum = std::get<rational>(lhs_const->value.value) + std::get<rational>(rhs_const->value.value);
					auto result = std::make_shared<exprs::constant>();
					result->value.value = sum;
					return result;
				}
			}
		}
		return std::nullopt;
	}, true);
	return addRes;
}

