#include "parseConst.hpp"
#include <cmath>

parser::parseRes<std::variant<double,rational>> parseDecimal(std::span<const parser::mediumToken> tokens){
	if(tokens.size() < 3)
		return std::nullopt;

	unsigned int outputSize = 3;
	int isNegative = 1;
	if(tokens.size() > 3 && std::holds_alternative<parser::basicToken>(tokens.front().value) && std::get<parser::basicToken>(tokens.front().value).val == "-"){
		isNegative = -1;
		outputSize++;
		tokens = tokens.subspan(1);
	}

	const auto& beforeDotT = tokens[0].value;
	const auto& dotT = tokens[1].value;
	const auto& afterDotT = tokens[2].value;

	for(const auto& tok : {beforeDotT, dotT, afterDotT}){
		if(!std::holds_alternative<parser::basicToken>(tok))
			return std::nullopt;
	}

	if(std::get<parser::basicToken>(dotT).val != ".")
		return std::nullopt;

	if(std::get<parser::basicToken>(afterDotT).val.size() <= 3){
		//if there's 3 or less decimal places, try to interpret as a rational
		char* p;
		int conv = std::strtol((std::get<parser::basicToken>(beforeDotT).val + std::get<parser::basicToken>(afterDotT).val).c_str(), &p, 10);
		if(*p)
			return std::nullopt;
		constexpr int tenPowers[] = {0, 10, 100, 1000, 10000, 100000};
		return parser::makeParseRes<std::variant<double, rational>>(rational{conv * isNegative, tenPowers[std::get<parser::basicToken>(afterDotT).val.size()]}, outputSize);
	}

	char* p;
	double conv = std::strtod((std::get<parser::basicToken>(beforeDotT).val + "." + std::get<parser::basicToken>(afterDotT).val).c_str(), &p);
	if(*p)
		return std::nullopt;
	return parser::makeParseRes<std::variant<double,rational>>(conv * isNegative, outputSize);
}

parser::parseRes<rational> parseFraction(std::span<const parser::mediumToken> tokens){
	if(tokens.size() < 3)
		return std::nullopt;
	
	unsigned int outputSize = 3;
	int isNegative = 1;
	if(tokens.size() > 3 && std::holds_alternative<parser::basicToken>(tokens.front().value) && std::get<parser::basicToken>(tokens.front().value).val == "-"){
		isNegative = -1;
		outputSize++;
		tokens = tokens.subspan(1);
	}

	const auto& beforeSlashT = tokens[0].value;
	const auto& slashT = tokens[1].value;
	const auto& afterSlashT = tokens[2].value;

	for(const auto& tok : {beforeSlashT, slashT, afterSlashT}){
		if(!std::holds_alternative<parser::basicToken>(tok))
			return std::nullopt;
	}

	if(std::get<parser::basicToken>(slashT).val != "/")
		return std::nullopt;

	char* p;
	int top = std::strtol((std::get<parser::basicToken>(beforeSlashT).val).c_str(), &p, 10);
	if(*p)
		return std::nullopt;
	int bottom = std::strtol((std::get<parser::basicToken>(afterSlashT).val).c_str(), &p, 10);
	if(*p)
		return std::nullopt;
	return parser::makeParseRes(rational{top * isNegative, bottom}, outputSize);
}

parser::parseRes<rational> parseInt(std::span<const parser::mediumToken> tokens){
	if(tokens.empty())
		return std::nullopt;

	unsigned int outputSize = 1;
	int isNegative = 1;
	if(tokens.size() > 1 && std::holds_alternative<parser::basicToken>(tokens.front().value) && std::get<parser::basicToken>(tokens.front().value).val == "-"){
		isNegative = -1;
		outputSize++;
		tokens = tokens.subspan(1);
	}

	char* p;
	int val = std::strtol((std::get<parser::basicToken>(tokens.front().value).val).c_str(), &p, 10);
	if(*p)
		return std::nullopt;

	return parser::makeParseRes(rational{val * isNegative, 1}, outputSize);
}

parser::parseRes<mathEngine::constantName> parseUniversalConstant(std::span<const parser::mediumToken> tokens){
	if(tokens.empty())
		return std::nullopt;
	if(!std::holds_alternative<parser::basicToken>(tokens.front().value))
		return std::nullopt;

	const auto& name = std::get<parser::basicToken>(tokens.front().value).val;
	if(name == "pi")
		return parser::makeParseRes(mathEngine::PI, 1);
	if(name == "e")
		return parser::makeParseRes(mathEngine::E, 1);

	return std::nullopt;
}

parser::parseRes<std::shared_ptr<mathEngine::exprs::constant>> parser::parseConst(std::span<const parser::mediumToken> tokens){
	const auto& frac = parseFraction(tokens);
	if(frac){
		auto output = std::make_shared<mathEngine::exprs::constant>();
		output->value.value = frac->val;
		return makeParseRes(output, frac->toksConsumed);
	}

	const auto& decimal = parseDecimal(tokens);
	if(decimal){
		auto output = std::make_shared<mathEngine::exprs::constant>();
		if(std::holds_alternative<rational>(decimal->val))
			output->value.value = std::get<rational>(decimal->val);
		if(std::holds_alternative<double>(decimal->val))
			output->value.value = std::get<double>(decimal->val);
		return makeParseRes(output, decimal->toksConsumed);
	}

	const auto& in = parseInt(tokens);
	if(in){	
		auto output = std::make_shared<mathEngine::exprs::constant>();
		output->value.value = in->val;
		return makeParseRes(output, in->toksConsumed);
	}

	const auto& mathConst = parseUniversalConstant(tokens);
	if(mathConst){
		auto output = std::make_shared<mathEngine::exprs::constant>();
		output->value.value = mathConst->val;
		return makeParseRes(output, mathConst->toksConsumed);
	}

	return std::nullopt;
}
