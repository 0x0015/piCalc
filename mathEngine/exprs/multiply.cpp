#include "multiply.hpp"
#include "../hashCombine.hpp"

mathEngine::exprs::multiply::multiply(){
	type = typeID;
}

double mathEngine::exprs::multiply::evalDouble() const{
	if(terms.empty())
		return -1;
	double output = terms.front()->evalDouble();
	for(unsigned int i=1;i<terms.size();i++)
		output *= terms[i]->evalDouble();
	return output;
}

void mathEngine::exprs::multiply::propegateDFS(const std::function<void(std::shared_ptr<expr>)>& func){
	func(shared_from_this());
	for(const auto& term : terms)
		term->propegateDFS(func);
}

void mathEngine::exprs::multiply::propegateDFS_replace_internal(const expr::DFS_replacement_functype& func){
	for(unsigned int i=0;i<terms.size();i++){
		auto res = func(terms[i]);
		if(res)
			terms[i] = *res;
		else
			terms[i]->propegateDFS_replace_internal(func);
	}
}

std::shared_ptr<mathEngine::expr> mathEngine::exprs::multiply::propegateDFS_replace(const expr::DFS_replacement_functype& func){
	auto res = func(shared_from_this());
	if(res)
		return *res;
	propegateDFS_replace_internal(func);
	return shared_from_this();
}

std::string mathEngine::exprs::multiply::toLatex() const{
	std::string output;
	for(unsigned int i=0;i<terms.size();i++){
		output += terms[i]->toLatex();
		if(i+1 < terms.size())
			output += " \\cdot ";
	}
	return output;
}

std::string mathEngine::exprs::multiply::toCode(const std::unordered_set<std::string>& wrtVars) const{
	std::string output;
	for(unsigned int i=0;i<terms.size();i++){
		output += terms[i]->toCode(wrtVars);
		if(i+1 < terms.size())
			output += " * ";
	}
	return output;
}

std::shared_ptr<mathEngine::expr> mathEngine::exprs::multiply::clone() const{
	auto output = std::make_shared<multiply>();
	for(const auto& term : terms)
		output->terms.push_back(term->clone());
	return output;
}

std::size_t mathEngine::exprs::multiply::hash() const{
	std::size_t outputHash = COMPILE_TIME_CRC32_STR("add");
	for(const auto& term : terms)
		mathEngine::hash_combine(outputHash, term->hash());
	return outputHash;
}

std::string mathEngine::exprs::multiply::getTypeString() const{
	std::string output = "(";
	for(unsigned int i=0;i<terms.size();i++){
		output += terms[i]->getTypeString();
		if(i+1 < terms.size())
			output += '*';
	}
	output += ')';
	return output;
}
