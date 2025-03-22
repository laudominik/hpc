#pragma once

#define SAMPLE(name, ...) int main() { name(__VA_ARGS__); }
#define P99_PROTECT(...) __VA_ARGS__
