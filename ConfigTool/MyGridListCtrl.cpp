#include "stdafx.h"
#include "MyGridListCtrl.h"
MyGridListCtrl::MyGridListCtrl()
{
}

MyGridListCtrl::~MyGridListCtrl()
{
}

bool MyGridListCtrl::OnDisplayCellColor(int nRow, int nCol, COLORREF& textColor, COLORREF& backColor)
{
	CString sRowCol = CPicaSoftAssistant::FormatString("%d-%d", nRow, nCol);
	for (int i=0;i<m_redRowCol.size();i++)
	{
		if (sRowCol == m_redRowCol[i])
		{
			textColor = RGB(0, 255, 0);
			backColor = RGB(0, 0, 255);
			return true;  // I want to override the color of this cell
		}
	}
	return false;  // Use default color
}