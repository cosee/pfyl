//
// Created by mohamad on 25.01.20.
//

#ifndef UART_INFLUX_CLIENT_H
#define UART_INFLUX_CLIENT_H


class CInfluxClient {
public:
    CInfluxClient(std::string host, std::string database);
    void initDb(std::string host, std::string database);
    void sendToDB(sink_entity *se);
private:
    std::unique_ptr<influxdb::InfluxDB> influxdb;
};


#endif //UART_INFLUX_CLIENT_H
