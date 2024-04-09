#include "led.h"

esp_err_t setupLEDC(void)
{
    const char *TAG = "setupLEDC";
    esp_err_t status = ESP_OK;

    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_num = LEDC_TIMER_0,
        .duty_resolution = LEDC_TIMER_13_BIT,
        .freq_hz = 4000,
        .clk_cfg = LEDC_AUTO_CLK};
    status = ledc_timer_config(&ledc_timer);
    if (status != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to set up LEDC timer: %s", esp_err_to_name(status));
        return status;
    }

    return status;
}

esp_err_t setupRGBLED(rgb_led_t *led) {
    const char *TAG = "setupRGBLED";
    esp_err_t status = ESP_OK;

    ESP_LOGI(TAG, "Setting up RGB LED on pins R: %d, G: %d, B: %d", led->red_pin, led->green_pin, led->blue_pin);

    status = setupChannel(&led->red_channel, led->red_pin);
    if (status != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set up red channel: %s", esp_err_to_name(status));
        return status;
    }

    status = setupChannel(&led->green_channel, led->green_pin);
    if (status != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set up green channel: %s", esp_err_to_name(status));
        return status;
    }

    status = setupChannel(&led->blue_channel, led->blue_pin);
    if (status != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set up blue channel: %s", esp_err_to_name(status));
        return status;
    }

    return status;
}

esp_err_t setupLED(led_t *led) {
    const char *TAG = "setupLED";
    esp_err_t status = ESP_OK;

    ESP_LOGI(TAG, "Setting up LED on pin %d", led->pin);
    status = setupChannel(&led->channel, led->pin);
    if (status != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set up LED: %s", esp_err_to_name(status));
        return status;
    }

    return status;
}

esp_err_t setupChannel(ledc_channel_t *channel, int pin)
{
    const char *TAG = "setupChannel";
    esp_err_t status = ESP_OK;

    ledc_channel_config_t channel_conf = {
        .gpio_num = pin,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = *channel,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER_0, // Use a fixed timer
        .duty = 0,
        .hpoint = 0};

    ESP_LOGD(TAG, "Setting up LEDC channel on pin %d", pin);
    status = ledc_channel_config(&channel_conf);
    if (status != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to set up LEDC channel: %s", esp_err_to_name(status));
        return status;
    }

    return status;
}

esp_err_t setRGBLED(rgb_led_t *led, uint32_t color, uint8_t brightness)
{
    const char *TAG = "setRGBLED";
    esp_err_t status = ESP_OK;

    float brightnessFactor = brightness / 255.0f;
    uint32_t dutyMax = (1 << LEDC_TIMER_13_BIT) - 1;
    uint32_t redDuty = ((color >> 16) & 0xFF) * brightnessFactor * dutyMax / 255;
    uint32_t greenDuty = ((color >> 8) & 0xFF) * brightnessFactor * dutyMax / 255;
    uint32_t blueDuty = (color & 0xFF) * brightnessFactor * dutyMax / 255;

    ESP_LOGD(TAG, "Setting RGB LED duty to R: %d, G: %d, B: %d", redDuty, greenDuty, blueDuty);
    status = ledc_set_duty(LEDC_LOW_SPEED_MODE, led->red_channel, redDuty);
    if (status != ESP_OK)
    {
        ESP_LOGW(TAG, "Failed to set duty on red channel: %s", esp_err_to_name(status));
        return status;
    }
    status = ledc_set_duty(LEDC_LOW_SPEED_MODE, led->green_channel, greenDuty);
    if (status != ESP_OK)
    {
        ESP_LOGW(TAG, "Failed to set duty on green channel: %s", esp_err_to_name(status));
        return status;
    }
    status = ledc_set_duty(LEDC_LOW_SPEED_MODE, led->blue_channel, blueDuty);
    if (status != ESP_OK)
    {
        ESP_LOGW(TAG, "Failed to set duty on blue channel: %s", esp_err_to_name(status));
        return status;
    }

    ESP_LOGD(TAG, "Updating RGB LED duty");
    status = ledc_update_duty(LEDC_LOW_SPEED_MODE, led->red_channel);
    if (status != ESP_OK)
    {
        ESP_LOGW(TAG, "Failed to update duty on red channel: %s", esp_err_to_name(status));
        return status;
    }
    status = ledc_update_duty(LEDC_LOW_SPEED_MODE, led->green_channel);
    if (status != ESP_OK)
    {
        ESP_LOGW(TAG, "Failed to update duty on green channel: %s", esp_err_to_name(status));
        return status;
    }
    status = ledc_update_duty(LEDC_LOW_SPEED_MODE, led->blue_channel);
    if (status != ESP_OK)
    {
        ESP_LOGW(TAG, "Failed to update duty on blue channel: %s", esp_err_to_name(status));
        return status;
    }

    ESP_LOGI(TAG, "RGB LED Color: %06X, Brightness: %d", color, brightness);
    return status;
}

esp_err_t setLED(led_t *led, uint8_t brightness)
{
    const char *TAG = "setLED";
    esp_err_t status = ESP_OK;
    uint32_t duty = (uint32_t)(brightness / 255.0f * ((1 << LEDC_TIMER_13_BIT) - 1));

    ESP_LOGD(TAG, "Setting LED duty to %d", duty);
    status = ledc_set_duty(LEDC_LOW_SPEED_MODE, led->channel, duty);
    if (status != ESP_OK)
    {
        ESP_LOGW(TAG, "Failed to set LED duty: %s", esp_err_to_name(status));
        return status;
    }

    ESP_LOGD(TAG, "Updating LED duty");
    status = ledc_update_duty(LEDC_LOW_SPEED_MODE, led->channel);
    if (status != ESP_OK)
    {
        ESP_LOGW(TAG, "Failed to update LED duty: %s", esp_err_to_name(status));
        return status;
    }

    ESP_LOGI(TAG, "LED brightness: %d", brightness);
    return status;
}
