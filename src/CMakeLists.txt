set(SOURCES Asset.cpp Date.cpp PriceRecord.cpp)

add_library(finance_core STATIC ${SOURCES})

target_include_directories(finance_core PUBLIC ${CMAKE_SOURCE_DIR}/include)