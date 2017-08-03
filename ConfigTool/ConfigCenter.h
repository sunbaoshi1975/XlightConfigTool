#pragma once
#include "SerialProcess.h"
#include "Public.h"
class ConfigCenter;
typedef boost::shared_ptr<ConfigCenter> ConfigCenterPtr;
typedef boost::weak_ptr<ConfigCenter> ConfigCenterWeakPtr;

typedef struct 
{
	BaseDeviceInfo_t baseInfo;
	CTime tmTime;
}ScanDevBaseInfo;

typedef struct _tagSettings
{
	// 数据库连接串
	CString sComPort;
}SSettings;
extern SSettings g_Settings;
class ConfigCenter
{
public:
	ConfigCenter();
	~ConfigCenter();
public:
	BOOL Init();
	static ConfigCenterPtr GetInstance();
	BOOL AddCfgJsonDoc(UC version, UC devicetype, LPCTSTR lpszJsonCfg);
	BOOL AddDeviceBaseInfo(LPCTSTR lpszUnqueid, BaseDeviceInfo_t baseInfo);
	BOOL AddLastSetDevBaseInfo(LPCTSTR lpszUnqueid, BaseDeviceInfo_t baseInfo);
	BOOL GetLastSetDevBaseInfoByUnique(LPCTSTR lpszUnqueid, ScanDevBaseInfo& baseinfo);
	BOOL GetDeviceBaseInfoByUnique(LPCTSTR lpszUnqueid, ScanDevBaseInfo& baseinfo);
	CString GetStructKey(LPCTSTR lpszUnqueid);
	BOOL SetConfigStrByUniqueid(LPCTSTR lpszUnqueid,UC offset, UC *pCfgBuf, UC len);
	BOOL GetConfigStrByUniqueid(LPCTSTR lpszUnqueid);
	BOOL ReloadSystemParameter();
	void SetScannerStatus(ERFScannerStatus scannerStat) { m_ScannerStatus = scannerStat; }
	ERFScannerStatus GetScannerStatus() { return m_ScannerStatus; }
private:
	static ConfigCenterPtr m_spCfgCenter;
	static CPicaSoftMutex m_mtxInstance;
public:
	CDataProcesserPtr m_spSerialDP;
private:
	CPicaSoftLog m_iLog;
	// device config map
	//CPicaSoftMap<CString,
	// device type and version ensure cfg struct(key:device type - version)
	CPicaSoftMap<CString, JsonDocPtr> m_mapJsonCfg;
	// last send config(key - uniqueid)
	CPicaSoftMap<CString, TSTLBufferPtr> m_mapLastCfg;
	// last send base info(key - uniqueid)
	CPicaSoftMap<CString, ScanDevBaseInfo> m_mapLastSetBaseInfo;
	// config template(key - uniqueid)
	CPicaSoftMap<CString, TSTLBufferPtr> m_mapBlockTemtCfg;
public:
	// base info(key - uniqueid)
	CPicaSoftMap<CString, ScanDevBaseInfo> m_mapBaseInfoCfg;
	// config info(key - uniqueid)
	CPicaSoftMap<CString, TSTLBufferPtr> m_mapBlockCfg;

	ERFScannerStatus m_ScannerStatus;
};

