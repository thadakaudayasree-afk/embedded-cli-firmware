# Embedded CLI Firmware Simulator

A bare-metal-style embedded firmware simulator built in C, demonstrating core concepts used in real microcontroller firmware: circular buffers, pointer-based memory management, layered architecture using structs, and function-pointer based command dispatch.

This project was built in three phases as I learned embedded C, to build a practical understanding of each concept before applying it — not just following tutorials.

## Why This Project

Most embedded firmware needs to handle data arriving faster or slower than it can be processed (e.g., UART receiving bytes one at a time), organize related data cleanly (e.g., sensor readings), and respond to commands safely and dynamically. This project builds each of those pieces from scratch, in isolated, testable stages.

## Features

- **Ring Buffer** — fixed-size circular buffer using pointer arithmetic and modulo-based wraparound, with `volatile`-qualified state for interrupt-safe access
- **Full/Empty State Handling** — tested explicitly: pushing to a full buffer and popping from an empty buffer both fail safely instead of corrupting data or returning garbage
- **Struct-Based Sensor Data** — `SensorData_t` bundles sensor ID, temperature, and humidity into one unit
- **Struct-Based Command Metadata** — `Command_t` bundles a command's name, description, and handler function together
- **Function-Pointer Command Dispatcher** — commands are matched by name at runtime and their handler function is called dynamically, rather than using a large if/else or switch chain
- **Header Guards** — all header files use `#ifndef`/`#define`/`#endif` to prevent duplicate inclusion errors

## What I Implemented

- Pointer arithmetic for a circular data structure (`head`, `tail` indices)
- Wraparound logic using modulo indexing
- The `volatile` keyword — necessary when data is shared between an interrupt service routine (ISR) and the main loop on real hardware
- Full/empty state tracking without wasting memory (using a `count` field instead of the "always leave one slot empty" trick)
- Layered file structure separating sensor logic (`sensor.h`) from command logic (`cli.h`)
- Function pointers (`typedef void (*cmd_handler)(char *args)`) to store and call functions dynamically based on user input
- A command table (array of `Command_t`) mapping command names to their handler functions

## Testing

The ring buffer was tested for edge cases, not just the happy path:
- Buffer full correctly rejects a push instead of overwriting unread data
- All 16 values pop out in the exact order they went in (FIFO)
- Buffer empty correctly rejects a pop instead of returning garbage

The CLI dispatcher was tested by simulating multiple typed commands (`led_on`, `sensor`) and confirming each correctly triggers its matching handler function.

## Project Structure

- `ring_buffer.c` — Main program: ring buffer implementation, command handlers, and dispatcher logic
- `sensor.h` — `SensorData_t` struct and simulated `sensor_read()`
- `cli.h` — `Command_t` struct, `cmd_handler` function pointer type
- `README.md`

## How to Run

Compile with: `gcc ring_buffer.c -o test`
Run with: `./test`

## Roadmap

- [x] Phase 1 — Ring buffer with pointer arithmetic, wraparound, and `volatile`
- [x] Phase 2 — Layered architecture using structs (sensor + command data)
- [x] Phase 3 — Function-pointer based CLI command dispatcher

## Hardware Concepts Demonstrated

- Memory-constrained data types (`uint8_t` instead of `int`)
- Interrupt-safe variable access (`volatile`)
- Fixed-size, statically allocated buffers (no dynamic memory)
- Modular header file design with include guards
- Function pointers for dynamic dispatch — the same pattern used in real embedded CLI shells and RTOS task/command tables

## Possible Future Improvements

- Replace modulo (`%`) wraparound with a bitwise AND (`& (BUFFER_SIZE - 1)`) for faster operation on real hardware, since `BUFFER_SIZE` is a power of 2
- Test on real interrupt-driven hardware to verify ISR-vs-main-loop race conditions, rather than only in a simulated single-threaded environment
- Add real user input parsing (reading from stdin) instead of hardcoded test strings

## Tools Used

- GCC (C compiler)
- Git & GitHub
- VS Code