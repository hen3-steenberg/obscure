# obscure
A 2-D rendering framework.
The name is a wordplay on 2-D render. Which can be taken as "to de-render" of which obscure is a synonym.
This is also indicative that this is the first time I will be attempting to write a rendering framework and everything is obscure to me.

## Aim
The aim is to build a wrapper around vulkan, where it is still possible to benefit from the
low level nature of vulkan but make the setup of vulkan easier and more optional for mainstream use cases.

## Overview
### Graphics Context
This is an object holding everything needed to draw to a single window/screen.
Drawing to multiple windows screens are possible by creating multiple graphics contexts.

This object is the main object through which to interact with vulkan. It holds and own handles
to the underlying vulkan resources such as a logical device, swap chain, various buffers, display surface
and more.

The graphics context definition is a template taking multiple pipeline definitions as
template parameters. This allows the user mix and match pipelines or even program their own 
pipeline to use.

The constructor for a graphics context takes an optional callable used to "score" the available
devices. The device with the highest score is picked by the graphics context to use.

Call `begin_frame` to get a frame context object which manages the state of the current frame.
Use the frame context to make draw calls to the GPU.
The lifetime of the frame context should end before calling the next two functions.

Call `submit_frame` to start processing the draw calls submitted to the GPU for the next frame.
Call `draw_frame` to wait for the GPU to process all draw calls for the next frame and then puts that frame into the display buffer.

These two functions were split as there is an opportunity to do some computation while the GPU is rendering the scene.

### Pipeline Definition
A pipeline can be any object definition which satisfy some requirements.

There are three requirements:
1. A compile time set of shaders which the pipeline needs to load
   - This should be a sub-type named `shader_list`.
   - Use the `obscure::make_set<auto ... value>` helper template.
   - The shaders are represented by [shader keys](#shader-keys).
   - Example `using shader_list = make_set<color_2d_vertex, color_fragment>;`
2. A static function `initialize`
   - This function does the configuration for the pipeline object and is perhaps the most exposed to the verbosity and complexity of the vulkan API
   - Taking as parameters
     - A device handle of type `vk::Device`
     - A render pass handle of type `vk::RenderPass`
     - A specification of the multisampling expected of type `vk::SampleCountFlagBits`
     - An array of N shader modules of type `std::array<vk::ShaderModule, N> const&`
       - N is equal to the number of shaders specified in `shader_list`
   - Returns a **pipeline builder** object
     - Again any type of object which satisfy the `obscure::vulkan::pipeline_builder` concept, or which has the functions
       - `get_create_info()` which returns a `vk::GraphicsPipelineCreateInfo`
       - `get_layout()` which returns a `vk::PipelineLayout`
       - `get_uniform_descriptor_layout()` which returns a `vk::DescriptorSetLayout`
       - `get_texture_descriptor_layout()` which returns a `vk::DescriptorSetLayout`
     - The object should manage the lifetime of any pointers referenced by the `vk::GraphicsPipelineCreateInfo` returned by the `get_create_info()` function
       - The lifetime of the return value only ends after the pipeline is already created.
     - The `obscure::vulkan::static_pipeline_builder` type is provided.
     - The `obscure::vulkan::default_pipeline_builder` helper function abstracts a lot of boilerplate code and returns a `static_pipeline_builder`.
3. A `draw_calls` sub-type which inherits the `obscure::vulkan::draw_call_base` type.
   - This type should implement any draw calls that should be handled by the pipeline
   - The `obscure::vulkan::draw_call_base` provides an interface to query the graphics context for various handles.
   - All public functions are exposed to the user of the graphics context through the frame context when they call `begin_frame` on the graphics context.

See examples of pipelines in the `./builtin` directory.

### Shader keys
Shader keys is any compile time constant which has a specialization of the template
```c++
template<auto Key>
struct shader_loader
{
    static obscure::vulkan::shader_data auto load_shader() = delete;
};
```
Where the `load_shader` function has an implementation which returns any object that is convertable to
a `std::span<const uint32_t>` (Also expressed through the `obscure::vulkan::shader_data` concept).

The `load_shader` function is only ever run at runtime, therefore even if the key has to be a compile time constant,
the shader could be loaded dynamically.

But because the key has to be a compile time constant the shader could also be embedded at compile time.


# TODO
- [ ] Make uniform buffers useful for lighting. 
  - At the moment each uniform buffer binds only to a single pipeline, making it harder to use truly global data such as lighting information in uniforms.
- [ ] Properly implement window resizing
