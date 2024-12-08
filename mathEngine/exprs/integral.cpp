#include "integral.hpp"
#include "../hashCombine.hpp"
#include "variable.hpp"
#include "../simplifications/evaluateIntegrals.hpp"

mathEngine::exprs::integral::integral(){
	type = typeID;
}

double mathEngine::exprs::integral::evalDouble() const{
	return -1;
}

void mathEngine::exprs::integral::propegateDFS(const std::function<void(std::shared_ptr<expr>)>& func){
	func(shared_from_this());
	expression->propegateDFS(func);
}

void mathEngine::exprs::integral::propegateDFS_replace_internal(const expr::DFS_replacement_functype& func){
	auto res = func(expression);
	if(res)
		expression = *res;
	else
		expression->propegateDFS_replace_internal(func);
}

std::shared_ptr<mathEngine::expr> mathEngine::exprs::integral::propegateDFS_replace(const expr::DFS_replacement_functype& func){
	auto val = func(shared_from_this());
	if(val)
		return *val;
	propegateDFS_replace_internal(func);
	return shared_from_this();
}


std::string mathEngine::exprs::integral::toLatex() const{
	return "\\int \\left(" + expression->toLatex() + "\\right) dx";
}

std::string mathEngine::exprs::integral::toCode(const std::unordered_set<std::string>& wrtVars) const{
	return "0.0";//fill in later
}

std::shared_ptr<mathEngine::expr> mathEngine::exprs::integral::clone() const{
	auto output = std::make_shared<integral>();
	output->expression = expression->clone();
	output->wrtVar = wrtVar;
	return output;
}

std::size_t mathEngine::exprs::integral::hash() const{
	std::size_t outputHash = expression->hash();
	mathEngine::hash_combine(outputHash, wrtVar, COMPILE_TIME_CRC32_STR("integral"));
	return outputHash;
}

std::string mathEngine::exprs::integral::getTypeString() const{
	return "int(" + expression->getTypeString() + ") d" + wrtVar;
}
