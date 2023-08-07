set(SRC
    BaseUnit.cpp
    FlightUnit.cpp
    GroundUnit.cpp
    MapRegion.cpp
    NonMovableUnit.cpp
    Utils.cpp
    )

add_library(zonghengUnits SHARED ${SRC})
