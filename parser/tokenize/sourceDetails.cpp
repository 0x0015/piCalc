#include "sourceDetails.hpp"

std::string parser::sourceInfo::toString() const{
	return std::to_string(fileLocation) + "-" + std::to_string(fileLocation+fileSize);
}
