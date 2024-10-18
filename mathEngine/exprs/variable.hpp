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
				constVal eval() const override;
				void propegateDFS(const std::function<void(std::shared_ptr<expr>)>& func, bool includeConstants) override;
				std::string toLatex() const override;
				std::shared_ptr<expr> clone() const override;
				std::size_t hash() const;
		};
	}
}
