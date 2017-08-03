#include "StdAfx.h"
#include "SerialProcess.h"
#include "global.h"
#include "Protocol.h"
#include "ConfigCenter.h"

// Assemble message
void build(SPScanMsgPtr spMsg,UC _destination, UC _sensor, UC _command, UC _type, bool _enableAck, bool _isAck)
{
	spMsg->header.version_length = PROTOCOL_VERSION;
	spMsg->header.sender = 250;
	spMsg->header.destination = _destination;
	spMsg->header.sensor = _sensor;
	spMsg->header.type = _type;
	mSetCommand(spMsg,_command);
	mSetRequestAck(spMsg, _enableAck);
	mSetAck(spMsg, _isAck);
}
CDataProcesser::CDataProcesser(void)
{
}


CDataProcesser::~CDataProcesser(void)
{
	Stop(3000);
}

UINT CDataProcesser::WorkProc(LPVOID)
{
	do
	{
		if (m_evtOutData.Lock(1000))
		{
			for (CPicaSoftArray<SPScanMsgPtr>::CIterator it = m_arrOutMsg.GetIterator(); !it.IsEnd(); it.MoveNext())
			{
				SPScanMsgPtr spScanMsg = it.Get();
				SendSerialMsg(spScanMsg);
			}
		}
		DWORD datalen = m_serial.GetReadableCount();
		CString sData = "";
		for (DWORD i = 0; i < datalen; i++)
		{
			BYTE data[1024*10] = { 0 };
			if (datalen > 0)
			{
				m_serial.Read(data, datalen, FALSE);
			}
			CString sTmp = "";
			sTmp.Format("%02X", data[i]);
			sData += sTmp;
		}
		PLOG(ELL_DEBUG, "Serial buf len=%d data=%s", datalen, sData);
		for (ULONG nReadableCount = m_serial.GetReadableCount(); nReadableCount; nReadableCount = m_serial.GetReadableCount())
		{  // 55AA(2) + length(1) + checksum(2) + playload(7+n)	
			if (nReadableCount >= 12)
			{
				BYTE headerIden[2] = { 0 };
				m_serial.Read(headerIden, 2, FALSE);
				if (headerIden[0] == 0x55 && headerIden[1] == 0xAA )
				{
					m_serial.DeleteReadable(2);
					BYTE length = 0;
					BYTE checksum[2] = { 0 };
					m_serial.Read(&length, 1, TRUE);
					TSTLBufferPtr spBuffer = TSTLBufferPtr(new TSTLBuffer);
					spBuffer->resize(length-1);
					m_serial.Read(&(*spBuffer)[0], length-1, TRUE);
					m_serial.Read(checksum, 2, TRUE);
					// check if is valid data
					DWORD got_check_sum = length;
					DWORD check_sum = checksum[0] * 256 + checksum[1];
					for (int i = 0; i < spBuffer->size(); i++)
					{
						got_check_sum += (*spBuffer)[i];
					}
					if ((got_check_sum & 0x0000FFFF) == check_sum)
					{ //
						SPScanMsgPtr spNewMessage = boost::make_shared<MyScanMsg_t>();
						if (spNewMessage)
						{
							memcpy(&(*spNewMessage), &(*spBuffer)[0], length - 1);
							ProcessSerialMsg(spNewMessage);
						}
						else
						{
							PLOG(ELL_DEBUG, "what? new msg failed!");
						}
					}
				}
				else
				{
					m_serial.DeleteReadable(1);
				}				
			}
			else break;
		}
	} while (!m_evtStop.Lock(5 * 1000));
	return TRUE;
}

BOOL CDataProcesser::Init()
{
	if (!m_serial.Connect(g_Settings.sComPort))
	{
		PLOG(ELL_ERROR, _T("Connect %s failed!"), g_Settings.sComPort);
		return FALSE;
	}
	else
	{
		m_thdMainWork.SetThreadProc(this, &CDataProcesser::WorkProc, NULL, _T("串口数据处理线程"));
		Start();
	}
	return TRUE;
}

BOOL CDataProcesser::Start()
{
	m_evtStop.ResetEvent();
	m_thdMainWork.StartThread();
	return TRUE;
}

