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
