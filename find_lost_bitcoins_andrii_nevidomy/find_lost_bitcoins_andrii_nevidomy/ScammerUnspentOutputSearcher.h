#pragma once
#include "IScammerSearcher.h"
#include <vector>
#include <string>

class ScammerUnspentOutputSearcher :
	public IScammerSearcher
{
private:
	const size_t _maxdepth = 5;

public:
	ScammerUnspentOutputSearcher(size_t maxdepth);

	// Inherited via IScammerSearcher
	virtual std::vector<std::string> FindScammerWallets(const std::string& txid) override;
    std::vector<std::string> FindScammerWallets(const std::string& txId, const size_t currentDepth, const size_t maxDepth);
};

