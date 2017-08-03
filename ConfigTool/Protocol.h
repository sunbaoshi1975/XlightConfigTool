#pragma once
typedef struct
{
	uint8_t subtype;
	uint8_t offset;
	uint8_t ConfigBlock[12];                 // 配置数据块（二进制数据）
}MySetConfigByNode_t;

typedef struct
{
	uint8_t subtype;
	uint8_t offset;
	uint8_t uniqueid[8];
	uint8_t ConfigBlock[12];                 // 配置数据块（二进制数据）
}MySetConfigByUnique_t;

typedef struct
{
	uint8_t subtype;
	uint8_t offset;
	uint8_t cfgblocklen;
}MyGetConfigByNode_t;

typedef struct
{
	uint8_t subtype;
	uint8_t offset;
	uint8_t uniqueid[8];
	uint8_t cfgblocklen;
}MyGetConfigByUnique_t;

//////set rf /////////////////////////////////////////////////
//typedef struct
//{
//    uint8_t subtype;
//    uint8_t uniqueid[8];
//    uint8_t channel;
//    uint8_t datarate;
//    uint8_t powerlevel;
//    uint8_t network[6];
//    uint8_t nodeid;        //unnecessary data field，has this field，need change nodeid，0 indicate ignore this parameter
//    uint8_t subid;         //unnecessary data field，has this field，need change subid
//}MySetUpRF_t;

typedef struct
{
	uint8_t channel;
	uint8_t datarate;
	uint8_t powerlevel;
	uint8_t network[6];
	uint8_t nodeid;        //unnecessary data field，has this field，need change nodeid，0 indicate ignore this parameter
	uint8_t subid;         //unnecessary data field，has this field，need change subid
}MySetUpRF_t;
//////set rf /////////////////////////////////////////////////


//////////////device type/////////////////////////////////////
typedef enum
{
	devtypUnknown = 0,
	// Color ring - Rainbow
	devtypCRing3,
	devtypCRing2,
	devtypCBar,
	devtypCFrame,
	devtypCWave,
	devtypCRing1 = 31,

	// White ring - Sunny
	devtypWRing3 = 32,
	devtypWRing2,
	devtypWBar,
	devtypWFrame,
	devtypWWave,
	devtypWSquare60,      // 60 * 60
	devtypWPanel120_30,   // 120 * 30
	devtypWBlackboard,    // Blackboard lamp
	devtypWRing1 = 95,

	// Color & Motion ring - Mirage
	devtypMRing3 = 96,
	devtypMRing2,
	devtypMBar,
	devtypMFrame,
	devtypMWave,
	devtypMRing1 = 127,

	devtypDummy = 255
} devicetype_t;


// Remote type
typedef enum
{
	remotetypUnknown = 224,
	remotetypRFSimply,
	remotetypRFStandard,
	remotetypRFEnhanced,
	remotetypDummy
} remotetype_t;

// Target Type (mask)
#define ZEN_TARGET_CURTAIN      0x80
#define ZEN_TARGET_AIRPURIFIER  0x90
#define ZEN_TARGET_AIRCONDITION 0xA0
#define ZEN_TARGET_SPOTLIGHT    0xB0
#define ZEN_TARGET_SUPERSENSOR  0xC0

#define IS_SUNNY(DevType)           ((DevType) >= devtypWRing3 && (DevType) <= devtypWRing1)
#define IS_RAINBOW(DevType)         ((DevType) >= devtypCRing3 && (DevType) <= devtypCRing1)
#define IS_MIRAGE(DevType)          ((DevType) >= devtypMRing3 && (DevType) <= devtypMRing1)
#define IS_VALID_REMOTE(DevType)    ((DevType) >= remotetypRFSimply && (DevType) <= remotetypRFEnhanced)
#define IS_TARGET_CURTAIN(DevType)         (((DevType) & 0xF0) == ZEN_TARGET_CURTAIN)
#define IS_TARGET_AIRPURIFIER(DevType)     (((DevType) & 0xF0) == ZEN_TARGET_AIRPURIFIER)
#define IS_TARGET_AIRCONDITION(DevType)    (((DevType) & 0xF0) == ZEN_TARGET_AIRCONDITION)
#define IS_TARGET_SPOTLIGHT(DevType)       (((DevType) & 0xF0) == ZEN_TARGET_SPOTLIGHT)
#define IS_TARGET_SUPERSENSOR(DevType)     (((DevType) & 0xF0) == ZEN_TARGET_SUPERSENSOR)
//////////////device type/////////////////////////////////////