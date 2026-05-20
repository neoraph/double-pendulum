#pragma once

#include <SDL3/SDL.h>
#include <cmath>
#include <vector>

struct Pendulum {
    double l1 = 140.0, l2 = 140.0;
    double m1 = 12.0, m2 = 12.0;

    double theta1, theta2;
    double omega1 = 0.0, omega2 = 0.0;

    const double g = 9.81;
    std::vector<SDL_FPoint> trail;
    SDL_Color color;

    Pendulum(double t1, double t2, SDL_Color c) : theta1(t1), theta2(t2), color(c) {}

    void update(double dt) {
        double deltaTheta = theta1 - theta2;
        double mu = 1.0 + m1 / m2;

        double den1 = l1 * (mu - std::cos(deltaTheta) * std::cos(deltaTheta));
        double den2 = l2 * (mu - std::cos(deltaTheta) * std::cos(deltaTheta));

        double num1 = g * (std::sin(theta2) * std::cos(deltaTheta) - mu * std::sin(theta1))
                    - (l2 * omega2 * omega2 + l1 * omega1 * omega1 * std::cos(deltaTheta)) * std::sin(deltaTheta);

        double num2 = g * mu * (std::sin(theta1) * std::cos(deltaTheta) - std::sin(theta2))
                    + (mu * l1 * omega1 * omega1 + l2 * omega2 * omega2 * std::cos(deltaTheta)) * std::sin(deltaTheta);

        double alpha1 = num1 / den1;
        double alpha2 = num2 / den2;

        omega1 += alpha1 * dt;
        omega2 += alpha2 * dt;

        // Anti-dissipation numérique pour maintenir le chaos à l'infini
        omega1 *= 1.0001;
        omega2 *= 1.0001;

        theta1 += omega1 * dt;
        theta2 += omega2 * dt;
    }

    void getPositions(float x0, float y0, float& x1, float& y1, float& x2, float& y2) const {
        x1 = x0 + static_cast<float>(l1 * std::sin(theta1));
        y1 = y0 + static_cast<float>(l1 * std::cos(theta1));
        x2 = x1 + static_cast<float>(l2 * std::sin(theta2));
        y2 = y1 + static_cast<float>(l2 * std::cos(theta2));
    }
};
