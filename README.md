# UtyMapUnreal

## How to use

1. copy Utymap.Shared.dll from my [Utymap fork](https://github.com/RaceTheMaSe/utymap.git) with a modified exportlib to Binaries/Win64 directory
2. compile UtyMapUnreal VS project as DebugGameEditor and run
3. drag an instance of UtyMapLoader from the projects C++ classes from the content browser on the bottom into the scene and Play. There is an instance ob a test map laoded.

Drawbacks from the current implementation
It uses global variables as I wasn't able to bind the API to a class function yet. Maybe its a cup of cake for you.