BOOL CDataProcesser::Stop(DWORD dwTimeout)
{
	m_evtStop.SetEvent();
	if (!m_thdMainWork.WaitThreadStop(dwTimeout))
	{
		PLOG(ELL_ERROR, _T("等待DataProcess工作线程退出超时!"));
		return FALSE;
	}
	return TRUE;
}

BOOL CDataProcesser::SendSerialMsg(SPScanMsgPtr spMsg)
{
	BYTE headerIden[2] = { 0x55,0xAA };
	BYTE playloadSize = mGetLength(spMsg);

	BYTE length = sizeof(MyMsgHeader_t) + playloadSize +1;

	UINT checksum = length;
	TSTLBuffer vBuffer;
	vBuffer.resize(length - 1);
	memcpy(&vBuffer[0], &(*spMsg),length - 1);
	for (int i = 0; i < length-1; i++)
	{
		checksum += vBuffer[i];
	}

	checksum = checksum & 0x0000FFFF;
	BYTE cs[2];
	cs[0] = checksum / 256;
	cs[1] = checksum % 256;
	m_serial.Write(headerIden, 2);
	m_serial.Write(&length, 1);
	m_serial.Write(&vBuffer[0], length-1);
	m_serial.Write(cs, 2);
	return TRUE;
}


BOOL CDataProcesser::SendStartScan(BYTE channel,BYTE bandwidth,BYTE powerlevel,BYTE network[5])
{
	//BYTE headerIden[2] = { 0x55,0xAA };
	//BYTE length = 16;
	//BYTE sensormsgheader[7] = { 'S',0,0,0,0,0,0 };
	//BYTE scanmsg[8];
	//scanmsg[0] = channel;
	//scanmsg[1] = bandwidth;
	//scanmsg[2] = powerlevel;
	//memcpy(&scanmsg[3], network, 5);
	//UINT checksum = length;
	//for (int i = 0; i < sizeof(sensormsgheader); i++)
	//{
	//	checksum += sensormsgheader[i];
	//}
	//for (int i = 0;i< sizeof(scanmsg);i++)
	//{
	//	checksum += scanmsg[i];
	//}
	//checksum = checksum & 0x0000FFFF;
	//BYTE cs[2];
	//cs[0] = checksum / 256;
	//cs[1] = checksum % 256;
	//m_serial.Write(headerIden, 2);
	//m_serial.Write(&length,1);
	//m_serial.Write(sensormsgheader,7);
	//m_serial.Write(scanmsg, 8);
	//m_serial.Write(cs, 2);
	//return TRUE;
	SPScanMsgPtr spMsg = boost::make_shared<MyScanMsg_t>();
	build(spMsg, 250, 0, 0, 0, 0, 0);
	spMsg->header.last = RFS_CMD_SCAN_START;
	spMsg->data[0] = channel;
	spMsg->data[1] = bandwidth;
	spMsg->data[2] = powerlevel;
	memcpy(&spMsg->data[3], network, 5);
	mSetLength(spMsg, 8);
	SendSerialMsg(spMsg);
	return TRUE;
}

BOOL CDataProcesser::SendStopScan(BYTE channel, BYTE bandwidth, BYTE powerlevel, BYTE network[5])
{
	SPScanMsgPtr spMsg = boost::make_shared<MyScanMsg_t>();
	build(spMsg, 250, 0, 0, 0, 0, 0);
	spMsg->header.last = RFS_CMD_SCAN_STOP;
	spMsg->data[0] = channel;
	spMsg->data[1] = bandwidth;
	spMsg->data[2] = powerlevel;
	memcpy(&spMsg->data[3], network, 5);
	mSetLength(spMsg, 8);
	SendSerialMsg(spMsg);
	return TRUE;
}

BOOL CDataProcesser::SendSetConfigByNodeid(UC nodeid,UC subid,const UC * pCfgData, UC offset,UC size)
{
	int left = size;
	int sendoffset = 0;
	do
	{
		MySetConfigByNode_t setCfgByNode;
		setCfgByNode.subtype = SCANNER_SETCONFIG;
		setCfgByNode.offset = offset;
		int payl_len = left < sizeof(setCfgByNode.ConfigBlock) ? left : sizeof(setCfgByNode.ConfigBlock);
		memcpy(setCfgByNode.ConfigBlock, pCfgData + sendoffset, payl_len);
		UC length = 2 + payl_len;
		SPScanMsgPtr spMsg = boost::make_shared<MyScanMsg_t>();
		build(spMsg, nodeid, subid, C_INTERNAL, I_GET_NONCE, 1, 0);
		mSetLength(spMsg, length);
		memcpy(&(spMsg->data[0]), &setCfgByNode, length);
		m_arrOutMsg.Add(spMsg);
		m_evtOutData.SetEvent();
		sendoffset += payl_len;
		offset += payl_len;
		left -= payl_len;
	} while (left > 0);
	return TRUE;
}

