#include "constVal.hpp"
#include <cmath>
#include "exprs/add.hpp"
#include "exprs/multiply.hpp"
#include "exprs/exponent.hpp"
#include "exprs/constant.hpp"

double mathEngine::constVal::toDouble() const{
	if(std::holds_alternative<double>(value)){
		return std::get<double>(value);
	}else if(std::holds_alternative<rational>(value)){
		return std::get<rational>(value).toDouble();
	}else if(std::holds_alternative<std::shared_ptr<const expr>>(value)){
		return std::get<std::shared_ptr<const expr>>(value)->evalDouble();
	}else if(std::holds_alternative<constantName>(value)){
		return constantDoubleValues[std::get<constantName>(value)];
	}else{
		return NAN;
	}
}

mathEngine::constVal mathEngine::operator+(const mathEngine::constVal& a, const mathEngine::constVal& b){
	mathEngine::constVal output;
	if(std::holds_alternative<rational>(a.value) && std::holds_alternative<rational>(b.value)){
		output.value = std::get<rational>(a.value) + std::get<rational>(b.value);
	}else if(std::holds_alternative<double>(a.value) && std::holds_alternative<double>(b.value)){
		output.value = std::get<double>(a.value) + std::get<double>(b.value);
	}else{
		auto add = std::make_shared<mathEngine::exprs::add>();
		auto lhs = std::make_shared<mathEngine::exprs::constant>();
		auto rhs = std::make_shared<mathEngine::exprs::constant>();
		lhs->value = a;
		rhs->value = b;
		add->lhs = lhs;
		add->rhs = rhs;
		output.value = add;
	}
	return output;
}

mathEngine::constVal mathEngine::operator-(const mathEngine::constVal& a, const mathEngine::constVal& b){
	mathEngine::constVal output;
	if(std::holds_alternative<rational>(a.value) && std::holds_alternative<rational>(b.value)){
		output.value = std::get<rational>(a.value) - std::get<rational>(b.value);
	}else if(std::holds_alternative<double>(a.value) && std::holds_alternative<double>(b.value)){
		output.value = std::get<double>(a.value) - std::get<double>(b.value);
	}else{
		auto add = std::make_shared<mathEngine::exprs::add>();
		auto smul = std::make_shared<mathEngine::exprs::multiply>();
		auto minusOneConst = std::make_shared<mathEngine::exprs::constant>();
		auto lhs = std::make_shared<mathEngine::exprs::constant>();
		auto rhs = std::make_shared<mathEngine::exprs::constant>();
		lhs->value = a;
		rhs->value = b;
		minusOneConst->value.value = rational(-1, 1);
		smul->lhs = minusOneConst;
		smul->rhs = rhs;
		add->lhs = lhs;
		add->rhs = smul;
		output.value = add;
	}
	return output;
}

mathEngine::constVal mathEngine::operator*(const mathEngine::constVal& a, const mathEngine::constVal& b){
	mathEngine::constVal output;
	if(std::holds_alternative<rational>(a.value) && std::holds_alternative<rational>(b.value)){
		output.value = std::get<rational>(a.value) * std::get<rational>(b.value);
	}else if(std::holds_alternative<double>(a.value) && std::holds_alternative<double>(b.value)){
		output.value = std::get<double>(a.value) * std::get<double>(b.value);
	}else{
		auto mul = std::make_shared<mathEngine::exprs::multiply>();
		auto lhs = std::make_shared<mathEngine::exprs::constant>();
		auto rhs = std::make_shared<mathEngine::exprs::constant>();
		lhs->value = a;
		rhs->value = b;
		mul->lhs = lhs;
		mul->rhs = rhs;
		output.value = mul;
	}
	return output;
}

mathEngine::constVal mathEngine::operator/(const mathEngine::constVal& a, const mathEngine::constVal& b){
	mathEngine::constVal output;
	if(std::holds_alternative<rational>(a.value) && std::holds_alternative<rational>(b.value)){
		output.value = std::get<rational>(a.value) / std::get<rational>(b.value);
	}else if(std::holds_alternative<double>(a.value) && std::holds_alternative<double>(b.value)){
		output.value = std::get<double>(a.value) / std::get<double>(b.value);
	}else{
		auto mul = std::make_shared<mathEngine::exprs::multiply>();
		auto sexp = std::make_shared<mathEngine::exprs::exponent>();
		auto minusOneConst = std::make_shared<mathEngine::exprs::constant>();
		auto lhs = std::make_shared<mathEngine::exprs::constant>();
		auto rhs = std::make_shared<mathEngine::exprs::constant>();
		lhs->value = a;
		rhs->value = b;
		minusOneConst->value.value = rational(-1, 1);
		sexp->exp = minusOneConst;
		sexp->base = rhs;
		mul->lhs = lhs;
		mul->rhs = sexp;
		output.value = mul;
	}
	return output;
}

std::string mathEngine::constVal::toLatex() const{
	if(std::holds_alternative<double>(value)){
		return std::to_string(std::get<double>(value));
	}else if(std::holds_alternative<rational>(value)){
		const auto& rat = std::get<rational>(value);
		if(rat.denom == 1)
			return std::to_string(rat.num);
		else
			return "\\frac{" + std::to_string(rat.num) + "}{" + std::to_string(rat.denom) + "}";
	}else if(std::holds_alternative<std::shared_ptr<const expr>>(value)){
		return std::get<std::shared_ptr<const expr>>(value)->toLatex();
	}else if(std::holds_alternative<constantName>(value)){
		return std::string(constantLatexNames[std::get<constantName>(value)]);
	}else{
		return "\\text{unknown constant}";
	}
}

