#include "odu_33.h"

const double delta = 1e-6;

// f1(x) == y2;

double f2(double x, double y1, double y2) {
    return f(x) - p(x) * y2 - q(x) * y1;
}

double RollBack(double y, double f_, double h) {
    return y - h * f_;
}

double AdamsMoulton_y2(double x, const double y1[3], const double y2[3], double h) {
    double dividend =
            y2[2] + (h / 24) * (9 * (f(x + h) - q(x + h) * (y1[2] + (h / 24) * (19 * y2[2] - 5 * y2[1] + y2[0]))) +
                                19 * f(x) - 5 * f(x - h) + f(x - 2 * h));
    double divider = 1 + (3.0 / 8.0) * h * p(x + h) + (9.0 / 64.0) * h * h * q(x + h);
    return dividend / divider;
}

double AdamsMoulton_y1(double y1, double y2_new, const double y2[3], double h) {
    return y1 + h / 24 * (9 * y2_new + 19 * y2[2] - 5 * y2[1] + y2[0]);
}

double AdamsMoulton(double *y, double a, double b, double lambda, double h, int n) {
    double x = a;
    double y1[3] = {0, 0, y[0]};
    double y2[3] = {0, 0, lambda};

    // откатываем назад
    double f2_ = f2(x, y[0], lambda);
    y1[1]= RollBack(y1[2],y2[2],h);
    y2[1]= RollBack(y2[2],f2_,h);

    f2_ = f2(x-h, y1[1], y2[1]);
    y1[0] = RollBack(y1[1],y2[1],h);
    y2[0] = RollBack(y2[1],f2_,h);

    // Начинаме Адамса-Мултона
    double y1_new, y2_new;
    int n_ = (int) round((b - a) / h) + 1;
    int step = (n_ - 1) / (n - 1);

    for (int i = 1; i < n_; i++) {
        y2_new = AdamsMoulton_y2(x, y1, y2, h);
        y1_new = AdamsMoulton_y1(y1[2], y2_new, y2, h);
        x += h;

        y1[0] = y1[1];
        y2[0] = y2[1];
        y1[1] = y1[2];
        y2[1] = y2[2];
        y1[2] = y1_new;
        y2[2] = y2_new;

        if (i % step == 0)
            y[i / step] = y1_new;
    }
    return y[n - 1];
}

void Newton(double *y, double a, double b, double d, double h, double e, int n) {
    double lambda = 1;
    AdamsMoulton(y, a, b, lambda, h, n);

    //проверяем точность и находим новый параметр
    double y_b = (y)[n - 1];
    double F_minus, F_plus, F_diff;

    while (fabs(y_b - d) > e) {
        F_minus = AdamsMoulton(y, a, b, lambda - delta, h, n);
        F_plus = AdamsMoulton(y, a, b, lambda + delta, h, n);
        F_diff = (F_minus + F_plus) / (2 * delta);
        lambda -= (y_b - d) / F_diff;
        y_b = AdamsMoulton(y, a, b, lambda, h, n);
    }
    printf("%lf ", lambda);
}