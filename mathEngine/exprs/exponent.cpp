#include "exponent.hpp"
#include <cmath>

double mathEngine::exprs::exponent::evalDouble() const{
	return std::pow(base->evalDouble(), exp->evalDouble());
}

mathEngine::constVal mathEngine::exprs::exponent::eval() const{
	return {shared_from_this()};//really not much simplifying or anything can happen right now other then just passing on what we know
}

void mathEngine::exprs::exponent::propegateDFS(const std::function<void(std::shared_ptr<expr>)>& func, bool includeConstants){
	func(shared_from_this());
	base->propegateDFS(func, includeConstants);
	exp->propegateDFS(func, includeConstants);
}

std::string mathEngine::exprs::exponent::toLatex() const{
	return "{" + base->toLatex() + "}^{" + exp->toLatex() + "}";
}
