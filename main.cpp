// main.cpp - Main entry point.
//// Jun 17, 2021
// Chris M.
// https://github.com/RealTimeChris

#include "Commands/CommandsList.hpp"

int32_t main() {
	DiscordCoreAPI::ShardingOptions shardOptions{};
	shardOptions.numberOfShardsForThisProcess = 1;
	shardOptions.startingShard = 0;
	shardOptions.totalNumberOfShards = 1;
	DiscordCoreAPI::LoggingOptions logOptions{};
	logOptions.logFFMPEGErrorMessages = true;
	logOptions.logGeneralErrorMessages = true;
	logOptions.logHttpsErrorMessages = true;
	logOptions.logHttpsSuccessMessages = true;
	logOptions.logWebSocketErrorMessages = true;
	logOptions.logWebSocketSuccessMessages = true;
	DiscordCoreAPI::DiscordCoreClientConfig clientConfig{};
	clientConfig.botToken = "YOUR_BOT_TOKEN_HERE";
	clientConfig.logOptions = logOptions;
	clientConfig.shardOptions = shardOptions;
	std::vector<DiscordCoreAPI::ActivityData> activities{};
	DiscordCoreAPI::ActivityData activity{};
	activity.name = "/help for my commands!";
	activity.type = DiscordCoreAPI::ActivityType::Game;
	activities.push_back(activity);
	clientConfig.connectionAddress = "127.0.0.1";
	clientConfig.presenceData.activities = activities;
	clientConfig.presenceData.afk = false;
	clientConfig.presenceData.since = 0;
	clientConfig.presenceData.status = "online";
	auto thePtr = std::make_unique<DiscordCoreAPI::DiscordCoreClient>(clientConfig);
	DiscordCoreAPI::CreateGlobalApplicationCommandData createBotInfoCommandData{};
	createBotInfoCommandData.dmPermission = true;
	createBotInfoCommandData.applicationId = thePtr->getBotUser().id;
	createBotInfoCommandData.type = DiscordCoreAPI::ApplicationCommandType::Chat_Input;
	createBotInfoCommandData.defaultMemberPermissions = DiscordCoreAPI::Permission::Use_Application_Commands;
	createBotInfoCommandData.description = "Displays info about the current bot.";
	createBotInfoCommandData.name = "botinfo";
	DiscordCoreAPI::CreateGlobalApplicationCommandData playCommandData{};
	playCommandData.defaultMemberPermissions = DiscordCoreAPI::Permission::Use_Application_Commands;
	playCommandData.dmPermission = false;
	playCommandData.applicationId = thePtr->getBotUser().id;
	playCommandData.type = DiscordCoreAPI::ApplicationCommandType::Chat_Input;
	playCommandData.description = "Search for and play a song.";
	playCommandData.name = "play";
	DiscordCoreAPI::ApplicationCommandOptionData playCommandDataOptionOne;
	DiscordCoreAPI::CreateGlobalApplicationCommandData createHelpData{};
	createHelpData.dmPermission = true;
	createHelpData.defaultMemberPermissions = DiscordCoreAPI::Permission::Use_Application_Commands;
	createHelpData.applicationId = thePtr->getBotUser().id;
	createHelpData.type = DiscordCoreAPI::ApplicationCommandType::Chat_Input;
	createHelpData.name = "help";
	createHelpData.description = "A help command for this bot.";
	playCommandDataOptionOne.name = "songname";
	playCommandDataOptionOne.type = DiscordCoreAPI::ApplicationCommandOptionType::String;
	playCommandDataOptionOne.description = "The name of the song that you would like to search.";
	playCommandDataOptionOne.required = false;
	playCommandData.options.push_back(playCommandDataOptionOne);
	thePtr->registerFunction(std::vector<std::string>{ "play" }, std::make_unique<DiscordCoreAPI::Play>(), playCommandData);
	thePtr->registerFunction(std::vector<std::string>{ "botinfo" }, std::make_unique<DiscordCoreAPI::BotInfo>(), createBotInfoCommandData);
	thePtr->registerFunction(std::vector<std::string>{ "help" }, std::make_unique<DiscordCoreAPI::Help>(), createHelpData);
	thePtr->runBot();
	return 0;
};
