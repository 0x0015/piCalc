#include "cosine.hpp"
#include <cmath>
#include "../hashCombine.hpp"

double mathEngine::exprs::cosine::evalDouble() const{
	return std::cos(inside->evalDouble());
}

mathEngine::constVal mathEngine::exprs::cosine::eval() const{
	return {shared_from_this()};//can be simplified in certain, cases.  will be handled in a simplification pass though
}

void mathEngine::exprs::cosine::propegateDFS(const std::function<void(std::shared_ptr<expr>)>& func, bool includeConstants){
	func(shared_from_this());
	inside->propegateDFS(func, includeConstants);
}

void mathEngine::exprs::cosine::propegateDFS_replace_internal(const expr::DFS_replacement_functype& func, bool includeConstants){
	auto ins_res = func(inside);

	if(ins_res)
		inside = *ins_res;
	else
		inside->propegateDFS_replace_internal(func, includeConstants);
}

std::shared_ptr<mathEngine::expr> mathEngine::exprs::cosine::propegateDFS_replace(const expr::DFS_replacement_functype& func, bool includeConstants){
	auto res = func(shared_from_this());
	if(res)
		return *res;
	propegateDFS_replace_internal(func, includeConstants);
	return shared_from_this();
}

std::string mathEngine::exprs::cosine::toLatex() const{
	return "\\cos{" + inside->toLatex() + "}";
}

std::shared_ptr<mathEngine::expr> mathEngine::exprs::cosine::clone() const{
	auto output = std::make_shared<cosine>();
	output->inside = inside->clone();
	return output;
}

std::size_t mathEngine::exprs::cosine::hash() const{
	std::size_t insHash = inside->hash();
	mathEngine::hash_combine(insHash, COMPILE_TIME_CRC32_STR("cosine"));
	return insHash;
}

