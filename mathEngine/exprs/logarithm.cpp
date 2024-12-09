#include "logarithm.hpp"
#include <cmath>
#include "../hashCombine.hpp"
#include "constant.hpp"

mathEngine::exprs::logarithm::logarithm(){
	type = typeID;
}

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
	if(base->isInstance<constant>()){
		const auto& baseConst = base->getAs<constant>();
		if(std::holds_alternative<constantName>(baseConst->value.value) && std::get<constantName>(baseConst->value.value) == constantName::E){
			return "ln(" + inside->toLatex() + ")";
		}
	}
	return "log_{" + base->toLatex() + "}(" + inside->toLatex() + ")";
}

std::string mathEngine::exprs::logarithm::toCode(const std::unordered_set<std::string>& wrtVars) const{
	if(base->isInstance<constant>()){
		const auto& baseConst = base->getAs<constant>();
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
	return mathEngine::hashValues(base->hash(), inside->hash(), typeID);
}

std::size_t mathEngine::exprs::logarithm::hashTypeSig(bool allConstSame, std::optional<std::string_view> constWrtVar) const{
	if(allConstSame && isConst(constWrtVar)){
		return COMPILE_TIME_CRC32_STR("constantExpression");
	}else{
		return mathEngine::hashValues(typeID, base->hashTypeSig(allConstSame, constWrtVar), inside->hashTypeSig(allConstSame, constWrtVar));
	}
}

std::string mathEngine::exprs::logarithm::getTypeString() const{
	return "log base(" + base->getTypeString() + ")(" + inside->getTypeString() + ")";
}

bool mathEngine::exprs::logarithm::isConst(std::optional<std::string_view> wrtVar) const{
	return base->isConst(wrtVar) && inside->isConst(wrtVar);
}

bool mathEngine::exprs::logarithm::isEqual(const expr* other) const{
	if(type != other->type)
		return false;
	const auto& otherLog = other->getAs<const logarithm>();
	return base->isEqual(otherLog->base.get()) && inside->isEqual(otherLog->inside.get());
}
