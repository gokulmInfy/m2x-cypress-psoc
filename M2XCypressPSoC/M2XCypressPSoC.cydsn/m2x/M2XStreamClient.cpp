#include "M2XStreamClient.h"

const char* M2XStreamClient::kDefaultM2XHost = "api-m2x.att.com";

int print_encoded_string(Print* print, const char* str);
int tolower(int ch);

#if defined(ARDUINO_PLATFORM) || defined(MBED_PLATFORM) || defined(CYPRESS_PLATFORM)
int tolower(int ch)
{
  // Arduino and mbed use ASCII table, so we can simplify the implementation
  if ((ch >= 'A') && (ch <= 'Z')) {
    return (ch + 32);
  }
  return ch;
}
#else
// For other platform, we use libc's tolower by default
#include <ctype.h>
#endif

M2XStreamClient::M2XStreamClient(Client* client,
                                 const char* key,
                                 int case_insensitive,
                                 const char* host,
                                 int port) : _client(client),
                                             _key(key),
                                             _case_insensitive(case_insensitive),
                                             _host(host),
                                             _port(port),
                                             _null_print() {
}

// Encodes and prints string using Percent-encoding specified
// in RFC 1738, Section 2.2
int print_encoded_string(Print* print, const char* str) {
  int bytes = 0;
  for (int i = 0; str[i] != 0; i++) {
    if (((str[i] >= 'A') && (str[i] <= 'Z')) ||
        ((str[i] >= 'a') && (str[i] <= 'z')) ||
        ((str[i] >= '0') && (str[i] <= '9')) ||
        (str[i] == '-') || (str[i] == '_') ||
        (str[i] == '.') || (str[i] == '~')) {
      bytes += print->print(str[i]);
    } else {
      // Encode all other characters
      bytes += print->print('%');
      bytes += print->print(HEX(str[i] / 16));
      bytes += print->print(HEX(str[i] % 16));
    }
  }
  return bytes;
}

void M2XStreamClient::writePostHeader(const char* feedId,
                                      const char* streamName,
                                      int contentLength) {
  _client->print("PUT /v1/feeds/");
  print_encoded_string(_client, feedId);
  _client->print("/streams/");
  print_encoded_string(_client, streamName);
  _client->println(" HTTP/1.0");

  writeHttpHeader(contentLength);
}

void M2XStreamClient::writeHttpHeader(int contentLength) {
  _client->println(USER_AGENT);
  _client->print("X-M2X-KEY: ");
  _client->println(_key);

  _client->print("Host: ");
  print_encoded_string(_client, _host);
  if (_port != kDefaultM2XPort) {
    _client->print(":");
    // port is an integer, does not need encoding
    _client->print(_port);
  }
  _client->println();

  if (contentLength > 0) {
    _client->println("Content-Type: application/json");
    DBG("%s", "Content Length: ");
    DBGLN("%d", contentLength);

    _client->print("Content-Length: ");
    _client->println(contentLength);
  }
  _client->println();
}

int M2XStreamClient::waitForString(const char* str) {
  int currentIndex = 0;
  if (str[currentIndex] == '\0') return E_OK;

  while (true) {
    while (_client->available()) {
      char c = _client->read();
      DBG("%c", c);

      int cmp;
      if (_case_insensitive) {
        cmp = tolower(c) - tolower(str[currentIndex]);
      } else {
        cmp = c - str[currentIndex];
      }

      if ((str[currentIndex] == '*') || (cmp == 0)) {
        currentIndex++;
        if (str[currentIndex] == '\0') {
          return E_OK;
        }
      } else {
        // start from the beginning
        currentIndex = 0;
      }
    }

    if (!_client->connected()) {
      DBGLN("%s", "ERROR: The client is disconnected from the server!");

      close();
      return E_DISCONNECTED;
    }

    delay(1000);
  }
  // never reached here
  return E_NOTREACHABLE;
}

int M2XStreamClient::readStatusCode(bool closeClient) {
  int responseCode = 0;
  int ret = waitForString("HTTP/*.* ");
  if (ret != E_OK) {
    if (closeClient) close();
    return ret;
  }

  // ret is not needed from here(since it must be E_OK), so we can use it
  // as a regular variable now.
  ret = 0;
  while (true) {
    while (_client->available()) {
      char c = _client->read();
      DBG("%c", c);

      responseCode = responseCode * 10 + (c - '0');
      ret++;
      if (ret == 3) {
        if (closeClient) close();
        return responseCode;
      }
    }

    if (!_client->connected()) {
      DBGLN("%s", "ERROR: The client is disconnected from the server!");

      if (closeClient) close();
      return E_DISCONNECTED;
    }

    delay(1000);
  }

  // never reached here
  return E_NOTREACHABLE;
}

void M2XStreamClient::close() {
  // Eats up buffered data before closing
  _client->flush();
  _client->stop();
}

