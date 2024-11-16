#include "OmniWheelController.h"


// Constructor: inicializa motores
OmniWheelController::OmniWheelController(int stepPins[4], int dirPins[4]) {
    for (int i = 0; i < 4; i++) {
        motors[i].portStep = portOutputRegister(digitalPinToPort(stepPins[i]));
        motors[i].bitStep = digitalPinToBitMask(stepPins[i]);
        pinMode(stepPins[i], OUTPUT);

        motors[i].portDir = portOutputRegister(digitalPinToPort(dirPins[i]));
        motors[i].bitDir = digitalPinToBitMask(dirPins[i]);
        pinMode(dirPins[i], OUTPUT);

        motors[i].stepInterval = 0;
        motors[i].active = false;
    }
}

//
void OmniWheelController::setMotorSpeed(int motorIndex, unsigned long interval, bool direction) {
    motors[motorIndex].stepInterval = interval;
    if (direction) {
        *motors[motorIndex].portDir |= motors[motorIndex].bitDir; // HIGH
    } else {
        *motors[motorIndex].portDir &= ~motors[motorIndex].bitDir; // LOW
    }
    motors[motorIndex].active = (interval > 0);
}

// Movimiento hacia adelante
void OmniWheelController::forward(unsigned long speed) {
    for (int i = 0; i < 4; i++) {
        setMotorSpeed(i, speed, true);
    }
}

// Movimiento hacia atrás
void OmniWheelController::backward(unsigned long speed) {
    for (int i = 0; i < 4; i++) {
        setMotorSpeed(i, speed, false);
    }
}

// Movimiento lateral a la izquierda
void OmniWheelController::left(unsigned long speed) {
    setMotorSpeed(0, speed, false); // FL
    setMotorSpeed(1, speed, true);  // FR
    setMotorSpeed(2, speed, true);  // RL
    setMotorSpeed(3, speed, false); // RR
}

// Movimiento lateral a la derecha
void OmniWheelController::right(unsigned long speed) {
    setMotorSpeed(0, speed, true);  // FL
    setMotorSpeed(1, speed, false); // FR
    setMotorSpeed(2, speed, false); // RL
    setMotorSpeed(3, speed, true);  // RR
}

// Rotación en sentido horario
void OmniWheelController::rotateClockwise(unsigned long speed) {
    setMotorSpeed(0, speed, true);  // FL
    setMotorSpeed(1, speed, false); // FR
    setMotorSpeed(2, speed, true);  // RL
    setMotorSpeed(3, speed, false); // RR
}

// Rotación en sentido antihorario
void OmniWheelController::rotateCounterClockwise(unsigned long speed) {
    setMotorSpeed(0, speed, false); // FL
    setMotorSpeed(1, speed, true);  // FR
    setMotorSpeed(2, speed, false); // RL
    setMotorSpeed(3, speed, true);  // RR
}

// Movimiento diagonal
void OmniWheelController::diagonal(String direction, unsigned long speed) {
    if (direction == "NE") {
        setMotorSpeed(0, 0, true);    // FL
        setMotorSpeed(1, speed, true); // FR
        setMotorSpeed(2, speed, true); // RL
        setMotorSpeed(3, 0, true);    // RR
    } else if (direction == "NW") {
        setMotorSpeed(0, speed, true); // FL
        setMotorSpeed(1, 0, true);    // FR
        setMotorSpeed(2, 0, true);    // RL
        setMotorSpeed(3, speed, true); // RR
    } else if (direction == "SE") {
        setMotorSpeed(0, 0, true);    // FL
        setMotorSpeed(1, speed, false); // FR
        setMotorSpeed(2, speed, false); // RL
        setMotorSpeed(3, 0, true);    // RR
    } else if (direction == "SW") {
        setMotorSpeed(0, speed, false); // FL
        setMotorSpeed(1, 0, true);    // FR
        setMotorSpeed(2, 0, true);    // RL
        setMotorSpeed(3, speed, false); // RR
    }
}

// Detener todos los motores
void OmniWheelController::stop() {
    for (int i = 0; i < 4; i++) {
        setMotorSpeed(i, 0, true);
    }
}

// Interrupción para manejar pasos
void OmniWheelController::handleInterrupt() {
    unsigned long currentTime = micros();
    for (int i = 0; i < 4; i++) {
        if (motors[i].active && currentTime - motors[i].stepInterval >= motors[i].stepInterval) {
            *motors[i].portStep |= motors[i].bitStep;  // HIGH
            delayMicroseconds(5);
            *motors[i].portStep &= ~motors[i].bitStep; // LOW
        }
    }
}
