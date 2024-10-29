#include "reduceTrig.hpp"
#include "../exprs/add.hpp"
#include "../exprs/constant.hpp"
#include "../exprs/exponent.hpp"
#include "../exprs/multiply.hpp"
#include "../exprs/variable.hpp"
#include "../exprs/sine.hpp"
#include "../exprs/cosine.hpp"
#include <unordered_map>

const std::unordered_map<rational, std::pair<mathEngine::constVal, mathEngine::constVal>> constructUnitCirclesValueList(){
	std::unordered_map<rational, std::pair<mathEngine::constVal, mathEngine::constVal>> circle;

	auto oneConst = mathEngine::constVal{rational{1,1}};
	auto zeroConst = mathEngine::constVal{rational{0,1}};
	auto minusOneConst = mathEngine::constVal{rational{-1,1}};

	//most basic
	circle[rational{0,1}] = {oneConst, zeroConst};
	circle[rational{1,2}] = {zeroConst, oneConst};
	circle[rational{1,1}] = {minusOneConst, zeroConst};
	circle[rational{3,2}] = {zeroConst, minusOneConst};

	return circle;
}

const std::unordered_map<rational, std::pair<mathEngine::constVal, mathEngine::constVal>> unitCircleValues = constructUnitCirclesValueList();


std::optional<std::shared_ptr<mathEngine::expr>> mathEngine::simplification::simplifySin(std::shared_ptr<expr> exp){
	//simplify known rational multiples of pi
	if(dynamic_cast<exprs::constant*>(exp.get()) != nullptr){
		const auto& constExp = std::dynamic_pointer_cast<exprs::constant>(exp);
		if(std::holds_alternative<std::shared_ptr<const expr>>(constExp->value.value)){
			const auto& cE_exp = std::get<std::shared_ptr<const expr>>(constExp->value.value);
			if(dynamic_cast<const exprs::multiply*>(cE_exp.get()) != nullptr){
				const auto& cE_mul = std::dynamic_pointer_cast<const exprs::multiply>(cE_exp);
				//check if there are two terms which are exactly a rational, and pi -> a rational multiple of pi -> simplifiable
				if(cE_mul->terms.size() == 2){
					if(dynamic_cast<const exprs::constant*>(cE_mul->terms[0].get()) != nullptr && dynamic_cast<const exprs::constant*>(cE_mul->terms[1].get()) != nullptr){
						const auto& piTerm = std::dynamic_pointer_cast<const exprs::constant>(cE_mul->terms[0]);
						const auto& rationalTerm = std::dynamic_pointer_cast<const exprs::constant>(cE_mul->terms[1]);
						if(std::holds_alternative<constantName>(piTerm->value.value) && std::holds_alternative<rational>(rationalTerm->value.value)){
							const auto& rationalTerm_rat = std::get<rational>(rationalTerm->value.value);
							const auto& piTerm_name = std::get<constantName>(piTerm->value.value);
							if(piTerm_name == constantName::PI && unitCircleValues.contains(rationalTerm_rat)){
								auto output = std::make_shared<exprs::constant>();
								output->value = unitCircleValues.at(rationalTerm_rat).second;
								return output;
							}
						}
					}
				}
			}
		}
		if(std::holds_alternative<constantName>(constExp->value.value)){
			const auto& constname = std::get<constantName>(constExp->value.value);
			if(constname == constantName::PI){
				auto output = std::make_shared<exprs::constant>();
				output->value = unitCircleValues.at(rational{1,1}).second;
				return output;
			}
		}
		if(std::holds_alternative<rational>(constExp->value.value)){	
			const auto& rat = std::get<rational>(constExp->value.value);
			if(rat == rational{0,1}){
				auto output = std::make_shared<exprs::constant>();
				output->value = unitCircleValues.at(rational{0,1}).second;
				return output;
			}
		}
	}

	//todo: add sin sum formula

	return std::nullopt;
}

//basically copy of sin code.  Maybe make general trig lookup function and then take first or second?  TODO: refactor later
std::optional<std::shared_ptr<mathEngine::expr>> mathEngine::simplification::simplifyCos(std::shared_ptr<expr> exp){	//simplify known rational multiples of pi
	if(dynamic_cast<exprs::constant*>(exp.get()) != nullptr){
		const auto& constExp = std::dynamic_pointer_cast<exprs::constant>(exp);
		if(std::holds_alternative<std::shared_ptr<const expr>>(constExp->value.value)){
			const auto& cE_exp = std::get<std::shared_ptr<const expr>>(constExp->value.value);
			if(dynamic_cast<const exprs::multiply*>(cE_exp.get()) != nullptr){
				const auto& cE_mul = std::dynamic_pointer_cast<const exprs::multiply>(cE_exp);
				//check if there are two terms which are exactly a rational, and pi -> a rational multiple of pi -> simplifiable
				if(cE_mul->terms.size() == 2){
					if(dynamic_cast<const exprs::constant*>(cE_mul->terms[0].get()) != nullptr && dynamic_cast<const exprs::constant*>(cE_mul->terms[1].get()) != nullptr){
						const auto& piTerm = std::dynamic_pointer_cast<const exprs::constant>(cE_mul->terms[0]);
						const auto& rationalTerm = std::dynamic_pointer_cast<const exprs::constant>(cE_mul->terms[1]);
						if(std::holds_alternative<constantName>(piTerm->value.value) && std::holds_alternative<rational>(rationalTerm->value.value)){
							const auto& rationalTerm_rat = std::get<rational>(rationalTerm->value.value);
							const auto& piTerm_name = std::get<constantName>(piTerm->value.value);
							if(piTerm_name == constantName::PI && unitCircleValues.contains(rationalTerm_rat)){
								auto output = std::make_shared<exprs::constant>();
								output->value = unitCircleValues.at(rationalTerm_rat).first;
								return output;
							}
						}
					}
				}
			}
		}
		if(std::holds_alternative<constantName>(constExp->value.value)){
			const auto& constname = std::get<constantName>(constExp->value.value);
			if(constname == constantName::PI){
				auto output = std::make_shared<exprs::constant>();
				output->value = unitCircleValues.at(rational{1,1}).first;
				return output;
			}
		}
		if(std::holds_alternative<rational>(constExp->value.value)){	
			const auto& rat = std::get<rational>(constExp->value.value);
			if(rat == rational{0,1}){
				auto output = std::make_shared<exprs::constant>();
				output->value = unitCircleValues.at(rational{0,1}).first;
				return output;
			}
		}
	}

	//todo: add cos sum formula

	return std::nullopt;
}

std::shared_ptr<mathEngine::expr> mathEngine::simplification::reduceTrig(std::shared_ptr<expr> exp){
	auto retVal = exp->propegateDFS_replace([](std::shared_ptr<expr> exp)->std::optional<std::shared_ptr<expr>>{
		if(dynamic_cast<exprs::sine*>(exp.get()) != nullptr){
			auto der = std::dynamic_pointer_cast<exprs::sine>(exp);
			auto evaluated = simplifySin(der->inside);
			if(evaluated){
				return *evaluated;
			}
		}
		if(dynamic_cast<exprs::cosine*>(exp.get()) != nullptr){
			auto der = std::dynamic_pointer_cast<exprs::cosine>(exp);
			auto evaluated = simplifyCos(der->inside);
			if(evaluated){
				return *evaluated;
			}
		}
		return std::nullopt;
	}, true);
	return retVal;
}

