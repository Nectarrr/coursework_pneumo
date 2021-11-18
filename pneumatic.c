//
// Created by EQUINOXY on 01.06.2021.
//

#include "pneumatic.h"

#define TIMEOUT_DELTA(timeout) ((timeout)*1000)
#define DELAY_DELTA(delay) ((delay)*1000)

#define TIMEOUT_GE(engine) ((engine)->timeout > (engine)->timeouts[(engine)->state])
#define DELAY_GE(engine) ((engine)->delay > (engine)->delays[(engine)->state])

void engine_init(struct PneumaticEngine *engine) {
    if (engine != 0) {
        int cylinder;
        int state;

        engine->state = STATE_INIT;
        engine->delay = 0;
        engine->timeout = 0;

        int t[] = {INT_MAX, 120, 30, 56, 30, 30, 45, 60, 120, 45, 120, 56, 120, 56, 60, 120, 45, 56, 120};

        int d[] = {INT_MAX, 78, 45, 78, 70, 78, 45, 78, 33, 45, 45, 33, 33, 60, 60, 60, 60, 60, 78};

        for (cylinder = 0; cylinder <= Y8; cylinder++) {
            engine->cylinders[cylinder].output_signal = 0;
            engine->cylinders[cylinder].input_signal[UP] = 0;
            engine->cylinders[cylinder].input_signal[DOWN] = 0;
        }

        for (state = 0; state < STATE_FAIL; state++) {
            engine->timeouts[state] = TIMEOUT_DELTA(t[state]);
            engine->delays[state] = DELAY_DELTA(d[state]);
        }
    }
}

void automate_demolish(struct PneumaticEngine *engine) {
    if (engine != 0) {

    }
}

void configure_output_signals(struct PneumaticEngine *engine, const int cylind[], const int signal[]) {
    int cylinder;
    for (cylinder = 0; cylinder++;)
        engine->cylinders[cylind[cylinder]].output_signal = signal[cylinder];
}

void state_motion(struct PneumaticEngine *engine, enum PneumaticState to_state)
        {
    engine->state = to_state;
    engine->timeout = 0;
}

void automate_capacity(bool transition, struct PneumaticEngine *engine, enum PneumaticState to_success_state, enum PneumaticState to_failure_state){

    if (transition) {
            engine->timeout = 0;
            if (DELAY_GE(engine)) state_motion(engine, to_success_state);
        } else {
            if (TIMEOUT_GE(engine)) {
                state_motion(engine, to_failure_state);
                configure_output_signals(engine, (int[]) {Y1, Y2, Y3, Y4, Y5, Y6, Y7, Y8},
                                   (int[]) {0, 0, 0, 0, 0, 0, 0, 0});
            }
            engine->delay = 0;
        }
    }

