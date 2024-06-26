//
// Created by mctrivia on 26/03/24.
//

#include "AppMain.h"
#include "RPC/Response.h"
#include "RPC/Server.h"
#include <jsoncpp/json/value.h>

namespace RPC {
    namespace Methods {
        /**
        * Returns address kyc information
        * params[0] - address(string)
        *
        * return an object with assetIndex as key and asset quantity as value.
        * Please note assetIndex 1 is DigiByte and this will be included if storenonassetutxo=1
        */
        extern const Response getaddressholdings(const Json::Value& params) {
            if (params.size() != 1) throw DigiByteException(RPC_INVALID_PARAMS, "Invalid params");
            if (!params[0].isString()) throw DigiByteException(RPC_INVALID_PARAMS, "Invalid params");

            string address=params[0].asString();

            //get desired exchange rates
            Database* db = AppMain::GetInstance()->getDatabase();
            auto data = db->getAddressHoldings(address);

            //convert to json
            Value result=Json::objectValue;
            for (const auto& entry: data) {
                result[to_string(entry.assetIndex)]=static_cast<Json::UInt64>(entry.count);
            }

            //return response
            Response response;
            response.setResult(result);
            response.setBlocksGoodFor(5760); //day
            response.addInvalidateOnAddressChange(address);
            return response;
        }

    }
}