BOOL CDataProcesser::SendSetConfigByUniqueid(UC* uniqueid, UC uniLen, const UC * pCfgData,UC offset, UC size)
{
	int left = size;
	int sendoffset = 0;
	do
	{
		MySetConfigByUnique_t setCfgByUni;
		setCfgByUni.subtype = SCANNER_SETDEV_CONFIG;
		setCfgByUni.offset = offset;
		memcpy(setCfgByUni.uniqueid, uniqueid, uniLen);
		int payl_len = left < sizeof(setCfgByUni.ConfigBlock) ? left : sizeof(setCfgByUni.ConfigBlock);
		memcpy(setCfgByUni.ConfigBlock, pCfgData+ sendoffset, payl_len);
		UC length = 10 + payl_len;
		SPScanMsgPtr spMsg = boost::make_shared<MyScanMsg_t>();
		build(spMsg, 255, 0, C_INTERNAL, I_GET_NONCE, 1, 0);
		mSetLength(spMsg, length);
		memcpy(&(spMsg->data[0]), &setCfgByUni, length);
		m_arrOutMsg.Add(spMsg);
		m_evtOutData.SetEvent();
		sendoffset += payl_len;
		offset += payl_len;
		left -= payl_len;
	} while (left > 0);
	return TRUE;
}

BOOL CDataProcesser::SendGetConfigByUniqueid(UC* uniqueid,UC uniLen,UC offset, UC size)
{
	MyGetConfigByUnique_t getCfgByUni;
	getCfgByUni.subtype = SCANNER_GETDEV_CONFIG;
	getCfgByUni.offset = offset;
	memcpy(getCfgByUni.uniqueid, uniqueid, uniLen);
	getCfgByUni.cfgblocklen = size;
	UC length = sizeof(MyGetConfigByUnique_t);
	SPScanMsgPtr spMsg = boost::make_shared<MyScanMsg_t>();
	build(spMsg, 255, 0, C_INTERNAL, I_GET_NONCE, 1, 0);
	mSetLength(spMsg, length);
	memcpy(&(spMsg->data[0]), &getCfgByUni, length);
	SendSerialMsg(spMsg);
	return TRUE;
}

BOOL CDataProcesser::SendGetConfigByNode(UC nodeid, UC subid, UC offset, UC size)
{
	MyGetConfigByNode_t getCfgByUni;
	getCfgByUni.subtype = SCANNER_GETCONFIG;
	getCfgByUni.offset = offset;
	getCfgByUni.cfgblocklen = size;
	UC length = sizeof(MyGetConfigByUnique_t) + 1;
	SPScanMsgPtr spMsg = boost::make_shared<MyScanMsg_t>();
	build(spMsg, nodeid, subid, C_INTERNAL, I_GET_NONCE, 1, 0);
	mSetLength(spMsg, length);
	memcpy(&(spMsg->data[0]), &getCfgByUni, length - 1);
	SendSerialMsg(spMsg);
	return TRUE;
}

BOOL CDataProcesser::SendRFByNode(UC Nodeid, UC subid, MySetUpRF_t& rfparam, UC paramlen)
{
	UC length = paramlen;
	SPScanMsgPtr spMsg = boost::make_shared<MyScanMsg_t>();
	spMsg->data[0] = SCANNER_SETUPDEV_RF;
	length++;
	//SPScanMsgPtr spMsg = boost::make_shared<MyScanMsg_t>();
	build(spMsg, Nodeid, subid, C_INTERNAL, I_GET_NONCE, 1, 0);
	mSetLength(spMsg, length);
	memcpy(&(spMsg->data[1]), &rfparam, paramlen);
	SendSerialMsg(spMsg);
	return TRUE;
}

