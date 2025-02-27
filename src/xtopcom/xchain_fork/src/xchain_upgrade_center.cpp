// Copyright (c) 2017-2018 Telos Foundation & contributors
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.


#include "xchain_fork/xchain_upgrade_center.h"

#include "xconfig/xpredefined_configurations.h"
#include "xconfig/xconfig_register.h"
#include "xconfig/xchain_names.h"
#include "xvledger/xvblock.h"

#include <cinttypes>

namespace top {
    namespace chain_fork {

        xchain_fork_config_t      xtop_chain_fork_config_center::m_fork_config;

#if defined(XCHAIN_FORKED_BY_DEFAULT)
#   if defined(XBUILD_CI)
        const uint32_t BLOCK_FORK_POINT = 6000000;
#   else
        const uint32_t BLOCK_FORK_POINT = 0;
#   endif

            xchain_fork_config_t  mainnet_chain_config{
                xfork_point_t{xfork_point_type_t::logic_time, 0, "block fork point"},
                xfork_point_t{xfork_point_type_t::logic_time, 0, "blacklist function fork point"},
                xfork_point_t{xfork_point_type_t::logic_time, 0, "node_initial_credit_fork_point"},
                xfork_point_t{xfork_point_type_t::logic_time, BLOCK_FORK_POINT, "v3 block fork point"},
                xfork_point_t{xfork_point_type_t::logic_time, 0, "enable fullnode election"},
                xfork_point_t{xfork_point_type_t::logic_time, 0, "enable fullnode related func"},
            };

            // !!!change!!! fork time for galileo
            xchain_fork_config_t  testnet_chain_config{
                xfork_point_t{xfork_point_type_t::logic_time, 0, "block fork point"},
                xfork_point_t{xfork_point_type_t::logic_time, 0, "blacklist function fork point"},
                xfork_point_t{xfork_point_type_t::logic_time, 0, "node_initial_credit_fork_point"},
                xfork_point_t{xfork_point_type_t::logic_time, BLOCK_FORK_POINT, "v3 block fork point"},
                xfork_point_t{xfork_point_type_t::logic_time, 0, "enable fullnode election"},
                xfork_point_t{xfork_point_type_t::logic_time, 0, "enable fullnode related func"},
           };

            xchain_fork_config_t default_chain_config {
                xfork_point_t{xfork_point_type_t::logic_time, 0, "block fork point"},
                xfork_point_t{xfork_point_type_t::logic_time, 0, "blacklist function fork point"},
                xfork_point_t{xfork_point_type_t::logic_time, 0, "node_initial_credit_fork_point"},
                xfork_point_t{xfork_point_type_t::logic_time, BLOCK_FORK_POINT, "v3 block fork point"},
                xfork_point_t{xfork_point_type_t::logic_time, 0, "enable fullnode election"},
                xfork_point_t{xfork_point_type_t::logic_time, 0, "enable fullnode related func"},
            };
#else   // #if defined(XCHAIN_FORKED_BY_DEFAULT)
        xchain_fork_config_t  mainnet_chain_config{
            xfork_point_t{xfork_point_type_t::logic_time, 6859080, "block fork point"},
            xfork_point_t{xfork_point_type_t::logic_time, 6859080, "blacklist function fork point"},
            xfork_point_t{xfork_point_type_t::logic_time, 6859080, "node_initial_credit_fork_point"},
            xfork_point_t{xfork_point_type_t::logic_time, 7126740, "v3 block fork point"},
            xfork_point_t{xfork_point_type_t::logic_time, 7126740, "enable fullnode election"},
            xfork_point_t{xfork_point_type_t::logic_time, 7129260, "enable fullnode related func"},
        };

