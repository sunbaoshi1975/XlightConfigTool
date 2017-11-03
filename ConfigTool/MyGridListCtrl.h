#pragma once
#include "CGridListCtrlEx/CGridListCtrlGroups.h"
class MyGridListCtrl : public CGridListCtrlGroups
{
public:
	MyGridListCtrl();
	~MyGridListCtrl();

	virtual bool OnDisplayCellColor(int nRow, int nCol, COLORREF& textColor, COLORREF& backColor);
public:
	vector<CString> m_redRowCol;
};