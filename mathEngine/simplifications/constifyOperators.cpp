#include "constifyOperators.hpp"
#include "../expr.hpp"
#include "../exprs/constant.hpp"
#include "../exprs/add.hpp"
#include "../exprs/multiply.hpp"
#include "../exprs/exponent.hpp"

std::shared_ptr<mathEngine::expr> mathEngine::simplification::constifyOperators(std::shared_ptr<expr> exp){
	//adds
	auto addRes = exp->propegateDFS_replace([](std::shared_ptr<expr> exp)->std::optional<std::shared_ptr<expr>>{
		if(dynamic_cast<exprs::add*>(exp.get()) != nullptr){
			auto add = std::dynamic_pointer_cast<exprs::add>(exp);
			for(const auto& term : add->terms){
				if(dynamic_cast<exprs::constant*>(term.get()) == nullptr)
					return std::nullopt;
			}
			//all const
			auto output = std::make_shared<exprs::constant>();
			output->value = constVal{add};
			return output;
		}
		return std::nullopt;
	}, false);

	//muls
	auto mulRes = addRes->propegateDFS_replace([](std::shared_ptr<expr> exp)->std::optional<std::shared_ptr<expr>>{
		if(dynamic_cast<exprs::multiply*>(exp.get()) != nullptr){
			auto mul = std::dynamic_pointer_cast<exprs::multiply>(exp);
			for(const auto& term : mul->terms){
				if(dynamic_cast<exprs::constant*>(term.get()) == nullptr)
					return std::nullopt;
			}
			//all const
			auto output = std::make_shared<exprs::constant>();
			output->value = constVal{mul};
			return output;
		}
		return std::nullopt;
	}, false);

	//exponent
	auto expRes = mulRes->propegateDFS_replace([](std::shared_ptr<expr> exp)->std::optional<std::shared_ptr<expr>>{
		if(dynamic_cast<exprs::exponent*>(exp.get()) != nullptr){
			auto expo = std::dynamic_pointer_cast<exprs::exponent>(exp);
			if(!(dynamic_cast<exprs::constant*>(expo->base.get()) == nullptr && dynamic_cast<exprs::constant*>(expo->exp.get()) == nullptr))
				return std::nullopt;
			//all const
			auto output = std::make_shared<exprs::constant>();
			output->value = constVal{expo};
			return output;
		}
		return std::nullopt;
	}, false);

	return expRes;
}

std::shared_ptr<mathEngine::expr> mathEngine::simplification::unconstifyOverconstedOperators(std::shared_ptr<expr> exp){
	auto unconsted = exp->propegateDFS_replace([](std::shared_ptr<expr> exp)->std::optional<std::shared_ptr<expr>>{
		if(dynamic_cast<exprs::constant*>(exp.get()) != nullptr){
			auto constant = std::dynamic_pointer_cast<exprs::constant>(exp);
			if(std::holds_alternative<std::shared_ptr<const expr>>(constant->value.value)){
				const auto& subConst = std::get<std::shared_ptr<const expr>>(constant->value.value);
				return std::const_pointer_cast<expr>(subConst);
			}
		}
		return std::nullopt;
	}, true);

	return unconsted;
}
