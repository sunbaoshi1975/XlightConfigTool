#include "stdafx.h"
#include "ConfigCenter.h"
#include "Public.h"
#include "PicaSoftINIFile.h"

SuperSensorConfig_t g_SuperSensorCfg;
ScannerStatus_t g_ScannerStatus;
ConfigCenterPtr ConfigCenter::m_spCfgCenter;
CPicaSoftMutex ConfigCenter::m_mtxInstance;
SSettings g_Settings;
ConfigCenter::ConfigCenter()
{
}


ConfigCenter::~ConfigCenter()
{
}

BOOL ConfigCenter::Init()
{
	ReloadSystemParameter();
	m_ScannerStatus = SYS_INIT;
	m_spSerialDP = boost::make_shared<CDataProcesser>();
	PICASOFT_SET_GLOBALLOGPOINTER(&m_iLog);
	m_iLog.SetLevel(ELL_TRACE);
	m_iLog.EnableFileLog(_T("$(ApplicationPath)ConfigTool.Log"), 100 * 1024 * 1024);
	if (m_spSerialDP->Init())
	{
		return TRUE;
	}
	return FALSE;
}

ConfigCenterPtr ConfigCenter::GetInstance()
{
	m_mtxInstance.Lock();
	if (!m_spCfgCenter)
	{
		m_spCfgCenter = boost::make_shared<ConfigCenter>();
		if (!m_spCfgCenter->Init())
		{
			PLOG(ELL_ERROR, _T("Init com failed!"));
		}
	}
	m_mtxInstance.Unlock();
	return m_spCfgCenter;
}

