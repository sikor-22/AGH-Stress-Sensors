# AGH-Stress-Sensors
Repo for AGH Stress Concious project - Sensor "dept"
main driver -> /Driver/main_collector.cpp

> compile against pthread and lwiringPi, requires https://github.com/hallgrimur1471/Adafruit_ADS1X15_RPi


> g++ -o [executable] main_collector.cpp -lwiringPi -lpthread Adafruit_ADS1015.cpp
