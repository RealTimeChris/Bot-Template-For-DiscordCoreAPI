// main.cpp - Main entry point.
//// Jun 17, 2021
// Chris M.
// https://github.com/RealTimeChris

#include "Commands/BotInfo.hpp"

int32_t main() {
	std::string botToken = "";
	discord_core_api::sharding_options shardOptions{};
	shardOptions.numberOfShardsForThisProcess = 1;
	shardOptions.startingShard = 0;
	shardOptions.totalNumberOfShards = 1;
	discord_core_api::logging_options logOptions{};
	logOptions.logGeneralErrorMessages = true;
	logOptions.logHttpsErrorMessages = true;
	logOptions.logWebSocketErrorMessages = true;
	logOptions.logHttpsSuccessMessages = true;
	logOptions.logGeneralSuccessMessages = true;
	logOptions.logWebSocketSuccessMessages = true;
	discord_core_api::discord_core_client_config clientConfig{};
	//clientConfig.connectionAddress = "127.0.0.1";
	clientConfig.botToken = botToken;
	clientConfig.logOptions = logOptions;
	clientConfig.shardOptions = shardOptions;
	clientConfig.cacheOptions.cacheChannels = true;
	clientConfig.cacheOptions.cacheGuilds = true;
	clientConfig.cacheOptions.cacheUsers = true;
	clientConfig.cacheOptions.cacheRoles = true;
	jsonifier::vector<discord_core_api::activity_data> activities{};
	discord_core_api::activity_data activity{};
	activity.name = "/help for my commands!";
	activity.type = discord_core_api::activity_type::game;
	activities.emplace_back(activity);
	clientConfig.presenceData.activities = activities;
	clientConfig.presenceData.afk = false;
	clientConfig.presenceData.status = discord_core_api::presence_update_state::online;
	clientConfig.textFormat = discord_core_api::text_format::etf;
	clientConfig.presenceData.since = 0;
	auto the = makeUnique<discord_core_api::discord_core_client>(clientConfig);
	discord_core_api::create_global_application_command_data createBotInfoCommandData{};
	createBotInfoCommandData.dmPermission = true;
	createBotInfoCommandData.type = discord_core_api::application_command_type::Chat_Input;
	createBotInfoCommandData.defaultMemberPermissions = discord_core_api::permission::Use_Application_Commands;
	createBotInfoCommandData.description = "Displays info about the current bot.";
	createBotInfoCommandData.name = "botinfo";
	the->registerFunction(jsonifier::vector<jsonifier::string>{ "botinfo" }, discord_core_api::makeUnique<discord_core_api::bot_info>(),
		createBotInfoCommandData);
	the->runBot();
	return 0;
};