BOOL CDataProcesser::SendRFByUniqueid(UC* uniqueid, UC uniLen, MySetUpRF_t& rfparam,UC paramlen)
{
	UC length = paramlen;
	SPScanMsgPtr spMsg = boost::make_shared<MyScanMsg_t>();
	spMsg->data[0] = SCANNER_SETUPDEV_RF;
	length++;
	memcpy(&spMsg->data[1], uniqueid, uniLen);
	length += uniLen;
	build(spMsg, 255, 0, C_INTERNAL, I_GET_NONCE, 1, 0);
	mSetLength(spMsg, length);
	memcpy(&(spMsg->data[1+UNIQUE_ID_LEN]), &rfparam, paramlen);
	SendSerialMsg(spMsg);
	return TRUE;
}

int CDataProcesser::ProcessSerialMsg(SPScanMsgPtr lpMsg)
{
	if (lpMsg->header.last != 0)
	{
		if (lpMsg->header.last == 'C')
		{ // status change report
		}
	}
	else
	{
		EScanAction scanaction = (EScanAction)lpMsg->data[0];
		if (scanaction == SCANNER_PROBE)
		{
			ProcessProbe(lpMsg);
		}
		else if (scanaction == SCANNER_GETCONFIG)
		{
			ProcessGetConfig(lpMsg);
		}
		else if (scanaction == SCANNER_GETDEV_CONFIG)
		{
			ProcessGetConfig(lpMsg);
		}
		else
		{
			PLOG(ELL_ERROR, "Unkown msg type=%d!", scanaction);
		}
	}
	return 1;
}

int CDataProcesser::ProcessProbe(SPScanMsgPtr lpMsg)
{
	UC uniqueid[8] = { 0 };
	memcpy(uniqueid, &(lpMsg->data[1]), sizeof(uniqueid));
	UC nodeid = lpMsg->header.sender;
	UC subid = lpMsg->header.sensor;
	BaseDeviceInfo_t baseInfo;
	UC playl_len = 9;
	baseInfo.version = lpMsg->data[playl_len++];
	baseInfo.type = lpMsg->data[playl_len++];
	baseInfo.nodeID = lpMsg->data[playl_len++];
	baseInfo.subID = lpMsg->data[playl_len++];
	baseInfo.rfChannel = lpMsg->data[playl_len++];
	baseInfo.rfDataRate = lpMsg->data[playl_len] >> 2;
	baseInfo.rfPowerLevel = lpMsg->data[playl_len++] & 0x03;
	memcpy(baseInfo.networkID, &(lpMsg->data[playl_len]), sizeof(baseInfo.networkID));
	ConfigCenter::GetInstance()->AddDeviceBaseInfo(GetHexStr(uniqueid, 8), baseInfo);
	
	PLOG(ELL_INFORMATION, "Recv probe nodeid=%d,subid=%d,uniqueid=%s,version=%d,type=%d,channel=%d,rfDatarate=%d,rfpowerlevel=%d,networkid=%s", 
		baseInfo.nodeID, baseInfo.subID, GetHexStr(uniqueid,8), baseInfo.version, baseInfo.type, baseInfo.rfChannel, baseInfo.rfDataRate, baseInfo.rfPowerLevel, GetHexStr(baseInfo.networkID, 5,TRUE));
	return 1;
}

