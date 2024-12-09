#include "exponent.hpp"
#include <cmath>
#include "../hashCombine.hpp"

mathEngine::exprs::exponent::exponent(){
	type = typeID;
}

double mathEngine::exprs::exponent::evalDouble() const{
	return std::pow(base->evalDouble(), exp->evalDouble());
}

void mathEngine::exprs::exponent::propegateDFS(const std::function<void(std::shared_ptr<expr>)>& func){
	func(shared_from_this());
	base->propegateDFS(func);
	exp->propegateDFS(func);
}

void mathEngine::exprs::exponent::propegateDFS_replace_internal(const expr::DFS_replacement_functype& func){
	auto base_res = func(base);
	auto exp_res = func(exp);

	if(base_res)
		base = *base_res;
	else
		base->propegateDFS_replace_internal(func);

	if(exp_res)
		exp = *exp_res;
	else
		exp->propegateDFS_replace_internal(func);
}

std::shared_ptr<mathEngine::expr> mathEngine::exprs::exponent::propegateDFS_replace(const expr::DFS_replacement_functype& func){
	auto res = func(shared_from_this());
	if(res)
		return *res;
	propegateDFS_replace_internal(func);
	return shared_from_this();
}

std::string mathEngine::exprs::exponent::toLatex() const{
	return "{" + base->toLatex() + "}^{" + exp->toLatex() + "}";
}

std::string mathEngine::exprs::exponent::toCode(const std::unordered_set<std::string>& wrtVars) const{
	return exponentCodeFuncName + "(" + base->toCode(wrtVars) + ", " + exp->toCode(wrtVars) + ")";
}

std::shared_ptr<mathEngine::expr> mathEngine::exprs::exponent::clone() const{
	auto output = std::make_shared<exponent>();
	output->base = base->clone();
	output->exp = exp->clone();
	return output;
}

std::size_t mathEngine::exprs::exponent::hash() const{
	return mathEngine::hashValues(base->hash(), exp->hash(), typeID);
}

std::size_t mathEngine::exprs::exponent::hashTypeSig(bool allConstSame, std::optional<std::string_view> constWrtVar) const{
	if(allConstSame && isConst(constWrtVar)){
		return COMPILE_TIME_CRC32_STR("constantExpression");
	}else{
		return mathEngine::hashValues(typeID, base->hashTypeSig(allConstSame, constWrtVar), exp->hashTypeSig(allConstSame, constWrtVar));
	}
}

std::string mathEngine::exprs::exponent::getTypeString() const{
	return "(" + base->getTypeString() + ")^(" + exp->getTypeString() + ")";
}

bool mathEngine::exprs::exponent::isConst(std::optional<std::string_view> wrtVar) const{
	return base->isConst(wrtVar) && exp->isConst(wrtVar);
}

bool mathEngine::exprs::exponent::isEqual(const expr* other) const{
	if(type != other->type)
		return false;
	const auto& otherExp = other->getAs<const exponent>();
	return base->isEqual(otherExp->base.get()) && exp->isEqual(otherExp->exp.get());
}
