#include "stdafx.h"
#include "ConfigCenter.h"
#include "Public.h"

SuperSensorConfig_t g_SuperSensorCfg;
ScannerStatus_t g_ScannerStatus;
ConfigCenterPtr ConfigCenter::m_spCfgCenter;
CPicaSoftMutex ConfigCenter::m_mtxInstance;
ConfigCenter::ConfigCenter()
{
	m_spSerialDP = boost::make_shared<CDataProcesser>();
	PICASOFT_SET_GLOBALLOGPOINTER(&m_iLog);
	m_iLog.SetLevel(ELL_TRACE);
	m_iLog.EnableFileLog(_T("$(ApplicationPath)ConfigTool.Log"), 100 * 1024 * 1024);
}

ConfigCenter::~ConfigCenter()
{
}

ConfigCenterPtr ConfigCenter::GetInstance()
{
	m_mtxInstance.Lock();
	if (!m_spCfgCenter)
	{
		m_spCfgCenter = boost::make_shared<ConfigCenter>();
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
	m_mapBaseInfoCfg.SetAt(lpszUnqueid, baseInfo);
	return TRUE;
}

CString ConfigCenter::GetStructKey(LPCTSTR lpszUnqueid)
{
	CString key="";
	BaseDeviceInfo_t baseinfo;
	if (m_mapBaseInfoCfg.Lookup(lpszUnqueid, baseinfo))
	{
		key.Format("%d-%d", baseinfo.type, baseinfo.version);
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
			PLOG(ELL_INFORMATION, "supersensor config is :");
			PLOG(ELL_INFORMATION, "version = %d", superSensorCfg->version);
			PLOG(ELL_INFORMATION, "nodeid = %d", superSensorCfg->nodeID);
			PLOG(ELL_INFORMATION, "networkid=%s", GetNetworkidHexStr(superSensorCfg->NetworkID, 5));
			PLOG(ELL_INFORMATION, "present=%d", superSensorCfg->present);
			PLOG(ELL_INFORMATION, "state=%d", superSensorCfg->state);
			PLOG(ELL_INFORMATION, "reserved=%d", superSensorCfg->reserved0);
			PLOG(ELL_INFORMATION, "subid=%d", superSensorCfg->subID);
			PLOG(ELL_INFORMATION, "type=%d", superSensorCfg->type);
			PLOG(ELL_INFORMATION, "token=0x%02X", superSensorCfg->token[0] * 256 + superSensorCfg->token[1]);
			PLOG(ELL_INFORMATION, "powerlevel=%d", superSensorCfg->rfPowerLevel);
			PLOG(ELL_INFORMATION, "swtimes=%d", superSensorCfg->swTimes);
			PLOG(ELL_INFORMATION, "rpttimes=%d", superSensorCfg->rptTimes);
			PLOG(ELL_INFORMATION, "reserverd1=%d", superSensorCfg->reserved1);
			PLOG(ELL_INFORMATION, "senmap=0x%02X", superSensorCfg->senMap[0] * 256 + superSensorCfg->senMap[1]);
			PLOG(ELL_INFORMATION, "relaykey=%01x", superSensorCfg->relay_key_value);
			PLOG(ELL_INFORMATION, "channel=%d", superSensorCfg->rfChannel);
			PLOG(ELL_INFORMATION, "datarate=%d", superSensorCfg->rfDataRate);
			PLOG(ELL_INFORMATION, "reserved=%d", superSensorCfg->reserved2);
			CString sbtnActionHex = "";
			for (DWORD i = 0; i < MAX_NUM_BUTTONS; i++)
			{
				for (DWORD j = 0; j < 8; j++)
				{
					CString sTmp = "";
					sTmp.Format("0x%02X", superSensorCfg->btnAction[i][j]);
					sbtnActionHex += sTmp;
				}
			}
			PLOG(ELL_INFORMATION, "btnaction=%s", sbtnActionHex);
		}
	}
	return TRUE;
}
