idf_component_register(SRCS "main.c"
                       INCLUDE_DIRS "."
                       REQUIRES mpu6050
                       REQUIRES ssd1306
                       REQUIRES ds18b20
                       REQUIRES ssr
                       REQUIRES driver)



