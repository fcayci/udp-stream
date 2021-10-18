#ifndef TYPES_H_
#define TYPES_H_

#include <ostream>
#include <string>

struct point_t {
    int x;
    int y;

    friend std::ostream& operator<<(std::ostream& os, const point_t p) {
        os << p.x << "," << p.y;
        return os;
    }
};

typedef enum { SUCCESS, FAILURE } status_t;

struct serial_port_t {
    std::string port;
    long baud;
    long databits;
    char parity;
    float stopbits;
};

struct pid_control_param_t {
    double kp;
    double ki;
    double kd;
};

#endif  // TYPES_H_