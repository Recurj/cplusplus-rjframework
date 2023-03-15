#pragma once
#include <recurj/rjframework/modules/modbase.h>
namespace rjBase {
    class CRJPagination;
    class RJBASE_API CRJPagination {
    public:
        CRJPagination(size_t);
        virtual ~CRJPagination();
        struct Metrics {
            size_t Count,active;
            Metrics() { Reset(); }
            void Reset() { Count = active = 0;}
            size_t diffActive(size_t a) const { return active > a ? active - a : 0; }
            size_t diffCount(size_t c) const { return Count > c ? Count - c : 0; }
        };
        inline bool hasRecords() const { return records.Count > 0; }
        bool hasMetrics() const { return m_bHasMetrics; }
        bool notFirstPage() const { return  pages.active > 1; }
        bool notLastPage() const { return pages.active < pages.Count; }
        bool notFirstRecord() const { return  records.active > 0; }
        bool notLastRecord() const { return records.active < (records.Count - 1); }
        bool needPagination() const { return  pages.Count > 1; }

        size_t getPageActive() const { return  pages.active; }
        size_t getPageCount() const { return pages.Count; }
        size_t getPageSize() const { return  pageVolume; }
        size_t getPageTail() const { return  pageTail; }
        size_t getRecordCount() const { return  records.Count; }
        size_t getRecordActive() const { return  records.active; }
        size_t getRecordPageOffset() const { return records.diffActive(pageOffset(pages.active)); }
        size_t pageOffset(size_t p) const { return  (p - 1) * pageVolume; }
        void setPageSize(size_t p) { pageVolume = max(1, p); }
        void setPageActive(size_t p) { pages.active = max(1, p); }
        bool inserted(size_t c) { return  knownMetrics(records.Count + c); }
        bool deleted(size_t c) { return  knownMetrics(records.diffCount(c)); }

        void Reset();
        size_t getRecordTail();
        void setRecordActive(size_t, size_t);
        bool knownMetrics(size_t);

        bool pageHome();
        bool pageEnd();
        bool pagePrev();
        bool pageNext();
        bool pageGoto(size_t);
        bool recordNext();
        bool recordPrev();

        bool page(size_t, size_t);
        bool record(size_t);
    protected:
        virtual bool navigationPage(size_t) = 0;
        virtual bool readPageOnMetrics();
    protected:
        Metrics records,pages;
        size_t pageVolume, pageTail;
        bool m_bHasMetrics;
    };
}

