#pragma once

namespace ncompiler {
    template<typename T>
    struct ReusableStream {
        virtual T readNextObject() noexcept(false)  = 0;

        virtual T get() = 0;
        virtual T next() = 0;
        virtual void redo() = 0;
        virtual void redo(int num) = 0;
        virtual void saveState() = 0;
        virtual void redoState() = 0;
        virtual void disposeState() = 0;
        virtual int getPos() = 0;
        virtual ~ReusableStream() = default;
        ReusableStream() = default;
    };
}
