# ESP-IDF LED Library

This library provides basic functionality to control single LEDs and RGB LEDs using the LEDC peripheral on the ESP-IDF framework. It includes functions for setting up LEDs, RGB LEDs, and controlling their brightness and color through PWM channels.

## Features

- Setup individual LEDs and RGB LEDs on specified GPIO pins.
- Control the brightness of single LEDs.
- Set the color and brightness of RGB LEDs using 24-bit RGB values.
- Uses the ESP-IDF's LEDC peripheral for PWM control.

## Setup

1. **Setup LEDC Peripheral**
    - Initializes the LEDC timer for PWM signal generation.
  
    ```c
    esp_err_t setupLEDC(void);
    ```

2. **Setup RGB LED**
    - Configures an RGB LED with separate channels for Red, Green, and Blue.

    ```c
    esp_err_t setupRGBLED(rgb_led_t *led);
    ```

3. **Setup Single LED**
    - Configures a single LED channel.

    ```c
    esp_err_t setupLED(led_t *led);
    ```

4. **Set RGB LED Color and Brightness**
    - Set the color and brightness of an RGB LED. Colors are represented as a 24-bit value (0xRRGGBB).

    ```c
    esp_err_t setRGBLED(rgb_led_t *led, uint32_t color, uint8_t brightness);
    ```

5. **Set LED Brightness**
    - Adjusts the brightness of a single LED.

    ```c
    esp_err_t setLED(led_t *led, uint8_t brightness);
    ```

## Example Usage

```c
// Define an RGB LED on specific pins
rgb_led_t rgbLed = {
    .red_channel = LEDC_CHANNEL_0,
    .red_pin = 4,
    .green_channel = LEDC_CHANNEL_1,
    .green_pin = 5,
    .blue_channel = LEDC_CHANNEL_2,
    .blue_pin = 6
};

// Setup the LEDC and RGB LED
setupLEDC();
setupRGBLED(&rgbLed);

// Set the RGB LED to blue at half brightness
setRGBLED(&rgbLed, 0x0000FF, 128);
```

## TODOs

- Switch GPIO pin configuration from `int` to `gpio_num_t` for better type safety.
- Add LED naming for improved identification in multi-LED setups.

## License

Apache License 2.0
