
## Folder Structure

- L0 - Low level
- L1 - FreeRTOS
- L2 - Standalone utilities
- L3 - Drivers
- L4 - I/O
- L5 - Application

The function calls shall go from a layer downwards, but never upwards; so `L2 Standalone utilities` should never make a function call into the drivers or the Application functions.
