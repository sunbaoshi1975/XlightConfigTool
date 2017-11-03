//
// Created by lily on 5/2/17.
//
#include "stdafx.h"
#include "Public.h"
CString JsonDocToString(const rapidjson::Document& jsDoc)
{
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	jsDoc.Accept(writer);
	CString str = buffer.GetString();
	return str;
}

CString JsonDocToString(JsonDocPtr spDoc)
{
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	spDoc->Accept(writer);
	CString str = buffer.GetString();
	return str;
}

CString JsonValueToString(const rapidjson::Value& jsValue)
{
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	jsValue.Accept(writer);
	CString str = buffer.GetString();
	return str;
}

void AddStrMember(JsonDocPtr doc, const char *key, const char *value)
{
	rapidjson::Value strValue(rapidjson::kStringType);
	strValue.SetString(value, doc->GetAllocator());
	rapidjson::Value strKey(rapidjson::kStringType);
	strKey.SetString(key, doc->GetAllocator());
	doc->AddMember(strKey, strValue, doc->GetAllocator());
}

void RemoveStrMember(JsonDocPtr doc, const char *key, const char *value)
{
	if (doc->HasMember(key)) {
		doc->RemoveMember(key);
	}
}

void SetStrMember(JsonDocPtr doc, const char *key, const char *value)
{
	RemoveStrMember(doc, key, value);
	AddStrMember(doc, key, value);
}

void AddStrMember(rapidjson::Value& jsonValue, rapidjson::Document::AllocatorType& allocator, const char *key, const char *value)
{
	rapidjson::Value strValue(rapidjson::kStringType);
	strValue.SetString(value, allocator);
	rapidjson::Value strKey(rapidjson::kStringType);
	strKey.SetString(key, allocator);
	jsonValue.AddMember(strKey, strValue, allocator);
}

void RemoveStrMember(rapidjson::Value& jsonValue, rapidjson::Document::AllocatorType& allocator, const char *key, const char *value)
{
	if (jsonValue.HasMember(key)) {
		jsonValue.RemoveMember(key);
	}
}

void SetStrMember(rapidjson::Value& jsonValue, rapidjson::Document::AllocatorType& allocator, const char *key, const char *value)
{
	RemoveStrMember(jsonValue,allocator, key, value);
	AddStrMember(jsonValue,allocator, key, value);
}

CString GetHexStr(const UC* UniqueID, int len,BOOL Des/* = FALSE*/)
{
	CString hexstr = "";
	if (Des)
	{
		for (int i = len-1; i >= 0; i--)
		{
			CString sTmp = "";
			sTmp.Format("%02X", UniqueID[i]);
			hexstr += sTmp;
		}
	}
	else
	{
		for (DWORD i = 0; i < len; i++)
		{
			CString sTmp = "";
			sTmp.Format("%02X", UniqueID[i]);
			hexstr += sTmp;
		}
	}
	return hexstr;
}

void ConvertHexStr2ByteArr(CString &sUniqueid, UC* uniqueid,UC len)
{
	for (int i=0;i<len;i++)
	{
		CString sTmp = sUniqueid.Mid(i*2,2);
		uniqueid[i] = strtol(sTmp, NULL,16);
	}
}

CString ToStrbyDevtype(UC devType)
{
	if (devType == 2)
	{
		return "sc";
	}
	if (IS_SUNNY(devType))
	{
		if (devType == devtypWSquare60)
		{
			return "lamp_classroom";
		}
		else if (devType == devtypWBlackboard)
		{
			return "lamp_blackboard";
		}
		return "sunny";
	}
	if (IS_RAINBOW(devType))
	{

		return "rainbow";
	}
	if (IS_MIRAGE(devType))
	{
		return "mirage";
	}
	if (IS_VALID_REMOTE(devType))
	{
		return "remote";
	}
	if (IS_TARGET_CURTAIN(devType))
	{
		return "curtain";
	}
	if (IS_TARGET_AIRPURIFIER(devType))
	{
		return "airpurifier";
	}
	if (IS_TARGET_AIRCONDITION(devType))
	{
		return "aircondition";
	}
	if (IS_TARGET_SPOTLIGHT(devType))
	{
		return "spotlight";
	}
	if (IS_TARGET_SUPERSENSOR(devType))
	{
		return "sensor";
	}
	return "unkown";
}

void ConverDevBase2RFSetStruct(BaseDeviceInfo_t& base, MySetUpRF_t& rf)
{
	rf.channel = base.rfChannel;
	rf.datarate = base.rfDataRate;
	rf.powerlevel = base.rfPowerLevel;
	memcpy(rf.network,base.networkID,sizeof(rf.network));
	rf.nodeid = base.nodeID;
	rf.subid = base.subID;
}




