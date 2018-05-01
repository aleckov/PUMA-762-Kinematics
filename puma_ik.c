#include <stdio.h>
#include <math.h>

int main()
{
	double T_06[5][5];
	double x, y, z, O_deg, A_deg, T_deg, O, A, T, px, py, pz;

	// Link Parameters:
	// link length a_i-1 in mm
	double a[6] = {0, 0, 650, 0, 0, 0};
	// link offset d_i, extra element d[0] allows for consistent notation
	double d[7] = {0, 0, 0, 191, 600, 0, 0};
	
	printf("\nEnter the values of (x, y, z, O, A, T):\n\n");
	
	printf("x = ");
	scanf("%lf", &x); // in mm
	printf("y = ");
	scanf("%lf", &y); // in mm
	printf("z = ");
	scanf("%lf", &z); // in mm	
	
	printf("O = ");
	scanf("%lf", &O_deg); // in degrees
	O = O_deg * M_PI / 180.0; // in radians
	printf("A = ");
	scanf("%lf", &A_deg); // in degrees
	A = A_deg * M_PI / 180.0; // in radians
	printf("T = ");
	scanf("%lf", &T_deg); // in degrees
	T = T_deg * M_PI / 180.0; // in radians

	/*
	x = 154.818;
	y = -283.594;
	z = 1309.937;
	O = -78.048 * M_PI / 180.0;
	A = -68.788 * M_PI / 180.0;
	T = 103.049 * M_PI / 180.0;
	*/

	// T_06[i][j] corresponds to r_ij
	
	T_06[1][1] = sin(O)*sin(A)*cos(T) - cos(O)*sin(T);
	T_06[2][1] = -sin(O)*sin(T) - cos(O)*sin(A)*cos(T);
	T_06[3][1] = cos(A)*cos(T);
	T_06[4][1] = 0;
	
	T_06[1][2] = -cos(O)*cos(T) - sin(O)*sin(A)*sin(T);
	T_06[2][2] = cos(O)*sin(A)*sin(T) - sin(O)*cos(T);
	T_06[3][2] = -cos(A)*sin(T);
	T_06[4][2] = 0;
	
	T_06[1][3] = sin(O)*cos(A);
	T_06[2][3] = -cos(O)*cos(A);
	T_06[3][3] = -sin(A);
	T_06[4][3] = 0;
	
	T_06[1][4] = -125*sin(O)*cos(A) + x;
	T_06[2][4] = 125*cos(O)*cos(A) + y;
	T_06[3][4] = 125*sin(A) + z;
	T_06[4][4] = 1;
	
	px = T_06[1][4];
	py = T_06[2][4];
	pz = T_06[3][4];
	
	// Solving for joint coordinates
	
	int i, j, k;
	double K, s23, c23, s1, c1, s3, c3, s4, c4, s5, c5, s6, c6;
	double th1[2], th1_deg[2], th3[2], th3_deg[2], th23[2][2], th2[2][2], th2_deg[2][2], th4[2][2], th4_deg[2][2][2], th5[2][2], th5_deg[2][2][2], th6[2][2], th6_deg[2][2][2];
	
	K = (pow(px,2) + pow(py,2) + pow(pz,2) - pow(a[2],2) - pow(a[3],2) - pow(d[3],2) - pow(d[4],2)) / (2*a[2]);
	
	th1[0] = atan2(py, px) - atan2(d[3], sqrt(pow(px,2) + pow(py,2) - pow(d[3],2)));
	th1[1] = atan2(py, px) - atan2(d[3], -sqrt(pow(px,2) + pow(py,2) - pow(d[3],2)));
	th1_deg[0] = th1[0] * 180.0 / M_PI;
	th1_deg[1] = th1[1] * 180.0 / M_PI;
	
	th3[0] = atan2(a[3], d[4]) - atan2(K, sqrt(pow(a[3],2) + pow(d[4],2) - pow(K,2)));
	th3[1] = atan2(a[3], d[4]) - atan2(K, -sqrt(pow(a[3],2) + pow(d[4],2) - pow(K,2)));
	th3_deg[0] = th3[0] * 180.0 / M_PI + 180.0;
	th3_deg[1] = th3[1] * 180.0 / M_PI + 180.0;

	// i and j give the corresponding angles th1[i] and th3[j]
	for (i = 0; i < 2; ++i)
	{
		for (j = 0; j < 2; ++j)
		{
			s1 = sin(th1[i]);
			c1 = cos(th1[i]);
			s3 = sin(th3[j]);
			c3 = cos(th3[j]);
			s23 = ((-a[3] - a[2]*c3)*pz + (c1*px + s1*py)*(a[2]*s3 - d[4])) / (pow(pz, 2) + pow(c1*px + s1*py, 2));
			c23 = ((a[2]*s3 - d[4])*pz + (a[3] + a[2]*c3)*(c1*px + s1*py)) / (pow(pz, 2) + pow(c1*px + s1*py, 2));
			
			th23[i][j] = atan2(s23, c23);
			th2[i][j] = th23[i][j] - th3[j];
			th2_deg[i][j] = th2[i][j] * 180.0 / M_PI;

			th4[i][j] = atan2(-T_06[1][3]*s1 + T_06[2][3]*c1, -T_06[1][3]*c1*c23 - T_06[2][3]*s1*c23 + T_06[3][3]*s23);
			th4_deg[i][j][0] = th4[i][j] * 180.0 / M_PI;
			th4_deg[i][j][1] = th4_deg[i][j][0] + 180.0;

			s4 = sin(th4[i][j]);
			c4 = cos(th4[i][j]);

			s5 = -(T_06[1][3]*(c1*c23*c4 + s1*s4) + T_06[2][3]*((s1*c23*c4) - c1*s4) - T_06[3][3]*s23*c4);
			c5 = -T_06[1][3]*c1*s23 - T_06[2][3]*s1*s23 - T_06[3][3]*c23;
			th5[i][j] = atan2(s5, c5);
			th5_deg[i][j][0] = th5[i][j] * 180.0 / M_PI;
			th5_deg[i][j][1] = -th5_deg[i][j][0];

			s6 = -T_06[1][1]*(c1*c23*s4 - s1*c4) - T_06[2][1]*(s1*c23*s4 + c1*c4) + T_06[3][1]*s23*s4;
			c6 = T_06[1][1]*((c1*c23*c4 + s1*s4)*c5 - c1*s23*s5) + T_06[2][1]*((s1*c23*c4 - c1*s4)*c5 - s1*s23*s5) - T_06[3][1]*(s23*c4*c5 + c23*s5);
			th6[i][j] = atan2(s6, c6);
			th6_deg[i][j][0] = th6[i][j] * 180.0 / M_PI;
			th6_deg[i][j][1] = th6_deg[i][j][0] - 180.0;
		}
	}

	printf("\nEight possible solutions:\n\n");

	for (i = 0; i < 2; ++i)
		for (j = 0; j < 2; ++j)
			for (k = 0; k < 2; ++k)
				printf("Jt 1 = %.3lf, Jt 2 = %.3lf, Jt 3 = %.3lf, Jt 4 = %.3lf, Jt 5 = %.3lf, Jt 6 = %.3lf\n\n", th1_deg[i], th2_deg[i][j], th3_deg[j], th4_deg[i][j][k], th5_deg[i][j][k], th6_deg[i][j][k]);

	return 0;
}
