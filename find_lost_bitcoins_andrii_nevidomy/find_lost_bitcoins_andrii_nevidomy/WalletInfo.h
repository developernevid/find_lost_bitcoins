#pragma once
#include "json.hpp"
#include <string>

struct WalletInfo
{
	struct Data
	{
		std::string confirmed_balance;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(Data, confirmed_balance)
	} data;

	double GetConfirmedBalance() const
	{
		return std::stod(data.confirmed_balance);
	}

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(WalletInfo, data)
};

