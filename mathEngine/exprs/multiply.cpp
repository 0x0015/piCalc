#include "multiply.hpp"
#include "../hashCombine.hpp"

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

void mathEngine::exprs::multiply::propegateDFS_replace_internal(const expr::DFS_replacement_functype& func, bool includeConstants){
	auto lhs_res = func(lhs);
	auto rhs_res = func(rhs);

	if(lhs_res)
		lhs = *lhs_res;
	else
		lhs->propegateDFS_replace_internal(func, includeConstants);

	if(rhs_res)
		rhs = *rhs_res;
	else
		rhs->propegateDFS_replace_internal(func, includeConstants);
}

std::shared_ptr<mathEngine::expr> mathEngine::exprs::multiply::propegateDFS_replace(const expr::DFS_replacement_functype& func, bool includeConstants){
	auto res = func(shared_from_this());
	if(res)
		return *res;
	propegateDFS_replace_internal(func, includeConstants);
	return shared_from_this();
}

std::string mathEngine::exprs::multiply::toLatex() const{
	return "(" + lhs->toLatex() + "*" + rhs->toLatex() + ")";
}

std::shared_ptr<mathEngine::expr> mathEngine::exprs::multiply::clone() const{
	auto output = std::make_shared<multiply>();
	output->lhs = lhs->clone();
	output->rhs = rhs->clone();
	return output;
}

std::size_t mathEngine::exprs::multiply::hash() const{
	std::size_t lhsHash = lhs->hash();
	mathEngine::hash_combine(lhsHash, rhs->hash(), COMPILE_TIME_CRC32_STR("multiply"));
	return lhsHash;
}

