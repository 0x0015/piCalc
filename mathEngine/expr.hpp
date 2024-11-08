#pragma once
#include <functional>
#include <memory>
#include <optional>
#include <unordered_set>
#include "constVal.hpp"

namespace mathEngine{
	namespace exprs{
		class constant;
	}
	class expr : public std::enable_shared_from_this<expr>{
		public:
		virtual double evalDouble() const = 0;
		using DFS_functype = std::function<void(std::shared_ptr<expr>)>;
		virtual void propegateDFS(const DFS_functype& func) = 0;
		using DFS_replacement_functype = std::function<std::optional<std::shared_ptr<expr>>(std::shared_ptr<expr>)>;
		virtual std::shared_ptr<expr> propegateDFS_replace(const DFS_replacement_functype& func) = 0;
		virtual void propegateDFS_replace_internal(const DFS_replacement_functype& func) = 0;
		virtual std::string toLatex() const = 0;
		virtual std::shared_ptr<expr> clone() const = 0;
		virtual std::size_t hash() const = 0;
		virtual std::string getTypeString() const = 0;
		virtual std::string toCode(const std::unordered_set<std::string>& wrtVars) const = 0;
		std::shared_ptr<expr> substiteVariable(const std::string& varName, std::shared_ptr<expr> subVal);
	};
}
