#include <iostream>
#include "parser/ptParse/ptParse.hpp"
#include "mathEngine/expr.hpp"
#include "mathEngine/simplify.hpp"
#include "mathEngine/exprs/variable.hpp"
#include "parser/ptParse/parseConst.hpp"

int main(int argc, char** argv){
	std::string input;
	unsigned int lastArg = argc;
	bool doEval = false;
	if(argc >= 4 && std::string(argv[argc-2]) == "@"){
		lastArg -= 2;
		doEval = true;
	}
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
		auto& eq = std::get<mathEngine::equation>(parsedResult->value);
		std::cout<<"Got input equation: "<<eq.toLatex()<<std::endl;
		std::cout<<"Reduced to: "<<mathEngine::fullySimplify(eq).toLatex()<<std::endl;
	}else{
		std::shared_ptr<mathEngine::expr> exp = std::get<std::shared_ptr<mathEngine::expr>>(parsedResult->value);
		std::cout<<"Got input expression: "<<exp->toLatex()<<" (type string: "<<exp->getTypeString()<<")"<<std::endl;
		auto simplified = mathEngine::fullySimplify(exp);
		std::cout<<"Reduced to: "<<simplified->toLatex()<<" (type string: "<<simplified->getTypeString()<<")"<<std::endl;
		if(doEval){
			std::string evalAtArg = argv[argc-1];
			if(evalAtArg.size() >= 3 && evalAtArg[1] == '='){
				std::string evalVarName = evalAtArg.substr(0, 1);
				std::string evalVal = evalAtArg.substr(2);
				auto tokens = parser::basicTokenize(evalVal);
				if(tokens){
					auto mediumTokens = parser::parseBrackets(*tokens);
					if(mediumTokens){
						auto constant = parser::parseConst(*mediumTokens);
						mathEngine::exprs::variable::varVals[evalVarName] = constant->val->value;
						auto subbedEq = simplified->substiteVariable(evalVarName, constant->val);
						auto simplifiedSubbedEq = mathEngine::fullySimplify(subbedEq);
						std::cout<<"Evaluated at "<<evalVarName<<"="<<constant->val->value.toLatex()<<": "<<simplifiedSubbedEq->toLatex()<<" (decimal: "<<simplifiedSubbedEq->evalDouble()<<")"<<std::endl;
					}
				}
			}
		}
	}
};
