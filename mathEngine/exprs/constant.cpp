#include "constant.hpp"
#include <cmath>
#include "add.hpp"
#include "multiply.hpp"
#include "../hashCombine.hpp"

double mathEngine::exprs::constant::evalDouble() const{
	return value.toDouble();
}

mathEngine::constVal mathEngine::exprs::constant::eval() const{
	return value;
}

void mathEngine::exprs::constant::propegateDFS(const std::function<void(std::shared_ptr<expr>)>& func, bool includeConstants){
	func(shared_from_this());
	if(includeConstants && std::holds_alternative<std::shared_ptr<const expr>>(value.value)){
		auto constShared = std::get<std::shared_ptr<const expr>>(value.value);
		auto shared = std::const_pointer_cast<expr>(constShared); //illegal jank, but permissable for now
		shared->propegateDFS(func, includeConstants);
	}
}

std::string mathEngine::exprs::constant::toLatex() const{
	return value.toLatex();
}

std::shared_ptr<mathEngine::expr> mathEngine::exprs::constant::clone() const{
	auto output = std::make_shared<constant>();
	output->value = value.clone();
	return output;
}

std::size_t mathEngine::exprs::constant::hash() const{
	std::size_t outputHash = value.hash();
	mathEngine::hash_combine(outputHash, COMPILE_TIME_CRC32_STR("constant"));
	return outputHash;
}

