//
// Created by EQUINOXY on 01.06.2021.
//

#ifndef PNEUMATICAUTOMATKURS_PNEUMATICAUTOMATKURS_H
#define PNEUMATICAUTOMATKURS_PNEUMATICAUTOMATKURS_H
#include <stdbool.h>
#include <stdio.h>
#include <limits.h>

#if defined(__cplusplus)
extern "C"{
#endif

enum PneumaticState {
    STATE_INIT = 0,
    STATE_0, STATE_1, STATE_2, STATE_3, STATE_4, STATE_5, STATE_6, STATE_7, STATE_8,
    STATE_9, STATE_10, STATE_11, STATE_12, STATE_13, STATE_14, STATE_15, STATE_16,
    STATE_17, STATE_FAIL,
};

#define UP 0
#define DOWN 1

struct PneumaticCylinder {
    int input_signal[2];
    int output_signal;
};

#define Y1 0
#define Y2 1
#define Y3 2
#define Y4 3
#define Y5 4
#define Y6 5
#define Y7 6
#define Y8 7

struct PneumaticEngine {
    enum PneumaticState state;
    int timeout;
    int delay;
    int timeouts[STATE_FAIL];
    int delays[STATE_FAIL];
    struct PneumaticCylinder cylinders[8];
};

void engine_init(struct PneumaticEngine *engine);
bool engine_tick(struct PneumaticEngine *engine);

void automate_demolish(struct PneumaticEngine *engine);
void configure_output_signals(struct PneumaticEngine *engine, const int cylind[], const int signal[]);
void automate_capacity(bool transition, struct PneumaticEngine *engine, enum PneumaticState to_success_state, enum PneumaticState to_failure_state);

void state_motion(struct PneumaticEngine *engine, enum PneumaticState to_state);

#if defined(__cplusplus)
}
#endif

#endif //PNEUMATICAUTOMATKURS_PNEUMATICAUTOMATKURS_H
