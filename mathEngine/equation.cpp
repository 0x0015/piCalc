#include "equation.hpp"
#include "expr.hpp"

double mathEngine::equation::evalDiffDouble() const{
	return lhs->evalDouble() - rhs->evalDouble();
}

double mathEngine::equation::evalUnsignedDiffDouble() const{
	return std::abs(evalDiffDouble());
}

mathEngine::constVal mathEngine::equation::evalDiff() const{
	return lhs->eval() - rhs->eval();
}

void mathEngine::equation::propegateDFS(const std::function<void(std::shared_ptr<expr>)>& func, bool includeConstants){
	lhs->propegateDFS(func, includeConstants);
	rhs->propegateDFS(func, includeConstants);
}

std::string mathEngine::equation::toLatex() const{
	return lhs->toLatex() + " = " + rhs->toLatex();
}

mathEngine::equation mathEngine::equation::clone() const{
	return equation{lhs->clone(), rhs->clone()};
}
