/**
 * @file:   debounce.h
 * @breif:  This is a debounce driver for the debounce button.
 * @author: Julian Bustamante
 * @date:   25-Oct-2021
 */

#ifndef DEBOUNCE_H
#define DEBOUNCE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

#define DEBOUNCE_DELAY 40 /*40 mS*/
#define DEBOUNCE_VERSION_NUMBER "2.0.0"
#define DEBOUNCE_CAPTION "debounce " DEBOUNCE_VERSION_NUMBER /*Caption of the program*/

#define DEBOUNCE_LOW 0
#define DEBOUNCE_HIGH 1

typedef void (*delayFcn_t)(uint32_t );   /*ms*/

 typedef enum
  {
    DEBOUNCE_COUNTERTICK = 0,
    DEBOUNCE_WAIT
  } debounceMode_t;

  typedef enum
  {
    DEBOUNCE_OFF = 0,
    DEBOUNCE_ON,
    DEBOUNCE_RISING,
    DEBOUNCE_FALLING,
    DEBOUNCE_FLAG_UNKNOWN,
    DEBOUNCE_PULL_UP,
    DEBOUNCE_PULL_DOWN
  } debounceState_t;

  typedef struct
  {
    volatile uint8_t pinState, previousState_, flagFalling, flagRising;
    volatile uint8_t delay; /*mS*/
    volatile uint8_t timeCounter;
    volatile uint8_t status;
    uint8_t         mode;
    delayFcn_t      delayFcn;
  } debounceData_t;

  /**
 * @brief Initialize the debounce driver
 * @param ptrData: Pointer to the debounceData_t struct
 * @param debounceTick: Time in mS to debounce the button, depends on the from call debounceUpdate, default DebounceTick is 40 mS if is called from Systick (1ms)
 * @param pull_x: Pull up or down the pin
 * @example: Debounce_Init(&DebounceData,40, DEBOUNCE_PULL_UP);
 * 
*/
  void debounceInit(debounceData_t *ptrData, uint32_t debounceTick, debounceMode_t mode, delayFcn_t delay, debounceState_t pull_x);

  /**
   * @brief Update the state of the button
   * @param ptrData: Pointer to the debounceData_t struct 
   * @param pinState: Pin to read
   * @example: Debounce_Update(&DebounceData,GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)); called from Systick
  */
  void debounceUpdate(debounceData_t *ptrData, uint8_t pinState);

  /**
   * @brief Get the falling edge and clear flag, not storage data
   * @param ptrData: Pointer to the debounceData_t struct
   * @return 0 or 1
  */
  uint8_t debounceIsFallingEdgeCf(debounceData_t *ptrData);

  /**
   * @brief Get the falling edge and storage data, should clear manually flag
   * @param ptrData: Pointer to the debounceData_t struct
   * @return 0 or 1
  */
  uint8_t debounceIsFallingEdge(debounceData_t *ptrData);

  /**
   * @brief Get the falling edge and clear flag, not storage data
   * @param ptrData: Pointer to the debounceData_t struct
   * @return 0 or 1
  */
  uint8_t debounceIsRisingEdgeCf(debounceData_t *ptrData);

  /**
   * @brief Get the falling edge and storage data, should clear manually flag
   * @param ptrData: Pointer to the debounceData_t struct
   * @return 0 or 1
  */
  uint8_t debounceIsRisingEdge(debounceData_t *ptrData);
  /**
   * @brief Get the state of the button
   * @param ptrData: Pointer to the debounceData_t struct
   * @return DEBOUNCE_ON or DEBOUNCE_OFF
   */
  uint8_t debounceGetStatus(debounceData_t *ptrData);

  /**
   * @brief set delay time
   * @param ptrData: Pointer to the debounceData_t struct
   * @param debounceTick: Delay time in mS, depends on the from call debounceUpdate
  */
  void debounceSetDelay(debounceData_t *ptrData, uint32_t debounceTick);

  /**
 * @brief set the pull up or pull down initial state
 * @param ptrData: Pointer to the debounceData_t struct
 * @param pull_x: DEBOUNCE_PULL_UP or DEBOUNCE_PULL_DOWN initial state
*/
  void debounceSetPull(debounceData_t *ptrData, debounceState_t pull_x);

  /**
 * @brief clear the flag falling or rising
 * @param ptrData: Pointer to the debounceData_t struct
 * @param edge: DEBOUNCE_FALLING or DEBOUNCE_RISING
 * @use: call this function when you want to clear the flag after you read the flag
*/
  void debounceClearFlag(debounceData_t *ptrData, debounceState_t flag);

#ifdef __cplusplus
}
#endif

#endif /* DEBOUNCE_H */