int CDataProcesser::ProcessGetConfig(SPScanMsgPtr lpMsg)
{
	unsigned char offset = lpMsg->data[1];
	if (offset < 0)
	{
		PLOG(ELL_ERROR, "ProcessProbe error offset less than 0,invalid");
		return -1;
	}
	// tests
	UC uniqueid[8] = { 0x00,0x24,0x00,0x3A,0x0E,0x47,0x31,0x32 };
	if (lpMsg->data[0] == SCANNER_GETCONFIG)
	{
		// get uniqueid by nodeid & subid
	}
	else
	{

	}
	memcpy(uniqueid, &lpMsg->data[2], 8);
	unsigned char playloadsize = mGetLength(lpMsg);
	unsigned char cfgblocksize = playloadsize - 10;
	ConfigCenter::GetInstance()->SetConfigStrByUniqueid(GetHexStr(uniqueid, 8), offset, &(lpMsg->data[10]), cfgblocksize);
	ConfigCenter::GetInstance()->GetConfigStrByUniqueid(GetHexStr(uniqueid,8));
	//if (lpMsg->header.sender == 130)
	//{
	//	if (offset < sizeof(SuperSensorConfig_t))
	//	{
	//		unsigned char playloadsize = mGetLength(lpMsg);
	//		unsigned char cfgblocksize = playloadsize - 1;
	//		memcpy((void *)((size_t)(&g_SuperSensorCfg) + offset), &(lpMsg->data[2]), cfgblocksize);
	//		CString sbtnActionHex = "";
	//		for (DWORD i = 0; i < MAX_NUM_BUTTONS; i++)
	//		{
	//			for (DWORD j = 0; j < 8; j++)
	//			{
	//				CString sTmp = "";
	//				sTmp.Format("0x%02X", g_SuperSensorCfg.btnAction[i][j]);
	//				sbtnActionHex += sTmp;
	//			}
	//		}
	//		PLOG(ELL_INFORMATION, "supersensor config is :");
	//		PLOG(ELL_INFORMATION, "version = %d", g_SuperSensorCfg.version);
	//		PLOG(ELL_INFORMATION, "nodeid = %d", g_SuperSensorCfg.nodeID);
	//		PLOG(ELL_INFORMATION, "networkid=%s", GetNetworkidHexStr(g_SuperSensorCfg.NetworkID,5));
	//		PLOG(ELL_INFORMATION, "present=%d", g_SuperSensorCfg.present);
	//		PLOG(ELL_INFORMATION, "state=%d", g_SuperSensorCfg.state);
	//		PLOG(ELL_INFORMATION, "reserved=%d", g_SuperSensorCfg.reserved0);
	//		PLOG(ELL_INFORMATION, "subid=%d", g_SuperSensorCfg.subID);
	//		PLOG(ELL_INFORMATION, "type=%d", g_SuperSensorCfg.type);
	//		PLOG(ELL_INFORMATION, "token=0x%02X", g_SuperSensorCfg.token[0] * 256 + g_SuperSensorCfg.token[1]);
	//		PLOG(ELL_INFORMATION, "powerlevel=%d", g_SuperSensorCfg.rfPowerLevel);
	//		PLOG(ELL_INFORMATION, "swtimes=%d", g_SuperSensorCfg.swTimes);
	//		PLOG(ELL_INFORMATION, "rpttimes=%d", g_SuperSensorCfg.rptTimes);
	//		PLOG(ELL_INFORMATION, "reserverd1=%d", g_SuperSensorCfg.reserved1);
	//		PLOG(ELL_INFORMATION, "senmap=0x%02X", g_SuperSensorCfg.senMap[0]*256 + g_SuperSensorCfg.senMap[1]);
	//		PLOG(ELL_INFORMATION, "relaykey=%01x", g_SuperSensorCfg.relay_key_value);
	//		PLOG(ELL_INFORMATION, "channel=%d", g_SuperSensorCfg.rfChannel);
	//		PLOG(ELL_INFORMATION, "datarate=%d", g_SuperSensorCfg.rfDataRate);
	//		PLOG(ELL_INFORMATION, "reserved=%d", g_SuperSensorCfg.reserved2);
	//		PLOG(ELL_INFORMATION, "btnaction=%s", sbtnActionHex);
	//	}
	//	else
	//	{
	//		PLOG(ELL_ERROR, "ProcessProbe error offset over max length,invalid");
	//		return -1;
	//	}
	//}
	return 1;
}

int CDataProcesser::ProcessScannerStatus(SPScanMsgPtr lpMsg)
{
	unsigned char playloadsize = mGetLength(lpMsg);
	memcpy(&g_ScannerStatus, &(lpMsg->data[0]), playloadsize);
	PLOG(ELL_INFORMATION, "scanner status is :");
	PLOG(ELL_INFORMATION, "channel = %d", g_ScannerStatus.channel);
	PLOG(ELL_INFORMATION, "bandwidth = %d", g_ScannerStatus.bandwidth);
	PLOG(ELL_INFORMATION, "powerlevel=%s", g_ScannerStatus.powerlevel);
	PLOG(ELL_INFORMATION, "network=%s", GetHexStr(g_ScannerStatus.network,TRUE));
	return 1;
}

