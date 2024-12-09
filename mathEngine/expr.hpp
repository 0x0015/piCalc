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
		uint32_t type = 0;
		virtual double evalDouble() const = 0;
		using DFS_functype = std::function<void(std::shared_ptr<expr>)>;
		virtual void propegateDFS(const DFS_functype& func) = 0;
		using DFS_replacement_functype = std::function<std::optional<std::shared_ptr<expr>>(std::shared_ptr<expr>)>;
		virtual std::shared_ptr<expr> propegateDFS_replace(const DFS_replacement_functype& func) = 0;
		virtual void propegateDFS_replace_internal(const DFS_replacement_functype& func) = 0;
		virtual std::string toLatex() const = 0;
		virtual std::shared_ptr<expr> clone() const = 0;
		virtual std::size_t hash() const = 0;
		virtual std::size_t hashTypeSig(bool allConstSame = true, std::optional<std::string_view> constWrtVar = std::nullopt) const = 0;
		virtual bool isConst(std::optional<std::string_view> wrtVar = std::nullopt) const = 0;
		virtual bool isEqual(const expr* other) const = 0;
		virtual std::string getTypeString() const = 0;
		virtual std::string toCode(const std::unordered_set<std::string>& wrtVars) const = 0;
		std::shared_ptr<expr> substiteVariable(const std::string& varName, std::shared_ptr<expr> subVal);
		template<typename T> constexpr bool isInstance() const{
			return type == T::typeID;
		}
		template<typename T> std::shared_ptr<T> getAs(){
			//maybe add a check here to make sure isInstance is true first.  Not sure what we'd do about it though (sorta too late)
			return std::static_pointer_cast<T>(shared_from_this());
		}
		template<typename T> std::shared_ptr<const T> getAs() const{
			//maybe add a check here to make sure isInstance is true first.  Not sure what we'd do about it though (sorta too late)
			return std::static_pointer_cast<const T>(shared_from_this());
		}
	};
}
