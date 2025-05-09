add_library(utils)
target_sources(utils
        PUBLIC
        FILE_SET CXX_MODULES FILES
        "obscure.utils.key_list.cppm"
        "obscure.utils.key_map.cppm"
        "obscure.utils.key_set.cppm"
        "obscure.utils.name_list.cppm"
        "obscure.utils.stopwatch.cppm"
        "obscure.utils.value_list.cppm"
        "obscure.utils.version.cppm"
)