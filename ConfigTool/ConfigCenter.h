#pragma once
#include "SerialProcess.h"
#include "Public.h"
class ConfigCenter;
typedef boost::shared_ptr<ConfigCenter> ConfigCenterPtr;
typedef boost::weak_ptr<ConfigCenter> ConfigCenterWeakPtr;
class ConfigCenter
{
public:
	ConfigCenter();
	~ConfigCenter();
public:
	static ConfigCenterPtr GetInstance();
	BOOL AddCfgJsonDoc(UC version, UC devicetype, LPCTSTR lpszJsonCfg);
	BOOL AddDeviceBaseInfo(LPCTSTR lpszUnqueid, BaseDeviceInfo_t baseInfo);
	CString GetStructKey(LPCTSTR lpszUnqueid);
	BOOL SetConfigStrByUniqueid(LPCTSTR lpszUnqueid,UC offset, UC *pCfgBuf, UC len);
	BOOL GetConfigStrByUniqueid(LPCTSTR lpszUnqueid);
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
	// config template(key - uniqueid)
	CPicaSoftMap<CString, TSTLBufferPtr> m_mapBlockTemtCfg;
	// base info(key - uniqueid)
	CPicaSoftMap<CString, BaseDeviceInfo_t> m_mapBaseInfoCfg;
	// config info(key - uniqueid)
	CPicaSoftMap<CString, TSTLBufferPtr> m_mapBlockCfg;
};

