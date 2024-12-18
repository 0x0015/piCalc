#include "derivative.hpp"
#include "../hashCombine.hpp"
#include "variable.hpp"
#include "../simplifications/evaluateDerivatives.hpp"

mathEngine::exprs::derivative::derivative(){
	type = typeID;
}

//note: see the section on step size at https://en.wikipedia.org/wiki/Numerical_differentiation
double mathEngine::exprs::derivative::evalDouble() const{
	auto oldVal = variable::varVals[wrtVar];
	double aroundPoint = oldVal.toDouble();

	variable::varVals[wrtVar].value = aroundPoint + (dx / 2.0);
	double upperVal = expression->evalDouble();
	variable::varVals[wrtVar].value = aroundPoint - (dx / 2.0);
	double lowerVal = expression->evalDouble();

	variable::varVals[wrtVar] = oldVal;
	return (upperVal-lowerVal) / dx;
}

void mathEngine::exprs::derivative::propegateDFS(const std::function<void(std::shared_ptr<expr>)>& func){
	func(shared_from_this());
	expression->propegateDFS(func);
}

void mathEngine::exprs::derivative::propegateDFS_replace_internal(const expr::DFS_replacement_functype& func){
	auto res = func(expression);
	if(res)
		expression = *res;
	else
		expression->propegateDFS_replace_internal(func);
}

std::shared_ptr<mathEngine::expr> mathEngine::exprs::derivative::propegateDFS_replace(const expr::DFS_replacement_functype& func){
	auto val = func(shared_from_this());
	if(val)
		return *val;
	propegateDFS_replace_internal(func);
	return shared_from_this();
}


std::string mathEngine::exprs::derivative::toLatex() const{
	return "D_" + (wrtVar.size() == 1 ? wrtVar : "{" + wrtVar + "}") + "(" + expression->toLatex() + ")";
}

std::string mathEngine::exprs::derivative::toCode(const std::unordered_set<std::string>& wrtVars) const{
	return "0.0";//fill in later
}

std::shared_ptr<mathEngine::expr> mathEngine::exprs::derivative::clone() const{
	auto output = std::make_shared<derivative>();
	output->expression = expression->clone();
	output->wrtVar = wrtVar;
	return output;
}

std::size_t mathEngine::exprs::derivative::hash() const{
	return mathEngine::hashValues(expression->hash(), wrtVar, typeID);
}

std::size_t mathEngine::exprs::derivative::hashTypeSig(bool allConstSame, std::optional<std::string_view> constWrtVar) const{
	if(allConstSame && isConst(constWrtVar)){
		return COMPILE_TIME_CRC32_STR("constantExpression");
	}else{
		return mathEngine::hashValues(typeID, expression->hashTypeSig(allConstSame, constWrtVar));
	}
}

std::string mathEngine::exprs::derivative::getTypeString() const{
	return "D" + wrtVar + "(" + expression->getTypeString() + ")";
}

bool mathEngine::exprs::derivative::isConst(std::optional<std::string_view> constWrtVar) const{
	//unknowable
	return expression->isConst(constWrtVar);
}

bool mathEngine::exprs::derivative::isEqual(const expr* other) const{
	if(type != other->type)
		return false;
	const auto& otherDer = other->getAs<const derivative>();
	return wrtVar == otherDer->wrtVar && expression->isEqual(otherDer->expression.get());
}
