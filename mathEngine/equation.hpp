#pragma once
#include <memory>
#include <optional>
#include <functional>
#include "constVal.hpp"

namespace mathEngine{
	class expr;
	class equation{
	public:
		std::shared_ptr<expr> lhs, rhs;
		double evalUnsignedDiffDouble() const;
		double evalDiffDouble() const;
		std::shared_ptr<expr> getDiff() const;
		void propegateDFS(const std::function<void(std::shared_ptr<expr>)>& func);
		using DFS_replacement_functype = std::function<std::optional<std::shared_ptr<expr>>(std::shared_ptr<expr>)>;
		virtual void propegateDFS_replace(const DFS_replacement_functype& func);
		std::string toLatex() const;
		equation clone() const;
		std::size_t hash() const;
		equation substiteVariable(const std::string& varName, std::shared_ptr<expr> subVal);
	};
}
