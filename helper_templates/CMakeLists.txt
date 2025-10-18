add_library(obscure_helper_templates)
add_library(obscure::helper_templates ALIAS obscure_helper_templates)
target_sources(obscure_helper_templates
        PUBLIC
        FILE_SET CXX_MODULES FILES
        "obscure.helper_templates.array_initialize.cppm"
        "obscure.helper_templates.index_of.cppm"
        "obscure.helper_templates.max_set_bit.cppm"
        "obscure.helper_templates.parent_reference.cppm"
        "obscure.helper_templates.sum.cppm"
        "obscure.helper_templates.vector_initialize.cppm"
        "obscure.helper_templates.cppm"
)