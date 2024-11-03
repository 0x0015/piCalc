#pragma once
#include "../expr.hpp"
#include <memory>
#include <vector>

namespace mathEngine{
	namespace exprs{
		class add : public expr{
			public:
				std::vector<std::shared_ptr<expr>> terms;
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
