#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "sensor.h"
#include "cli.h"

#define BUFFER_SIZE 16

typedef struct {
    uint8_t data[BUFFER_SIZE];
    volatile uint8_t head;
    volatile uint8_t tail;
    volatile uint8_t count;
} RingBuffer_t;

void rb_init(RingBuffer_t *rb) {
    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;
}

bool rb_is_full(RingBuffer_t *rb) {
    return rb->count == BUFFER_SIZE;
}

bool rb_is_empty(RingBuffer_t *rb) {
    return rb->count == 0;
}

bool rb_push(RingBuffer_t *rb, uint8_t byte) {
    if (rb_is_full(rb)) {
        return false;
    }
    rb->data[rb->head] = byte;
    rb->head = (rb->head + 1) % BUFFER_SIZE;
    rb->count++;
    return true;
}

bool rb_pop(RingBuffer_t *rb, uint8_t *byte_out) {
    if (rb_is_empty(rb)) {
        return false;
    }
    *byte_out = rb->data[rb->tail];
    rb->tail = (rb->tail + 1) % BUFFER_SIZE;
    rb->count--;
    return true;
}

int main() {
    RingBuffer_t rb;
    rb_init(&rb);

    // Fill the buffer completely
    for (int i = 0; i < BUFFER_SIZE; i++) {
        rb_push(&rb, i);
    }

    // This push should fail - buffer is full
    bool result = rb_push(&rb, 99);
    printf("Push when full: %s\n", result ? "succeeded" : "failed (correct)");

    // Pop everything out
    uint8_t out;
    for (int i = 0; i < BUFFER_SIZE; i++) {
        rb_pop(&rb, &out);
        printf("Popped: %d\n", out);
    }

    // This pop should fail - buffer is empty
    result = rb_pop(&rb, &out);
    printf("Pop when empty: %s\n", result ? "succeeded" : "failed (correct)");

    // ---- PHASE 2 STARTS HERE ----

    // Test multiple sensor readings
    printf("\n--- Sensor Readings ---\n");
    for (uint8_t i = 1; i <= 3; i++) {
        SensorData_t sensor;
        sensor_read(&sensor, i);
        printf("Sensor %d: Temp=%.1fC, Humidity=%.1f%%\n",
               sensor.id, sensor.temperature, sensor.humidity);
    }

    // Test multiple commands
    printf("\n--- Available Commands ---\n");
    Command_t commands[3] = {
        {"led", "Turn LED on or off"},
        {"sensor", "Read sensor values"},
        {"help", "List all commands"}
    };
    for (int i = 0; i < 3; i++) {
        printf("%s - %s\n", commands[i].name, commands[i].description);
    }

    return 0;
}