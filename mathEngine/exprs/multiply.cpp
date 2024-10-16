#include "multiply.hpp"

double mathEngine::exprs::multiply::evalDouble() const{
	return lhs->evalDouble() * rhs->evalDouble();
}

mathEngine::constVal mathEngine::exprs::multiply::eval() const{
	return lhs->eval() * rhs->eval();
}

void mathEngine::exprs::multiply::propegateDFS(const std::function<void(std::shared_ptr<expr>)>& func, bool includeConstants){
	func(shared_from_this());
	lhs->propegateDFS(func, includeConstants);
	rhs->propegateDFS(func, includeConstants);
}

std::string mathEngine::exprs::multiply::toLatex() const{
	return "(" + lhs->toLatex() + "+" + rhs->toLatex() + ")";
}

