#pragma once
#include "../expr.hpp"
#include <memory>

namespace mathEngine{
	namespace exprs{
		class integral : public expr{
			public:
				constexpr static uint32_t typeID = COMPILE_TIME_CRC32_STR("integral");
				integral();
				std::shared_ptr<expr> expression;
				std::string wrtVar;
				static inline double dx = 0.001; //almost certainly egregously high
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
