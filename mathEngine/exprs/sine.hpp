#pragma once
#include "../expr.hpp"
#include <memory>

namespace mathEngine{
	namespace exprs{
		class sine : public expr{
			public:
				constexpr static uint32_t typeID = COMPILE_TIME_CRC32_STR("sine");
				sine();
				std::shared_ptr<expr> inside;
				double evalDouble() const override;
				void propegateDFS(const std::function<void(std::shared_ptr<expr>)>& func) override;
				std::shared_ptr<expr> propegateDFS_replace(const expr::DFS_replacement_functype& func) override;
				void propegateDFS_replace_internal(const expr::DFS_replacement_functype& func) override;
				std::string toLatex() const override;
				std::shared_ptr<expr> clone() const override;
				std::size_t hash() const override;
				std::size_t hashTypeSig(bool allConstSame = true, std::optional<std::string_view> constWrtVar = std::nullopt) const override;
				bool isConst(std::optional<std::string_view> wrtVar = std::nullopt) const override;
				bool isEqual(const expr* other) const override;
				std::string getTypeString() const override;
				std::string toCode(const std::unordered_set<std::string>& wrtVars) const override;
		};
	}
}
