add_library(utils)
target_sources(utils
        PUBLIC
        FILE_SET CXX_MODULES FILES
        "key_list.cppm"
        "key_map.cppm"
        "key_set.cppm"
        "name_list.cppm"
        "stopwatch.cppm"
        "value_list.cppm"
        "version.cppm"
)