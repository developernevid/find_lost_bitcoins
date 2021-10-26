#pragma once
#include <string>
#include <vector>

class IScammerSearcher
{
public:
	IScammerSearcher() = default;
	virtual std::vector<std::string> FindScammerWallets(const std::string& txid) = 0;
};

