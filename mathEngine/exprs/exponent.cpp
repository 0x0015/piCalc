#include "exponent.hpp"
#include <cmath>
#include "../hashCombine.hpp"

double mathEngine::exprs::exponent::evalDouble() const{
	return std::pow(base->evalDouble(), exp->evalDouble());
}

void mathEngine::exprs::exponent::propegateDFS(const std::function<void(std::shared_ptr<expr>)>& func){
	func(shared_from_this());
	base->propegateDFS(func);
	exp->propegateDFS(func);
}

void mathEngine::exprs::exponent::propegateDFS_replace_internal(const expr::DFS_replacement_functype& func){
	auto base_res = func(base);
	auto exp_res = func(exp);

	if(base_res)
		base = *base_res;
	else
		base->propegateDFS_replace_internal(func);

	if(base_res)
		base = *base_res;
	else
		base->propegateDFS_replace_internal(func);
}

std::shared_ptr<mathEngine::expr> mathEngine::exprs::exponent::propegateDFS_replace(const expr::DFS_replacement_functype& func){
	auto res = func(shared_from_this());
	if(res)
		return *res;
	propegateDFS_replace_internal(func);
	return shared_from_this();
}

std::string mathEngine::exprs::exponent::toLatex() const{
	return "{" + base->toLatex() + "}^{" + exp->toLatex() + "}";
}

std::string mathEngine::exprs::exponent::toCode(const std::unordered_set<std::string>& wrtVars) const{
	return "pow(" + base->toCode(wrtVars) + ", " + exp->toCode(wrtVars) + ")";
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

std::string mathEngine::exprs::exponent::getTypeString() const{
	return "(" + base->getTypeString() + ")^(" + exp->getTypeString() + ")";
}

