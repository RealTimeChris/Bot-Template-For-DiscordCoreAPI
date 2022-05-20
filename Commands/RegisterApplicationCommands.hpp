// RegisterApplicationCommands.hpp - Registers the slash commands of this bot.
// May 27, 2021
// Chris M.
// https://github.com/RealTimeChris

#pragma once

#include "HelperFunctions.hpp"

namespace DiscordCoreAPI {

	class RegisterApplicationCommands : public BaseFunction {
	  public:
		RegisterApplicationCommands() {
			this->commandName = "registerapplicationcommands";
			this->helpDescription = "Register some application commands.";
			EmbedData msgEmbed;
			msgEmbed.setDescription("------\nSimply enter /RegisterApplicationCommandsn------");
			msgEmbed.setTitle("__**Register Slash Commands Usage:**__");
			msgEmbed.setTimeStamp(getTimeAndDate());
			msgEmbed.setColor("FeFeFe");
			this->helpEmbed = msgEmbed;
		}

		std::unique_ptr<BaseFunction> create() {
			return std::make_unique<RegisterApplicationCommands>();
		}

		virtual void execute(BaseFunctionArguments& argsNew) {
			try {
				Channel channel = Channels::getCachedChannelAsync({ .channelId = argsNew.eventData.getChannelId() }).get();

				InputEvents::deleteInputEventResponseAsync(argsNew.eventData).get();

				RespondToInputEventData dataPackage(argsNew.eventData);
				dataPackage.setResponseType(InputEventResponseType::Deferred_Response);
				auto newEvent = InputEvents::respondToInputEventAsync(dataPackage).get();

				CreateGlobalApplicationCommandData RegisterApplicationCommandsCommandData{};
				RegisterApplicationCommandsCommandData.dmPermission = true;
				RegisterApplicationCommandsCommandData.applicationId = argsNew.discordCoreClient->getBotUser().id;
				RegisterApplicationCommandsCommandData.type = ApplicationCommandType::Chat_Input;
				RegisterApplicationCommandsCommandData.dmPermission = true;
				RegisterApplicationCommandsCommandData.description = "Register the programmatically designated slash commands.";
				RegisterApplicationCommandsCommandData.name = "registerapplicationcommands";
				ApplicationCommands::createGlobalApplicationCommandAsync(RegisterApplicationCommandsCommandData);

				CreateGlobalApplicationCommandData createTestData{};
				createTestData.dmPermission = true;
				createTestData.applicationId = argsNew.discordCoreClient->getBotUser().id;
				createTestData.type = ApplicationCommandType::Chat_Input;
				createTestData.name = "test";
				createTestData.dmPermission = true;
				createTestData.description = "Test command.";
				ApplicationCommands::createGlobalApplicationCommandAsync(createTestData).get();

				EmbedData msgEmbed{};
				msgEmbed.setAuthor(argsNew.eventData.getUserName(), argsNew.eventData.getAvatarUrl());
				msgEmbed.setColor("fefefe");
				msgEmbed.setDescription("------\nNicely done, you've registered some commands!\n------");
				msgEmbed.setTimeStamp(getTimeAndDate());
				msgEmbed.setTitle("__**Register Application Commands Complete:**__");
				RespondToInputEventData responseData(newEvent);
				responseData.setResponseType(InputEventResponseType::Edit_Interaction_Response);
				responseData.addMessageEmbed(msgEmbed);
				auto event = InputEvents::respondToInputEventAsync(responseData).get();
				return;
			} catch (...) {
				reportException("RegisterApplicationCommands::execute()");
			}
		}
		virtual ~RegisterApplicationCommands(){};
	};
}// namespace DiscordCoreAPI
