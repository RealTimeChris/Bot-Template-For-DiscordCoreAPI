/// Test.hpp - Header for the "test" command.
/// https://discordcoreapi.com

#pragma once

#include <discordcoreapi/Index.hpp>

namespace DiscordCoreAPI {

	class Test : public BaseFunction {
	  public:
		Test() {
			this->commandName = "test";
			this->helpDescription = "Testing purposes!";
			EmbedData msgEmbed{};
			msgEmbed.setDescription("------\nSimply enter !test or /test!\n------");
			msgEmbed.setTitle("__**Test Usage:**__");
			msgEmbed.setTimeStamp(getTimeAndDate());
			msgEmbed.setColor("FeFeFe");
			this->helpEmbed = msgEmbed;
		}

		std::unique_ptr<BaseFunction> create() {
			return std::make_unique<Test>();
		}

		virtual void execute(BaseFunctionArguments& argsNew) {
			RespondToInputEventData dataPackage{ argsNew.eventData };
			dataPackage.setResponseType(InputEventResponseType::Interaction_Response);
			dataPackage.addContent("This is a test response!");
			InputEvents::respondToInputEventAsync(dataPackage).get();

			return;
		}
		virtual ~Test() = default;
	};
}// namespace DiscordCoreAPI
