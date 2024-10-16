#include <iostream>
#include "parser/ptParse/ptParse.hpp"
#include "mathEngine/expr.hpp"

int main(int argc, char** argv){
	std::string input;
	for(int i=1;i<argc;i++){
		input += argv[i];
		if(i+1 < argc)
			input += ' ';
	}
	if(input.empty()){
		std::cout<<"Error: no input"<<std::endl;
		return 0;
	}
	std::optional<parser::parseResult> parsedResult = parser::ptParse::parse(input);
	if(!parsedResult){
		std::cout<<"Unable to parse"<<std::endl;
		return 1;
	}
	if(std::holds_alternative<mathEngine::equation>(parsedResult->value)){
		std::cout<<"Got input: "<<std::get<mathEngine::equation>(parsedResult->value).toLatex()<<std::endl;
	}else{
		std::cout<<"Got input: "<<std::get<std::shared_ptr<mathEngine::expr>>(parsedResult->value)->toLatex()<<std::endl;
	}
};
