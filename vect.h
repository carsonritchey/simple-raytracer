// wrote my own vector arithmatic methods because i didnt feel like looking for a library for them

#include <vector>

vector<double> v_add(vector<double> a, vector<double> b) {
    for(double i = 0; i < a.size(); i++)
        a[i] = a[i] + b[i];

    return a;
}

vector<double> v_sub(vector<double> a, vector<double> b) {
    for(double i = 0; i < a.size(); i++)
        a[i] = a[i] - b[i];

    return a;
}

vector<double> v_scalar_mult(vector<double> a, double scalar) {
    for(double i = 0; i < a.size(); i++)
        a[i] *= scalar;

    return a;
}

vector<double> v_scalar_div(vector<double> a, double scalar) {
    for(double i = 0; i < a.size(); i++)
        a[i] /= scalar;

    return a;
}

double v_dot(vector<double> a, vector<double> b) {
    double t = 0;
    for(double i = 0; i < a.size(); i++)
        t += a[i] * b[i];

    return t;
}

double v_len(vector<double> a) {
    double t = 0;
    for(double i = 0; i < a.size(); i++)
        t += a[i] * a[i];

    return sqrt(t);
}
