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

void mathEngine::exprs::add::propegateDFS(const std::function<void(std::shared_ptr<expr>)>& func){
	func(shared_from_this());
	for(const auto& term : terms)
		term->propegateDFS(func);
}

void mathEngine::exprs::add::propegateDFS_replace_internal(const expr::DFS_replacement_functype& func){
	for(unsigned int i=0;i<terms.size();i++){
		auto res = func(terms[i]);
		if(res)
			terms[i] = *res;
		else
			terms[i]->propegateDFS_replace_internal(func);
	}
}

std::shared_ptr<mathEngine::expr> mathEngine::exprs::add::propegateDFS_replace(const expr::DFS_replacement_functype& func){
	auto val = func(shared_from_this());
	if(val)
		return *val;
	propegateDFS_replace_internal(func);
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

