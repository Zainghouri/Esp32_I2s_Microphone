#include <WiFi.h>
#include <WiFiUdp.h>
#include <driver/i2s.h>

#define I2S_SD 22
#define I2S_WS 19
#define I2S_SCK 21
#define I2S_PORT I2S_NUM_0

#define bufferLen 1024
int16_t sBuffer[bufferLen];
size_t bytesIn = 0;

void i2s_install() {
  const i2s_config_t i2s_config = {
    .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = 16000,
    .bits_per_sample = i2s_bits_per_sample_t(16),
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_STAND_I2S),
    .intr_alloc_flags = 0,
    .dma_buf_count = 10,
    .dma_buf_len = 1024,
    .use_apll = false
  };

  i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
}

void i2s_setpin() {
  const i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_SCK,
    .ws_io_num = I2S_WS,
    .data_out_num = -1,
    .data_in_num = I2S_SD
  };

  i2s_set_pin(I2S_PORT, &pin_config);
}

void setup() {
  Serial.begin(115200);
  i2s_install();
  i2s_setpin();
  i2s_start(I2S_PORT);
}

void loop() {

    esp_err_t result = i2s_read(I2S_PORT, &sBuffer, bufferLen * sizeof(int16_t), &bytesIn, portMAX_DELAY);
    if (result == ESP_OK && bytesIn > 0) {
        Serial.print("Bytes read: ");
        Serial.println(bytesIn);
        for (int i = 0; i < bufferLen; i++) {
            Serial.println(sBuffer[i]);
        }
    } else {
        Serial.println("Error reading I2S data");
    }
    
}
