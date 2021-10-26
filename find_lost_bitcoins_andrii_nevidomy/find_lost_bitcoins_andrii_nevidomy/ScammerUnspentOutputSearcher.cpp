#include "ScammerUnspentOutputSearcher.h"
#include "BlockchainClient.h"


ScammerUnspentOutputSearcher::ScammerUnspentOutputSearcher(size_t maxdepth) : _maxdepth(maxdepth) {}

std::vector<std::string> ScammerUnspentOutputSearcher::FindScammerWallets(const std::string& txid)
{
    return FindScammerWallets(txid, 0, _maxdepth);
}

std::vector<std::string> ScammerUnspentOutputSearcher::FindScammerWallets(const std::string& txId, const size_t currentDepth, const size_t maxDepth)
{
    if (currentDepth > maxDepth)
    {
        return std::vector<std::string>();
    }

    std::vector<std::string> potentialScammerWallets;
    BlockchainClient client;
    const TransactionInfo rootTransaction = client.GetTransactionInfo(txId);

    for (const auto& output : rootTransaction.data.outputs)
    {
        IsTransactionSpent isSpent = client.GetIsTransactionSpent(rootTransaction.data.txid, output.output_no);
        if (!isSpent.data.is_spent)
        {
            potentialScammerWallets.push_back(output.address);
            continue;
        }

        auto resultToMerge = FindScammerWallets(isSpent.data.spent->txid, currentDepth + 1, maxDepth);
        potentialScammerWallets.insert(potentialScammerWallets.end(), resultToMerge.begin(), resultToMerge.end());
    }

    return potentialScammerWallets;
}

