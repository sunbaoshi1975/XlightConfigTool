#pragma once
#include "global.h"
#include "stdafx.h"
#ifndef BIT
#define BIT(n)                  ( 1<<(n) )
#endif
// Create a bitmask of length len.
#define BIT_MASK(len)           ( BIT(len)-1 )
// Create a bitfield mask of length starting at bit 'start'.
#define BF_MASK(start, len)     ( BIT_MASK(len)<<(start) )

// Prepare a bitmask for insertion or combining.
#define BF_PREP(x, start, len)  ( ((x)&BIT_MASK(len)) << (start) )
// Extract a bitfield of length len starting at bit 'start' from y.
#define BF_GET(y, start, len)   ( ((y)>>(start)) & BIT_MASK(len) )
// Insert a new bitfield value x into y.
#define BF_SET(y, x, start, len)    ( y= ((y) &~ BF_MASK(start, len)) | BF_PREP(x, start, len) )

// Getters/setters for special bit fields in header
#define mSetVersion(_pmsg,_version) BF_SET(_pmsg->header.version_length, _version, 0, 2)
#define mGetVersion(_pmsg) BF_GET(_pmsg->header.version_length, 0, 2)

#define mSetSigned(_pmsg,_signed) BF_SET(_pmsg->header.version_length, _signed, 2, 1)
#define mGetSigned(_pmsg) BF_GET(_pmsg->header.version_length, 2, 1)

#define mSetLength(_pmsg,_length) BF_SET(_pmsg->header.version_length, _length, 3, 5)
#define mGetLength(_pmsg) BF_GET(_pmsg->header.version_length, 3, 5)

#define mSetCommand(_pmsg,_command) BF_SET(_pmsg->header.command_ack_payload, _command, 0, 3)
#define mGetCommand(_pmsg) BF_GET(_pmsg->header.command_ack_payload, 0, 3)

#define mSetRequestAck(_pmsg,_rack) BF_SET(_pmsg->header.command_ack_payload, _rack, 3, 1)
#define mGetRequestAck(_pmsg) BF_GET(_pmsg->header.command_ack_payload, 3, 1)

#define mSetAck(_pmsg,_ackMsg) BF_SET(_pmsg->header.command_ack_payload, _ackMsg, 4, 1)
#define mGetAck(_pmsg) BF_GET(_pmsg->header.command_ack_payload, 4, 1)

#define mSetPayloadType(_pmsg, _pt) BF_SET(_pmsg->header.command_ack_payload, _pt, 5, 3)
#define mGetPayloadType(_pmsg) BF_GET(_pmsg->header.command_ack_payload, 5, 3)

#define SENDER 250
#define PROTOCOL_VERSION 1
#define MAX_MESSAGE_LENGTH 32
#define HEADER_SIZE 7
#define MAX_PAYLOAD (MAX_MESSAGE_LENGTH - HEADER_SIZE)

// Message types
typedef enum {
	C_PRESENTATION = 0,
	C_SET = 1,
	C_REQ = 2,
	C_INTERNAL = 3,
	C_STREAM = 4 // For Firmware and other larger chunks of data that need to be divided into pieces.
} mysensor_command;
// Type of internal messages (for internal messages)
typedef enum {
	I_BATTERY_LEVEL, I_TIME, I_VERSION, I_ID_REQUEST, I_ID_RESPONSE,
	I_INCLUSION_MODE, I_CONFIG, I_FIND_PARENT, I_FIND_PARENT_RESPONSE,
	I_LOG_MESSAGE, I_CHILDREN, I_SKETCH_NAME, I_SKETCH_VERSION,
	I_REBOOT, I_GATEWAY_READY, I_REQUEST_SIGNING, I_GET_NONCE, I_GET_NONCE_RESPONSE
} mysensor_internal;
// I_GET_NONCE sub-type
enum EScanAction{
	SCANNER_PROBE = 0,
	SCANNER_SETUP_RF,           // by NodeID & SubID
	SCANNER_SETUPDEV_RF,        // by UniqueID

	SCANNER_GETCONFIG = 8,      // by NodeID & SubID
	SCANNER_SETCONFIG,
	SCANNER_GETDEV_CONFIG,      // by UniqueID
	SCANNER_SETDEV_CONFIG,

	SCANNER_TEST_NODE = 16,     // by NodeID & SubID
	SCANNER_TEST_DEVICE,        // by UniqueID
};

