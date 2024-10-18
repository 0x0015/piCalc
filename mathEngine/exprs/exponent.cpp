#include "exponent.hpp"
#include <cmath>
#include "../hashCombine.hpp"

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

void mathEngine::exprs::exponent::propegateDFS_replace_internal(const expr::DFS_replacement_functype& func, bool includeConstants){
	auto base_res = func(base);
	auto exp_res = func(exp);

	if(base_res)
		base = *base_res;
	else
		base->propegateDFS_replace_internal(func, includeConstants);

	if(base_res)
		base = *base_res;
	else
		base->propegateDFS_replace_internal(func, includeConstants);
}

std::shared_ptr<mathEngine::expr> mathEngine::exprs::exponent::propegateDFS_replace(const expr::DFS_replacement_functype& func, bool includeConstants){
	auto res = func(shared_from_this());
	if(res)
		return *res;
	propegateDFS_replace_internal(func, includeConstants);
	return shared_from_this();
}

std::string mathEngine::exprs::exponent::toLatex() const{
	return "{" + base->toLatex() + "}^{" + exp->toLatex() + "}";
}

std::shared_ptr<mathEngine::expr> mathEngine::exprs::exponent::clone() const{
	auto output = std::make_shared<exponent>();
	output->base = base->clone();
	output->exp = exp->clone();
	return output;
}

std::size_t mathEngine::exprs::exponent::hash() const{
	std::size_t lhsHash = base->hash();
	mathEngine::hash_combine(lhsHash, exp->hash(), COMPILE_TIME_CRC32_STR("exponent"));
	return lhsHash;
}

