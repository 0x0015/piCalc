#include "variable.hpp"
#include "../hashCombine.hpp"

double mathEngine::exprs::variable::evalDouble() const{
	return varVals[name].toDouble();
}

mathEngine::constVal mathEngine::exprs::variable::eval() const{
	return varVals[name];
}

void mathEngine::exprs::variable::propegateDFS(const std::function<void(std::shared_ptr<expr>)>& func, bool includeConstants){
	func(shared_from_this());
}

std::string mathEngine::exprs::variable::toLatex() const{
	return name;
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
