add_library(helper_templates)
target_sources(helper_templates
        PUBLIC
        FILE_SET CXX_MODULES FILES
        "obscure.helper_templates.max_set_bit.cppm"
        "obscure.helper_templates.parent_reference.cppm"
        "obscure.helper_templates.sum.cppm"
        "obscure.helper_templates.vector_initialize.cppm"
)