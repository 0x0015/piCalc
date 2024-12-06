#include "simplificationDatabase.hpp"
#include "../../parser/ptParse/ptParse.hpp"
#include "../../parser/parseUtil.hpp"
#include "../../mathEngine/expr.hpp"
#include <unordered_map>
#include <fstream>
#include <filesystem>
#include "../simplify.hpp"

std::unordered_map<std::size_t, std::pair<std::shared_ptr<mathEngine::expr>, std::shared_ptr<mathEngine::expr>>> simplificationDB;

//todo: add some sort of arbitrary variable and constant
//eg. int_x cos(x) = sin(x) <=> int_y cos(y) = sin(y) so make the actual variable name not matter
//similarly, int_x cos(a*x) = sin(a*x)/a for all a
void mathEngine::simplification::loadSimplificationDatabase(const std::string_view filename){
	std::ifstream file((std::string)filename);
	auto fileSize = std::filesystem::file_size(filename);
	std::string fileContent;
	fileContent.resize(fileSize);
	file.read(fileContent.data(), fileSize);
	auto lines = parser::strSplit(fileContent, "\n");
	if(lines.empty())
		return;
	for(unsigned int i=0;i<lines.size();i++){
		if(lines[i].empty())
			continue;
		auto parts = parser::strSplit(lines[i], "->");
		if(parts.size() != 2){
			std::cerr<<"Error loading simplification database: unable to determine sections at line "<<i<<std::endl;
			continue;
		}
		auto fromEq = parser::ptParse::parse(parts[0]);
		if(!fromEq || !std::holds_alternative<std::shared_ptr<mathEngine::expr>>(fromEq->value)){
			std::cerr<<"Error loading simplification database: unable to parse first equation at line"<<i<<std::endl;
			continue;
		}
		auto toEq = parser::ptParse::parse(parts[1]);
		if(!toEq || !std::holds_alternative<std::shared_ptr<mathEngine::expr>>(toEq->value)){
			std::cerr<<"Error loading simplification database: unable to parse second equation at line"<<i<<std::endl;
			continue;
		}
		auto fromExpr = std::get<std::shared_ptr<mathEngine::expr>>(fromEq->value);
		fromExpr = fullySimplify(fromExpr);
		auto toExpr = std::get<std::shared_ptr<mathEngine::expr>>(toEq->value);
		toExpr = fullySimplify(toExpr);
		simplificationDB[fromExpr->hash()] = {fromExpr, toExpr};
	}
}

std::shared_ptr<mathEngine::expr> mathEngine::simplification::simplifyByDatabase(std::shared_ptr<expr> exp){
	auto retVal = exp->propegateDFS_replace([](std::shared_ptr<expr> exp)->std::optional<std::shared_ptr<expr>>{
		auto expHash = exp->hash();
		if(simplificationDB.contains(expHash)){
			return simplificationDB[expHash].second->clone();
		}
		return std::nullopt;
	});
	return retVal;
}

