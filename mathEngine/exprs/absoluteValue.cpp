#include "absoluteValue.hpp"
#include <cmath>
#include "../hashCombine.hpp"

mathEngine::exprs::absoluteValue::absoluteValue(){
	type = typeID;
}

double mathEngine::exprs::absoluteValue::evalDouble() const{
	return std::abs(inside->evalDouble());
}

void mathEngine::exprs::absoluteValue::propegateDFS(const std::function<void(std::shared_ptr<expr>)>& func){
	func(shared_from_this());
	inside->propegateDFS(func);
}

void mathEngine::exprs::absoluteValue::propegateDFS_replace_internal(const expr::DFS_replacement_functype& func){
	auto inside_res = func(inside);

	if(inside_res)
		inside = *inside_res;
	else
		inside->propegateDFS_replace_internal(func);
}

std::shared_ptr<mathEngine::expr> mathEngine::exprs::absoluteValue::propegateDFS_replace(const expr::DFS_replacement_functype& func){
	auto res = func(shared_from_this());
	if(res)
		return *res;
	propegateDFS_replace_internal(func);
	return shared_from_this();
}

std::string mathEngine::exprs::absoluteValue::toLatex() const{
	return "|" + inside->toLatex() + "|";
}

std::string mathEngine::exprs::absoluteValue::toCode(const std::unordered_set<std::string>& wrtVars) const{
	return "abs(" + inside->toCode(wrtVars) + ")";
}

std::shared_ptr<mathEngine::expr> mathEngine::exprs::absoluteValue::clone() const{
	auto output = std::make_shared<absoluteValue>();
	output->inside = inside->clone();
	return output;
}

std::size_t mathEngine::exprs::absoluteValue::hash() const{
	return mathEngine::hashValues(inside->hash(), typeID);
}

std::size_t mathEngine::exprs::absoluteValue::hashTypeSig(bool allConstSame, std::optional<std::string_view> constWrtVar) const{
	if(allConstSame && isConst(constWrtVar)){
		return COMPILE_TIME_CRC32_STR("constantExpression");
	}else{
		return mathEngine::hashValues(typeID, inside->hashTypeSig(allConstSame, constWrtVar));
	}
}

std::string mathEngine::exprs::absoluteValue::getTypeString() const{
	return "abs(" + inside->getTypeString() + ")";
}

bool mathEngine::exprs::absoluteValue::isConst(std::optional<std::string_view> wrtVar) const{
	return inside->isConst(wrtVar);
}

bool mathEngine::exprs::absoluteValue::isEqual(const expr* other) const{
	return type == other->type && inside->isEqual(other->getAs<const absoluteValue>()->inside.get());
}
