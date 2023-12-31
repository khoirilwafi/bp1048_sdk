/**
 *******************************************************************************
 * @file    ble_api.h
 * @author  Halley
 * @version V1.0.0
 * @date    26-June-2012
 * @brief   Bluetooth LE releated APIs
 * @maintainer Halley
 *******************************************************************************
 * @attention
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, MVSILICON SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
 * INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * <h2><center>&copy; COPYRIGHT 2013 MVSilicon </center></h2>
 */

/**
* @addtogroup Bluetooth
* @{
* @defgroup ble_api ble_api.h
* @{
*/

#include "type.h"

#ifndef __BLE_API_H__
#define __BLE_API_H__

/**
* GAP_BROADCASE_MODE
*/
enum
{
	NON_BROADCAST_MODE,
	BROADCAST_MODE
}; 

/**
* GAP_DISCOVERABLE_MODE
*/
enum
{
	NON_DISCOVERABLE_MODE,
	LIMITED_DISCOVERABLE_MODE,
	GENERAL_DISCOVERABLE_MODE
};

/**
* GAP_CONNECTABLE_MODE
*/
enum
{
	NON_CONNECTABLE_MODE,
	DIRECTED_CONNECTABLE_MODE,
	UNDIRECTED_CONNECTABLE_MODE
} ;

/**
* GAP_BONDABLE_MODE
*/
enum
{
	NON_BONDABLE_MODE,
	BONDABLE_MODE
};


typedef struct _GAP_MODE
{
	uint8_t broadcase_mode;
	uint8_t discoverable_mode;
	uint8_t connectable_mode;
	uint8_t bondable_mode;
} GAP_MODE;

/**
* if this transaction mode, short value written by client (Write Request)
*/
#define TRANSACTION_MODE_NONE      0x0
/**
* if this transaction mode, prepare write command send by client (Prepare Write Request)
*/
#define TRANSACTION_MODE_ACTIVE    0x1
/**
* if this transaction mode, execute write command send by client (Execute Write Request)
*/
#define TRANSACTION_MODE_EXECUTE   0x2
/**
* execute write command end
*/
#define TRANSACTION_MODE_CANCEL    0x3


/**
* The role of BLE device
*/
typedef enum{
	CENTRAL_DEVICE,
	PERIPHERAL_DEVICE
}BLE_DEVICE_ROLE;

/**
* Function define
* 根据handle获取uuid，如果是自己定义的uuid，则需要返回 >= 0 的值，否则返回 -1
*/
typedef int16_t (*att_read_f)(uint16_t /*con_handle*/, uint16_t /*attribute_handle*/, uint16_t /*offset*/, uint8_t * /*buffer*/, uint16_t /*buffer_size*/);

/*
* Function define
* 根据handle获取uuid，如果是自己定义的uuid，则需要返回 >= 0 的值，否则返回 -1
*/
typedef int16_t (*att_write_f)(uint16_t /*con_handle*/, uint16_t /*attribute_handle*/, uint16_t /*transaction_mode*/, uint16_t /*offset*/, uint8_t * /*buffer*/, uint16_t /*buffer_size*/);


typedef struct{
	uint16_t			flags;//if current is characteristic value, need add ATT_PROPERTY_DYNAMIC flag, is is 128 uuid, need add ATT_PROPERTY_UUID128
	uint16_t			handle;    
	uint16_t			uuid;//all use 16bit uuid
	uint8_t *			uuid128;
	uint8_t *			value;
	uint8_t				value_len;
}attribute_item_t;


typedef struct  _GATT_SERVER_PROFILE
{
	uint8_t *			profile_data;

	att_read_f			attr_read;
	att_write_f			attr_write;
	
}GATT_SERVER_PROFILE;

typedef struct _BLE_APP_CONTEXT
{
	uint8_t				ble_device_addr[6];
	BLE_DEVICE_ROLE		ble_device_role;
}BLE_APP_CONTEXT;


////////////////////////////////////////////////////////////////////////////////
/**
* BLE stack callback event
*/
typedef enum{
	BLE_STACK_INIT_OK = 0,					/**< BLE stack initialize successful */
	BLE_STACK_CONNECTED,					/**< BLE connected */
	BLE_STACK_DISCONNECTED,					/**< BLE disconnected */

	GATT_SERVER_INDICATION_TIMEOUT, 		/**< Send data in indication mode timeout */
	GATT_SERVER_INDICATION_COMPLETE, 		/**< Send data  in indication mode complete */

	BLE_CONN_UPDATE_COMPLETE,
	BLE_ATT_EXCHANGE_MTU_RESPONSE,
} BLE_CALLBACK_EVENT;
	
