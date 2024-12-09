#include "constant.hpp"
#include <cmath>
#include "add.hpp"
#include "multiply.hpp"
#include "../hashCombine.hpp"

mathEngine::exprs::constant::constant(){
	type = typeID;
}
double mathEngine::exprs::constant::evalDouble() const{
	return value.toDouble();
}

void mathEngine::exprs::constant::propegateDFS(const std::function<void(std::shared_ptr<expr>)>& func){
	func(shared_from_this());
}

void mathEngine::exprs::constant::propegateDFS_replace_internal(const expr::DFS_replacement_functype& func){}

std::shared_ptr<mathEngine::expr> mathEngine::exprs::constant::propegateDFS_replace(const expr::DFS_replacement_functype& func){
	auto res = func(shared_from_this());
	if(res)
		return *res;
	propegateDFS_replace_internal(func);
	return shared_from_this();
}

std::string mathEngine::exprs::constant::toLatex() const{
	return value.toLatex();
}

std::string mathEngine::exprs::constant::toCode(const std::unordered_set<std::string>& wrtVars) const{
	return std::to_string(value.toDouble());
}

std::shared_ptr<mathEngine::expr> mathEngine::exprs::constant::clone() const{
	auto output = std::make_shared<constant>();
	output->value = value.clone();
	return output;
}

std::size_t mathEngine::exprs::constant::hash() const{
	return mathEngine::hashValues(value.hash(), typeID);
}

std::size_t mathEngine::exprs::constant::hashTypeSig(bool allConstSame, std::optional<std::string_view> constWrtVar) const{
	if(allConstSame){
		return COMPILE_TIME_CRC32_STR("constantExpression");
	}else{
		return typeID;
	}
}

std::string mathEngine::exprs::constant::getTypeString() const{
	return "constant";
}

bool mathEngine::exprs::constant::isConst(std::optional<std::string_view> wrtVar) const{
	return true;
}

bool mathEngine::exprs::constant::isEqual(const expr* other) const{
	return type == other->type && value.value == other->getAs<const constant>()->value.value;
}
