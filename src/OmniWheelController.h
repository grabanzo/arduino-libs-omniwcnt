#ifndef OMNIWHEELCONTROLLER
#define OMNIWHEELCONTROLLER

#include <Arduino.h>

class OmniWheelController {
private:
    struct Motor {
        volatile uint8_t* portStep;
        volatile uint8_t* portDir;
        uint8_t bitStep;
        uint8_t bitDir;
        unsigned long stepInterval;
        volatile bool active;
    };

    Motor motors[4];

    void setMotorSpeed(int motorIndex, unsigned long interval, bool direction);

public:
    // Constructor: inicializa motores
    OmniWheelController(int stepPins[4], int dirPins[4]);

    // Movimiento hacia adelante
    void forward(unsigned long speed);

    // Movimiento hacia atrás
    void backward(unsigned long speed);

    // Movimiento lateral a la izquierda
    void left(unsigned long speed);

    // Movimiento lateral a la derecha
    void right(unsigned long speed);

    // Rotación en sentido horario
    void rotateClockwise(unsigned long speed);

    // Rotación en sentido antihorario
    void rotateCounterClockwise(unsigned long speed);

    // Movimiento diagonal
    void diagonal(String direction, unsigned long speed);

    // Detener todos los motores
    void stop();

    // Interrupción para manejar pasos
    void handleInterrupt();
};

#endif // OMNIWHEELCONTROLLER