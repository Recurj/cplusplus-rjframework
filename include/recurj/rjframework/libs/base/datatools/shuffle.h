#pragma once
#include <recurj/rjframework/modules/modbase.h>
#include <vector>
namespace rjBase {
    class RJBASE_API CRJShuffle {
    public:
        CRJShuffle();
        CRJShuffle & Prepare(size_t);
        CRJShuffle& Build(size_t);
        CRJShuffle& Rebuild();
        CRJShuffle& Push(size_t);
        CRJShuffle& Pop();
        size_t operator[](size_t p) const { return m_clArray[p]; }
        bool OK(size_t c) const { return m_clArray.size() == c; }
    protected:
        void init(size_t);
    protected:
        std::vector<size_t> m_clArray;
    };
}

