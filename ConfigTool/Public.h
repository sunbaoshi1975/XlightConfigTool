#pragma once
//
// Created by lily on 5/2/17.
//
#ifndef PROJECT_COMMON_H
#define PROJECT_COMMON_H
#include "global.h"
#include "stdafx.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "Protocol.h"
typedef boost::shared_ptr<rapidjson::Document> JsonDocPtr;
typedef boost::weak_ptr<rapidjson::Document> JsonDocWeakPtr;
CString JsonDocToString(const rapidjson::Document& jsDoc);
CString JsonValueToString(const rapidjson::Value& jsValue);
CString JsonDocToString(JsonDocPtr spDoc);
void RemoveStrMember(rapidjson::Value& jsonValue, rapidjson::Document::AllocatorType& allocator, const char *key, const char *value);
void RemoveStrMember(JsonDocPtr doc, const char *key, const char *value);
void SetStrMember(rapidjson::Value& jsonValue, rapidjson::Document::AllocatorType& allocator, const char *key, const char *value);
void SetStrMember(JsonDocPtr doc, const char *key, const char *value);
CString GetHexStr(const UC* UniqueID, int len, BOOL Des = FALSE);
void ConvertHexStr2ByteArr(CString &sUniqueid, UC* uniqueid, UC len);
CString ToStrbyDevtype(UC devType);
void ConverDevBase2RFSetStruct(BaseDeviceInfo_t& base, MySetUpRF_t& rf);
void AddStrMember(rapidjson::Value& jsonValue, rapidjson::Document::AllocatorType& allocator, const char *key, const char *value);
void AddStrMember(JsonDocPtr doc, const char *key, const char *value);
#define JSON_GETSTRING(jsValue,key,str) if (jsValue.HasMember(key) && jsValue[key].IsString()) str=jsValue[key].GetString();
#define JSON_GETDOUBLE(jsValue,key,dou) if (jsValue.HasMember(key) && jsValue[key].IsDouble()) dou=jsValue[key].GetDouble();
#define JSON_GETNUMBER(jsValue,key,num) if (jsValue.HasMember(key) && jsValue[key].IsInt()) num=jsValue[key].GetInt();
#define JSON_GETBOOLEAN(jsValue,key,b) if (jsValue.HasMember(key) && jsValue[key].IsBool()) b=jsValue[key].GetBool();
#define JSON_GETOBJECT(jsValue,key,obj) if (jsValue.HasMember(key) && jsValue[key].IsObject()) obj=jsValue[key];

#endif //PROJECT_COMMON_H
