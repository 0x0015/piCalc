#include "basicTokenize.hpp"
#include <iostream>
#include "../parseUtil.hpp"

std::string parser::basicToken::toString() const{
	return val + " @ " + source.toString();
}

std::optional<std::vector<parser::basicToken>> parseStringsAndComments(const std::string_view file){
	std::vector<parser::basicToken> output;
	output.push_back({});
	output.back().source.fileLocation = 0;
	output.back().source.fileSize = 0;
	//ignore filename for now
	for(unsigned int i=0;i<file.size();i++){
		if(file[i] == '\"'){
			output.push_back({});
			output.back().source.fileLocation = i;
			output.back().source.fileSize = 0;

			output.back().val += file[i];
			output.back().source.fileSize++;
			i++;
			bool foundEnd = false;
			for(;i<file.size();i++){
				output.back().val += file[i];
				output.back().source.fileSize++;
				if(file[i] == '\"'){
					foundEnd = true;
					i++;
					break;
				}
			}
			if(!foundEnd){
				std::cerr<<"Did not find end of string literal"<<std::endl;
				return std::nullopt;
			}
			output.push_back({});
			output.back().source.fileLocation = i;
			output.back().source.fileSize = 0;
		}else if(i+1 < file.size() && file.substr(i, 2) == "//"){
			i+=2;
			bool foundEnd = false;
			for(;i<file.size();i++){
				if(file[i] == '\n'){
					foundEnd = true;
					i++;
					break;
				}
			}
			if(foundEnd){
				output.push_back({});
				output.back().source.fileLocation = i;
				output.back().source.fileSize = 0;
			}
		}else if(i+1 < file.size() && file.substr(i, 2) == "/*"){
			i+=2;
			bool foundEnd = false;
			for(;i<file.size();i++){
				if(i+1 < file.size() && file.substr(i, 2) == "*/"){
					foundEnd = true;
					i+=2;
					break;
				}
			}
			if(!foundEnd){
				std::cerr<<"Did not find end of bounded comment"<<std::endl;
				return std::nullopt;
			}
			output.push_back({});
			output.back().source.fileLocation = i;
			output.back().source.fileSize = 0;
		}
		output.back().val += file[i];
		output.back().source.fileSize++;
	}

	if(!output.empty() && output.back().val.empty())
		output.pop_back();

	return output;
}

std::optional<std::vector<parser::basicToken>> parser::basicTokenize(const std::string_view contents){
	auto commentAndStringed = parseStringsAndComments(contents);

	if(!commentAndStringed)
		return std::nullopt;
	

	std::vector<basicToken> output;

	for(auto& part : *commentAndStringed){
		if(part.val[0] == '\"'){
			output.push_back(part);
			continue;
		}

		for(auto& i : part.val){
			constexpr char whitespace[] = {' ', '\t', '\n', '\r'};
			if(isInList(i, whitespace))
				i = ' ';
		}

		auto& contents_val = part.val;
		for(unsigned int i=0;i<contents_val.size();i++){
			if(contents_val[i] != ' '){
				if(i == 0 || contents_val[i-1] == ' '){
					if(!(!output.empty() && output.back().val.size() == 0)){
						output.push_back({});
						output.back().source.fileLocation = i+part.source.fileLocation;
						output.back().source.fileSize = 0;
					}
				}
				constexpr char singleCharToks[] = {'+', '-', '*', '/', '^', '(', ')', '{', '}', '[', ']', ';', ',', '.', '='};
				if(isInList(contents_val[i], singleCharToks)){
					if(output.back().val.size() == 0){
						output.back().val += contents_val[i];
						output.back().source.fileSize++;
						if(i+1 < contents_val.size()){
							output.push_back({});
							output.back().source.fileLocation = i+1+part.source.fileLocation;
							output.back().source.fileSize = 0;
						}
					}else{	
						output.push_back({});
						output.back().source.fileLocation = i+1+part.source.fileLocation;
						output.back().source.fileSize = 1;
						output.back().val += contents_val[i];
						if(i+1 < contents_val.size()){
						output.push_back({});
							output.back().source.fileLocation = i+1+part.source.fileLocation;
							output.back().source.fileSize = 0;
						}
					}
					continue;
				}
				output.back().val += contents_val[i];
				output.back().source.fileSize++;
			}
		}

		if(!output.empty() && output.back().val.empty())
			output.pop_back();

	}

	return output;
}

