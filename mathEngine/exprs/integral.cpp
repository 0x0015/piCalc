#include "integral.hpp"
#include "../hashCombine.hpp"
#include "variable.hpp"
#include "../simplifications/evaluateIntegrals.hpp"

mathEngine::exprs::integral::integral(){
	type = typeID;
}

double mathEngine::exprs::integral::evalDouble() const{
	return -1;
}

void mathEngine::exprs::integral::propegateDFS(const std::function<void(std::shared_ptr<expr>)>& func){
	func(shared_from_this());
	expression->propegateDFS(func);
}

void mathEngine::exprs::integral::propegateDFS_replace_internal(const expr::DFS_replacement_functype& func){
	auto res = func(expression);
	if(res)
		expression = *res;
	else
		expression->propegateDFS_replace_internal(func);
}

std::shared_ptr<mathEngine::expr> mathEngine::exprs::integral::propegateDFS_replace(const expr::DFS_replacement_functype& func){
	auto val = func(shared_from_this());
	if(val)
		return *val;
	propegateDFS_replace_internal(func);
	return shared_from_this();
}


std::string mathEngine::exprs::integral::toLatex() const{
	return "\\int \\left(" + expression->toLatex() + "\\right) dx";
}

std::string mathEngine::exprs::integral::toCode(const std::unordered_set<std::string>& wrtVars) const{
	return "0.0";//fill in later
}

std::shared_ptr<mathEngine::expr> mathEngine::exprs::integral::clone() const{
	auto output = std::make_shared<integral>();
	output->expression = expression->clone();
	output->wrtVar = wrtVar;
	return output;
}

std::size_t mathEngine::exprs::integral::hash() const{
	return mathEngine::hashValues(expression->hash(), wrtVar, typeID);
}

std::size_t mathEngine::exprs::integral::hashTypeSig(bool allConstSame, std::optional<std::string_view> constWrtVar) const{
	if(allConstSame && isConst(constWrtVar)){
		return COMPILE_TIME_CRC32_STR("constantExpression");
	}else{
		return mathEngine::hashValues(typeID, expression->hashTypeSig(allConstSame, constWrtVar));
	}
}

std::string mathEngine::exprs::integral::getTypeString() const{
	return "int(" + expression->getTypeString() + ") d" + wrtVar;
}

bool mathEngine::exprs::integral::isConst(std::optional<std::string_view> constWrtVar) const{
	//unknowable
	if(constWrtVar)
		return expression->isConst(constWrtVar) && wrtVar != *constWrtVar;
	return false;
}

bool mathEngine::exprs::integral::isEqual(const expr* other) const{
	if(type != other->type)
		return false;
	const auto& otherInt = other->getAs<const integral>();
	return wrtVar == otherInt->wrtVar && expression->isEqual(otherInt->expression.get());
}
