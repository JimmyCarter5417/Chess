#ifndef HASH_H
#define HASH_H

#include "util/def.h"

#include <functional>

namespace std
{
    // TPos散列函数
    template <>
    struct hash<def::TPos>
    {
        size_t operator()(const def::TPos& k) const
        {
            return ((hash<int>()(k.row) ^ (hash<int>()(k.col) << 1)) >> 1);
        }
    };

    // TDelta散列函数
    template <>
    struct hash<def::TDelta>
    {
        size_t operator()(const def::TDelta& k) const
        {
            return ((hash<int>()(k.deltaRow) ^ (hash<int>()(k.deltaCol) << 1)) >> 1);
        }
    };
};

#endif // HASH_H
