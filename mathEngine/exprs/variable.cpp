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
	std::size_t nameHash = std::hash<std::string>{}(name);
	mathEngine::hash_combine(nameHash, COMPILE_TIME_CRC32_STR("variable"));
	return nameHash;
}

std::string mathEngine::exprs::variable::getTypeString() const{
	return "variable";
}
