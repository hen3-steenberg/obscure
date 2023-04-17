#ifndef OBSCURE_VULKAN_COMMAND_SEQUENCE_DEFINITION
#define OBSCURE_VULKAN_COMMAND_SEQUENCE_DEFINITION 1

#include "obscure/vulkan/render_context.h"
#include "obscure/vulkan/transfer_context.h"
#include "obscure/vulkan/fence.h"
#include "obscure/vulkan/semaphore.h"

namespace obscure
{
	namespace vulkan
	{
		struct command_sequence
		{
		protected:
			application_context * context;
			uint32_t current_buffer_index;
			
		public:
			command_sequence(application_context * ctx);
			transfer_context begin_transfers();
			render_context begin_rendering();
			void submit_commands();
			void display();
		};
	}
}
#endif