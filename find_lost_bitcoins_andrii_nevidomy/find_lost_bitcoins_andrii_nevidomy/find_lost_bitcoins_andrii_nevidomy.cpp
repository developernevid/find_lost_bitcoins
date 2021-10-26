// find_lost_bitcoins_andrii_nevidomy.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define CURL_STATICLIB
#include <iostream>
#include <string>
#include <memory>
#include <unordered_map>
#include "curl.h"
#include "json.hpp"

#include "NonFictiveWalletSearcher.h"
#include "ScammerUnspentOutputSearcher.h"
#include "BlockchainClient.h"

void RunSearchers(const std::string& txid);

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "Unexpected count of arguments. You have to specify only transactionId. Example: <executableName> f02bade07bc44ca860889b1a27349f6aa5145ec575e586cc15ffe758f63b38ec"
			<< std::endl;
		return -1;
	}

	std::string txidArg = argv[1];
	try
	{
		RunSearchers(txidArg);
	}
	catch (const std::exception& ex)
	{
		std::cout << "Something went wrong. " << ex.what() << std::endl;
		return -1;
	}

	return 0;
}

void RunSearchers(const std::string&  txid)
{
	std::unordered_map<std::string, std::unique_ptr<IScammerSearcher>> searchersMap;
	const size_t MAX_DEPTH = 5;
	searchersMap["ScammerUnspentOutputSearcher(max_depth = 5)"] = std::make_unique<ScammerUnspentOutputSearcher>(MAX_DEPTH);
	searchersMap["NonFictiveWalletSearcher"] = std::make_unique<NonFictiveWalletSearcher>();

	for (const auto& searcherPair: searchersMap)
	{
		std::cout << "------------------------------------------------" << std::endl;
		std::cout << "Searcher " << searcherPair.first << ":"<< std::endl;

		auto addresses = searcherPair.second->FindScammerWallets(txid);
		if (addresses.empty())
			std::cout << "\t not found" << std::endl;
		for (const auto& address : addresses)
		{
			std::cout << "\t" << address << std::endl;
		}
		std::cout << "------------------------------------------------" << std::endl;
	}
}
