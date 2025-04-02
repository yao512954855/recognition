#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_freertos_hooks.h"
#include "esp_vfs.h"
#include "driver/gpio.h"
#include "app_speech_rec.h"
#include "app_main.h"
#include "esp_log.h"


static void configure_led(void)
{
	gpio_reset_pin(BLINK_GPIO);
	/* Set the GPIO as a push/pull output */
	gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
}

void app_main(void)
{
	configure_led();
	gpio_set_level(BLINK_GPIO, 1);
	app_speech_wakeup_init();
}
