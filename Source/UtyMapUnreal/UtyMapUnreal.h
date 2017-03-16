#pragma once

#include "Engine.h"

//General Log
DECLARE_LOG_CATEGORY_EXTERN(UtyLog, Log, All);

//Logging during game startup
DECLARE_LOG_CATEGORY_EXTERN(UtyInit, Log, All);

//Logging during map and mesh processing
DECLARE_LOG_CATEGORY_EXTERN(UtyProcessing, Log, All);

//Logging for Critical Errors that must always be addressed
DECLARE_LOG_CATEGORY_EXTERN(UtyCriticalErrors, Log, All);