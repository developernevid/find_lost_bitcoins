#pragma once
#include <string>
#include <memory>
#include <json.hpp>


struct IsTransactionSpent
{
	struct Data
	{

		std::string txid;
		size_t output_no;
		bool is_spent;

		struct Spent
		{
			std::string txid;

			NLOHMANN_DEFINE_TYPE_INTRUSIVE(Spent, txid)
		};
		std::unique_ptr<Spent> spent = nullptr;

		friend void to_json(nlohmann::json& j, const Data& p) {
			j = nlohmann::json
			{ 
				{"txid", p.txid},
				{"output_no", p.output_no},
				{"is_spent", p.is_spent},
				{"spent", p.spent ? *(p.spent) : Spent()}
			};
		}

		friend void from_json(const nlohmann::json& j, Data& p) {
			j.at("txid").get_to(p.txid);
			j.at("output_no").get_to(p.output_no);
			j.at("is_spent").get_to(p.is_spent);

			auto jSpent = j.at("spent");
			if (jSpent.is_null() || jSpent.empty())
			{
				p.spent = nullptr;
			}
			else
			{
				Spent spent;
				jSpent.get_to(spent);

				p.spent = std::make_unique<Spent>();
				p.spent->txid = spent.txid;
			}
		}

	} data;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(IsTransactionSpent, data)
};

