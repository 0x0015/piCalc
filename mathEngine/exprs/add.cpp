#include "add.hpp"

double mathEngine::exprs::add::evalDouble() const{
	return lhs->evalDouble() + rhs->evalDouble();
}

mathEngine::constVal mathEngine::exprs::add::eval() const{
	return lhs->eval() + rhs->eval();
}

void mathEngine::exprs::add::propegateDFS(const std::function<void(std::shared_ptr<expr>)>& func, bool includeConstants){
	func(shared_from_this());
	lhs->propegateDFS(func, includeConstants);
	rhs->propegateDFS(func, includeConstants);
}

std::string mathEngine::exprs::add::toLatex() const{
	return "(" + lhs->toLatex() + "+" + rhs->toLatex() + ")";
}

