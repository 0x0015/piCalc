#include "equation.hpp"
#include "expr.hpp"
#include "hashCombine.hpp"

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

void mathEngine::equation::propegateDFS_replace(const expr::DFS_replacement_functype& func, bool includeConstants){
	auto lhs_res = func(lhs);
	auto rhs_res = func(rhs);

	if(lhs_res)
		lhs = *lhs_res;
	else
		lhs->propegateDFS_replace(func, includeConstants);

	if(rhs_res)
		rhs = *rhs_res;
	else
		rhs->propegateDFS_replace(func, includeConstants);
}

std::string mathEngine::equation::toLatex() const{
	return lhs->toLatex() + " = " + rhs->toLatex();
}

mathEngine::equation mathEngine::equation::clone() const{
	equation eq;
	eq.lhs = lhs->clone();
	eq.rhs = rhs->clone();
	return eq;
}

std::size_t mathEngine::equation::hash() const{
	std::size_t lhsHash = lhs->hash();
	mathEngine::hash_combine(lhsHash, rhs->hash(), COMPILE_TIME_CRC32_STR("equation"));
	return lhsHash;
}
