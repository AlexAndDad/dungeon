//
// Created by rhodges on 22/02/18.
//

#ifndef DUNGEON_SERVIE_BASE_HPP
#define DUNGEON_SERVIE_BASE_HPP

#include <boost/noncopyable.hpp>

namespace freetype
{
    struct service_base : boost::noncopyable
    {
        virtual ~service_base() = default;
    };
}

#endif //DUNGEON_SERVIE_BASE_HPP
