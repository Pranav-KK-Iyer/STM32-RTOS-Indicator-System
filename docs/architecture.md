# System Architecture

This system follows an event-driven RTOS architecture.

- Button Task → Generates events
- Queue → Transfers events safely
- Indicator Task → State machine logic
- LED Task → Output behavior

Design ensures:
- Deterministic behavior
- Task separation
- Scalability
