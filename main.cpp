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
	args->getBotUser().updatePresence({ .activities = activities, .status = "online", .afk = false });
}

int32_t main() {
	std::string botToken = "OTU4NDg0MTkxMzcxMjEwODEy.YkN_9g.i6L9-s_b-i6hyktOS_a1HujWmyQ";
	std::vector<DiscordCoreAPI::RepeatedFunctionData> functionVector{};
	DiscordCoreAPI::RepeatedFunctionData function01{};
	function01.function = onBoot01;
	function01.intervalInMs = 1000;
	function01.repeated = false;
	functionVector.push_back(function01);
	auto thePtr = std::make_unique<DiscordCoreAPI::DiscordCoreClient>(botToken, functionVector);
	thePtr->registerFunction(std::vector<std::string>{ "registerapplicationcommands" }, std::make_unique<DiscordCoreAPI::RegisterApplicationCommands>());
	thePtr->registerFunction(std::vector<std::string>{ "test" }, std::make_unique<DiscordCoreAPI::Test>());
	thePtr->runBot();
	return 0;
}
