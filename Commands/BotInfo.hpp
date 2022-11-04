// BotInfo.hpp - Header for the "bot info" command.
// Jun 19, 2021
// Chris M.
// https://github.com/RealTimeChris

#pragma once

#include "../HelperFunctions.hpp"

namespace DiscordCoreAPI {
	class BotInfo : public BaseFunction {
	  public:
		BotInfo() {
			this->commandName = "botinfo";
			this->helpDescription = "Displays some info about this bot.";
			EmbedData msgEmbed;
			msgEmbed.setDescription("------\nEnter /botinfo.\n------");
			msgEmbed.setTitle("__**Bot Info Usage:**__");
			msgEmbed.setTimeStamp(getTimeAndDate());
			msgEmbed.setColor("FeFeFe");
			this->helpEmbed = msgEmbed;
		}

		std::unique_ptr<BaseFunction> create() {
			return std::make_unique<BotInfo>();
		}

		void execute(BaseFunctionArguments& argsNew) {
			try {
				std::cout << "WERE HERE THIS IS IT!" << std::endl;
				auto guilds = Guilds::getAllGuildsAsync().get();
				int32_t userCount{ 0 };
				for (auto& value: guilds) {
					userCount += value.memberCount;
				}

				EmbedData messageEmbed;
				messageEmbed.setAuthor(argsNew.eventData.getUserName(), argsNew.eventData.getAvatarUrl());
				messageEmbed.setImage(argsNew.discordCoreClient->getBotUser().getAvatarUrl());
				messageEmbed.setColor("FEFEFE");
				messageEmbed.setTitle("__**Bot Info:**__");
				messageEmbed.setTimeStamp(getTimeAndDate());
				messageEmbed.addField("__Bot Name:__",
					argsNew.discordCoreClient->getBotUser().userName + "#" + std::string{ argsNew.discordCoreClient->getBotUser().discriminator }, true);
				messageEmbed.addField("__Bot ID:__", std::to_string(argsNew.discordCoreClient->getBotUser().id), true);
				messageEmbed.addField("__Guild Count:__", std::to_string(guilds.size()), true);
				messageEmbed.addField("__Created At:__", argsNew.discordCoreClient->getBotUser().getCreatedAtTimestamp(TimeFormat::LongDateTime), true);
				messageEmbed.addField("__Serving Users:__", std::to_string(userCount), true);
				messageEmbed.addField("__Running On:__", "[DiscordCoreAPI Bot Library](https://discordcoreapi.com)", true);
				messageEmbed.addField("__Created By:__", "RealTime Chris#3627", true);
				RespondToInputEventData dataPackage(argsNew.eventData);
				dataPackage.setResponseType(InputEventResponseType::Interaction_Response);
				dataPackage.addMessageEmbed(messageEmbed);
				auto eventNew = InputEvents::respondToInputEventAsync(dataPackage).get();
				return;
			} catch (...) {
				reportException("BotInfo::execute()");
			}
		}
		~BotInfo(){};
	};
}
