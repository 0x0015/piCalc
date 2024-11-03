#include "cosine.hpp"
#include <cmath>
#include "../hashCombine.hpp"

double mathEngine::exprs::cosine::evalDouble() const{
	return std::cos(inside->evalDouble());
}

void mathEngine::exprs::cosine::propegateDFS(const std::function<void(std::shared_ptr<expr>)>& func){
	func(shared_from_this());
	inside->propegateDFS(func);
}

void mathEngine::exprs::cosine::propegateDFS_replace_internal(const expr::DFS_replacement_functype& func){
	auto ins_res = func(inside);

	if(ins_res)
		inside = *ins_res;
	else
		inside->propegateDFS_replace_internal(func);
}

std::shared_ptr<mathEngine::expr> mathEngine::exprs::cosine::propegateDFS_replace(const expr::DFS_replacement_functype& func){
	auto res = func(shared_from_this());
	if(res)
		return *res;
	propegateDFS_replace_internal(func);
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

std::string mathEngine::exprs::cosine::getTypeString() const{
	return "cos(" + inside->getTypeString() + ")";
}
