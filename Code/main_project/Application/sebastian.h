#ifndef SEBASTIAN_H_
#define SEBASTIAN_H_

void MadgwickAHRSupdate(float* quaternion, float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz, float dt, float beta);
void MadgwickAHRSupdateIMU(float* quaternion, float gx, float gy, float gz, float ax, float ay, float az, float dt, float beta);

#endif /* SEBASTIAN_H_ */
