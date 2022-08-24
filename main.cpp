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
	thePtr->registerFunction(std::vector<std::string>{ "play" }, std::make_unique<DiscordCoreAPI::Play>());
	thePtr->registerFunction(std::vector<std::string>{ "botinfo" }, std::make_unique<DiscordCoreAPI::BotInfo>());
	thePtr->registerFunction(std::vector<std::string>{ "help" }, std::make_unique<DiscordCoreAPI::Help>());
	thePtr->runBot();
	return 0;
};
