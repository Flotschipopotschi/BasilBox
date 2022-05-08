/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    rtc.c
  * @brief   This file provides code for the configuration
  *          of the RTC instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "rtc.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

RTC_HandleTypeDef hrtc;

/* RTC init function */
void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0;
  sTime.Minutes = 0;
  sTime.Seconds = 0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 1;
  sDate.Year = 0;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

void HAL_RTC_MspInit(RTC_HandleTypeDef* rtcHandle)
{

  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspInit 0 */

  /* USER CODE END RTC_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
    PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* RTC clock enable */
    __HAL_RCC_RTC_ENABLE();
  /* USER CODE BEGIN RTC_MspInit 1 */

  /* USER CODE END RTC_MspInit 1 */
  }
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspDeInit 0 */

  /* USER CODE END RTC_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_RTC_DISABLE();
  /* USER CODE BEGIN RTC_MspDeInit 1 */

  /* USER CODE END RTC_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void rtc_setTime(rtc_time_t time)
{
	if (!IS_RTC_HOUR24(time.hour) || !IS_RTC_MINUTES(time.min) || !IS_RTC_SECONDS(time.sec))
	{
		//TODO: Handle error
		return;
	}

	RTC_TimeTypeDef rtcTime;
	rtcTime.Hours = time.hour;
	rtcTime.Minutes = time.min;
	rtcTime.Seconds = time.sec;
	rtcTime.SubSeconds = 0;
	rtcTime.SecondFraction = 0;
	rtcTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	rtcTime.StoreOperation = RTC_STOREOPERATION_SET;

	rtcTime.TimeFormat = RTC_HOURFORMAT_24;

	if(HAL_RTC_SetTime(&hrtc, &rtcTime, RTC_FORMAT_BIN) != HAL_OK)
	{
		//TODO: Handle error
		return;
	}
}

void rtc_getTime(rtc_time_t* time)
{
	RTC_TimeTypeDef rtcTime;

	if (HAL_RTC_GetTime(&hrtc, &rtcTime, RTC_FORMAT_BIN) != HAL_OK)
	{
		//TODO: Handle error
	}

	time->hour = rtcTime.Hours;
	time->min = rtcTime.Minutes;
	time->sec = rtcTime.Seconds;
}

void rtc_setDate(rtc_date_t date)
{
	if (!IS_RTC_YEAR(date.year) || !IS_RTC_MONTH(date.month) || !IS_RTC_DATE(date.day))
	{
		//TODO: Handle error
		return;
	}


	RTC_DateTypeDef rtcDate;
	rtcDate.Date = date.day;
	rtcDate.WeekDay = RTC_WEEKDAY_SUNDAY;
	rtcDate.Month = date.month;
	rtcDate.Year = date.year;

	if(HAL_RTC_SetDate(&hrtc, &rtcDate, RTC_FORMAT_BIN) != HAL_OK)
	{
		//TODO: Handle error
	}
}

void rtc_getDate(rtc_date_t* date)
{
	RTC_DateTypeDef rtcDate;

	if (HAL_RTC_GetDate(&hrtc, &rtcDate, RTC_FORMAT_BIN) != HAL_OK)
	{
		//TODO: Handle error
	}

	date->day = rtcDate.Date;
	date->month = rtcDate.Month;
	date->year = rtcDate.Year;
}

void rtc_setTs(rtc_ts_t ts)
{
	rtc_setTime(ts.time);
	rtc_setDate(ts.date);
}

void rtc_getTs(rtc_ts_t* ts)
{
	rtc_getTime(&ts->time);
	rtc_getDate(&ts->date);
}
/* USER CODE END 1 */