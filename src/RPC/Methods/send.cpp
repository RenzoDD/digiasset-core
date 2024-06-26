//
// Created by mctrivia on 17/03/24.
//

#include "AppMain.h"
#include "DigiByteDomain.h"
#include "RPC/Response.h"
#include "RPC/Server.h"
#include <jsoncpp/json/value.h>

namespace RPC {
    namespace Methods {
        /**
        * params - see https://developer.bitcoin.org/reference/rpc/send.html
        * only difference is we now accept domains
        */
        extern const Response send(const Json::Value& params) {

            if (params.size() < 1 || params.size() > 5) {
                throw DigiByteException(RPC_INVALID_PARAMS, "Invalid params");
            }
            if (!params[0].isArray()) throw DigiByteException(RPC_INVALID_PARAMS, "Invalid params");

            //check if any domains in outputs
            Value newParams = params;
            for (Value& output: newParams[0]) {
                auto it = output.begin();
                string key = it.name();
                if (DigiByteDomain::isDomain(key)) {
                    //change the domain into an address
                    string newKey = DigiByteDomain::getAddress(key);
                    output[newKey] = *it;
                    output.removeMember(key);
                }
            }

            //send modified params to wallet
            Json::Value result=AppMain::GetInstance()->getDigiByteCore()->sendcommand("send", newParams);

            //return response
            Response response;
            response.setResult(result);
            response.setBlocksGoodFor(-1); //do not cache
            return response;
        }

    }
}