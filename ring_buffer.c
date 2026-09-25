#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
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

// ---- PHASE 3: Command handler functions ----

void led_on(char *args) {
    printf("[GPIO] LED: ON\n");
}

void led_off(char *args) {
    printf("[GPIO] LED: OFF\n");
}

void read_sensor(char *args) {
    SensorData_t sensor;
    sensor_read(&sensor, 1);
    printf("[SENSOR] Temp=%.1fC, Humidity=%.1f%%\n", sensor.temperature, sensor.humidity);
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

    // ---- PHASE 2: Structs ----

    printf("\n--- Sensor Readings ---\n");
    for (uint8_t i = 1; i <= 3; i++) {
        SensorData_t sensor;
        sensor_read(&sensor, i);
        printf("Sensor %d: Temp=%.1fC, Humidity=%.1f%%\n",
               sensor.id, sensor.temperature, sensor.humidity);
    }

    // ---- PHASE 3: Function pointer CLI dispatcher ----

    Command_t commands[3] = {
        {"led_on", "Turn LED on", led_on},
        {"led_off", "Turn LED off", led_off},
        {"sensor", "Read sensor values", read_sensor}
    };

    char *user_input = "led_on";  // pretend the user typed this

    printf("\n--- CLI Dispatcher Test ---\n");
    printf("User typed: %s\n", user_input);

    int found = 0;
    for (int i = 0; i < 3; i++) {
        if (strcmp(commands[i].name, user_input) == 0) {
            commands[i].handler(NULL);
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("Unknown command\n");
    }

    // Try a second command too
    user_input = "sensor";
    printf("\nUser typed: %s\n", user_input);
    found = 0;
    for (int i = 0; i < 3; i++) {
        if (strcmp(commands[i].name, user_input) == 0) {
            commands[i].handler(NULL);
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("Unknown command\n");
    }

    return 0;
}