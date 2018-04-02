#include <stdio.h>
#include <math.h>

int main()
{
  int i;
  double theta[7], jt[7], T[4][4];
  double c23, s23, px, py, pz;

  // Link Parameters:
  // link length a_i-1 in mm
  double a[6] = {0, 0, 650, 0, 0, 0};
  // link offset d_i, extra element d[0] allows for consistent notation
  double d[7] = {0, 0, 0, 191, 600, 0, 0};

  printf("\nEnter the values of the following joint coordinates:\n\n");

  for (i = 1; i <= 6; ++i)
  {
    printf("Theta %d = ", i);
    scanf("%lf", &theta[i]); // in degrees
    jt[i] = theta[i] * M_PI / 180.0; // in radians
  }

  jt[3] = jt[3] - M_PI;

  c23 = cos(jt[2])*cos(jt[3]) - sin(jt[2])*sin(jt[3]);
  s23 = cos(jt[2])*sin(jt[3]) + sin(jt[2])*cos(jt[3]);

  printf("c23 = %lf, s23 = %lf\n\n", c23, s23);

  // T[i][j] corresponds to r_ij

  T[1][1] = cos(jt[1]) * (c23*(cos(jt[4])*cos(jt[5])*cos(jt[6]) - sin(jt[4])*sin(jt[6])) - s23*sin(jt[5])*cos(jt[6])) + sin(jt[1]) * (sin(jt[4])*cos(jt[5])*cos(jt[6]) + cos(jt[4])*sin(jt[6]));
  T[2][1] = sin(jt[1]) * (c23*(cos(jt[4])*cos(jt[5])*cos(jt[6]) - sin(jt[4])*sin(jt[6])) - s23*sin(jt[5])*cos(jt[6])) - cos(jt[1]) * (sin(jt[4])*cos(jt[5])*cos(jt[6]) + cos(jt[4])*sin(jt[6]));
  T[3][1] = -s23 * (cos(jt[4])*cos(jt[5])*cos(jt[6]) - sin(jt[4])*sin(jt[6])) - c23*sin(jt[5])*cos(jt[6]);
  T[1][1] *= -1;
  T[2][1] *= -1;
  T[3][1] *= -1;

  T[1][2] = cos(jt[1]) * (c23*(-cos(jt[4])*cos(jt[5])*sin(jt[6]) - sin(jt[4])*cos(jt[6])) + s23*sin(jt[5])*sin(jt[6])) + sin(jt[1]) * (cos(jt[4])*cos(jt[6]) - sin(jt[4])*cos(jt[5])*sin(jt[6]));
  T[2][2] = sin(jt[1]) * (c23*(-cos(jt[4])*cos(jt[5])*sin(jt[6]) - sin(jt[4])*cos(jt[6])) + s23*sin(jt[5])*sin(jt[6])) - cos(jt[1]) * (cos(jt[4])*cos(jt[6]) - sin(jt[4])*cos(jt[5])*sin(jt[6]));
  T[3][2] = -s23 * (-cos(jt[4])*cos(jt[5])*sin(jt[6]) - sin(jt[4])*cos(jt[6])) + c23*sin(jt[5])*sin(jt[6]);
  T[1][2] *= -1;
  T[2][2] *= -1;
  T[3][2] *= -1;

  T[1][3] = -cos(jt[1]) * (c23*cos(jt[4])*sin(jt[5]) + s23*cos(jt[5])) - sin(jt[1])*sin(jt[4])*sin(jt[5]);
  T[2][3] = -sin(jt[1]) * (c23*cos(jt[4])*sin(jt[5]) + s23*cos(jt[5])) + cos(jt[1])*sin(jt[4])*sin(jt[5]);
  T[3][3] = s23*cos(jt[4])*sin(jt[5]) - c23*cos(jt[5]);

  px = cos(jt[1]) * (a[2]*cos(jt[2]) + a[3]*c23 - d[4]*s23) - d[3]*sin(jt[1]);
  py = sin(jt[1]) * (a[2]*cos(jt[2]) + a[3]*c23 - d[4]*s23) + d[3]*cos(jt[1]);
  pz = -a[3]*s23 - a[2]*sin(jt[2]) - d[4]*c23;
  T[1][4] = px;
  T[2][4] = py;
  T[3][4] = pz + 125;

  // Finding the (O, A, T) angles

  double O, A, t, degO, degA, degT;
  double cA = sqrt(pow(T[3][1],2) + pow(T[3][2],2));

  A = atan2(-T[3][3],cA);

  if (cA != 0)
  {
    t = atan2(T[3][2]/cA,-T[3][1]/cA);
    O = atan2(T[1][3]/cA,-T[2][3]/cA);
  }
  else if (T[3][3] == -1)
  {
    t = 0;
    O = atan2(-T[1][1],T[1][2]);
  }
  else if (T[3][3] == 1)
  {
    t = 0;
    O = atan2(T[1][1],T[1][2]);
  }

  // Convert to degrees and print

  degO = O * 180.0 / M_PI;
  degA = A * 180.0 / M_PI;
  degT = t * 180.0 / M_PI;

  printf("x = %.3lf mm\n", T[1][4]);
  printf("y = %.3lf mm\n", T[2][4]);
  printf("z = %.3lf mm\n", T[3][4]);
  printf("O = %.3lf deg\n", degO);
  printf("A = %.3lf deg\n", degA);
  printf("T = %.3lf deg\n", degT);

  return 0;
}