BOOL ConfigCenter::AddCfgJsonDoc(UC version,UC devicetype, LPCTSTR lpszJsonCfg )
{
	JsonDocPtr spJsonDoc = boost::make_shared<rapidjson::Document>();
	if (!spJsonDoc->Parse<0>(lpszJsonCfg).HasParseError()
		&& spJsonDoc->IsObject())
	{
		CString key = "";
		key.Format("%d-%d", version, devicetype);
		m_mapJsonCfg.SetAt(key, spJsonDoc);
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

BOOL ConfigCenter::AddDeviceBaseInfo(LPCTSTR lpszUnqueid, BaseDeviceInfo_t baseInfo)
{
	ScanDevBaseInfo scanBaseInfo;
	scanBaseInfo.baseInfo = baseInfo;
	scanBaseInfo.tmTime = CTime::GetCurrentTime();
	m_mapBaseInfoCfg.SetAt(lpszUnqueid, scanBaseInfo);
	return TRUE;
}

BOOL ConfigCenter::GetDeviceBaseInfoByUnique(LPCTSTR lpszUnqueid, ScanDevBaseInfo& baseinfo)
{
	if (m_mapBaseInfoCfg.Lookup(lpszUnqueid, baseinfo))
	{
		return TRUE;
	}
	return FALSE;
}

BOOL ConfigCenter::AddLastSetDevBaseInfo(LPCTSTR lpszUnqueid, BaseDeviceInfo_t baseInfo)
{
	ScanDevBaseInfo scanBaseInfo;
	scanBaseInfo.baseInfo = baseInfo;
	scanBaseInfo.tmTime = CTime::GetCurrentTime();
	m_mapLastSetBaseInfo.SetAt(lpszUnqueid, scanBaseInfo);
	return TRUE;
}

BOOL ConfigCenter::GetLastSetDevBaseInfoByUnique(LPCTSTR lpszUnqueid, ScanDevBaseInfo& baseinfo)
{
	if (m_mapLastSetBaseInfo.Lookup(lpszUnqueid, baseinfo))
	{
		return TRUE;
	}
	return FALSE;
}

CString ConfigCenter::GetStructKey(LPCTSTR lpszUnqueid)
{
	CString key="";
	ScanDevBaseInfo baseinfo;
	if (GetDeviceBaseInfoByUnique(lpszUnqueid, baseinfo))
	{
		key.Format("%d-%d", baseinfo.baseInfo.type, baseinfo.baseInfo.version);
	}
	return key;
}

BOOL ConfigCenter::SetConfigStrByUniqueid(LPCTSTR lpszUnqueid,UC offset,UC *pCfgBuf,UC len)
{
	TSTLBufferPtr spBuff;
	m_mapBlockCfg.Lookup(lpszUnqueid, spBuff);
	if (!spBuff)
	{
		spBuff = boost::make_shared<TSTLBuffer>(256);
		m_mapBlockCfg.SetAt(lpszUnqueid, spBuff);
	}
	if (spBuff)
	{
		memcpy(&(*spBuff)[0] + offset, pCfgBuf, len);
	}
	return TRUE;
}

BOOL ConfigCenter::GetConfigStrByUniqueid(LPCTSTR lpszUnqueid)
{
	TSTLBufferPtr spBuff;
	m_mapBlockCfg.Lookup(lpszUnqueid,spBuff);
	if (spBuff)
	{
		CString key = GetStructKey(lpszUnqueid);
		if (key == "")
		{ // supersensor
			SuperSensorConfig_t* superSensorCfg = (SuperSensorConfig_t*)(&(*spBuff)[0]);
			PLOG(ELL_INFORMATION, _T("supersensor config is :"));
			PLOG(ELL_INFORMATION, _T("version = %d"), superSensorCfg->version);
			PLOG(ELL_INFORMATION, _T("nodeid = %d"), superSensorCfg->nodeID);
			PLOG(ELL_INFORMATION, _T("networkid=%s"), GetHexStr(superSensorCfg->NetworkID, 5,TRUE));
			PLOG(ELL_INFORMATION, _T("present=%d"), superSensorCfg->present);
			PLOG(ELL_INFORMATION, _T("state=%d"), superSensorCfg->state);
			PLOG(ELL_INFORMATION, _T("reserved=%d"), superSensorCfg->reserved0);
			PLOG(ELL_INFORMATION, _T("subid=%d"), superSensorCfg->subID);
			PLOG(ELL_INFORMATION, _T("type=%d"), superSensorCfg->type);
			PLOG(ELL_INFORMATION, _T("token=0x%02X"), superSensorCfg->token[0] * 256 + superSensorCfg->token[1]);
			PLOG(ELL_INFORMATION, _T("powerlevel=%d"), superSensorCfg->rfPowerLevel);
			PLOG(ELL_INFORMATION, _T("swtimes=%d"), superSensorCfg->swTimes);
			PLOG(ELL_INFORMATION, _T("rpttimes=%d"), superSensorCfg->rptTimes);
			PLOG(ELL_INFORMATION, _T("reserverd1=%d"), superSensorCfg->reserved1);
			PLOG(ELL_INFORMATION, _T("senmap=0x%02X"), superSensorCfg->senMap[0] * 256 + superSensorCfg->senMap[1]);
			PLOG(ELL_INFORMATION, _T("relaykey=%01x"), superSensorCfg->relay_key_value);
			PLOG(ELL_INFORMATION, _T("channel=%d"), superSensorCfg->rfChannel);
			PLOG(ELL_INFORMATION, _T("datarate=%d"), superSensorCfg->rfDataRate);
			PLOG(ELL_INFORMATION, _T("reserved=%d"), superSensorCfg->reserved2);
			CString sbtnActionHex = _T("");
			for (DWORD i = 0; i < MAX_NUM_BUTTONS; i++)
			{
				for (DWORD j = 0; j < 8; j++)
				{
					CString sTmp = _T("");
					sTmp.Format(_T("0x%02X"), superSensorCfg->btnAction[i][j]);
					sbtnActionHex += sTmp;
				}
			}
			PLOG(ELL_INFORMATION, _T("btnaction=%s"), sbtnActionHex);
		}
	}
	return TRUE;
}

// 重新加载系统参数
BOOL ConfigCenter::ReloadSystemParameter()
{
	// 加载参数
	CPicaSoftINIFile ini;
	CString sIniFilename = CPicaSoftAssistant::FormatString(_T("$(ApplicationFullPath)"));
	CPicaSoftAssistant::ChangeFileExtName(sIniFilename, ".ini");
	ini.Open(sIniFilename, _T("System"));
	PICASOFT_SETTINGS_STR(ini, _T("COM"), g_Settings.sComPort);
	return TRUE;
}
