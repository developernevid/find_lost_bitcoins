#pragma once
#include <string>
#include <json.hpp>


struct FromOutput
{
    std::string txid;
    size_t output_no;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(FromOutput, txid, output_no)
};

struct TransactionInput
{
    size_t input_no;
    std::string value;
    std::string address;
    FromOutput from_output;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(TransactionInput, input_no, value, address, from_output)
};

struct TransactionOutput
{
    size_t output_no;
    std::string value;
    std::string address;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(TransactionOutput, output_no, value, address)
};

struct TransactionInfo
{
    struct Data
    {
        std::string txid;
        std::vector<TransactionInput> inputs;
        std::vector<TransactionOutput> outputs;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Data, txid, inputs, outputs)
    } data;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(TransactionInfo, data)
};
