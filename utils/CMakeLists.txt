add_library(obscure_utils)
add_library(obscure::utils ALIAS obscure_utils)
target_sources(obscure_utils
        PUBLIC
        FILE_SET CXX_MODULES FILES
        "obscure.utils.check_result.cppm"
        "obscure.utils.key_list.cppm"
        "obscure.utils.key_map.cppm"
        "obscure.utils.key_set.cppm"
        "obscure.utils.name_list.cppm"
        "obscure.utils.stopwatch.cppm"
        "obscure.utils.value_list.cppm"
        "obscure.utils.version.cppm"
        "obscure.utils.cppm"
)