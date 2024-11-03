#include "equation.hpp"
#include "expr.hpp"
#include "hashCombine.hpp"
#include "exprs/add.hpp"

double mathEngine::equation::evalDiffDouble() const{
	return lhs->evalDouble() - rhs->evalDouble();
}

double mathEngine::equation::evalUnsignedDiffDouble() const{
	return std::abs(evalDiffDouble());
}

std::shared_ptr<mathEngine::expr> mathEngine::equation::getDiff() const{
	auto output = std::make_shared<exprs::add>();
	output->terms = {lhs, rhs};
	return output;
}

void mathEngine::equation::propegateDFS(const std::function<void(std::shared_ptr<expr>)>& func){
	lhs->propegateDFS(func);
	rhs->propegateDFS(func);
}

void mathEngine::equation::propegateDFS_replace(const expr::DFS_replacement_functype& func){
	auto lhs_res = func(lhs);
	auto rhs_res = func(rhs);

	if(lhs_res)
		lhs = *lhs_res;
	else
		lhs->propegateDFS_replace(func);

	if(rhs_res)
		rhs = *rhs_res;
	else
		rhs->propegateDFS_replace(func);
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

mathEngine::equation mathEngine::equation::substiteVariable(const std::string& varName, std::shared_ptr<expr> subVal){
	equation output;
	output.lhs = lhs->substiteVariable(varName, subVal);
	output.rhs = rhs->substiteVariable(varName, subVal);
	return output;
}


