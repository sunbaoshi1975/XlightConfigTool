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
typedef struct
{
    uint8_t subtype;
    uint8_t uniqueid[8];
    uint8_t channel;
    uint8_t datarate;
    uint8_t powerlevel;
    uint8_t network[6];
    uint8_t nodeid;        //unnecessary data field，has this field，need change nodeid，0 indicate ignore this parameter
    uint8_t subid;         //unnecessary data field，has this field，need change subid
}MySetUpRF_t;

typedef struct
{
	uint8_t subtype;
	uint8_t channel;
	uint8_t datarate;
	uint8_t powerlevel;
	uint8_t network[6];
	uint8_t nodeid;        //unnecessary data field，has this field，need change nodeid，0 indicate ignore this parameter
	uint8_t subid;         //unnecessary data field，has this field，need change subid
}MySetUpRFByNode_t;
//////set rf /////////////////////////////////////////////////
