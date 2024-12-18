#include "constVal.hpp"
#include <cmath>
#include "exprs/add.hpp"
#include "exprs/multiply.hpp"
#include "exprs/exponent.hpp"
#include "exprs/constant.hpp"
#include "hashCombine.hpp"

double mathEngine::constVal::toDouble() const{
	if(std::holds_alternative<double>(value)){
		return std::get<double>(value);
	}else if(std::holds_alternative<rational>(value)){
		return std::get<rational>(value).toDouble();
	}else if(std::holds_alternative<constantName>(value)){
		return constantDoubleValues[std::get<constantName>(value)];
	}else{
		return NAN;
	}
}

std::variant<mathEngine::constVal, std::shared_ptr<mathEngine::expr>> mathEngine::operator+(const mathEngine::constVal& a, const mathEngine::constVal& b){
	if(std::holds_alternative<rational>(a.value) && std::holds_alternative<rational>(b.value)){
		mathEngine::constVal output;
		output.value = std::get<rational>(a.value) + std::get<rational>(b.value);
		return output;
	}else if(std::holds_alternative<double>(a.value) && std::holds_alternative<double>(b.value)){
		mathEngine::constVal output;
		output.value = std::get<double>(a.value) + std::get<double>(b.value);
		return output;
	}else{
		auto add = std::make_shared<mathEngine::exprs::add>();
		auto lhs = std::make_shared<mathEngine::exprs::constant>();
		auto rhs = std::make_shared<mathEngine::exprs::constant>();
		lhs->value = a;
		rhs->value = b;
		add->terms = {lhs, rhs};
		return add;
	}
}

std::variant<mathEngine::constVal, std::shared_ptr<mathEngine::expr>> mathEngine::operator-(const mathEngine::constVal& a, const mathEngine::constVal& b){
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
		smul->terms = {minusOneConst, rhs};
		add->terms = {lhs, smul};
		return add;
	}
	return output;
}

std::variant<mathEngine::constVal, std::shared_ptr<mathEngine::expr>> mathEngine::operator*(const mathEngine::constVal& a, const mathEngine::constVal& b){
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
		mul->terms = {lhs, rhs};
		return mul;
	}
	return output;
}

std::variant<mathEngine::constVal, std::shared_ptr<mathEngine::expr>> mathEngine::operator/(const mathEngine::constVal& a, const mathEngine::constVal& b){
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
		mul->terms = {lhs, sexp};
		return mul;
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
	}else if(std::holds_alternative<constantName>(value)){
		return std::string(constantLatexNames[std::get<constantName>(value)]);
	}else{
		return "\\text{unknown constant}";
	}
}

mathEngine::constVal mathEngine::constVal::clone() const{
	return constVal{value};
}

std::size_t mathEngine::constVal::hash() const{
	if(std::holds_alternative<double>(value)){
		return std::hash<double>{}(std::get<double>(value));
	}else if(std::holds_alternative<rational>(value)){
		const auto& rat = std::get<rational>(value);
		std::size_t output = std::hash<int>{}(rat.num);
		mathEngine::hash_combine(output, rat.denom);
		return output;
	}else if(std::holds_alternative<constantName>(value)){
		return std::hash<int>{}(std::get<constantName>(value));
	}else{
		return -1;
	}
}

