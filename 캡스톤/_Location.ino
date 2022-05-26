//좌표변환용 구조체
struct lamc_parameter {
  float Re; /* 사용할 지구반경 [ km ] */
  float grid; /* 격자간격 [ km ] */
  float slat1; /* 표준위도 [degree] */
  float slat2; /* 표준위도 [degree] */
  float olon; /* 기준점의 경도 [degree] */
  float olat; /* 기준점의 위도 [degree] */
  float xo; /* 기준점의 X좌표 [격자거리] */
  float yo; /* 기준점의 Y좌표 [격자거리] */
  int first; /* 시작여부 (0 = 시작) */
};
