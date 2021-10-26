#pragma once
#include "IScammerSearcher.h"
#include "BlockchainClient.h"

class NonFictiveWalletSearcher :
    public IScammerSearcher
{
public:
    // Inherited via IScammerSearcher
    NonFictiveWalletSearcher() = default;

	//moved implementation to header due to link error
	virtual std::vector<std::string> FindScammerWallets(const std::string& txid) override
	{
		std::vector<std::string> potentialScammerWallets;
		BlockchainClient client;
		const TransactionInfo transactionInfo = client.GetTransactionInfo(txid);
		if (transactionInfo.data.outputs.size() > 1)
		{
			assert(transactionInfo.data.inputs.size() > 0 && "unexpected transaction info");
			potentialScammerWallets.push_back(transactionInfo.data.inputs[0].address);
			return potentialScammerWallets;
		}
		if (transactionInfo.data.outputs.size() != 1)
			return potentialScammerWallets;

		const auto& output = transactionInfo.data.outputs[0];
		const WalletInfo walletInfo = client.GetWalletInfo(output.address);
		if (walletInfo.GetConfirmedBalance() > 0.)
		{
			potentialScammerWallets.push_back(output.address);
			return potentialScammerWallets;
		}

		IsTransactionSpent isSpent = client.GetIsTransactionSpent(transactionInfo.data.txid, output.output_no);
		if (!isSpent.data.is_spent)
		{
			potentialScammerWallets.push_back(output.address);
			return potentialScammerWallets;
		}

		auto resultToMerge = FindScammerWallets(isSpent.data.spent->txid);
		potentialScammerWallets.insert(potentialScammerWallets.end(), resultToMerge.begin(), resultToMerge.end());
		return potentialScammerWallets;
	}
};

