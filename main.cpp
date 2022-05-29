// main.cpp - Main entry point.
// Jun 17, 2021
// Chris M.
// https://github.com/RealTimeChris

#include "Commands/CommandsList.hpp"

void onBoot01(DiscordCoreAPI::DiscordCoreClient* args) {
	std::vector<DiscordCoreAPI::ActivityData> activities;
	DiscordCoreAPI::ActivityData activity;
	activity.name = "/help for my commands!";
	activity.type = DiscordCoreAPI::ActivityType::Game;
	activities.push_back(activity);
	DiscordCoreAPI::UpdatePresenceData dataPackage{};
	dataPackage.activities = activities;
	dataPackage.status = "online";
	dataPackage.afk = false;
	args->getBotUser().updatePresence(dataPackage);
}

int32_t main() {
	std::string botToken = "YOUR_BOT_TOKEN_HERE";
	std::vector<DiscordCoreAPI::RepeatedFunctionData> functionVector{};
	DiscordCoreAPI::RepeatedFunctionData function01{};
	function01.function = std::ref(onBoot00);
	function01.intervalInMs = 50;
	function01.repeated = false;
	functionVector.push_back(function01);
	DiscordCoreAPI::DiscordCoreClientConfig clientOptions{};
	clientOptions.textFormat = DiscordCoreAPI::TextFormat::Etf;
	clientOptions.botToken = botToken;
	clientOptions.functionsToExecute = functionVector;
	auto thePtr = std::make_unique<DiscordCoreAPI::DiscordCoreClient>(clientOptions);
	thePtr->runBot();
	return 0;
}
