#include "add.hpp"
#include "../hashCombine.hpp"

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

void mathEngine::exprs::add::propegateDFS_replace_internal(const expr::DFS_replacement_functype& func, bool includeConstants){
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

std::shared_ptr<mathEngine::expr> mathEngine::exprs::add::propegateDFS_replace(const expr::DFS_replacement_functype& func, bool includeConstants){
	auto val = func(shared_from_this());
	if(val)
		return *val;
	propegateDFS_replace_internal(func, includeConstants);
	return shared_from_this();
}

std::string mathEngine::exprs::add::toLatex() const{
	return "(" + lhs->toLatex() + "+" + rhs->toLatex() + ")";
}

std::shared_ptr<mathEngine::expr> mathEngine::exprs::add::clone() const{
	auto output = std::make_shared<add>();
	output->lhs = lhs->clone();
	output->rhs = rhs->clone();
	return output;
}

std::size_t mathEngine::exprs::add::hash() const{
	std::size_t lhsHash = lhs->hash();
	mathEngine::hash_combine(lhsHash, rhs->hash(), COMPILE_TIME_CRC32_STR("add"));
	return lhsHash;
}

