#pragma once
#define _ENABLE_ATOMIC_ALIGNMENT_FIX


#include "stdafx.h"
#include <iostream>
#include <thread>
#include <chrono>
#include "Stack.h"
#include "Backoff.h"
#include "Node.h"
#include <algorithm>
#include <mutex>
#include <stdio.h>

Stack<int>* stack;
mutex mtx;