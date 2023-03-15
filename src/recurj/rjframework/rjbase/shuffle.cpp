#include "stdafx.h"
#include <recurj/rjframework/libs/base/datatools/shuffle.h>
#include <random>
using namespace rjBase;
CRJShuffle::CRJShuffle()
{
}
CRJShuffle& CRJShuffle::Prepare(size_t c)
{
    m_clArray.reserve(c);
    return *this;
}
CRJShuffle& CRJShuffle::Build(size_t c)
{
    init(c);
    Rebuild();
    return *this;
}
CRJShuffle& CRJShuffle::Rebuild()
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(m_clArray.begin(), m_clArray.end(), g);
    return *this;
}

CRJShuffle& CRJShuffle::Push(size_t v)
{
    m_clArray.push_back(v);
    return *this;
}
CRJShuffle& CRJShuffle::Pop()
{
    m_clArray.pop_back();
    return *this;
}


void CRJShuffle::init(size_t c) {
    size_t i = 0;
    while (i < c) {
        m_clArray.push_back(i);
        i++;
    }
}
