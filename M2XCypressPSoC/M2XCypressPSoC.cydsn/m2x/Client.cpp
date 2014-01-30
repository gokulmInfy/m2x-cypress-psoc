#include "Client.h"

#include <project.h>
#include <stdint.h>

Client::Client() : _len(0), _socket(0xFF) {
}

Client::~Client() {
}

int Client::connect(const char *host, uint16_t port) {
  uint32_t ip;

  ip = ETH0_ParseIP(host);
  if ((ip == 0xFFFFFFFF) || (ip == 0)) {
    return 0;
  }

  _socket = ETH0_TcpOpen(12345);
  ETH0_TcpConnect(_socket, ip, port);
  return 1;
}

size_t Client::write(uint8_t b) {
  return write(&b, 1);
}

size_t Client::write(const uint8_t *buf, size_t size) {
  return ETH0_TcpSend(_socket, const_cast<uint8*>(buf), size);
}

int Client::available() {
  if (_len > 0) { return 1; }
  int ret = read(_buf, 1);
  if (ret <= 0) { return 0; }
  _len = ret;
  return 1;
}

int Client::read() {
  if (_len > 0) {
    _len = 0;
    return _buf[0];
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