typedef struct _CONNECT_UPDATE_CONTEXT
{
	uint16_t					conn_interval;
	uint16_t					conn_latency;
	uint16_t					supervision_timeout;
}CONNECT_UPDATE_CONTEXT;

typedef struct _BLE_CALLBACK_PARAMS
{
	uint16_t					paramsLen;
	uint16_t					errorCode;

	union
	{
		CONNECT_UPDATE_CONTEXT 	conn_update_params;
		uint16_t				rx_mtu;
	}params;
}BLE_CALLBACK_PARAMS;

typedef void (*BleCallbackFunc)(BLE_CALLBACK_EVENT event, BLE_CALLBACK_PARAMS * param);

/*
* 注册BLE应用层回调函数
*/
void BleAppCallBackRegister(BleCallbackFunc callbackFunc);

/**
 * @brief
 *   Initialize BLE stack
 *
 * @param	ble_app_context		Pointer to the BLE app struct. See BLE_APP_CONTEXT
 * @param	gatt_server_profile	Pointer to the GATT server profile. See GATT_SERVER_PROFILE
 * @return
 *   0 means successful
 *
 * @note
 *	This initialization should be called after bt stack initialized successful
 */
int8_t	InitBleStack(BLE_APP_CONTEXT * ble_app_context, GATT_SERVER_PROFILE * gatt_server_profile);

/**
 * @brief
 *   Uninitialize BLE stack
 *
 * @return
 *   0 means successful
 *
 * @note
 *	This function must be called when BLE stack closing
 */
int8_t UninitBleStack(void);

/**
 * @brief
 *   Callback function when the BLE event post in stack
 *
 * @param	event		BLE event
 * @return
 *   NONE
 *
 * @note
 *	the event defined in BLE_STACK_CALLBACK
 */
void BLEStackCallBackFunc(BLE_CALLBACK_EVENT event, BLE_CALLBACK_PARAMS *params);

/**
 * @brief
 *   Get the BLE address.
 *
 * @param	bd_addr		BLE address 
 * @return
 *   NONE
 *
 * @note
 *	BLE address may not be the same as the classic BT address.
 *	The BLE address is a random number generated by the classic BT address
 *	
 */
void GetBleAddr(uint8_t * bd_addr);

/**
 * @brief
 *   Set the gap mode.
 *
 * @param	gap_mode		see the GAP_MODE 
 * @return
 *   NONE
 *
 * @note
 *	Gap mode is combined by GAP_BONDABLE_MODE, GAP_CONNECTABLE_MODE,
 *	GAP_DISCOVERABLE_MODE, GAP_BROADCASE_MODE
 */
void SetGapMode(GAP_MODE gap_mode);

/*
 * @brief
 *	Set advertising interval params
 * 
 * @param
 * 	advertising interval min range: 0x0020 ~ 0x4000  //default:0x0020
 * 	advertising interval max range: 0x0020 ~ 0x4000  //default:0x0100
 *
 * @return
 * 	NONE
 *
 * @note 
*/
void SetAdvIntervalParams(uint16_t minValue, uint16_t maxValue);

/**
 * @brief
 *   Get the gap discoverable mode.
 *
 * @param	NONE
 * @return
 *   discoverable mode
 *
 * @note
 *	see the GAP_DISCOVERABLE_MODE
 */
uint8_t GetGapDiscoverableMode(void);

/**
 * @brief
 *   Set the content of the BLE advertising data.
 *
 * @param	advertisement_data	a buffer with advertisement data
 * @param data_len	the buffer length
 * @return
 *   NONE
 *
 * @note
 *	The maximum advertisement data length is 32 bytes.
 *	Set the advertisement data will open the advertisement function
 *	and the device will advertise the new information
 */

void SetAdvertisingData(uint8_t * advertisement_data, uint32_t data_len);

/**
 * @brief
 *   Set the content of the BLE response data.
 *
 * @param	scan_resp_data	a buffer with scan response data
 * @param data_len	the buffer length
 * @return
 *   NONE
 *
 * @note
 *	The maximum response data length is 32 bytes.
 *	Set the response data will open the advertisement function
 *	and the device will response the new information when be scanned
 */
void SetScanResponseData(uint8_t * scan_resp_data, uint32_t data_len);


/**
 * @brief
 *   Disable advertising
 *
 * @param	NONE
 * @return
 *   NONE
 *
 * @note
 *	
 */
