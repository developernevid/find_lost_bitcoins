#pragma once
#include <string>
#include "TransactionInfo.h"
#include "IsTransactionSpent.h"
#include "WalletInfo.h"
#include "curl.h"

// All rest api provided here: https://sochain.com/api
class BlockchainClient
{
private:
	static size_t WriterFn(void* buffer, size_t size, size_t nmemb, void* param)
	{
		std::string& text = *static_cast<std::string*>(param);
		size_t totalsize = size * nmemb;
		text.append(static_cast<char*>(buffer), totalsize);
		return totalsize;
	}

	template <class T>
	T GetRequest(const std::string& url) const
	{
		CURL* curl = nullptr;
		curl = curl_easy_init();
		assert(curl && "curl hasn't been init");
		CURLcode res;

		auto deleter = [](CURL* curl)
		{
			if (curl) curl_easy_cleanup(curl);
		};
		auto uptr_curl = std::unique_ptr<CURL, decltype(deleter)>(curl, deleter);

		std::string result;
		if (uptr_curl) {
			curl_easy_setopt(uptr_curl.get(), CURLOPT_CUSTOMREQUEST, "GET");
			curl_easy_setopt(uptr_curl.get(), CURLOPT_WRITEFUNCTION, WriterFn);
			curl_easy_setopt(uptr_curl.get(), CURLOPT_WRITEDATA, &result);
			curl_easy_setopt(uptr_curl.get(), CURLOPT_URL, url.c_str());
			curl_easy_setopt(uptr_curl.get(), CURLOPT_FOLLOWLOCATION, 1L);
			curl_easy_setopt(uptr_curl.get(), CURLOPT_DEFAULT_PROTOCOL, "https");
			res = curl_easy_perform(uptr_curl.get());
		}

		switch (res)
		{
		case 0: // OK
		{
			using namespace nlohmann;
			try
			{
				json jObject = json::parse(result);
				T deserializedObj = jObject.get<T>();
				return deserializedObj;
			}
			catch (const std::exception& ex)
			{
				const std::string message = "Json parsing has been failed... Raw content: " + result;
				throw std::exception();
			}
		}
		default:
			const std::string message = "Unexpected responce from the endpoint "
				+ url
				+ ". Error code "
				+ std::to_string(res);

			throw std::exception(message.c_str());
		}
	}

public:
	BlockchainClient() = default;

	TransactionInfo GetTransactionInfo(const std::string& txid) const
	{
		const std::string endpoint = "https://chain.so/api/v2/get_tx/Bitcoin/" + txid;
		TransactionInfo transactionInfo = GetRequest<TransactionInfo>(endpoint);
		return transactionInfo;
	}

	IsTransactionSpent GetIsTransactionSpent(const std::string& txid, size_t output_no) const
	{
		const std::string endpoint = "https://chain.so/api/v2/is_tx_spent/Bitcoin/" + txid + "/" + std::to_string(output_no);
		auto isTransactionSpent = GetRequest<IsTransactionSpent>(endpoint);
		return isTransactionSpent;
	}

	WalletInfo GetWalletInfo(const std::string& walletId) const
	{
		const std::string endpoint = "https://chain.so/api/v2/get_address_balance/Bitcoin/" + walletId;
		auto walletInfo = GetRequest<WalletInfo>(endpoint);
		return walletInfo;
	}
};

