#pragma once
typedef struct
{
	uint8_t subtype;
	uint8_t offset;
	uint8_t ConfigBlock[12];                 // �������ݿ飨���������ݣ�
}MySetConfigByNode_t;

typedef struct
{
	uint8_t subtype;
	uint8_t offset;
	uint8_t uniqueid[8];
	uint8_t ConfigBlock[12];                 // �������ݿ飨���������ݣ�
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
