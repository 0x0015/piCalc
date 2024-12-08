#include "reduceTrig.hpp"
#include "../exprs/constant.hpp"
#include "../exprs/exponent.hpp"
#include "../exprs/multiply.hpp"
#include "../exprs/sine.hpp"
#include "../exprs/cosine.hpp"
#include <unordered_map>

std::shared_ptr<mathEngine::expr> quickConstructConstant(const mathEngine::constVal& c){
	auto output = std::make_shared<mathEngine::exprs::constant>();
	output->value = c;
	return output;
}

std::shared_ptr<mathEngine::expr> quickConstructMinusOf(std::shared_ptr<mathEngine::expr> exp){
	auto minusOneConst = quickConstructConstant(mathEngine::constVal{rational{-1,1}});
	auto output = std::make_shared<mathEngine::exprs::multiply>();
	output->terms = {minusOneConst, exp};
	return output;
}

const std::unordered_map<rational, std::pair<std::shared_ptr<mathEngine::expr>, std::shared_ptr<mathEngine::expr>>> constructUnitCirclesValueList(){
	std::unordered_map<rational, std::pair<std::shared_ptr<mathEngine::expr>, std::shared_ptr<mathEngine::expr>>> circle;

	auto oneConst = quickConstructConstant(mathEngine::constVal{rational{1,1}});
	auto zeroConst = quickConstructConstant(mathEngine::constVal{rational{0,1}});
	auto minusOneConst = quickConstructConstant(mathEngine::constVal{rational{-1,1}});
	auto twoConst = quickConstructConstant(mathEngine::constVal{rational{2, 1}});
	auto threeConst = quickConstructConstant(mathEngine::constVal{rational{3, 1}});
	auto halfConst = quickConstructConstant(mathEngine::constVal{rational{1, 2}});
	auto thirdConst = quickConstructConstant(mathEngine::constVal{rational{1, 3}});
	auto sqrt2 = std::make_shared<mathEngine::exprs::exponent>();
	sqrt2->base = twoConst;
	sqrt2->exp = halfConst;
	auto sqrt3 = std::make_shared<mathEngine::exprs::exponent>();
	sqrt3->base = threeConst;
	sqrt3->exp = halfConst;
	auto sqrt2Over2 = std::make_shared<mathEngine::exprs::multiply>();
	sqrt2Over2->terms = {sqrt2, halfConst};
	auto sqrt3Over2 = std::make_shared<mathEngine::exprs::multiply>();
	sqrt3Over2->terms = {sqrt3, halfConst};

	//wholes and halves
	circle[rational{0,1}] = {oneConst, zeroConst};
	circle[rational{1,2}] = {zeroConst, oneConst};
	circle[rational{1,1}] = {minusOneConst, zeroConst};
	circle[rational{3,2}] = {zeroConst, minusOneConst};

	//quarters
	auto minusSqrt2Over2 = quickConstructMinusOf(sqrt2Over2);

	circle[rational{1, 4}] = {sqrt2Over2, sqrt3Over2};
	circle[rational{3, 4}] = {minusSqrt2Over2, sqrt2Over2};
	circle[rational{5, 4}] = {minusSqrt2Over2, minusSqrt2Over2};
	circle[rational{7, 4}] = {sqrt2Over2, minusSqrt2Over2};

	//sixths
	auto minusSqrt3Over2 = quickConstructMinusOf(sqrt3Over2);
	auto minusHalf = quickConstructConstant(mathEngine::constVal{rational{-1, 2}});

	circle[rational{1, 6}] = {sqrt3Over2, halfConst};
	circle[rational{1, 3}] = {halfConst, sqrt3Over2};
	circle[rational{2, 3}] = {minusHalf, sqrt3Over2};
	circle[rational{5, 6}] = {minusSqrt3Over2, halfConst};
	circle[rational{7, 6}] = {minusSqrt3Over2, minusHalf};
	circle[rational{4, 3}] = {minusHalf, minusSqrt3Over2};
	circle[rational{5, 3}] = {halfConst, minusSqrt3Over2};
	circle[rational{11, 6}] = {sqrt3Over2, minusHalf};

	return circle;
}

const std::unordered_map<rational, std::pair<std::shared_ptr<mathEngine::expr>, std::shared_ptr<mathEngine::expr>>> unitCircleValues{constructUnitCirclesValueList()};

std::optional<std::pair<std::shared_ptr<mathEngine::expr>, std::shared_ptr<mathEngine::expr>>> mathEngine::simplification::getTrigVals(std::shared_ptr<expr> exp){
	if(exp->isInstance<exprs::multiply>()){
		const auto& cE_mul = exp->getAs<exprs::multiply>();
		//check if there are two terms which are exactly a rational, and pi -> a rational multiple of pi -> simplifiable
		if(cE_mul->terms.size() == 2){
			if(cE_mul->terms[0]->isInstance<exprs::constant>() && cE_mul->terms[1]->isInstance<exprs::constant>()){
				const auto& piTerm = cE_mul->terms[0]->getAs<exprs::constant>();
				const auto& rationalTerm = cE_mul->terms[1]->getAs<exprs::constant>();
				if(std::holds_alternative<constantName>(piTerm->value.value) && std::holds_alternative<rational>(rationalTerm->value.value)){
					const auto& rationalTerm_rat = std::get<rational>(rationalTerm->value.value);
					const auto& piTerm_name = std::get<constantName>(piTerm->value.value);
					if(piTerm_name == constantName::PI && unitCircleValues.contains(rationalTerm_rat)){
						return unitCircleValues.at(rationalTerm_rat);
					}
				}
			}
		}
	}
	if(exp->isInstance<exprs::constant>()){
		const auto& constExp = exp->getAs<exprs::constant>();
		if(std::holds_alternative<constantName>(constExp->value.value)){
			const auto& constname = std::get<constantName>(constExp->value.value);
			if(constname == constantName::PI){
				return unitCircleValues.at(rational{1,1});
			}
		}
		if(std::holds_alternative<rational>(constExp->value.value)){	
			const auto& rat = std::get<rational>(constExp->value.value);
			if(rat == rational{0,1}){
				return unitCircleValues.at(rational{0,1});
			}
		}
	}

	return std::nullopt;
}


std::shared_ptr<mathEngine::expr> mathEngine::simplification::reduceTrig(std::shared_ptr<expr> exp){
	auto retVal = exp->propegateDFS_replace([](std::shared_ptr<expr> exp)->std::optional<std::shared_ptr<expr>>{
		if(exp->isInstance<exprs::sine>()){
			auto der = exp->getAs<exprs::sine>();
			const auto& evaluated = getTrigVals(der->inside);
			if(evaluated){
				return evaluated->second;
			}
		}
		if(exp->isInstance<exprs::cosine>()){
			auto der = exp->getAs<exprs::cosine>();
			const auto& evaluated = getTrigVals(der->inside);
			if(evaluated){
				return evaluated->first;
			}
		}
		return std::nullopt;
	});
	return retVal;
}

