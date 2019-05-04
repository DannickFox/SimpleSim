#include <stdio.h>
#include <stdlib.h>

typedef struct {
    double mass, angspd, res;
} InertiaMass;

double accelCalcT (double, double);
double resistCalcT (double, double);
double integEuler (double, double);
double torqueFcnConst (double, InertiaMass);
double *simCycle (int, int, InertiaMass, double (*)(double, InertiaMass));

int main(int argc, char **argv) {
    
    // Initial conditions for spinning mass.
    InertiaMass J = (InertiaMass) {
        mass: 10.0,
        angspd: 0.0,
        res: 20.0
    };

    double *output = simCycle(100, 10, J, torqueFcnConst);

    for (int i = 0; i <= 100; i++) {
        // Display result.
        printf ("%f\n", output[i]);
    }

    free (output); // Clear memory.

    return 0;
}

double accelCalcT (double J, double torque) {
    // Calculate acceleration from torque applied.
    return torque / J;
}

double resistCalcT (double speed, double resist) {
    // Calculate deceleration from resistance.
    return -(speed * resist);
}

double integEuler (double x, double dt) {
    // Euler integration. ()
    return x * dt;
}

double torqueFcnConst (double t, InertiaMass J) {
    return 10;
} 

double *simCycle (int steps, int stepSamples, InertiaMass J, double (*torqueFcn) (double, InertiaMass)) {
    // Performs simulation.

    // Simulation time data.
    double delta = 1.0 / ((steps) * stepSamples);
    double t = 0;
    printf("delta = %f\n", delta);
    // Result stored in array and returned.
    double *simResult = malloc((steps + 1) * sizeof(double));

    // Counts the total amount of calculations performed.
    int totalSamples = 0;

    for (int i = 0; i <= steps; i++) {
        // Outer loop for steps. Stores information here.
        simResult[i] = J.angspd;
        for (int j = 0; j < stepSamples; j++) {
            // Inner loop for calculations.
            double alpha = accelCalcT(J.mass, torqueFcn(t, J)) + resistCalcT(J.angspd, J.res); // Rotational acceleration.
            J.angspd += integEuler(alpha, delta); // Speed increment.
            t += delta; // Increment time passed.
        }
    }

    return simResult;
}