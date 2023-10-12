/**
 *******************************************************************************
 * @file    bt_a2dp_api.h
 * @author  Halley
 * @version V1.0.1
 * @date    27-Apr-2016
 * @brief   A2DP related api
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
* @defgroup bt_a2dp_api bt_a2dp_api.h
* @{
*/

#include "type.h"

#ifndef __BT_A2DP_API_H__
#define __BT_A2DP_API_H__

/**
 * The audio data format received from A2DP of Bluetooth stack.
 *
 * PCM means the audio data has been decoded in the stack.
 *
 * SBC means the audio data has not been decoded yet, and the SBC
 * data can be decoded in the application level or stored and so on.
 *
 */
typedef uint8_t	A2dpAudioDataFormat;

#define A2DP_AUDIO_DATA_PCM			1	/*!< A2DP audio data is decoded PCM data*/

#define A2DP_AUDIO_DATA_SBC			2	/*!< A2DP audio data is undecoded SBC data*/


typedef enum{
	BT_STACK_EVENT_A2DP_NONE = 0,

	/**
	 *@brief
	 * Indicate a stream is opened
	 *
	 *@note
	 * params.bd_addr is vaild, which is the bluetooth address
	 * of the remote devices.
	 */
	BT_STACK_EVENT_A2DP_CONNECTED,

	/**
	 *@brief
	 * Indicate the stream is closed
	 *
	 *@note
	 * No params
	 */
	BT_STACK_EVENT_A2DP_DISCONNECTED,

	/**
	 *@brief
	 * Indicate connecting to remote device is timeout
	 * This event only can be generated by A2dpConnect()
	 *
	 *@note
	 * No params
	 */
	BT_STACK_EVENT_A2DP_CONNECT_TIMEOUT,

	/**
	 *@brief
	 * The stream has been started.
	 *
	 *@note
	 * No params
	 */
	BT_STACK_EVENT_A2DP_STREAM_START,

	/**
	 *@brief
	 * The stream is suspend
	 *
	 *@note
	 * No params
	 */
	BT_STACK_EVENT_A2DP_STREAM_SUSPEND,

	/**
	 *@brief
	 * The stream data arrived
	 *
	 *@note
	 * params.a2dpStreamData is vaild
	 * paramsLen is vaild, which indacate the length of a2dpStreamData
	 *
	 */
	BT_STACK_EVENT_A2DP_STREAM_DATA_IND
}BT_A2DP_CALLBACK_EVENT;

typedef struct _BT_A2DP_CALLBACK_PARAMS
{
	uint16_t 				paramsLen;
	bool					status;
	uint16_t 				errorCode;

	union
	{
		uint8_t					*bd_addr;
		uint8_t					*a2dpStreamData;
	}params;
}BT_A2DP_CALLBACK_PARAMS;

typedef void (*BTA2dpCallbackFunc)(BT_A2DP_CALLBACK_EVENT event, BT_A2DP_CALLBACK_PARAMS * param);

void BtA2dpCallback(BT_A2DP_CALLBACK_EVENT event, BT_A2DP_CALLBACK_PARAMS * param);

typedef struct _A2dpAppFeatures
{
	A2dpAudioDataFormat	a2dpAudioDataFormat;
	BTA2dpCallbackFunc	a2dpAppCallback;
}A2dpAppFeatures;

/**
 * @brief  User Command: A2DP connect command.
 * @param  addr The address of bt device to connect.
 * @return True for the command implement successful 
 * @Note If the device has been connected with a2dp profile.
 * 		The event BT_STACK_EVENT_A2DP_CONNECTED will be received in the callback.
 *		Otherwise BT_STACK_EVENT_PAGE_TIMEOUT will be received.
 *
 */
bool A2dpConnect(uint8_t * addr);


/**
 * @brief  User Command: A2DP disconnect command.
 * @param  None
 * @return True for the command implement successful 
 * @Note If the device has been disconnected.
 *		The event BT_STACK_EVENT_A2DP_DISCONNECTED will be received in the callback.
 */
bool A2dpDisconnect(void);
bool AvdtpAppDisconnect(void);

#endif /*__BT_A2DP_API_H__*/

/**
 * @}
 * @}
 */
 