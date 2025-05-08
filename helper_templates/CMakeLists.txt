add_library(helper_templates)
target_sources(helper_templates
        PUBLIC
        FILE_SET CXX_MODULES FILES
        "max_set_bit.cppm"
        "parent_reference.cppm"
        "sum.cppm"
        "vector_initialize.cppm"
)