#pragma once
#include <string>

namespace parser{
	struct sourceInfo{
		unsigned int fileLocation;
		unsigned int fileSize;
		std::string toString() const;
	};
}
