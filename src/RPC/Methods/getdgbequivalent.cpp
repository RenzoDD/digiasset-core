//
// Created by mctrivia on 17/03/24.
//

#include "AppMain.h"
#include "RPC/Response.h"
#include "RPC/Server.h"
#include <jsoncpp/json/value.h>

namespace RPC {
    namespace Methods {
        /**
        * Returns current DGB equivalent for an exchange amount
        * params[0] - address(string)
        * params[1] - index(unsigned int between 0 and 9)
        * params[2] - fixed precision amount to convert.  8 decimals(unsigned int)
        * Warning will throw error if sync is more then 120 blocks behind
        *
        * return DGB sats
        */
        extern const Response getdgbequivalent(const Json::Value& params) {
            AppMain* main=AppMain::GetInstance();
            if (params.size() != 3) throw DigiByteException(RPC_INVALID_PARAMS, "Invalid params");
            if (!params[0].isString()) throw DigiByteException(RPC_INVALID_PARAMS, "Invalid params");
            if (!params[1].isUInt()) throw DigiByteException(RPC_INVALID_PARAMS, "Invalid params");
            if (params[1].asUInt()>=10) throw DigiByteException(RPC_INVALID_PARAMS, "Invalid params");
            if (!params[2].isUInt64()) throw DigiByteException(RPC_INVALID_PARAMS, "Invalid params");
            if (main->getChainAnalyzer()->getSync() < -120) throw DigiByteException(RPC_MISC_ERROR, "To far behind to get current exchange rate");
            string address=params[0].asString();
            uint8_t index=params[1].asUInt();
            uint64_t amount=params[2].asUInt64();

            //get desired exchange rates
            Database* db = main->getDatabase();
            double rate = db->getCurrentExchangeRate({address,index});

            //calculate number of DGB sats
            Json::Value result= static_cast<Json::Int64>(ceil(amount*rate/100000000));

            //return response
            Response response;
            response.setResult(result);
            response.setBlocksGoodFor(5760); //day
            response.addInvalidateOnAddressChange(address);
            return response;
        }

    }
}