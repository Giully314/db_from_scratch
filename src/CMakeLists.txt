add_library(scratchdb)
target_sources(scratchdb 
    PUBLIC FILE_SET CXX_MODULES FILES 
    scratchdb_types.cppm)

target_link_libraries(scratchdb PRIVATE scratchdb_compiler_flags)