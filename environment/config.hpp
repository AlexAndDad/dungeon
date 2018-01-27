//
// Created by Richard Hodges on 27/01/2018.
//

#pragma once

#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/type_traits.hpp>
#include <ciso646>

namespace system_namespace = boost::system;
#define OPEN_SYSTEM_NAMESPACE namespace boost { namespace system {
#define CLOSE_SYSTEM_NAMESPACE }}
