#include "variable.hpp"

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
