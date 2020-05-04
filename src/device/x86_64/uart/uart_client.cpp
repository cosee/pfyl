#include <cstdio>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include "InfluxDB.h"
#include <raw_decoder.h>
#include "sink_entity.h"
#include "../common/influx_client.h"

#include <time.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"


void dumpSinkEntity(sink_entity *se) {
    printf("\n +--------------------------------------------+");
    printf("\n |   Sink Entity Dump                         |");
    printf("\n |   type:          0x%x", se->type);
    printf("\n |   tick:          0x%x", se->tick);
    printf("\n |   data:          0x%x", se->data);
    printf("\n |   data2:   0x%x", se->data2);
    printf("\n |   data3:   0x%x", se->data3);
    printf("\n |                                            |");
    printf("\n +--------------------------------------------+");
    fflush(stdout);
}

/** Configure according to your setup **/
const char* dev = "/dev/ttyACM0";
const char *influxDbConnection = "http://localhost:8086/?db=mydb";

int main(void) {

    CInfluxClient db(influxDbConnection, "");
    int serial_dev;
    serial_dev = open(dev, O_RDWR | O_NOCTTY);

    setbuf(stdout, NULL);
    if (serial_dev == -1) {
        printf("open failed for: %s", dev);
        return -1;
    }

    printf("opened %s", dev);

    struct termios SerialPortSettings;
    tcgetattr(serial_dev, &SerialPortSettings);

    /* Setting BAUD to 1MHz */
    cfsetispeed(&SerialPortSettings, B9600);
    cfsetospeed(&SerialPortSettings, B9600);

    /* No parity, 8 Data Bytes, 1 Stop Bit, no hwflow control */
    SerialPortSettings.c_cflag &= ~PARENB;
    SerialPortSettings.c_cflag &= ~CSTOPB;
    SerialPortSettings.c_cflag &= ~CSIZE;
    SerialPortSettings.c_cflag |= CS8;
    SerialPortSettings.c_cflag &= ~CRTSCTS;
    SerialPortSettings.c_cflag |= CREAD | CLOCAL;
    SerialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY);
    SerialPortSettings.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    SerialPortSettings.c_oflag &= ~OPOST;

    SerialPortSettings.c_cc[VMIN] = 33;
    SerialPortSettings.c_cc[VTIME] = 0;
    tcsetattr(serial_dev, TCSANOW, &SerialPortSettings);

    tcflush(serial_dev, TCIFLUSH);

    uint8_t read_buffer[33];
#ifndef USE_BENCHMARKING
    sink_entity se;
    for (;;) {
        read(serial_dev, &read_buffer, 33);
        convertByteArrayToSinkEntity(&read_buffer, 33, &se);
        dumpSinkEntity(&se);
        db.sendToDB(&se);
    }
#else
    struct timespec start, diff;
    clockid_t clk_id;
    clk_id = CLOCK_MONOTONIC;
    sink_entity se;
    int number_of_packets = 0;

    read(serial_dev, &read_buffer, 33);
    number_of_packets++;
    clock_gettime(clk_id, &start);
    for (;;) {
        read(serial_dev, &read_buffer, 33);
        number_of_packets++;
        clock_gettime(clk_id, &diff);
        if(diff.tv_sec - start.tv_sec >= 30) {
            break;
        }
    }
    printf("Test finished in: %ds with %d number of packets sent", diff.tv_sec - start.tv_sec, number_of_packets);
#endif
}

#pragma clang diagnostic pop