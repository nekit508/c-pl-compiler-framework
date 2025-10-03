#pragma once
#include <vector>
#include "ReusableStream.h"

namespace ncompiler {
    template<typename T>
    struct ReusableStreamVectorImpl : ReusableStream<T> {
        std::vector<T> obtainedObjects;
        std::vector<size_t> stateStack;
        int currentPos = -1;

        T readNextObject() noexcept(false) override = 0;
        
        T get() override {
            return obtainedObjects.at(currentPos);
        }
        
        T next() override {
            if (obtainedObjects.size()-1 == currentPos) {
                obtainedObjects.push_back(std::move(readNextObject()));
                currentPos++;
            }
            return get();
        }
        
        void redo() override {
            currentPos--;
        }
        
        void redo(int num) override {
            for (int i = 0; i < num; ++i)
                redo();
        }
        
        void saveState() override {
            stateStack.push_back(currentPos);
        }
        
        void redoState() override {
            int last = stateStack.back();
            stateStack.pop_back();
            redo(currentPos - last);
        }
        
        void disposeState() override {
            stateStack.pop_back();
        }
        
        int getPos() override {
            return currentPos;
        }
        
        ~ReusableStreamVectorImpl() override = default;
        ReusableStreamVectorImpl() = default;
    };
}