#ifndef OBSCURE_APPLICATION_2D_DEFINITION
#define OBSCURE_APPLICATION_2D_DEFINITION 1

#include <atomic>
#include <span>
#include "obscure/vulkan/vulkan_context.h"
#include "obscure/utilities/stopwatch.h"
#include "obscure/configuration/configuration_container.hpp"

namespace obscure
{
	struct application_2d //: canvas
	{
	protected:
		std::atomic_bool do_exit = false;
		vulkan::vulkan_context context;

	public:
		application_2d(obscure::configuration::configuration_container auto config)
			: context(config.get())
		{}

		virtual void loop(stopwatch::seconds elapsed_time) = 0;
		void run();

		void exit() noexcept;

		virtual ~application_2d() = default;
	};
}
#endif