bool engine_tick(struct PneumaticEngine *engine) {
    bool ret = true;
    if (engine == 0)
        return false;
    switch (engine->state) {
        case STATE_INIT: {
            state_motion(engine, STATE_1);
            engine->delay = 0;
            break;
        }
        case STATE_0: {
            bool transition = engine->cylinders[Y1].input_signal[DOWN] &&
                              engine->cylinders[Y2].input_signal[DOWN] &&
                              engine->cylinders[Y3].input_signal[DOWN] &&
                              engine->cylinders[Y4].input_signal[DOWN] &&
                              engine->cylinders[Y5].input_signal[DOWN] &&
                              engine->cylinders[Y6].input_signal[DOWN] &&
                              engine->cylinders[Y7].input_signal[DOWN] &&
                              engine->cylinders[Y8].input_signal[DOWN];
            configure_output_signals(engine, (int[]) {Y1, Y2, Y3, Y4, Y5, Y6, Y7, Y8},(int[]) {0, 0, 0, 0, 0, 0, 0, 0});
            automate_capacity(transition, engine, STATE_1, STATE_FAIL);
            break;
        }
        case STATE_1: {
            bool transition = engine->cylinders[Y2].input_signal[UP] &&
                              engine->cylinders[Y6].input_signal[UP];
            configure_output_signals(engine, (int[]) {Y2, Y6}, (int[]) {1, 1});
            automate_capacity(transition, engine, STATE_2, STATE_7);
            break;
        }
        case STATE_2: {
            bool transition = engine->cylinders[Y1].input_signal[UP] &&
                              engine->cylinders[Y2].input_signal[DOWN] &&
                              engine->cylinders[Y3].input_signal[UP] &&
                              engine->cylinders[Y4].input_signal[UP] &&
                              engine->cylinders[Y5].input_signal[UP] &&
                              engine->cylinders[Y6].input_signal[DOWN] &&
                              engine->cylinders[Y7].input_signal[UP] &&
                              engine->cylinders[Y8].input_signal[UP];
            configure_output_signals(engine, (int[]) {Y1, Y2, Y3, Y4, Y5, Y6, Y7, Y8},(int[]) {1, 0, 1, 1, 1, 0, 1, 1});
            automate_capacity(transition, engine, STATE_3, STATE_FAIL);
            break;
        }
        case STATE_3: {
            bool transition = engine->cylinders[Y4].input_signal[DOWN];
            configure_output_signals(engine, (int[]) {Y4}, (int[]) {0});
            automate_capacity(transition, engine, STATE_4, STATE_FAIL);
            break;
        }
        case STATE_4: {
            bool transition = engine->cylinders[Y2].input_signal[UP] &&
                              engine->cylinders[Y6].input_signal[UP] &&
                              engine->cylinders[Y7].input_signal[DOWN];
            configure_output_signals(engine, (int[]) {Y1, Y2, Y3},(int[]) {1, 1, 0});
            automate_capacity(transition, engine, STATE_5, STATE_15);
            break;
        }
        case STATE_5: {
            bool transition = engine->cylinders[Y1].input_signal[DOWN] &&
                              engine->cylinders[Y2].input_signal[DOWN] &&
                              engine->cylinders[Y3].input_signal[DOWN] &&
                              engine->cylinders[Y4].input_signal[UP] &&
                              engine->cylinders[Y5].input_signal[DOWN] &&
                              engine->cylinders[Y6].input_signal[DOWN] &&
                              engine->cylinders[Y7].input_signal[UP];
            configure_output_signals(engine, (int[]) {Y1, Y2, Y3, Y4, Y5, Y6, Y7}, (int[]) {0, 0, 0, 1, 0, 0, 1});
            automate_capacity(transition, engine, STATE_6, STATE_4  );
            break;
        }
        case STATE_6: {
            bool transition = engine->cylinders[Y1].input_signal[UP] &&
                              engine->cylinders[Y3].input_signal[UP] &&
                              engine->cylinders[Y6].input_signal[UP];
            configure_output_signals(engine, (int[]) {Y1, Y3, Y6}, (int[]) {1, 1, 1});
            automate_capacity(transition, engine, STATE_7, STATE_17);
            break;
        }
        case STATE_7: {
            bool transition = engine->cylinders[Y1].input_signal[DOWN] &&
                              engine->cylinders[Y2].input_signal[UP] &&
                              engine->cylinders[Y3].input_signal[DOWN] &&
                              engine->cylinders[Y4].input_signal[DOWN] &&
                              engine->cylinders[Y5].input_signal[UP] &&
                              engine->cylinders[Y6].input_signal[DOWN] &&
                              engine->cylinders[Y7].input_signal[DOWN] &&
                              engine->cylinders[Y8].input_signal[DOWN];
            configure_output_signals(engine, (int[]) {Y1, Y2, Y3, Y4, Y5, Y6, Y7, Y8},(int[]) {0, 1, 0, 0, 1, 0, 0, 0});
            automate_capacity(transition, engine, STATE_8, STATE_2);
            break;
        }
        case STATE_8: {
            bool transition = engine->cylinders[Y2].input_signal[DOWN] &&
                              engine->cylinders[Y3].input_signal[UP];
            configure_output_signals(engine, (int[]) {Y2, Y3},(int[]) {1, 0});
            automate_capacity(transition, engine, STATE_9, STATE_7);
            break;
        }
        case STATE_9: {
            bool transition = engine->cylinders[Y1].input_signal[UP] &&
                              engine->cylinders[Y3].input_signal[DOWN] &&
                              engine->cylinders[Y4].input_signal[UP];
            configure_output_signals(engine, (int[]) {Y1, Y3, Y4},(int[]) {1, 0, 1});
            automate_capacity(transition, engine, STATE_10, STATE_FAIL);
            break;
        }
        case STATE_10: {
            bool transition = engine->cylinders[Y2].input_signal[UP] &&
                              engine->cylinders[Y3].input_signal[UP] &&
                              engine->cylinders[Y4].input_signal[DOWN] &&
                              engine->cylinders[Y5].input_signal[DOWN] &&
                              engine->cylinders[Y6].input_signal[UP] &&
                              engine->cylinders[Y7].input_signal[UP] &&
                              engine->cylinders[Y8].input_signal[UP];
            configure_output_signals(engine, (int[]) {Y2, Y3, Y4, Y5, Y6, Y7, Y8},(int[]) {1, 1, 0, 0, 1, 1, 1});
            automate_capacity(transition, engine, STATE_11, STATE_FAIL);
            break;
        }
        case STATE_11: {
            bool transition =
                              engine->cylinders[Y2].input_signal[DOWN] &&
                              engine->cylinders[Y3].input_signal[DOWN] &&
                              engine->cylinders[Y6].input_signal[DOWN];
            configure_output_signals(engine, (int[]) {Y2, Y3, Y6},(int[]) {0, 0, 0});
            automate_capacity(transition, engine, STATE_12, STATE_FAIL);
            break;
        }
        case STATE_12: {
            bool transition = engine->cylinders[Y6].input_signal[UP];
            configure_output_signals(engine, (int[]) {Y6},(int[]) {1});
            automate_capacity(transition, engine, STATE_13, STATE_FAIL);
            break;
        }
        case STATE_13: {
            bool transition = engine->cylinders[Y1].input_signal[DOWN] &&
                              engine->cylinders[Y2].input_signal[UP] &&
                              engine->cylinders[Y4].input_signal[UP] &&
                              engine->cylinders[Y5].input_signal[UP] &&
                              engine->cylinders[Y6].input_signal[DOWN] &&
                              engine->cylinders[Y7].input_signal[DOWN] &&
                              engine->cylinders[Y8].input_signal[DOWN];
            configure_output_signals(engine, (int[]) {Y1, Y2, Y4, Y5, Y6, Y7, Y8},(int[]) {0, 1, 1, 1, 0, 0, 0});
            automate_capacity(transition, engine, STATE_14, STATE_FAIL);
            break;
        }
        case STATE_14: {
            bool transition = engine->cylinders[Y3].input_signal[UP] &&
                              engine->cylinders[Y4].input_signal[DOWN] &&
                              engine->cylinders[Y5].input_signal[DOWN] &&
                              engine->cylinders[Y6].input_signal[UP] ;
            configure_output_signals(engine, (int[]) {Y3, Y4, Y5, Y6},
                               (int[]) {1, 0, 0, 1});
            automate_capacity(transition, engine, STATE_15, STATE_FAIL);
            break;
        }
        case STATE_15: {
            bool transition = engine->cylinders[Y6].input_signal[DOWN] &&
                              engine->cylinders[Y3].input_signal[DOWN];
            configure_output_signals(engine, (int[]) {Y6, Y3},(int[]) {0, 0});
            automate_capacity(transition, engine, STATE_16, STATE_FAIL);
            break;
        }
        case STATE_16: {
            bool transition = engine->cylinders[Y4].input_signal[UP] &&
                              engine->cylinders[Y7].input_signal[UP];
            configure_output_signals(engine, (int[]) {Y4, Y7}, (int[]) {1, 1});
            automate_capacity(transition, engine, STATE_17, STATE_FAIL);
            break;
        }
        case STATE_17: {
            bool transition = engine->cylinders[Y1].input_signal[UP] &&
                              engine->cylinders[Y2].input_signal[DOWN] &&
                              engine->cylinders[Y3].input_signal[UP] &&
                              engine->cylinders[Y4].input_signal[DOWN] &&
                              engine->cylinders[Y5].input_signal[UP] &&
                              engine->cylinders[Y6].input_signal[UP] &&
                              engine->cylinders[Y7].input_signal[DOWN] &&
                              engine->cylinders[Y8].input_signal[UP];
            configure_output_signals(engine, (int[]) {Y1, Y2, Y3, Y4, Y5, Y6, Y7, Y8}, (int[]) {1, 0, 1, 0, 1, 1, 0, 1});
            automate_capacity(transition, engine, STATE_0, STATE_FAIL);
            break;
        }
        case STATE_FAIL: {
            ret = false;
            break;
        }
    }
    engine->timeout++;
    engine->delay++;
    return ret;
}