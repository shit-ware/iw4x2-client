#pragma once

#define BINARY_PAYLOAD_SIZE 0x20000000
#define TLS_PAYLOAD_SIZE 0x10000

auto manual_map() -> void;
auto verify_tls() -> void;

// tls data
extern __declspec(thread) char tls_data[TLS_PAYLOAD_SIZE];