typedef struct
{
	UC last;            	 	// 8 bit - Id of last node this message passed
	UC sender;          	 	// 8 bit - Id of sender node (origin)
	UC destination;     	 	// 8 bit - Id of destination node

	UC version_length;		 	// 2 bit - Protocol version
									// 1 bit - Signed flag
									// 5 bit - Length of payload
	UC command_ack_payload; // 3 bit - Command type
								 // 1 bit - Request an ack - Indicator that receiver should send an ack back.
								 // 1 bit - Is ack messsage - Indicator that this is the actual ack message.
								 // 3 bit - Payload data type
	UC type;            	 	// 8 bit - Type varies depending on command
	UC sensor;          	 	// 8 bit - Id of sensor that this message concerns.
} MyMsgHeader_t;
typedef struct {
	MyMsgHeader_t header;
	unsigned char data[MAX_PAYLOAD + 1];
}MyScanMsg_t;
typedef boost::shared_ptr<MyScanMsg_t> SPScanMsgPtr;
typedef struct _tagMessageHelperBase
{
	LPBYTE m_pBuffer;
	DWORD m_dwSize;
}SMessageHelperBase, *LPSMessageHelperBase;

typedef struct _tagMessageNobody
{
}SMessageNobody;

//template < class T >
//class CMessageHelper :
//	public SMessageHelperBase
//{
//public:
//	CMessageHelper(UC _destination,  UC _command, UC _type, bool _enableAck, bool _isAck)
//	{
//		m_bCreate = TRUE;
//		m_pBuffer = NULL;
//		m_dwSize = 0;
//		pHeader = NULL;
//		pPlayLoad = NULL;
//		if (nLength < sizeof(MyMsgHeader_t))
//		{
//			if (typeid(T) == typeid(SMessageNobody))
//				nLength = sizeof(MyMsgHeader_t);
//			else
//				nLength = sizeof(MyMsgHeader_t) + sizeof(T);
//		}
//
//		m_pBuffer = new BYTE[nLength];
//		_ASSERT(m_pBuffer);
//		if (m_pBuffer)
//		{
//			ZeroMemory(m_pBuffer, nLength);
//			pHeader = (MyMsgHeader_t*)m_pBuffer;
//			pPlayLoad = (T*)(m_pBuffer + sizeof(MyMsgHeader_t));
//			m_dwSize = nLength;
//			pHeader->version_length = PROTOCOL_VERSION;
//			pHeader->sender = 250;
//			pHeader->destination = _destination;
//			pHeader->sensor = 0;
//			pHeader->type = I_GET_NONCE;
//			mSetCommand(pHeader, C_INTERNAL);
//			mSetRequestAck(pHeader, _enableAck);
//			mSetAck(pHeader, _isAck);
//		}
//		else
//		{
//#ifdef _INC_PICASOFT_LOG
//			PLOG(ELL_ERROR, _T("根据给定的命令字创建消息时申请内存失败！！！！"));
//#endif
//		}
//	}
//	CMessageHelper(LPVOID lpMsg)
//	{
//		m_bCreate = FALSE;
//		m_pBuffer = LPBYTE(lpMsg);
//		m_dwSize = 0;
//		pHeader = NULL;
//		pPlayLoad = NULL;
//
//		ASSERT(m_pBuffer);
//		if (m_pBuffer)
//		{
//			pHeader = (MyMessage_t*)m_pBuffer;
//			pPlayLoad = (T*)(m_pBuffer + sizeof(MyMessage_t));
//			m_dwSize = mGetLength(pHeader);
//		}
//		else
//		{
//#ifdef _INC_PICASOFT_LOG
//			PLOG(ELL_ERROR, _T("申请内存失败！"));
//#endif
//		}
//	}
//	~CMessageHelper(void)
//	{
//		if (m_bCreate && m_pBuffer) delete[] m_pBuffer;
//	}
//	// 分离消息
//	MyMessage_t* detach(LPDWORD pdwSize)
//	{
//		if (m_pBuffer && m_dwSize)
//		{
//			if (pdwSize)
//			{
//				*pdwSize = m_dwSize;
//			}
//			LPBYTE pBuffer = m_pBuffer;
//			m_pBuffer = NULL;
//			m_dwSize = 0;
//			return (SMsgHeader*)pBuffer;
//		}
//		return NULL;
//	}
//	LPVOID endptr()
//	{
//		return LPBYTE(pPlayLoad) + sizeof(T);
//	}
//public:// --属性
//	MyMessage_t* pHeader;
//	T* pPlayLoad;
//private:
//	// 标记是否自己申请了消息内存
//	BOOL m_bCreate;
//};
