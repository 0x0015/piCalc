#pragma once
#include "../expr.hpp"
#include <memory>
#include <vector>

namespace mathEngine{
	namespace exprs{
		class multiply : public expr{
			public:
				std::vector<std::shared_ptr<expr>> terms;
				constVal eval() const override;
				double evalDouble() const override;
				void propegateDFS(const std::function<void(std::shared_ptr<expr>)>& func, bool includeConstants) override;
				std::shared_ptr<expr> propegateDFS_replace(const expr::DFS_replacement_functype& func, bool includeConstants) override;
				void propegateDFS_replace_internal(const expr::DFS_replacement_functype& func, bool includeConstants) override;
				std::string toLatex() const override;
				std::shared_ptr<expr> clone() const override;
				std::size_t hash() const override;
		};
	}
}
