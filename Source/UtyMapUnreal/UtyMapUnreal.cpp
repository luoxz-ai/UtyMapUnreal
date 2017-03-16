#include "UtyMapUnreal.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, UtyMapUnreal, "UtyMapUnreal" );

//General Log
DEFINE_LOG_CATEGORY(UtyLog);

//Logging during game startup
DEFINE_LOG_CATEGORY(UtyInit);

//Logging during map and mesh processing
DEFINE_LOG_CATEGORY(UtyProcessing);

//Logging for Critical Errors that must always be addressed
DEFINE_LOG_CATEGORY(UtyCriticalErrors);
