#pragma once
#include "Pos.h"

namespace ncompiler {
    struct PosProvider {
        virtual Pos getCurrentPosition() = 0;

        virtual ~PosProvider() = default;
    };
}
