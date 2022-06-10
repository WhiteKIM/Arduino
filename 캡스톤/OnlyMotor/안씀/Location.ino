//좌표 변환 함수
void dfs_xy_conf(double v1, double v2)
{
  double RE = 6371.00877; // 지구 반경(km)
  double GRID = 5.0; // 격자 간격(km)
  double SLAT1 = 30.0; // 투영 위도1(degree)
  double SLAT2 = 60.0; // 투영 위도2(degree)
  double OLON = 126.0; // 기준점 경도(degree)
  double OLAT = 38.0; // 기준점 위도(degree)
  double XO = 43.0; // 기준점 X좌표(GRID)
  double YO = 136.0; // 기준점 Y좌표(GRID)
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

  ra = tan(PI * 0.25f + (v1) * DEGRAD * 0.5f);
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
