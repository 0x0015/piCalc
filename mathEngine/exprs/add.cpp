#include "add.hpp"
#include "../hashCombine.hpp"

mathEngine::exprs::add::add(){
	type = typeID;
}

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

std::string mathEngine::exprs::add::toCode(const std::unordered_set<std::string>& wrtVars) const{
	std::string output = "(";
	for(unsigned int i=0;i<terms.size();i++){
		output += terms[i]->toCode(wrtVars);
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
	return hashValuesOrderInvarientAccessor([&](unsigned int i){return terms[i]->hash();}, terms.size(), typeID);
}

std::size_t mathEngine::exprs::add::hashTypeSig(bool allConstSame, std::optional<std::string_view> constWrtVar) const{
	if(allConstSame && isConst(constWrtVar)){
		return COMPILE_TIME_CRC32_STR("constantExpression");
	}else{
		return hashValuesOrderInvarientAccessor([&](unsigned int i){return terms[i]->hashTypeSig(allConstSame, constWrtVar);}, terms.size(), typeID);
	}
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

bool mathEngine::exprs::add::isConst(std::optional<std::string_view> wrtVar) const{
	for(const auto& term : terms){
		if(!term->isConst(wrtVar))
			return false;
	}
	return true;
}

bool mathEngine::exprs::add::isEqual(const expr* other) const{
	if(type != other->type)
		return false;
	const auto& otherAdd = other->getAs<const add>();
	if(otherAdd->terms.size() != terms.size())
		return false;
	if(hash() != other->hash())
		return false;
	std::vector<std::shared_ptr<expr>> outstandingTerms = terms;
	std::vector<std::shared_ptr<expr>> outstandingOtherTerms = otherAdd->terms;
	//very slow matching(to preserve commutivity).  Make sure it's actually true, but have many MANY failure points above
	std::erase_if(outstandingTerms, [&](const std::shared_ptr<expr> term){
		for(unsigned int i=0;i<outstandingOtherTerms.size();i++){
			if(term->isEqual(outstandingOtherTerms[i].get())){
				outstandingOtherTerms.erase(outstandingOtherTerms.begin() + i);
				return true;
			}
		}
		return false;
	});
	return outstandingTerms.empty() && outstandingOtherTerms.empty();
}
