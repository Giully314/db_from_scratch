add_library(scratchdb)
target_sources(scratchdb 
    PUBLIC FILE_SET CXX_MODULES FILES 
    scratchdb.cppm
    scratchdb_types.cppm
    scratchdb_constants.cppm
    scratchdb_row.cppm
    scratchdb_table.cppm
    scratchdb_serialize.cppm
    scratchdb_errors.cppm
    )

target_link_libraries(scratchdb PRIVATE scratchdb_compiler_flags)