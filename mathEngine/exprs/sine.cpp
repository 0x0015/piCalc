#include "sine.hpp"
#include <cmath>
#include "../hashCombine.hpp"

mathEngine::exprs::sine::sine(){
	type = typeID;
}

double mathEngine::exprs::sine::evalDouble() const{
	return std::sin(inside->evalDouble());
}

void mathEngine::exprs::sine::propegateDFS(const std::function<void(std::shared_ptr<expr>)>& func){
	func(shared_from_this());
	inside->propegateDFS(func);
}

void mathEngine::exprs::sine::propegateDFS_replace_internal(const expr::DFS_replacement_functype& func){
	auto ins_res = func(inside);

	if(ins_res)
		inside = *ins_res;
	else
		inside->propegateDFS_replace_internal(func);
}

std::shared_ptr<mathEngine::expr> mathEngine::exprs::sine::propegateDFS_replace(const expr::DFS_replacement_functype& func){
	auto res = func(shared_from_this());
	if(res)
		return *res;
	propegateDFS_replace_internal(func);
	return shared_from_this();
}

std::string mathEngine::exprs::sine::toLatex() const{
	return "\\sin{" + inside->toLatex() + "}";
}

std::string mathEngine::exprs::sine::toCode(const std::unordered_set<std::string>& wrtVars) const{
	return "sin(" + inside->toCode(wrtVars) + ")";
}

std::shared_ptr<mathEngine::expr> mathEngine::exprs::sine::clone() const{
	auto output = std::make_shared<sine>();
	output->inside = inside->clone();
	return output;
}

std::size_t mathEngine::exprs::sine::hash() const{
	return mathEngine::hashValues(inside->hash(), typeID);
}

std::size_t mathEngine::exprs::sine::hashTypeSig(bool allConstSame, std::optional<std::string_view> constWrtVar) const{
	if(allConstSame && isConst(constWrtVar)){
		return COMPILE_TIME_CRC32_STR("constantExpression");
	}else{
		return mathEngine::hashValues(typeID, inside->hashTypeSig(allConstSame, constWrtVar));
	}
}

std::string mathEngine::exprs::sine::getTypeString() const{
	return "sin(" + inside->getTypeString() + ")";
}

bool mathEngine::exprs::sine::isConst(std::optional<std::string_view> wrtVar) const{
	return inside->isConst(wrtVar);
}

bool mathEngine::exprs::sine::isEqual(const expr* other) const{
	return type == other->type && inside->isEqual(other->getAs<const sine>()->inside.get());
}
