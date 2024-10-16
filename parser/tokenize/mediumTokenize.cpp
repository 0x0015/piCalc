#include "mediumTokenize.hpp"
#include <iostream>
#include "../parseUtil.hpp"

std::string_view parser::mediumToken::tokList::type_t_to_string(const type_t& ty){
	switch(ty){
		case PAREN:
			return "PAREN";
		case CURL_BRACK:
			return "CURL_BRACK";
		case SQUARE_BRACK:
			return "SQUARE_BRACK";
		default:
			return "NONE";
	}
}

std::string parser::mediumToken::toString() const{
	currentRecur = -1;
	return toString_internal();
}

std::string parser::mediumToken::toString_internal() const{
	currentRecur++;

	std::string output;
	if(std::holds_alternative<mediumToken::tokList>(value)){
		const auto& list = std::get<mediumToken::tokList>(value);
		for(int i=0;i<currentRecur;i++)
			output += '\t';
		output += "Token list: " + std::string(mediumToken::tokList::type_t_to_string(list.type));
		if(!list.value.empty())
			output += '\n';
		for(unsigned int i=0;i<list.value.size();i++){
			output += list.value[i].toString_internal();
			if(i+1<list.value.size())
				output += '\n';
		}
	}else if(std::holds_alternative<basicToken>(value)){
		for(int i=0;i<currentRecur;i++)
			output += '\t';
		output += std::get<basicToken>(value).toString();
	}else{
		std::cerr<<"Unknown variant error (mediumToken)"<<std::endl;
	}

	currentRecur--;
	return output;
}

std::optional<parser::mediumToken::tokList> parseUntilEndbrack(const std::vector<parser::basicToken>& toks, unsigned int& i, char closing);

std::optional<parser::mediumToken> parseMediumToken(const std::vector<parser::basicToken>& toks, unsigned int& i){
	if(toks[i].val == "("){
		auto parseRes = parseUntilEndbrack(toks, i, ')');
		if(!parseRes)
			return std::nullopt;
		parseRes->type = parser::mediumToken::tokList::PAREN;
		return parser::mediumToken{*parseRes};
	}else if(toks[i].val == "{"){
		auto parseRes = parseUntilEndbrack(toks, i, '}');
		if(!parseRes)
			return std::nullopt;
		parseRes->type = parser::mediumToken::tokList::CURL_BRACK;
		return parser::mediumToken{*parseRes};
	}else if(toks[i].val == "["){
		auto parseRes = parseUntilEndbrack(toks, i, ']');
		if(!parseRes)
			return std::nullopt;
		parseRes->type = parser::mediumToken::tokList::SQUARE_BRACK;
		return parser::mediumToken{*parseRes};
	}else{
		return parser::mediumToken{toks[i]};
	}
}

std::optional<parser::mediumToken::tokList> parseUntilEndbrack(const std::vector<parser::basicToken>& toks, unsigned int& i, char closing){
	i++;
	parser::mediumToken::tokList output;
	bool foundClosing = false;
	for(;i<toks.size();i++){
		if(toks[i].val.size() == 1 && toks[i].val[0] == closing){
			foundClosing = true;
			break;
		}
		auto parsed = parseMediumToken(toks, i);
		if(parsed)
			output.value.push_back(*parsed);
	}

	if(!foundClosing){
		std::cout<<"Error: did not find closing \""<<closing<<"\"";
		return std::nullopt;
	}

	return output;
}

std::optional<std::vector<parser::mediumToken>> parser::parseBrackets(const std::vector<parser::basicToken>& toks){
	std::vector<parser::mediumToken> output;

	for(unsigned int i=0;i<toks.size();i++){
		auto parsed = parseMediumToken(toks, i);
		if(parsed)
			output.push_back(*parsed);
	}

	return output;
}

