#include "adc.h"
#include "gpio.h"
#include "services.h"
#include "callbacks.h"


ADC_HandleTypeDef hadc1;

static uint16_t max_value_detected;
static uint8_t no_of_beers;



void MX_ADC1_Init(void)
{
	ADC_ChannelConfTypeDef sConfig = {0};

	/** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
	 */
	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
	hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	hadc1.Init.ScanConvMode = DISABLE;
	hadc1.Init.ContinuousConvMode = DISABLE;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 1;
	hadc1.Init.DMAContinuousRequests = DISABLE;
	hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	if (HAL_ADC_Init(&hadc1) != HAL_OK)
	{
		Error_Handler();
	}
	/** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	 */
	sConfig.Channel = ADC_CHANNEL_13;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}

}

void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	if(adcHandle->Instance==ADC1)
	{
		/* ADC1 clock enable */
		__HAL_RCC_ADC1_CLK_ENABLE();

		__HAL_RCC_GPIOC_CLK_ENABLE();
		/**ADC1 GPIO Configuration
    PC3     ------> ADC1_IN13 
		 */
		GPIO_InitStruct.Pin = GPIO_PIN_3;
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	}
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

	if(adcHandle->Instance==ADC1)
	{
		/* Peripheral clock disable */
		__HAL_RCC_ADC1_CLK_DISABLE();

		/**ADC1 GPIO Configuration
    PC3     ------> ADC1_IN13 
		 */
		HAL_GPIO_DeInit(GPIOC, GPIO_PIN_3);

	}
} 


/*
 * @brief On PB pressed clear the max detected alcohol level
 * 	to enable for next reading
 */
void clear_detected_alcohol_levels(void){
	max_value_detected = 0;
	no_of_beers = 0;
	HAL_GPIO_WritePin(GreenLED_GPIO_Port, GreenLED_Pin, GPIO_PIN_SET);
}

/*
 * @brief If the device is connected to the mobile device thorugh BLE
 * 	collect and sample the MQ3 sensor data
 *
 * 	Depending on the values found and the observations previously taken
 * 	by calibrating the sensor with data from few people in social party
 * 	found the values as:
 *
 * 	The MQ-3 sensor is sample on a resolution of 12-bit,
 * 	So data varies from 0 - 4095
 * 	Observed that without any alcohol present, the raw value is around 1500
 * 	and depending on various observations the comparison is made.
 *
 * 	Please note that the value are not accurate and cannot be used to judge
 * 	how sober a person is.
 *
 * 	** Never drink and drive **
 */
void detect_alochol_levels(void){

	uint16_t raw = 0;
	static uint16_t raw_avg, prev_raw_avg;
	static uint32_t samples = 100000;

	if(is_connected() && max_value_detected == 0){
		HAL_ADC_Start(&hadc1);
		HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
		raw = HAL_ADC_GetValue(&hadc1);

		if(raw_avg == 0){
			raw_avg = raw;
		}
		else{
			raw_avg = (raw + raw_avg)/2;
		}

		if(samples > 0){
			samples--;
		}
		else{

			if(raw_avg > 1500 && raw_avg < prev_raw_avg){
				max_value_detected = raw;
				if(max_value_detected > 1500 && max_value_detected < 2500){
					no_of_beers = 1;
					set_notification_pending();
				}
				else if(max_value_detected > 2500 && max_value_detected < 3000){
					no_of_beers = 2;
					set_notification_pending();
				}
				else if(max_value_detected > 3000){
					no_of_beers = 3;
					set_notification_pending();
				}
			}

			prev_raw_avg = raw_avg;
			samples = 100000;
		}
	}
}


/*
 * @brief return number of beers
 * @retvalue no_of_beers global value
 */
uint8_t get_no_of_beers(void){
	return no_of_beers;
}
