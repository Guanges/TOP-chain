// Copyright (c) 2017-2018 Telos Foundation & contributors
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "xcommon/xrole_type.h"

#include "xbase/xutl.h"

#include <cassert>
#include <type_traits>

NS_BEG2(top, common)

std::int32_t operator<<(top::base::xstream_t & stream, xminer_type_t const & role_type) {
    return stream << static_cast<std::underlying_type<xminer_type_t>::type>(role_type);
}

std::int32_t operator>>(top::base::xstream_t & stream, xminer_type_t & role_type) {
    std::underlying_type<xminer_type_t>::type temp;
    auto r = stream >> temp;
    role_type = static_cast<xminer_type_t>(temp);
    return r;
}

std::string to_string(xminer_type_t const role) {
    switch (role) {
    case xminer_type_t::advance:
        return XMINER_TYPE_ADVANCE;

    case xminer_type_t::exchange:
        return XMINER_TYPE_EXCHANGE;

    case xminer_type_t::validator:
        return XMINER_TYPE_VALIDATOR;

#if defined(XENABLE_MOCK_ZEC_STAKE)
    case xminer_type_t::archive:
        return XMINER_TYPE_ARCHIVE;
#endif

    case xminer_type_t::edge:
        return XMINER_TYPE_EDGE;

    default:
        assert(false);
        return "invalid(" + std::to_string(static_cast<std::underlying_type<xminer_type_t>::type>(role)) + ")";
    }
}

xminer_type_t & operator&=(xminer_type_t & lhs, xminer_type_t const rhs) noexcept {
    lhs = lhs & rhs;
    return lhs;
}

xminer_type_t & operator|=(xminer_type_t & lhs, xminer_type_t const rhs) noexcept {
    lhs = lhs | rhs;
    return lhs;
}

bool has(xminer_type_t const target, xminer_type_t const input) noexcept {
    return target == (target & input);
}

common::xminer_type_t to_miner_type(std::string const & node_type) {
    common::xminer_type_t role_type = common::xminer_type_t::invalid;

    if (node_type == XMINER_TYPE_EDGE) {
        role_type = common::xminer_type_t::edge;
    } else if (node_type == XMINER_TYPE_ADVANCE) {
        role_type = common::xminer_type_t::advance;
    } else if (node_type == XMINER_TYPE_VALIDATOR) {
        role_type = common::xminer_type_t::validator;
#if defined(XENABLE_MOCK_ZEC_STAKE)
    } else if (node_type == XMINER_TYPE_ARCHIVE) {
        role_type = common::xminer_type_t::archive;
#endif
    } else if (node_type == XMINER_TYPE_EXCHANGE) {
        role_type = common::xminer_type_t::exchange;
    }

    return role_type;
}

NS_END2
