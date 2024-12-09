#include "simplificationDatabase.hpp"
#include "../../parser/ptParse/ptParse.hpp"
#include "../../parser/parseUtil.hpp"
#include "../../mathEngine/expr.hpp"
#include <unordered_map>
#include <fstream>
#include <filesystem>
#include "../simplify.hpp"

struct simplificationEntryDefs{
	std::string wrtVarName = "x";//if we don't know, we can only assume some default
	std::vector<std::string> unknownConstants;
};

struct simplificationDBEntry{
	simplificationEntryDefs defs;
	std::size_t fromHash;
	std::shared_ptr<mathEngine::expr> from;
	std::shared_ptr<mathEngine::expr> to;
};

std::unordered_map<std::size_t, simplificationDBEntry> simplificationDB;

namespace simplificationDB_impl{
std::string_view trim(const std::string_view str, const std::string_view whitespace = " "){
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return ""; // no content

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}
}

std::optional<simplificationEntryDefs> parseSimplificationEntryDefs(const std::string_view str, std::optional<int> errorLine = std::nullopt){
	auto lineParts = parser::strSplit(str, ",");
	simplificationEntryDefs output{};
	for(const auto& part : lineParts){
		auto defParts = parser::strSplit(simplificationDB_impl::trim(part), " ");
		if(defParts.size() != 2){
			std::cerr<<"Error loading simplification database: invalid def: \""<<part<<"\"";
			if(errorLine) std::cout<<" at line "<<*errorLine;
			std::cout<<std::endl;
			return std::nullopt;
		}
		const auto& defType = defParts[0];
		const auto& defName = defParts[1];
		if(defType == "var"){
			output.wrtVarName = defName;
		}else if(defType == "const"){
			output.unknownConstants.push_back((std::string)defName);
		}else{
			std::cerr<<"Error loading simplification database: invalid def type: \""<<defType<<"\"";
			if(errorLine) std::cout<<" at line "<<*errorLine;
			std::cout<<std::endl;
			return std::nullopt;
		}
	}
	return output;
}

std::optional<simplificationDBEntry> parseSimplificationDbEntry(const std::string_view str, std::optional<int> errorLine = std::nullopt){
	auto parts = parser::strSplit(str, ":");
	if(parts.size() != 2 && parts.size() != 1){
		std::cerr<<"Error loading simplification database: unable to determine defs vs eq section";
		if(errorLine) std::cout<<" at line "<<*errorLine;
		std::cout<<std::endl;
		return std::nullopt;
	}
	simplificationEntryDefs entryDefs{};
	std::string_view eqPart;
	if(parts.size() == 2){
		const auto& entryDefParse = parseSimplificationEntryDefs(parts[0]);
		if(!entryDefParse){
			return std::nullopt;
		}
		entryDefs = *entryDefParse;
		eqPart = parts[1];
	}else{
		eqPart = parts[0];
	}
	parts = parser::strSplit(eqPart, "->");
	if(parts.size() != 2){
		std::cerr<<"Error loading simplification database: unable to determine from vs to section";
		if(errorLine) std::cout<<" at line "<<*errorLine;
		std::cout<<std::endl;
		return std::nullopt;
	}
	auto fromEq = parser::ptParse::parse(parts[0]);
	if(!fromEq || !std::holds_alternative<std::shared_ptr<mathEngine::expr>>(fromEq->value)){
		std::cerr<<"Error loading simplification database: unable to parse first equation";
		if(errorLine) std::cout<<" at line "<<*errorLine;
		std::cout<<std::endl;
		return std::nullopt;
	}
	auto toEq = parser::ptParse::parse(parts[1]);
	if(!toEq || !std::holds_alternative<std::shared_ptr<mathEngine::expr>>(toEq->value)){
		std::cerr<<"Error loading simplification database: unable to parse second equation";
		if(errorLine) std::cout<<" at line "<<*errorLine;
		std::cout<<std::endl;
		std::cout<<std::endl;
	}
	auto fromExpr = std::get<std::shared_ptr<mathEngine::expr>>(fromEq->value);
	fromExpr = fullySimplify(fromExpr);
	auto toExpr = std::get<std::shared_ptr<mathEngine::expr>>(toEq->value);
	toExpr = fullySimplify(toExpr);
	return simplificationDBEntry{entryDefs, fromExpr->hash(), fromExpr, toExpr};
}

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
		const auto& parseLineTry = parseSimplificationDbEntry(lines[i], i);
		if(!parseLineTry)
			continue;
		simplificationDB[parseLineTry->from->hashTypeSig(true)] = *parseLineTry;
	}
}

//now change the logic below to actually make inferences about variable names and constant values
std::shared_ptr<mathEngine::expr> mathEngine::simplification::simplifyByDatabase(std::shared_ptr<expr> exp){
	auto retVal = exp->propegateDFS_replace([](std::shared_ptr<expr> exp)->std::optional<std::shared_ptr<expr>>{
		auto expTypeHash = exp->hashTypeSig(true);
		if(simplificationDB.contains(expTypeHash)){
			const auto& dbEntry = simplificationDB[expTypeHash];
			if(dbEntry.fromHash == exp->hash() && dbEntry.from->isEqual(exp.get()))
				return dbEntry.to;
		}
		return std::nullopt;
	});
	return retVal;
}

