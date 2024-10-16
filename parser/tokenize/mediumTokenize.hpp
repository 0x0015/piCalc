#pragma once
#include <vector>
#include <variant>
#include "basicTokenize.hpp"

namespace parser{
	struct mediumToken{
		struct tokList{
			enum type_t{
				PAREN,
				CURL_BRACK,
				SQUARE_BRACK
			};
			static std::string_view type_t_to_string(const type_t& ty);
			type_t type;
			std::vector<mediumToken> value;
		};
		std::variant<tokList, basicToken> value;
		std::string toString() const;
	
		static inline int currentRecur = 0;
		std::string toString_internal() const;
	};
	
	std::optional<std::vector<mediumToken>> parseBrackets(const std::vector<basicToken>& toks);
}
