#pragma once

#include "ReusableStream.h"

namespace ncompiler {
    /** Must obtain control over stream. */
    template<typename T>
    struct ReusableStreamWrapper : ReusableStream<T> {
        ReusableStream<T>* wrapped_stream;

        explicit ReusableStreamWrapper(ReusableStream<T> *stream) : wrapped_stream(stream) {
        }

        T readNextObject() noexcept(false) override {
            return wrapped_stream->readNextObject();
        }

        T get() override {
            return wrapped_stream->get();
        }

        T next() override {
            return wrapped_stream->next();
        }

        void redo() override {
            wrapped_stream->redo();
        }

        void redo(int num) override {
            wrapped_stream->redo(num);
        }

        void saveState() override {
            wrapped_stream->saveState();
        }

        void redoState() override {
            wrapped_stream->redoState();
        }

        void disposeState() override {
            wrapped_stream->disposeState();
        }

        int getPos() override {
            return wrapped_stream->getPos();
        }

        void dispose() {
            wrapped_stream->disposeState();
        }

        ~ReusableStreamWrapper() override {
            delete wrapped_stream;
        };
    };
}
