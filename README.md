# attiny85-semaphore-controller
Semaphore Controller

-- WIP --

ATTiny 85 controller for a simple 4 state semaphore.

Controller is waiting for interruptions (ex: pulse switch) on a specific pin (PB1) to alternate between states:

off -> red -> yellow -> green

Each state (except off) corresponds to a specific output pin with an attached led: PB4 (red), PB3 (yellow), PB0 (green)