        // !!!change!!! fork time for galileo
        xchain_fork_config_t  testnet_chain_config{
            xfork_point_t{xfork_point_type_t::logic_time, 6859080, "block fork point"},
            xfork_point_t{xfork_point_type_t::logic_time, 6859080, "blacklist function fork point"},
            xfork_point_t{xfork_point_type_t::logic_time, 6859080, "node_initial_credit_fork_point"},
            xfork_point_t{xfork_point_type_t::logic_time, 7126740, "v3 block fork point"},
            xfork_point_t{xfork_point_type_t::logic_time, 7126740, "enable fullnode election"},
            xfork_point_t{xfork_point_type_t::logic_time, 7129260, "enable fullnode related func"},
        };

        // !!!change!!! fork time for local develop net
        xchain_fork_config_t default_chain_config {
            xfork_point_t{xfork_point_type_t::logic_time, 6859080, "block fork point"},
            xfork_point_t{xfork_point_type_t::logic_time, 6859080, "blacklist function fork point"},
            xfork_point_t{xfork_point_type_t::logic_time, 6859080, "node_initial_credit_fork_point"},
            xfork_point_t{xfork_point_type_t::logic_time, 7126740, "v3 block fork point"},
            xfork_point_t{xfork_point_type_t::logic_time, 7126740, "enable fullnode election"},
            xfork_point_t{xfork_point_type_t::logic_time, 7129260, "enable fullnode related func"},
        };
#endif  // #if defined(XCHAIN_FORKED_BY_DEFAULT)
        xchain_fork_config_t const & xtop_chain_fork_config_center::chain_fork_config() noexcept {
            if (top::config::chain_name_mainnet == XGET_CONFIG(chain_name)) {
                return mainnet_chain_config;
            } else if (top::config::chain_name_testnet == XGET_CONFIG(chain_name)) {
                return testnet_chain_config;
            }

            return default_chain_config;
        }

        bool xtop_chain_fork_config_center::is_forked(top::optional<xfork_point_t> const& fork_point, uint64_t target) noexcept {
            if (!fork_point.has_value()) {
                return false;
            }

            xdbg("xtop_chain_fork_config_center::is_forked target:%llu, fork point:%llu", target, fork_point.value().point);
            return  target >= fork_point.value().point;
        }

        bool xtop_chain_fork_config_center::is_forked(top::optional<xfork_point_t> const & fork_point, uint64_t fork_point_offset, uint64_t target) noexcept {
            auto const fork_point_value = (fork_point.has_value() ? fork_point.value().point : static_cast<uint64_t>(0)) + fork_point_offset;

            xdbg("xtop_chain_fork_config_center::is_forked target:%" PRIu64 ", fork point:%" PRIu64, target, fork_point_value);
            return target >= fork_point_value;
        }

        bool xtop_chain_fork_config_center::is_block_forked(uint64_t target) noexcept {
            xchain_fork_config_t const & _fork_config = xtop_chain_fork_config_center::get_chain_fork_config();
            return  xtop_chain_fork_config_center::is_forked(_fork_config.V3_0_0_0_block_fork_point, target);
        }

        bool xtop_chain_fork_config_center::is_tx_forked_by_timestamp(uint64_t fire_timestamp) noexcept {
            xchain_fork_config_t const & _fork_config = xtop_chain_fork_config_center::get_chain_fork_config();
            auto clock = _fork_config.block_fork_point.value().point;
            auto clock_time_stamp = clock * 10 + base::TOP_BEGIN_GMTIME;
            return fire_timestamp >= clock_time_stamp;
        }

        void xtop_chain_fork_config_center::init() {
            if (top::config::chain_name_mainnet == XGET_CONFIG(chain_name)) {
                m_fork_config = mainnet_chain_config;
                xinfo("xtop_chain_fork_config_center::init mainnet config");
            } else if (top::config::chain_name_testnet == XGET_CONFIG(chain_name)) {
                m_fork_config = testnet_chain_config;
                xinfo("xtop_chain_fork_config_center::init testnet config");
            } else {
                m_fork_config = default_chain_config;
                xinfo("xtop_chain_fork_config_center::init default config");
            }
        }
    }
}
