#pragma once
#include "../expr.hpp"
#include <string>
#include <unordered_map>

namespace mathEngine{
	namespace exprs{
		class variable : public expr{
			public:
				constexpr static uint32_t typeID = COMPILE_TIME_CRC32_STR("variable");
				variable();
				static inline std::unordered_map<std::string, constVal> varVals;
				std::string name;
				double evalDouble() const override;
				void propegateDFS(const std::function<void(std::shared_ptr<expr>)>& func) override;
				std::shared_ptr<expr> propegateDFS_replace(const expr::DFS_replacement_functype& func) override;
				void propegateDFS_replace_internal(const expr::DFS_replacement_functype& func) override;
				std::string toLatex() const override;
				std::shared_ptr<expr> clone() const override;
				std::size_t hash() const override;
				std::string getTypeString() const override;
				std::string toCode(const std::unordered_set<std::string>& wrtVars) const override;
		};
	}
}
