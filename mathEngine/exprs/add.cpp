#include "add.hpp"
#include "../hashCombine.hpp"

double mathEngine::exprs::add::evalDouble() const{
	if(terms.empty())
		return -1;
	double output = terms.front()->evalDouble();
	for(unsigned int i=1;i<terms.size();i++)
		output += terms[i]->evalDouble();
	return output;
}

mathEngine::constVal mathEngine::exprs::add::eval() const{
	if(terms.empty())
		return {};
	constVal output = terms.front()->eval();
	for(unsigned int i=1;i<terms.size();i++)
		output = output + terms[i]->eval();
	return output;
}

void mathEngine::exprs::add::propegateDFS(const std::function<void(std::shared_ptr<expr>)>& func, bool includeConstants){
	func(shared_from_this());
	for(const auto& term : terms)
		term->propegateDFS(func, includeConstants);
}

void mathEngine::exprs::add::propegateDFS_replace_internal(const expr::DFS_replacement_functype& func, bool includeConstants){
	for(unsigned int i=0;i<terms.size();i++){
		auto res = func(terms[i]);
		if(res)
			terms[i] = *res;
		else
			terms[i]->propegateDFS_replace_internal(func, includeConstants);
	}
}

std::shared_ptr<mathEngine::expr> mathEngine::exprs::add::propegateDFS_replace(const expr::DFS_replacement_functype& func, bool includeConstants){
	auto val = func(shared_from_this());
	if(val)
		return *val;
	propegateDFS_replace_internal(func, includeConstants);
	return shared_from_this();
}

std::string mathEngine::exprs::add::toLatex() const{
	std::string output = "(";
	for(unsigned int i=0;i<terms.size();i++){
		output += terms[i]->toLatex();
		if(i+1 < terms.size())
			output += '+';
	}
	output += ')';
	return output;
}

std::shared_ptr<mathEngine::expr> mathEngine::exprs::add::clone() const{
	auto output = std::make_shared<add>();
	for(const auto& term : terms)
		output->terms.push_back(term->clone());
	return output;
}

std::size_t mathEngine::exprs::add::hash() const{
	std::size_t outputHash = COMPILE_TIME_CRC32_STR("add");
	for(const auto& term : terms)
		mathEngine::hash_combine(outputHash, term->hash());
	return outputHash;
}

std::string mathEngine::exprs::add::getTypeString() const{
	std::string output = "(";
	for(unsigned int i=0;i<terms.size();i++){
		output += terms[i]->getTypeString();
		if(i+1 < terms.size())
			output += '+';
	}
	output += ')';
	return output;
}

