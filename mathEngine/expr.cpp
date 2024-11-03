#include "expr.hpp"
#include "exprs/variable.hpp"

std::shared_ptr<mathEngine::expr> mathEngine::expr::substiteVariable(const std::string& varName, std::shared_ptr<expr> subVal){
	return propegateDFS_replace([&](std::shared_ptr<expr> exp)->std::optional<std::shared_ptr<expr>>{
		if(dynamic_cast<exprs::variable*>(exp.get()) != nullptr){
			auto var = std::dynamic_pointer_cast<exprs::variable>(exp);
			if(var->name == varName)
				return subVal;
		}
		return std::nullopt;
	});
}
