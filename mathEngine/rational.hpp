//adapted from https://stackoverflow.com/questions/68212028/a-class-for-rational-number-p-q-with-overloading-and-operator
#pragma once
#include <iostream>
#include <numeric>
#include <cmath>
#include "hashCombine.hpp"

class rational{
public:
	int num;
	int denom;
	constexpr rational(int p = 1, int q = 1) : num(p), denom(q) {
		reduce();
	}
	constexpr void reduce(){
		int devisor = std::gcd(num, denom);
		num /= devisor;
		denom /= devisor;
	}
	constexpr double toDouble() const{
		return (double)num/(double)denom;
	}
	inline std::string toString() const{
		return std::to_string(num) + "/" + std::to_string(denom);
	}
};

constexpr rational operator+(const rational r1, rational r2){
	rational temp;
	if(r1.denom == r2.denom)
	{
	    temp.num = r1.num + r2.num;
	    temp.denom = r1.denom;
	}
	else
	{
	    temp.num = ((r1.num) * (r2.denom)) + ((r2.num) * (r1.denom));
	    temp.denom = (r1.denom) * (r2.denom);
	}
	temp.reduce();
	return temp;
}

constexpr rational operator*(rational r1, rational r2){
	rational temp;
	temp.num = r1.num * r2.num;
	temp.denom = r1.denom * r2.denom;
	temp.reduce();
	return temp;
}

constexpr rational operator-(rational r1, rational r2){
	return r1 + rational(-r2.num, r2.denom);
}

constexpr rational operator/(rational r1, rational r2){
	return r1 * rational(r2.denom, r2.num);
}

constexpr bool operator==(rational r1, rational r2){
	return r1.num == r2.num && r1.denom == r2.denom;
}

inline std::ostream & operator<<(std::ostream &out, rational r3)
{
    out<<r3.num<<"/"<<r3.denom;
    return out;
}

template <> struct std::hash<rational>
{
	std::size_t operator()(const rational& k) const
	{
		std::size_t hash = std::hash<int>{}(k.num);
		mathEngine::hash_combine(hash, k.denom);
		return hash;
	}
};

