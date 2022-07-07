// main.cpp - Main entry point.
//// Jun 17, 2021
// Chris M.
// https://github.com/RealTimeChris

#include "Commands/CommandsList.hpp"

void onBoot(DiscordCoreAPI::DiscordCoreClient* args) {
	std::vector<DiscordCoreAPI::ActivityData> activities;
	DiscordCoreAPI::ActivityData activity;
	activity.name = "/help for my commands!";
	activity.type = DiscordCoreAPI::ActivityType::Game;
	activities.push_back(activity);
	auto botUser = args->getBotUser();
	DiscordCoreAPI::UpdatePresenceData dataPackage{ .activities = activities, .status = "online", .afk = false };
	botUser.updatePresence(dataPackage);
}

int32_t main() {
	std::string botToken = "YOUR_BOT_TOKEN_HERE";
	std::vector<DiscordCoreAPI::RepeatedFunctionData> functionVector{};
	DiscordCoreAPI::RepeatedFunctionData function{};
	function.function = onBoot;
	function.intervalInMs = 200;
	function.repeated = false;
	functionVector.push_back(function);
	DiscordCoreAPI::ShardingOptions shardOptions{};
	shardOptions.numberOfShardsForThisProcess = 1;
	shardOptions.startingShard = 0;
	shardOptions.totalNumberOfShards = 1;
	DiscordCoreAPI::LoggingOptions logOptions{};
	logOptions.logFFMPEGErrorMessages = true;
	logOptions.logGeneralErrorMessages = true;
	logOptions.logHttpsErrorMessages = true;
	logOptions.logWebSocketErrorMessages = true;
	DiscordCoreAPI::DiscordCoreClientConfig clientConfig{};
	clientConfig.botToken = botToken;
	clientConfig.logOptions = logOptions;
	clientConfig.shardOptions = shardOptions;
	clientConfig.functionsToExecute = functionVector;
	auto thePtr = std::make_unique<DiscordCoreAPI::DiscordCoreClient>(clientConfig);
	thePtr->registerFunction(std::vector<std::string>{ "play" }, std::make_unique<DiscordCoreAPI::Play>());
	thePtr->runBot();
	return 0;
};
