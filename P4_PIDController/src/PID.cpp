#include "PID.h"
#include <math.h>
#include <iostream>


using namespace std;

/*
* Complete the PID class.
*/

PID::PID() {}

PID::~PID() {}


void PID::Init(double Kp, double Ki, double Kd) {  
  PID::Kp = Kp;
  PID::Ki = Ki;
  PID::Kd = Kd;
  
  p_error = 0.1;
  i_error = 0.001;
  d_error = 0.1;
}

void PID::UpdateError(double cte) {
  double p[] = {Kp, Kd};
  double dp[] = {p_error, d_error};
  
  const double thresh = 0.01;
  
  // loop for twiddle
  if (TotalError() > thresh) {

    switch (current_state) {
      case 0: {
        p[last_optimize_index] += dp[last_optimize_index];
        current_state = 1;
        break;
      }
      case 1: {
        if (fabs(cte) < fabs(best_error)) {
          best_error = cte;
          dp[last_optimize_index] *= 1.1;
          current_state = 3;
        } else {
          p[last_optimize_index] -= 2 * dp[last_optimize_index];
          current_state = 2;
        }
        break;
      }
      case 2: {
        if (fabs(cte) < fabs(best_error)) {
          best_error = cte;
          dp[last_optimize_index] *= 1.1;
        } else {
          p[last_optimize_index] += dp[last_optimize_index];
          dp[last_optimize_index] *= 0.9;
        }
        current_state = 3;
        break;
      }
      case 3: {
        last_optimize_index = (last_optimize_index + 1) % 2;
        current_state = 0;
        break;
      }
    }
    cout << "P values " << p[0] << " "<< p[1]  << endl;
    p_error = dp[0];
    d_error = dp[1];
    Kp = p[0];
    Kd = p[1];
  }
}

/*
Compute total error
*/
double PID::TotalError() {
  return fabs(p_error) + fabs(i_error) + fabs(d_error);
}

