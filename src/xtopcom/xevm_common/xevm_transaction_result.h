// Copyright (c) 2017-2021 Telos Foundation & contributors
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include "xbasic/xbyte_buffer.h"
#include "xevm_common/fixed_hash.h"
#include "xevm_common/address.h"
#include "xutility/xhash.h"
#include <string>
#include <vector>

NS_BEG2(top, evm_common)

struct xevm_log_t {
    top::evm_common::Address address;
    top::evm_common::h256s topics; // hex string
    std::string data; // hex string
};
/// same as TransactionStatus in `evm_engine_rs/engine/src/parameters.rs`
enum xevm_transaction_status_t : uint32_t {
    Success = 0,
    Revert = 1,
    /// Execution runs out of gas (runtime).
    OutOfGas = 2,
    /// Not enough fund to start the execution (runtime).
    OutOfFund = 3,
    /// An opcode accesses external information, but the request is off offset limit (runtime).
    OutOfOffset = 4,
    OtherExecuteError = 5,
};

class xevm_transaction_result_t {
 public:
    xevm_transaction_result_t(){}
    xevm_transaction_result_t(const xevm_transaction_result_t & evm_transaction_result) {
        used_gas = evm_transaction_result.used_gas;
        status = evm_transaction_result.status;
        extra_msg = evm_transaction_result.extra_msg;
        logs = evm_transaction_result.logs;
    }

    xevm_transaction_result_t & operator = (const xevm_transaction_result_t & evm_transaction_result) {
        used_gas = evm_transaction_result.used_gas;
        status = evm_transaction_result.status;
        extra_msg = evm_transaction_result.extra_msg;
        logs = evm_transaction_result.logs;
        return *this;
    }

public:
    uint64_t used_gas{0}; // todo: calculate used gas to expense
    xevm_transaction_status_t status;
    std::string extra_msg;
    std::vector<xevm_log_t> logs;

    void set_status(uint32_t input) {
        status = static_cast<xevm_transaction_status_t>(input);
    }

    top::evm_common::h2048 get_logsbloom() {
        top::evm_common::h2048  logsbloom;
        for (auto & log : logs) {
            top::evm_common::h2048 bloom;
            top::uint256_t hash = top::utl::xkeccak256_t::digest(log.address.data(), log.address.size);
            top::evm_common::h256 hash_h256;
            top::evm_common::bytesConstRef((const unsigned char *)hash.data(), hash.size()).copyTo(hash_h256.ref());
            bloom.shiftBloom<3>(hash_h256);
            logsbloom |= bloom;

            for (auto & topic : log.topics) {
                top::evm_common::h2048 bloom;
                top::uint256_t hash = top::utl::xkeccak256_t::digest(topic.data(), topic.size);
                top::evm_common::h256 hash_h256;
                top::evm_common::bytesConstRef((const unsigned char *)hash.data(), hash.size()).copyTo(hash_h256.ref());
                bloom.shiftBloom<3>(hash_h256);
                logsbloom |= bloom;
            }
        }
        return logsbloom;
    }

    // debug
    std::string dump_info() {
        return "transaction_result[status:" + std::to_string(static_cast<std::underlying_type<xevm_transaction_status_t>::type>(status)) + ", extra_msg:" + extra_msg +
               "], logs.size():" + std::to_string(logs.size());
    }
};

class xevm_transaction_receipt_t {
 public:
    xevm_transaction_receipt_t(){}
    xevm_transaction_receipt_t(const xevm_transaction_receipt_t & evm_transaction_receipt) {
        used_gas = evm_transaction_receipt.used_gas;
        status = evm_transaction_receipt.status;
        extra_msg = evm_transaction_receipt.extra_msg;
        logs = evm_transaction_receipt.logs;
    }

    xevm_transaction_receipt_t & operator = (const xevm_transaction_receipt_t & evm_transaction_receipt) {
        used_gas = evm_transaction_receipt.used_gas;
        status = evm_transaction_receipt.status;
        extra_msg = evm_transaction_receipt.extra_msg;
        logs = evm_transaction_receipt.logs;
        return *this;
    }

public:
    xevm_transaction_status_t status;
    uint64_t cumulative_gas_used;
    std::vector<xevm_log_t> logs;
    top::evm_common::h265   tx_hash;
    top::evm_common::Address contract_address;
    uint64_t used_gas{0}; // todo: calculate used gas to expense
    std::string extra_msg;

    void set_status(uint32_t input) {
        status = static_cast<xevm_transaction_status_t>(input);
    }

    top::evm_common::h2048 get_logsbloom() {
        top::evm_common::h2048  logsbloom;
        for (auto & log : logs) {
            top::evm_common::h2048 bloom;
            top::uint256_t hash = top::utl::xkeccak256_t::digest(log.address.data(), log.address.size);
            top::evm_common::h256 hash_h256;
            top::evm_common::bytesConstRef((const unsigned char *)hash.data(), hash.size()).copyTo(hash_h256.ref());
            bloom.shiftBloom<3>(hash_h256);
            logsbloom |= bloom;

            for (auto & topic : log.topics) {
                top::evm_common::h2048 bloom;
                top::uint256_t hash = top::utl::xkeccak256_t::digest(topic.data(), topic.size);
                top::evm_common::h256 hash_h256;
                top::evm_common::bytesConstRef((const unsigned char *)hash.data(), hash.size()).copyTo(hash_h256.ref());
                bloom.shiftBloom<3>(hash_h256);
                logsbloom |= bloom;
            }
        }
        return logsbloom;
    }

    // debug
    std::string dump_info() {
        return "transaction_result[status:" + std::to_string(static_cast<std::underlying_type<xevm_transaction_status_t>::type>(status)) + ", extra_msg:" + extra_msg +
               "], logs.size():" + std::to_string(logs.size());
    }
};

NS_END2