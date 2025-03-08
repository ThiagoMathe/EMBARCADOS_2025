idf_component_register(SRCS "main.c"
                       INCLUDE_DIRS "."
                       REQUIRES mpu6050 ssd1306 ds18b20 ssr s12 
                                driver esp_event esp_websocket_client 
                                esp_wifi nvs_flash lwip websocket_server)