void DisableAdvertising(void);

/**
 * @brief
 *   Enable advertising
 *
 * @param	NONE
 * @return
 *   NONE
 *
 * @note
 *	SetAdvertisingData() and SetScanResponseData() can also enable
 *	advertising
 */
void EnableAdvertising(void);

/**
 *@brief
 *	Set BLE security mode
 *
 * @param	mode Security mode
 *
 * @return 
 *   NONE
 *
 * @note
 *	Mode 0 means password not required, mode 1 means password is required.
 *	To set password, use function SetBlePeripheralPassword().
 */

void SetBleSecurityMode(uint8_t mode);

/**
 *@brief
 *	Set password for peripheral device if security mode is 1
 *
 * @param	password The password number in decimal
 *
 * @return 
 *   NONE
 *
 * @note
 *	If security mode is 0, the password is ignored.
 *	The password is a number in decimal, not a string in characters
 *	The password '0000' is equal to '0'
 */

void SetBlePeripheralPassword(uint32_t password);

 
/**
 * @brief
 *   Get uuid with the given handle
 *
 * @param	handle A given handle
 * @return
 *   UUID with the given handle
 *
 * @note
 *
 */
uint16_t GetUuidForHandle(uint16_t handle);


/**
 * @brief
 *   Get the attribute item with the given handle
 *
 * @param	attr_list A list of the gap attributes
 * @param	attr_num The number of the gap attributes
 * @param attribute_handle The given handle
 * @return
 *   attribute item. A pointer to the attribute item
 *
 * @note
 *
 */
attribute_item_t * GetAttrForHandle(attribute_item_t* attr_list, uint16_t attr_num, uint16_t attribute_handle);

/**
 * @brief
 *   Create the profile data in database with the given attributes.
 *
 * @param	attribute_list A pointer to the list of the gap attributes
 * @param	list_num The number of the gap attributes
 * @param data_output A buffer receiving the profile data
 * @param offset The offset in the profile database
 * @return
 *   offset. The offset in the profile database
 *
 * @note
 *	The profile data (data_output) must be terminated with 0
 */
uint16_t CreateProfileData(attribute_item_t* attribute_list, uint16_t list_num, uint8_t* data_output, uint16_t offset);

/**
 * @brief
 *  Send the data to the GATT client by notify mode.
 *	Client don't send response when sever sending data to client by notify mode.
 *
 * @param	handle The handle of the data send to
 * @param	value The data will be sent
 * @param value_len The length of the data
 * @return
 * 		0 means Success
 *		0x57 means BT stack ACL buffer is full.
 *
 * @note
 *	It should be re-sent when returns 0x57.
 */
int GattServerNotify(uint16_t handle, uint8_t *value, uint16_t value_len);


/**
 * @brief
 *  Send the data to the GATT client by indicate mode.
 *	Client should send response when sever sending data to client by indicate mode.
 *	It will get the event GATT_SERVER_INDICATION_TIMEOUT or GATT_SERVER_INDICATION_COMPLETE
 *
 * @param	handle The handle of the data send to
 * @param	value The data will be sent
 * @param value_len The length of the data
 * @return
 * 		0 means Success
 *		0x57 means BT stack ACL buffer is full.
 *		0x90 means a indication in process
 *
 * @note
 *	It should be re-sent when returns 0x57 or 0x90.
 */
int GattServerIndicate(uint16_t handle, uint8_t *value, uint16_t value_len);


/**
* @brief
*	Check whether or not the data can be send via notify or indicate method
* 
* @param
*	NONE
*
* @return
*	TRUE for Yes and FALSE for No
*/
bool GattServerCanSendNotifyOrIndication(void);

/**
* @brief
*	Get the max length of data to indicate or to be read
*
* @param
*	NONE
* 
* @return
*	The length
* 
* @note
*	It should be called before sending data via notify or indicate
*	or read 
*/
uint16_t GattServerQueryMaxReadOrIndicationLen(void);

/*
* 断开BLE连接
*/
void BleDisconnect(void);

/*
* 设置BLE安全模式; 0=close;1=open
*/
void SetBleSecurityMode(uint8_t mode);

/*
* 设置BLE作为外设时安全秘钥
* 前提SecurityMode = 1;
*/
void SetBlePeripheralPassword(uint32_t password);

/*
* ble scan params set
*/
void BleScanParamSet(uint8_t scan_type, uint16_t interval, uint16_t window, uint8_t own_addr_type, uint8_t scan_filter_policy);

#endif

/**
 * @}
 * @}
 */
