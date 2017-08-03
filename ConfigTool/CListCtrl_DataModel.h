#pragma once
#include "ConfigCenter.h"
struct CListCtrl_DataRecord
{
	CListCtrl_DataRecord()
	{}

	CListCtrl_DataRecord(ScanDevBaseInfo& baseinfo)
		:m_baseinfo(baseinfo)
	{}
	ScanDevBaseInfo m_baseinfo;
	
	CString GetCellText(int col, bool title) const
	{
		BaseDeviceInfo_t baInfo = m_baseinfo.baseInfo;
		switch(col)
		{
		case 0: { static const CString title0(_T("Uniqueid")); return title ? title0 : GetHexStr(baInfo.Uniqueid,8); }
		case 1: { static const CString title1(_T("version")); return title ? title1 : CPicaSoftAssistant::FormatString("%d", baInfo.version); }
		case 2: { static const CString title2(_T("type")); return title ? title2 : CPicaSoftAssistant::FormatString("%d", baInfo.type); }
		case 3: { static const CString title3(_T("nodeid")); return title ? title3 : CPicaSoftAssistant::FormatString("%d", baInfo.nodeID); }
		case 4: { static const CString title4(_T("subid")); return title ? title4 : CPicaSoftAssistant::FormatString("%d", baInfo.subID); }
		case 5: { static const CString title5(_T("rfchannel")); return title ? title5 : CPicaSoftAssistant::FormatString("%d", baInfo.rfChannel); }
		case 6: { static const CString title6(_T("rfDataRate")); return title ? title6 : CPicaSoftAssistant::FormatString("%d", baInfo.rfDataRate); }
		case 7: { static const CString title7(_T("rfPowerLevel")); return title ? title7 : CPicaSoftAssistant::FormatString("%d", baInfo.rfPowerLevel); }
		case 8: { static const CString title8(_T("networkID")); return title ? title8 : GetHexStr(baInfo.networkID, 5, TRUE); }
		case 9: { static const CString title9(_T("设备类型")); return title ? title9 : ToStrbyDevtype(baInfo.type); }
		case 10: { static const CString title10(_T("扫描时间")); return title ? title10 : CPicaSoftAssistant::FormatString("%l", (CTime::GetCurrentTime() - m_baseinfo.tmTime).GetSeconds()); }
		default:{ static const CString emptyStr; return emptyStr; }
		}
	}
	int  GetColCount() const { return 11; }
};

class CListCtrl_DataModel
{
	vector<CListCtrl_DataRecord> m_Records;
	int	m_LookupTime;
	int m_RowMultiplier;

public:
	CListCtrl_DataModel()
		:m_RowMultiplier(0)
		,m_LookupTime(0)
	{
	}
	void DisplayDataModel()
	{
		m_Records.clear();
		CPicaSoftMap<CString, ScanDevBaseInfo> &devmap = ConfigCenter::GetInstance()->m_mapBaseInfoCfg;
		for (CPicaSoftMap<CString, ScanDevBaseInfo>::CIterator it = devmap.GetIterator(); !it.IsEnd(); it.MoveNext())
		{
			ScanDevBaseInfo dev = it.GetValue();
			m_Records.push_back(CListCtrl_DataRecord(dev));
		}
	}

	CString GetCellText(size_t lookupId, int col) const
	{
		if (lookupId >= m_Records.size())
		{
			static CString oob(_T("Out of Bound"));
			return oob;
		}
		// How many times should we search sequential for the row ?
		for(int i=0; i < m_LookupTime; ++i)
		{
			for(size_t rowId = 0; rowId < m_Records.size(); ++rowId)
			{
				if (rowId==lookupId)
					break;
			}
		}
		return m_Records.at(lookupId).GetCellText(col, false);
	}
	size_t GetRowIds() const { return m_Records.size(); }
	int GetColCount() const { return CListCtrl_DataRecord().GetColCount(); }
	CString GetColTitle(int col) const { return CListCtrl_DataRecord().GetCellText(col, true); }

	vector<CListCtrl_DataRecord>& GetRecords() { return m_Records; }
	void SetLookupTime(int lookupTimes) { m_LookupTime = lookupTimes; }
	void SetRowMultiplier(int multiply) { if (m_RowMultiplier != multiply ) { m_RowMultiplier = multiply;  } }
	int GetColIndexByTitle(CString title)
	{
		if (title.MakeLower() == "nodeid")
		{
			return 3+1;
		}
		if (title.MakeLower() == "subid")
		{
			return 4 + 1;
		}
		if (title.MakeLower() == "rfchannel")
		{
			return 5 + 1;
		}
		if (title.MakeLower() == "rfdatarate")
		{
			return 6 + 1;
		}
		if (title.MakeLower() == "rfpowerlevel")
		{
			return 7 + 1;
		}
		if (title.MakeLower() == "networkid")
		{
			return 8 + 1;
		}
		return -1;
	}
};