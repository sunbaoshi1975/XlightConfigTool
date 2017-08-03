#pragma once
#include "PicaSoftSerial.h"
#include "MyMessage.h"
#include "Protocol.h"
#define RFS_MESSAGE_HEAD_0      0x55
#define RFS_MESSAGE_HEAD_1      0xAA
#define RFS_CMD_SCAN_START      'S'
#define RFS_CMD_SCAN_STOP       'T'
#define RFS_CMD_SCAN_PAUSE      'P'
#define RFS_CMD_SCAN_RESUME     'R'
class CDataProcesser;
typedef boost::shared_ptr<CDataProcesser> CDataProcesserPtr;
typedef boost::weak_ptr<CDataProcesser> CDataProcesserWeakPtr;
class CDataProcesser
	: public boost::enable_shared_from_this<CDataProcesser>
{
public:
	CDataProcesser(void);
	~CDataProcesser(void);
	CDataProcesserPtr GetDataProcesserPtr()
	{
		return shared_from_this();
	}
	BOOL Init();
	BOOL Start();
	UINT WorkProc(LPVOID);
	BOOL Stop(DWORD dwTimeout);

	BOOL SendSerialMsg(SPScanMsgPtr spMsg);
	BOOL SendStartScan(BYTE channel, BYTE bandwidth, BYTE powerlevel, BYTE network[5]);

	BOOL SendStopScan(BYTE channel, BYTE bandwidth, BYTE powerlevel, BYTE network[5]);
	BOOL SendSetConfigByNodeid(UC nodeid, UC subid, const UC * pCfgData, UC offset, UC size);
	BOOL SendSetConfigByUniqueid(UC* uniqueid, UC uniLen, const UC * pCfgData, UC offset, UC size);
	BOOL SendGetConfigByUniqueid(UC* uniqueid, UC uniLen, UC offset, UC size);
	BOOL SendGetConfigByNode(UC nodeid, UC subid, UC offset, UC size);
	BOOL SendRFByNode(UC Nodeid,UC subid, MySetUpRF_t& rfparam, UC paramlen);
	BOOL SendRFByUniqueid(UC* uniqueid, UC uniLen, MySetUpRF_t& rfparam, UC paramlen);
	int ProcessSerialMsg(SPScanMsgPtr lpMsg);
	int ProcessProbe(SPScanMsgPtr lpMsg);
	int ProcessGetConfig(SPScanMsgPtr lpMsg);
	int ProcessScannerStatus(SPScanMsgPtr lpMsg);
private:
	CPicaSoftMemfunWorkThread<CDataProcesser> m_thdMainWork;

	//线程退出信号
	CPicaSoftEvent m_evtStop;
	CPicaSoftEvent m_evtOutData;
	CPicaSoftEvent m_evtInData;
	CPicaSoftArray<SPScanMsgPtr> m_arrOutMsg;
	CPicaSoftArray<SPScanMsgPtr> m_arrInMsg;

	CPicaSoftSerial m_serial;

};

