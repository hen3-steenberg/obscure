add_library(utils)
target_sources(utils
        PUBLIC
        FILE_SET CXX_MODULES FILES
        "key_list.cppm"
        "name_list.cppm"
        "value_list.cppm"
        "version.cppm"
)