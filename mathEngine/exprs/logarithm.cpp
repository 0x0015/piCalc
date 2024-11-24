#include "logarithm.hpp"
#include <cmath>
#include "../hashCombine.hpp"
#include "constant.hpp"

double mathEngine::exprs::logarithm::evalDouble() const{
	return std::log(inside->evalDouble()) / std::log(base->evalDouble()); //log base change.  We'll be using this a bit
}

void mathEngine::exprs::logarithm::propegateDFS(const std::function<void(std::shared_ptr<expr>)>& func){
	func(shared_from_this());
	inside->propegateDFS(func);
	base->propegateDFS(func);
}

void mathEngine::exprs::logarithm::propegateDFS_replace_internal(const expr::DFS_replacement_functype& func){
	auto inside_res = func(inside);
	auto base_res = func(base);

	if(inside_res)
		inside = *inside_res;
	else
		inside->propegateDFS_replace_internal(func);

	if(base_res)
		base = *base_res;
	else
		base->propegateDFS_replace_internal(func);
}

std::shared_ptr<mathEngine::expr> mathEngine::exprs::logarithm::propegateDFS_replace(const expr::DFS_replacement_functype& func){
	auto res = func(shared_from_this());
	if(res)
		return *res;
	propegateDFS_replace_internal(func);
	return shared_from_this();
}

std::string mathEngine::exprs::logarithm::toLatex() const{
	if(dynamic_cast<constant*>(base.get()) != nullptr){
		const auto& baseConst = std::dynamic_pointer_cast<constant>(base);
		if(std::holds_alternative<constantName>(baseConst->value.value) && std::get<constantName>(baseConst->value.value) == constantName::E){
			return "ln(" + inside->toLatex() + ")";
		}
	}
	return "log_{" + base->toLatex() + "}(" + inside->toLatex() + ")";
}

std::string mathEngine::exprs::logarithm::toCode(const std::unordered_set<std::string>& wrtVars) const{
	if(dynamic_cast<constant*>(base.get()) != nullptr){
		const auto& baseConst = std::dynamic_pointer_cast<constant>(base);
		if(std::holds_alternative<constantName>(baseConst->value.value) && std::get<constantName>(baseConst->value.value) == constantName::E){
			return naturalLogCodeFuncName + "(" + inside->toCode(wrtVars) + ")";
		}
	}
	return "(" + naturalLogCodeFuncName + "(" + inside->toCode(wrtVars) + ")/" + naturalLogCodeFuncName + "(" + base->toCode(wrtVars) + "))";
}

std::shared_ptr<mathEngine::expr> mathEngine::exprs::logarithm::clone() const{
	auto output = std::make_shared<logarithm>();
	output->base = base->clone();
	output->inside = inside->clone();
	return output;
}

std::size_t mathEngine::exprs::logarithm::hash() const{
	std::size_t lhsHash = base->hash();
	mathEngine::hash_combine(lhsHash, inside->hash(), COMPILE_TIME_CRC32_STR("logarithm"));
	return lhsHash;
}

std::string mathEngine::exprs::logarithm::getTypeString() const{
	return "log base(" + base->getTypeString() + ")(" + inside->getTypeString() + ")";
}

