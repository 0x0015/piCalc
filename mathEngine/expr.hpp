#pragma once
#include <functional>
#include <memory>
#include "constVal.hpp"

namespace mathEngine{
	namespace exprs{
		class constant;
	}
	class expr : public std::enable_shared_from_this<expr>{
		public:
		virtual double evalDouble() const = 0;
		virtual constVal eval() const = 0;
		virtual void propegateDFS(const std::function<void(std::shared_ptr<expr>)>& func, bool includeConstants) = 0;
		virtual std::string toLatex() const = 0;
		virtual std::shared_ptr<expr> clone() const = 0;
		virtual std::size_t hash() const = 0;
	};
}
