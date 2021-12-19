#pragma once

#include "RandomBBS.h"
#include <string>

std::string to_string(byte* key, size_t lenght);  // Преобразование массива байт в hex строку
bool miller_rabin(Integer p, int iteration = 15);
