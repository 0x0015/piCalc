#pragma once
#include <variant>
#include "../expr.hpp"
#include "../rational.hpp"

namespace mathEngine{
	namespace exprs{
		class constant : public expr{
			public:
				constVal value;
				constVal eval() const override;
				double evalDouble() const override;
				void propegateDFS(const std::function<void(std::shared_ptr<expr>)>& func, bool includeConstants) override;
				std::shared_ptr<expr> propegateDFS_replace(const expr::DFS_replacement_functype& func, bool includeConstants) override;
				void propegateDFS_replace_internal(const expr::DFS_replacement_functype& func, bool includeConstants) override;
				std::string toLatex() const override;
				std::shared_ptr<expr> clone() const override;
				std::size_t hash() const override;
				std::string getTypeString() const override;
		};
	}
}
