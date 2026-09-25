# Embedded CLI Firmware Simulator

A bare-metal-style embedded firmware simulator built in C, demonstrating core concepts used in real microcontroller firmware: circular buffers, pointer-based memory management, and layered architecture using structs.

This project is being developed in phases as I learn embedded C, to build a practical understanding of concepts before applying them — not just following tutorials.

## Why This Project

Most embedded firmware needs to handle data arriving faster or slower than it can be processed (e.g., UART receiving bytes one at a time), organize related data cleanly (e.g., sensor readings), and respond to commands safely. This project builds each of those pieces from scratch, in isolated, testable stages.

## Features (Phase 1 + 2 completed)

- **Ring Buffer** — fixed-size circular buffer using pointer arithmetic and modulo-based wraparound, with `volatile`-qualified state for interrupt-safe access
- **Full/Empty State Handling** — tested explicitly: pushing to a full buffer and popping from an empty buffer both fail safely instead of corrupting data or returning garbage
- **Struct-Based Sensor Data** — `SensorData_t` bundles sensor ID, temperature, and humidity into one unit
- **Struct-Based Command List** — `Command_t` bundles a command's name and description together
- **Header Guards** — `sensor.h` and `cli.h` use `#ifndef`/`#define`/`#endif` to prevent duplicate inclusion errors

## What I Implemented

- Pointer arithmetic for a circular data structure (`head`, `tail` indices)
- Wraparound logic using modulo indexing
- The `volatile` keyword — necessary when data is shared between an interrupt service routine (ISR) and the main loop on real hardware
- Full/empty state tracking without wasting memory (using a `count` field instead of the "always leave one slot empty" trick)
- Layered file structure separating sensor logic (`sensor.h`) from command logic (`cli.h`)

## Testing

The ring buffer was tested for edge cases, not just the happy path: buffer full correctly rejects a push, all 16 values pop out in FIFO order, and buffer empty correctly rejects a pop.

## Project Structure

- ring_buffer.c — Main program: ring buffer implementation + test code
- sensor.h — SensorData_t struct + simulated sensor_read()
- cli.h — Command_t struct for CLI command metadata

## How to Run

Compile with: gcc ring_buffer.c -o test
Run with: ./test

## Roadmap

- [x] Phase 1 — Ring buffer with pointer arithmetic, wraparound, and volatile
- [x] Phase 2 — Layered architecture using structs (sensor + command data)
- [ ] Phase 3 — Function-pointer based CLI command dispatcher (in progress)

## Hardware Concepts Demonstrated

- Memory-constrained data types (uint8_t instead of int)
- Interrupt-safe variable access (volatile)
- Fixed-size, statically allocated buffers (no dynamic memory)
- Modular header file design with include guards

## Tools Used

- GCC (C compiler)
- Git & GitHub
- VS Code