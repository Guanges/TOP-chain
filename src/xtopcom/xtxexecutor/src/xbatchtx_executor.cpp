// Copyright (c) 2017-2018 Telos Foundation & contributors
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <string>
#include <vector>
#include "xconfig/xconfig_register.h"
#include "xdata/xcons_transaction.h"
#include "xtxexecutor/xbatchtx_executor.h"
#include "xtxexecutor/xunit_service_error.h"
#include "xtxexecutor/xatomictx_executor.h"

NS_BEG2(top, txexecutor)

xbatchtx_executor_t::xbatchtx_executor_t(const statectx::xstatectx_face_ptr_t & statectx, const xvm_para_t & para)
: m_statectx(statectx), m_para(para) {

}

int32_t xbatchtx_executor_t::execute(const std::vector<xcons_transaction_ptr_t> & txs, std::vector<xatomictx_output_t> & outputs) {
    xatomictx_executor_t atomic_executor(m_statectx, m_para);
    xassert(!txs.empty());
    uint32_t eth_gas_limit = XGET_ONCHAIN_GOVERNANCE_PARAMETER(top_eth_transaction_gas_limit);
    uint32_t eth_gas_used = 0;
    for (auto & tx : txs) {
        if (eth_gas_limit - eth_gas_used < tx->get_transaction()->get_gaslimit())
        {
            break;
        }
        xatomictx_output_t output;
        output.m_tx = tx;
        enum_execute_result_type result = atomic_executor.execute(tx, output);
        output.m_result = result;
        if (eth_gas_limit - eth_gas_used < output.m_vm_output.m_tx_result.used_gas)
        {
            break;
        }
        eth_gas_used += output.m_vm_output.m_tx_result.used_gas;
        outputs.push_back(output);
        // xinfo("xbatchtx_executor_t::execute %s,batch_result=%d", output.dump().c_str(),result);
    }

    return xsuccess;
}

NS_END2
