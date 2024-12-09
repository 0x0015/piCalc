#include "variable.hpp"
#include "../hashCombine.hpp"

mathEngine::exprs::variable::variable(){
	type = typeID;
}

double mathEngine::exprs::variable::evalDouble() const{
	return varVals[name].toDouble();
}

void mathEngine::exprs::variable::propegateDFS(const std::function<void(std::shared_ptr<expr>)>& func){
	func(shared_from_this());
}

void mathEngine::exprs::variable::propegateDFS_replace_internal(const expr::DFS_replacement_functype& func){
	//non
}
std::shared_ptr<mathEngine::expr> mathEngine::exprs::variable::propegateDFS_replace(const expr::DFS_replacement_functype& func){
	auto res = func(shared_from_this());
	if(res)
		return *res;
	propegateDFS_replace_internal(func);
	return shared_from_this();
}

std::string mathEngine::exprs::variable::toLatex() const{
	return name;
}

std::string mathEngine::exprs::variable::toCode(const std::unordered_set<std::string>& wrtVars) const{
	if(wrtVars.contains(name)){
		return name;
	}
	return std::to_string(evalDouble());
}

std::shared_ptr<mathEngine::expr> mathEngine::exprs::variable::clone() const{
	auto output = std::make_shared<variable>();
	output->name = name;
	return output;
}

std::size_t mathEngine::exprs::variable::hash() const{
	return mathEngine::hashValues(name, typeID);
}

std::size_t mathEngine::exprs::variable::hashTypeSig(bool allConstSame, std::optional<std::string_view> constWrtVar) const{
	if(allConstSame && isConst(constWrtVar)){
		return COMPILE_TIME_CRC32_STR("constantExpression");
	}else{
		return typeID;
	}
}

std::string mathEngine::exprs::variable::getTypeString() const{
	return "variable";
}

bool mathEngine::exprs::variable::isConst(std::optional<std::string_view> wrtVar) const{
	if(wrtVar)
		return *wrtVar != name;
	return false;
}

bool mathEngine::exprs::variable::isEqual(const expr* other) const{
	return type == other->type && name == other->getAs<const variable>()->name;
}
