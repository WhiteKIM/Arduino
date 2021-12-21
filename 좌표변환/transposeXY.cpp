#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <math.h>

#define NX 149 /* X�� ������ �� */
#define NY 253 /* Y�� ������ �� */

#define PI 3.14159265358979323846

//��ǥ��ȯ�� ����ü
struct lamc_parameter {
	float Re; /* ����� �����ݰ� [ km ] */
	float grid; /* ���ڰ��� [ km ] */
	float slat1; /* ǥ������ [degree] */
	float slat2; /* ǥ������ [degree] */
	float olon; /* �������� �浵 [degree] */
	float olat; /* �������� ���� [degree] */
	float xo; /* �������� X��ǥ [���ڰŸ�] */
	float yo; /* �������� Y��ǥ [���ڰŸ�] */
	int first; /* ���ۿ��� (0 = ����) */
};

using namespace std;

string x;
string y;

//��ǥ ��ȯ �Լ�
void dfs_xy_conf(double v1, double v2)
{
	double RE = 6371.00877; // ���� �ݰ�(km)
	double GRID = 5.0; // ���� ����(km)
	double SLAT1 = 30.0; // ���� ����1(degree)
	double SLAT2 = 60.0; // ���� ����2(degree)
	double OLON = 126.0; // ������ �浵(degree)
	double OLAT = 38.0; // ������ ����(degree)
	double XO = 43.0; // ������ X��ǥ(GRID)
	double YO = 136.0; // ������ Y��ǥ(GRID)
	double DEGRAD = PI / 180.0;
	double RADDEG = 180.0 / PI;

	double re = RE / GRID;
	double slat1 = SLAT1 * DEGRAD;
	double slat2 = SLAT2 * DEGRAD;
	double olon = OLON * DEGRAD;
	double olat = OLAT * DEGRAD;

	double sn = tan(PI * 0.25f + slat2 * 0.5f) / tan(PI * 0.25f + slat1 * 0.5f);
	sn = log(cos(slat1) / cos(slat2)) / log(sn);
	double sf = tan(PI * 0.25f + slat1 * 0.5f);
	sf = pow(sf, sn) * cos(slat1) / sn;
	double ro = tan(PI * 0.25f + olat * 0.5f);
	ro = re * sf / pow(ro, sn);

	int dx;
	int dy;
	double ra, theta;

	ra = tan(PI * 0.25f + (v1)*DEGRAD * 0.5f);
	ra = re * sf / pow(ra, sn);
	theta = v2 * DEGRAD - olon;
	if (theta > PI)
		theta -= 2.0f * PI;
	if (theta < -PI)
		theta += 2.0f * PI;
	theta *= sn;
	dx = floor(ra * sin(theta) + XO + 0.5f);
	dy = floor(ro - ra * cos(theta) + YO + 0.5f);

	x = String(dx);
	y = String(dy);
}