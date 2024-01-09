/**
 * @file:   debounce.h
 * @breif:  This is a debounce driver for the debounce button.
 * @author: Julian Bustamante
 * @date:   18-Jul-2022
 * @version 1.0.0
 */

#ifndef DEBOUNCE_H
#define DEBOUNCE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#define DEBOUNCE_MULTIPLIER 1 /*1 mS*/
#define DEBOUNCE_DELAY 40 /*40 mS*/
#define DEBOUNCE_VERSION_NUMBER "2.1.0"
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
    volatile uint8_t delay_ms;
    volatile uint8_t timeCounter;
    int tickMultiplier;
    volatile uint8_t status;
    uint8_t         mode;
    delayFcn_t      delayFcn;
  } debounceData_t;

  /**
  * @brief Initialize the debounce driver
  * @param ptrData Pointer to the debounceData_t struct
  * @param debounceTickms Time in mS to debounce the button, depends on the from call debounceUpdate, default DebounceTick is 40 mS if is called from Systick (1ms)
  * @param mode DEBOUNCE_COUNTERTICK so you need update 'debounceUpdate' inside Systick function
                DEBOUNCE_WAIT you need update  'debounceUpdate' in other place where you want
  * @param delayFcn delayfuntion for DEBOUNCE_WAIT mode, blocking wait.
  * @param initialHwConfigState initial hardware (pin) config state, pull up or pull down
  * @param tickMultiplier multiplier, 1 if sysTick is 1ms, 10 if sysTick is 100ms and so on.
  * @example Debounce_Init(&DebounceData, 40 ,DEBOUNCE_COUNTERTICK, NULL, DEBOUNCE_PULL_DOWN);
  * @note debounceUpdate() must be call in function Systick for DEBOUNCE_COUNTERTICK or  set in anywhere for DEBOUNCE_WAIT
 */
  void debounceInit(debounceData_t *ptrData, uint32_t debounceTick_ms,  int tickMultiplier, debounceMode_t mode, delayFcn_t delayFcn, debounceState_t initialHwConfigState);

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
   * @param debounceTick_ms: Delay time in mS, depends on the from call debounceUpdate
  */
  void debounceSetDelay(debounceData_t *ptrData, uint32_t debounceTick_ms);

  /**
 * @brief set the pull up or pull down initial state
 * @param ptrData: Pointer to the debounceData_t struct
 * @param initialHwConfigState: DEBOUNCE_PULL_UP or DEBOUNCE_PULL_DOWN initial state
*/
  void debounceSetPull(debounceData_t *ptrData, debounceState_t initialHwConfigState);

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
