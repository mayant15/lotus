#pragma once

#include "lotus/lcommon.h"

namespace Lotus
{
    using OArchive = cereal::JSONOutputArchive;
    using IArchive = cereal::JSONInputArchive;

#define FIELD(x, y) cereal::make_nvp(x, y)

    struct ISerializable
    {
        virtual ~ISerializable() = default;
        virtual void Save(OArchive& archive) const = 0;
        virtual void Load(IArchive& archive) = 0;
    };
}
