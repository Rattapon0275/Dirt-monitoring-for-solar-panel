
//Sensor Data
typedef struct{
    float Volt;
    float Amp;
    float Pow;
    int Light;
    char Rain;
    int Dirt;
    float Temp;
    char clean;
    char cleantime;
    char reqclean;
}SENSOR_DATA;

extern SENSOR_DATA sensor1;