#pragma once

#include "esp_log.h"
#include <stdio.h>
#include "driver/ledc.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#ifdef __cplusplus
extern "C" {
#endif

// Structures to hold LED channel information
typedef struct {
    ledc_channel_t red_channel;
    ledc_channel_t green_channel;
    ledc_channel_t blue_channel;
} rgb_led_t;

typedef struct {
    ledc_channel_t channel;
} led_t;

/**
 * @brief Setup the LEDC peripheral
 * 
 * @return esp_err_t: ESP_OK on success, error code otherwise
*/
esp_err_t setupLEDC(void);

/**
 * @brief Setup an RGB LED
 * 
 * @param led Pointer to the RGB LED struct to set up
 * 
 * @param redPin GPIO pin number to attach the red channel to
 * 
 * @param greenPin GPIO pin number to attach the green channel to
 * 
 * @param bluePin GPIO pin number to attach the blue channel to
 * 
 * @return esp_err_t: ESP_OK on success, error code otherwise
 * 
 * @note todo - use gpio pin enum instead of int
*/
esp_err_t setupRGBLED(rgb_led_t *led, int redPin, int greenPin, int bluePin);

/**
 * @brief Setup an LED
 * 
 * @param led Pointer to the LED struct to set up
 * 
 * @param pin GPIO pin number to attach the LED to
 * 
 * @return esp_err_t: ESP_OK on success, error code otherwise
*/
esp_err_t setupLED(led_t *led, int pin);

/**
 * @brief Setup an LEDC channel
 * 
 * @param channel Pointer to the LEDC channel to set up
 * 
 * @param pin GPIO pin number to attach the channel to
 * 
 * @return esp_err_t: ESP_OK on success, error code otherwise
 * 
 * @note todo - use gpio pin enum instead of int
*/
esp_err_t setupChannel(ledc_channel_t *channel, int pin);

/**
 * @brief Set the color and brightness of an RGB LED
 *
 * @param led Pointer to the RGB LED to set
 *
 * @param color RGB color value (0xRRGGBB)
 *
 * @param brightness Brightness value from 0 to 255
 *
 * @return esp_err_t: ESP_OK on success, error code otherwise
 *
 * @note TODO - name LEDs
 */
esp_err_t setRGBLED(rgb_led_t *led, uint32_t color, uint8_t brightness);

/**
 * @brief Set the brightness of an LED
 *
 * @param led Pointer to the LED to set
 *
 * @param brightness Brightness value from 0 to 255
 *
 * @return esp_err_t: ESP_OK on success, error code otherwise
 */
esp_err_t setLED(led_t *led, uint8_t brightness);


#ifdef __cplusplus
}
#endif
