#include "stdafx.h"
#include <recurj/rjframework/libs/base/datatools/pagination.h>
using namespace rjBase;
CRJPagination::CRJPagination(size_t ps) {
	pageVolume = ps;
	Reset();
}
CRJPagination::~CRJPagination()
{}
void CRJPagination::Reset() {
	m_bHasMetrics = false;
	records.Reset();
	pages.Reset();
	pageTail = 0;
}

size_t CRJPagination::getRecordTail() {
	if (pageTail == 0) return pageVolume - 1;
	return pageTail - 1;
}

void CRJPagination::setRecordActive(size_t pageOffset, size_t recordOffset) {
	size_t r = records.diffCount(pageOffset);
	records.active = pageOffset + min(recordOffset, r);
}

bool CRJPagination::knownMetrics(size_t c) {
	m_bHasMetrics = true;
	records.Count = c;
	size_t p = pageVolume;
	pageTail = records.Count % pageVolume;
	pages.Count = records.Count / pageVolume;
	if (pageTail > 0) pages.Count++;
	if (pages.active < 1 || pages.active > pages.Count) pages.active = 1;
	size_t r = pageOffset(pages.active);
	p = p + r - 1;
	if (records.active < r || records.active > p) records.active = r;
	return readPageOnMetrics();
}
bool CRJPagination::readPageOnMetrics() {
	return page(pages.active, getRecordPageOffset());
}

bool CRJPagination::pageHome() { return page(1, 0); }
bool CRJPagination::pageEnd() { return page(pages.Count, getRecordTail()); }
bool CRJPagination::pagePrev() {
	return (notFirstPage()) ? page(pages.active - 1, getRecordPageOffset()) : false;
}
bool CRJPagination::pageNext() {
	if (notLastPage()) {
		size_t p = pages.active + 1;
		size_t off = getRecordPageOffset();
		if (p == pages.Count) {
			size_t s = getRecordTail();
			if (s < off) off = s;
		}
		return page(p, off);
	}
	return false;
}
bool CRJPagination::pageGoto(size_t p) {
	if (p == pages.active) return true;
	if (p == 1) return pageHome();
	if (p == pages.Count) return pageEnd();
	if (p > 0 && p <= pages.Count) return page(p, getRecordPageOffset());
	return false;
}

bool CRJPagination::recordNext() {
	return (notLastRecord()) ? record(records.active + 1) : false;
}

bool CRJPagination::recordPrev() {
	return (notFirstRecord()) ? record(records.active - 1) : false;
}
bool CRJPagination::page(size_t page, size_t recOff) {
	size_t pageOff = pageOffset(page);
	if (navigationPage(pageOff)) {
		setRecordActive(pageOff, recOff);
		setPageActive(page);
		return true;
	}
	return false;
}
bool CRJPagination::record(size_t pos)
{
	size_t topPage = pageOffset(pages.active);
	size_t botPage = topPage + pageVolume - 1;
	if (pos < topPage) {
		return page(pages.active - 1, pageVolume - 1);
	}
	else if (pos > botPage) return page(pages.active + 1, 0);
	records.active = pos;
	return true;
}

