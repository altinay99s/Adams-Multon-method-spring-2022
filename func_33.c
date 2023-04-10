#include "func_33.h"

// y''+py'+qy=f
//y(a)=c  y(b)=d

// y''+2y'+y=x^2, y(0)=0, y(1)=1
// 0, 1, 0, 1, 0.1, 0.01
double f(double x) {
    return x*x;
}
double p(double x) {
    return 2;
}
double q(double x) {
    return 1;
}
double exactAnswer(double x) {
    return x*x-4*x+6+6*exp(-x)*(x-1)-2*exp(1-x)*x;
}

//// y''+2y'+y=0, y(0)=0, y(1)=1
//// 0, 1, 0, 1, 0.1, 0.01
//double f(double x) {
//    return 0;
//}
//double p(double x) {
//    return 2;
//}
//double q(double x) {
//    return 1;
//}
//double exactAnswer(double x) {
//    return exp(1-x)*x;
//}

////1, 2, 0, 1.38629, 0.1, 0.4
//double f(double x){
//    return log(x)/(x*x);
//}
//
//double p(double x){
//    return 1.0/x;
//}
//
//double q(double x){
//    return -1.0/(x*x);
//}
//
//double exactAnswer(double x){
//    return x* log(x);
//}



////1, 2, 0, 0.6666666, 0.01, 0.001
//double f(double x){
//    return x;
//}
//
//double p(double x){
//    return 2/x;
//}
//
//double q(double x){
//    return -6/(x*x);
//}
//
//double exactAnswer(double x){
//    return x*x*(x-1)/6;
//}

