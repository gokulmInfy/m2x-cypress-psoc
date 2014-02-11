#include "Client.h"

#ifdef __cplusplus
extern "C" {
#endif
#include <project.h>
#include <stdint.h>
#ifdef __cplusplus
}
#endif

Client::Client() : _socket(0xFF) {
}

Client::~Client() {
}

int Client::connect(const char *host, uint16_t port) {
  int status;

  _socket = ETH0_TcpOpen(12344);
  ETH0_TcpConnect(_socket, ETH0_ParseIP(host), port);
  status = ETH0_TcpConnected(_socket);
  return status;
}

size_t Client::write(uint8_t b) {
  return write(&b, 1);
}

size_t Client::write(const uint8_t *buf, size_t size) {
  return ETH0_TcpSend(_socket, const_cast<uint8*>(buf), size);
}

int Client::available() {
  int len = ETH0_SocketRxDataWaiting(_socket);
  return len > 0;
}

int Client::read() {
  uint8_t buf;
  int ret = read(&buf, 1);

  if (ret > 0) {
    return buf;
  }
  return -1;
}

int Client::read(uint8_t *buf, size_t size) {
  return ETH0_TcpReceive(_socket, buf, size);
}

void Client::flush() {
  // does nothing, TCP stack takes care of this
}

void Client::stop() {
  ETH0_TcpDisconnect(_socket);
  ETH0_SocketClose(_socket);
}

uint8_t Client::connected() {
  return ETH0_TcpConnected(_socket);
}
