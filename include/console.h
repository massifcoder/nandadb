#ifndef CONSOLE_H
#define CONSOLE_H

#include <bits/stdc++.h>
#include <chrono>
#include <curses.h>
#include <filesystem>
#include "json.hpp"
#include "utility.h"
#include "loading.h"
#define username "vishal"
#define pwd "vishal"
using namespace std;

extern bool passed_post;

void Console();

void POST();

#endif