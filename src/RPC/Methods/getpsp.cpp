//
// Created by mctrivia on 27/03/24.
//

#include "AppMain.h"
#include "RPC/Response.h"
#include "RPC/Server.h"
#include <jsoncpp/json/value.h>

namespace RPC {
    namespace Methods {
        /**
        * Returns data about a PSP
        *  params[0] - index(unsigned int) - the psp index number
        */
        extern const Response getpsp(const Json::Value& params) {
            //make sure there is only 1 parameter
            if ( (params.size()!=1) || (!params[0].isUInt()) ) {
                throw DigiByteException(RPC_INVALID_PARAMS, "Invalid params");
            }

            //make sure valid index
            PermanentStoragePoolList* list=AppMain::GetInstance()->getPermanentStoragePoolList();
            if (params[0].asUInt()>=list->getPoolCount()) {
                throw DigiByteException(RPC_INVALID_PARAMS, "Invalid params");
            }

            //load the psp and return it to json
            Response response;
            response.setResult(list->getPool(params[0].asUInt())->toJSON());
            response.setBlocksGoodFor(5760);    //day
            response.setInvalidateOnNewAsset();
            return response;
        }
    }
}
