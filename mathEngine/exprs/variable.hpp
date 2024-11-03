#pragma once
#include "../expr.hpp"
#include <string>
#include <unordered_map>

namespace mathEngine{
	namespace exprs{
		class variable : public expr{
			public:
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
		};
	}
}
