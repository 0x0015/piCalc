#pragma once
#include "sourceDetails.hpp"
#include <string>
#include <optional>
#include <vector>

namespace parser{
	struct basicToken{
		std::string val;
		sourceInfo source;
		std::string toString() const;
	};
	
	std::optional<std::vector<basicToken>> basicTokenize(const std::string_view filename);
}
