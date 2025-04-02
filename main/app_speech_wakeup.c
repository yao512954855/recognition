#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "xtensa/core-macros.h"
#include "esp_partition.h"
#include "app_speech_rec.h"
#include "sdkconfig.h"
#include "esp_wn_iface.h"
#include "esp_wn_models.h"
#include "dl_lib_coefgetter_if.h"
#include "app_main.h"
#include "esp_mn_iface.h"
#include "esp_mn_models.h"
#include "esp_log.h"
#include "driver/gpio.h"
#define TAG "wakeup"
static const esp_wn_iface_t *wakenet = &WAKENET_MODEL;
static const model_coeff_getter_t *model_coeff_getter = &WAKENET_COEFF;
// MultiNet
static const esp_mn_iface_t *multinet = &MULTINET_MODEL;
model_iface_data_t *model_data_mn = NULL;

static src_cfg_t srcif;
static model_iface_data_t *model_data;

QueueHandle_t sndQueue;

/*
 * 语音识别
 */
static esp_err_t asr_multinet_control(int commit_id)
{

	if (commit_id >= 0 && commit_id < ID_MAX)
	{
		switch (commit_id)
		{
		case ID0_TIAODAOERSHIDU:
			gpio_set_level(BLINK_GPIO, 1);

			ESP_LOGI(TAG, "ID0_TIAODAOERSHIDU");

			break;
		case ID1_TIAODAOERSHIYIDU:

			gpio_set_level(BLINK_GPIO, 0);
			ESP_LOGI(TAG, "ID1_TIAODAOERSHIYIDU");

			break;
		case ID2_TIAODAOERSHIERDU:

			ESP_LOGI(TAG, "ID2_TIAODAOERSHIERDU");

			break;
		case ID3_TIAODAOERSHISANDU:

			ESP_LOGI(TAG, "ID3_TIAODAOERSHISANDU");

			break;
		case ID4_TIAODAOERSHISIDU:

			ESP_LOGI(TAG, "ID4_TIAODAOERSHISIDU");

			break;
		case ID5_TIAODAOERSHIWUDU:

			ESP_LOGI(TAG, "ID5_TIAODAOERSHIWUDU");

			break;
		case ID6_TIAODAOERSHLIUIDU:

			ESP_LOGI(TAG, "ID6_TIAODAOERSHLIUIDU");

			break;
		case ID7_TIAODAOERSHIQIDU:

			ESP_LOGI(TAG, "ID7_TIAODAOERSHIQIDU");

			break;
		case ID8_TIAODAOERSHIBADU:

			ESP_LOGI(TAG, "ID8_TIAODAOERSHIBADU");

			break;
		case ID9_QIDONGKONGTIAOSAOFENG:

			ESP_LOGI(TAG, "ID9_QIDONGKONGTIAOSAOFENG");

			break;
		case ID10_TINGZHIKONGTIAOSAOFENG:

			ESP_LOGI(TAG, "ID10_TINGZHIKONGTIAOSAOFENG");

			break;
		case ID11_TIAODAOZIDONGFENGSU:

			ESP_LOGI(TAG, "ID11_TIAODAOZIDONGFENGSU");
			break;
		case ID12_TIAODAOYIJIFENGSU:

			ESP_LOGI(TAG, "ID12_TIAODAOYIJIFENGSU");
			break;
		case ID13_TIAODAOERJIFENGSU:

			ESP_LOGI(TAG, "ID13_TIAODAOERJIFENGSU");
			break;
		case ID14_TIAODAOSANJIFENGSU:

			ESP_LOGI(TAG, "ID14_TIAODAOSANJIFENGSU");
			break;
		case ID15_DAKAIKONGTIAO:

			ESP_LOGI(TAG, "ID15_DAKAIKONGTIAO");

			break;
		case ID16_GUANBIKONGTIAO:

			ESP_LOGI(TAG, "ID16_GUANBIKONGTIAO");

			break;
		case ID17_GELIKONGTIAO:

			ESP_LOGI(TAG, "ID17_GELIKONGTIAO");

			break;
		case ID18_MEIDIKONGTIAO:

			ESP_LOGI(TAG, "ID18_MEIDIKONGTIAO");

			break;
		case ID19_AOKESIKONGTIAO:

			ESP_LOGI(TAG, "ID19_AOKESIKONGTIAO");

			break;
		case ID20_HAIERKONGTIAO:

			ESP_LOGI(TAG, "ID20_HAIERKONGTIAO");

			break;
		case ID21_KAIDENG:

			ESP_LOGI(TAG, "ID21_KAIDENG");

			break;
		case ID22_GUANDENG:

			ESP_LOGI(TAG, "ID22_GUANDENG");

			break;
		default:
			ESP_LOGI(TAG, "not supportint mode");
			break;
		}

		return ESP_OK;
	}
	return ESP_FAIL;
}
void tips_rec(void)
{
	static uint8_t i = 0;
	i++;
	if (i > 200)
	{
		i = 0;
	}
}
void tips_wake(void)
{
	static uint8_t i = 0;
	i++;
	if (i > 200)
	{
		i = 0;
		printf("please say hi,lexin\r\n");
	}
}
void nnTask(void *arg)
{
	int frequency = wakenet->get_samp_rate(model_data);
	int audio_chunksize = wakenet->get_samp_chunksize(model_data);
	int chunk_num = multinet->get_samp_chunknum(model_data_mn);

	int16_t *buffer = malloc(audio_chunksize * sizeof(int16_t));
	assert(buffer);
	int chunks = 0;
	int mn_chunks = 0;
	bool detect_flag = 0;
	while (1)
	{
		xQueueReceive(sndQueue, buffer, portMAX_DELAY);

		if (detect_flag == 0)
		{
			tips_wake();
			int r = wakenet->detect(model_data, buffer);
			if (r)
			{

				float ms = (chunks * audio_chunksize * 1000.0) / frequency;
				printf("%.2f: %s detected.\n", (float)ms / 1000.0, wakenet->get_word_name(model_data, r));

				detect_flag = 1;

				printf("please say what you want to say\r\n");
				vTaskDelay(10);
			}
			else
			{
				vTaskDelay(2);
			}
		}
		else
		{
			tips_rec();
			mn_chunks++;
			int command_id = multinet->detect(model_data_mn, buffer);

			if (asr_multinet_control(command_id) == ESP_OK)
			{

				mn_chunks = 0;
				detect_flag = 0;
				printf("please say hi,lexin\r\n");
				vTaskDelay(500);
			}
		}
	}

	free(buffer);
	vTaskDelete(NULL);
}

void app_speech_wakeup_init()
{
	// Initialize NN model
	model_data = wakenet->create(model_coeff_getter, DET_MODE_95);

	int wake_word_num = wakenet->get_word_num(model_data);
	char *wake_word_list = wakenet->get_word_name(model_data, 1);
	if (wake_word_num)
		printf("wake word number = %d, word1 name = %s\n",
			   wake_word_num, wake_word_list);

	int audio_chunksize = wakenet->get_samp_chunksize(model_data);

	model_data_mn = multinet->create(&MULTINET_COEFF, 4000);
	// Initialize sound source
	sndQueue = xQueueCreate(5, (audio_chunksize * sizeof(int16_t)));
	srcif.queue = &sndQueue;
	srcif.item_size = audio_chunksize * sizeof(int16_t);

	xTaskCreatePinnedToCore(&recsrcTask, "recsrcTask", 3 * 1024, (void *)&srcif, 13, NULL, 0);

	xTaskCreatePinnedToCore(&nnTask, "nnTask", 5 * 1024, NULL, 14, NULL, 0);
}
