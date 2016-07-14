#ifndef ESPASYNCUDP_H
#define ESPASYNCUDP_H

#include "IPAddress.h"
#include "Print.h"

extern "C" {
#include "osapi.h"
#include "ets_sys.h"
#include "user_interface.h"
}
#include <functional>

class AsyncUDP;
class AsyncUDPPacket;
struct udp_pcb;
struct pbuf;

class AsyncUDPPacket : public Print
{
protected:
    AsyncUDP *_udp;
    ip_addr_t *_localIp;
    uint16_t _localPort;
    ip_addr_t *_remoteIp;
    uint16_t _remotePort;
    uint8_t *_data;
    size_t _len;
public:
    AsyncUDPPacket(AsyncUDP *udp, ip_addr_t *localIp, uint16_t localPort, ip_addr_t *remoteIp, uint16_t remotePort, uint8_t *data, size_t len);
    ~AsyncUDPPacket();

    uint8_t * data();
    size_t length();
    bool isBroadcast();
    bool isMulticast();

    IPAddress localIP();
    uint16_t localPort();
    IPAddress remoteIP();
    uint16_t remotePort();

    size_t write(const uint8_t *data, size_t len);
    size_t write(uint8_t data);
};

typedef std::function<void(void*, AsyncUDPPacket packet)> AuPacketHandlerFunction;

class AsyncUDP : public Print
{
protected:
    udp_pcb *_pcb;
    bool _connected;
    AuPacketHandlerFunction _handler;
    void * _handlerArg;

    void _recv(udp_pcb *upcb, pbuf *pb, ip_addr_t *addr, u16_t port);
    static void _s_recv(void *arg, udp_pcb *upcb, pbuf *p, struct ip_addr *addr, uint16_t port);

public:
    AsyncUDP();
    ~AsyncUDP();

    void onPacket(AuPacketHandlerFunction cb, void * arg = NULL);

    bool listen(ip_addr_t *addr, uint16_t port);
    bool listen(const IPAddress addr, uint16_t port);
    bool listen(uint16_t port);

    bool listenMulticast(ip_addr_t *addr, uint16_t port, uint8_t ttl=1);
    bool listenMulticast(const IPAddress addr, uint16_t port, uint8_t ttl=1);

    bool connect(ip_addr_t *addr, uint16_t port);
    bool connect(const IPAddress addr, uint16_t port);

    void close();

    size_t write(const uint8_t *data, size_t len, ip_addr_t *addr, uint16_t port);
    size_t write(const uint8_t *data, size_t len, const IPAddress addr, uint16_t port);
    size_t write(const uint8_t *data, size_t len);
    size_t write(uint8_t data);

    size_t broadcast(uint8_t *data, size_t len, uint16_t port);
    size_t broadcast(const char * data, uint16_t port);
    size_t broadcast(uint8_t *data, size_t len);
    size_t broadcast(const char * data);

    bool connected();
    operator bool()
    {
        return connected();
    }
};

#endif
