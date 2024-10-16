#pragma once
#include <memory>
#include <functional>
#include "constVal.hpp"

namespace mathEngine{
	class expr;
	class equation{
	public:
		std::shared_ptr<expr> lhs, rhs;
		double evalUnsignedDiffDouble() const;
		double evalDiffDouble() const;
		constVal evalDiff() const;
		void propegateDFS(const std::function<void(std::shared_ptr<expr>)>& func, bool includeConstants);
		std::string toLatex() const;
	};
}
