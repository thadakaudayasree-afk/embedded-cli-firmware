#ifndef SENSOR_H
#define SENSOR_H

#include <stdint.h>

typedef struct {
    uint8_t id;
    float temperature;
    float humidity;
} SensorData_t;

void sensor_read(SensorData_t *data, uint8_t sensor_id) {
    data->id = sensor_id;
    data->temperature = 20.0 + sensor_id;   // fake changing values
    data->humidity = 50.0 + (sensor_id * 2);
}

#endif