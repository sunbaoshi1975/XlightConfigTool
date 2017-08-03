#pragma once
#define UC unsigned char
#define US unsigned short
#define MAX_NUM_BUTTONS 4
typedef struct
{
	UC action;                                // Type of action
	UC keyMap;                                // Button Key Map: 8 bits for each button, one bit corresponds to one relay key
} Button_Action_t;

typedef struct
{
	// Static & status parameters
	UC version : 8;           // Data version, other than 0xFF
	UC present : 1;           // 0 - not present; 1 - present
	UC state : 1;           // SuperSensor On/Off
	UC swTimes : 3;           // On/Off times
	UC reserved0 : 3;
	UC relay_key_value : 8;           // Relay Key Bitmap

	// Configurable parameters
	UC nodeID;                                // Node ID for this device
	UC subID;                                 // SubID
	UC NetworkID[6];
	UC rfChannel;                             // RF Channel: [0..127]
	UC rfPowerLevel : 2;           // RF Power Level 0..3
	UC rfDataRate : 2;           // RF Data Rate [0..2], 0 for 1Mbps, or 1 for 2Mbps, 2 for 250kbs
	UC rptTimes : 2;           // Sending message max repeat times [0..3]
	UC reserved1 : 2;
	UC type;                                  // Type of SuperSensor
	UC token[2];
	UC reserved2 : 8;
	UC senMap[2];          // Sensor Map 
	Button_Action_t btnAction[MAX_NUM_BUTTONS][8];
} SuperSensorConfig_t;
extern SuperSensorConfig_t g_SuperSensorCfg;

// System Startup Status
enum ERFScannerStatus
{
	SYS_INIT,
	SYS_RESET,
	SYS_WAIT_COMMAND,
	SYS_PAUSE=4,
	SYS_RUNNING
};
typedef struct
{
	UC channel;
	UC bandwidth;
	UC powerlevel;
	UC network[5];
	UC scanstatus;
}ScannerStatus_t;
extern ScannerStatus_t g_ScannerStatus;

typedef enum { RF24_PA_MIN = 0, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX, RF24_PA_ERROR } rf24_pa_dbm_e;
typedef enum { RF24_1MBPS = 0, RF24_2MBPS, RF24_250KBPS } rf24_datarate_e;

#define RFS_CMD_SCAN_START      'S'
#define RFS_CMD_SCAN_STOP       'T'
#define RFS_CMD_SCAN_PAUSE      'P'
#define RFS_CMD_SCAN_RESUME     'R'
#define RFS_RPT_SCAN_STATUS     'C'

#define UNIQUE_ID_LEN 8
#define NETWORK_ID_LEN 5
typedef struct
{
	UC Uniqueid[8];
	UC version;
	UC type;
	UC nodeID;
	UC subID;
	UC rfChannel;
	UC rfDataRate;
	UC rfPowerLevel;
	UC networkID[6];
}BaseDeviceInfo_t;