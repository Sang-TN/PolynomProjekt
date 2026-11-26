#pragma once
#include "Polynom.h"
#include <string>

Polynom ersteAbleitung(const Polynom& p);
Polynom zweiteAbleitung(const Polynom& p);

std::string alsText(const Polynom& p);
void ausgabe(const Polynom& p);
