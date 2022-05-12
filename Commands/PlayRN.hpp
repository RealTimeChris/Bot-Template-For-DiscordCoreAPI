// PlayRN.hpp - Header for the "play right now" command.
// Sep 9, 2021
// Chris M.
// https://github.com/RealTimeChris

#pragma once

#include "HelperFunctions.hpp"

namespace DiscordCoreAPI {

	class PlayRN : public BaseFunction {
	  public:
		PlayRN() {
			this->commandName = "playrn";
			this->helpDescription = "Add a song to the front of the queue and play it "
									"if nothing is playing.";
			EmbedData msgEmbed;
			msgEmbed.setDescription("------\nSimply enter /playrn SONGNAME OR Url.\n------");
			msgEmbed.setTitle("__**PlayRN Usage:**__");
			msgEmbed.setTimeStamp(getTimeAndDate());
			msgEmbed.setColor("FeFeFe");
			this->helpEmbed = msgEmbed;
		}

		std::unique_ptr<BaseFunction> create() {
			return std::make_unique<PlayRN>();
		}

		virtual void execute(BaseFunctionArguments& argsNew) {
			try {
				Channel channel = Channels::getCachedChannelAsync({ argsNew.eventData.getChannelId() }).get();

				InputEvents::deleteInputEventResponseAsync(argsNew.eventData).get();

				Guild guild = Guilds::getCachedGuildAsync({ argsNew.eventData.getGuildId() }).get();

				GuildMember guildMember =
					GuildMembers::getCachedGuildMemberAsync({ .guildMemberId = argsNew.eventData.getAuthorId(), .guildId = argsNew.eventData.getGuildId() }).get();

				InputEventData newEvent = argsNew.eventData;

				VoiceConnection* voiceConnection = guild.connectToVoice(guildMember.voiceData.channelId, true, false);

				if (voiceConnection == nullptr) {
					EmbedData newEmbed;
					newEmbed.setAuthor(argsNew.eventData.getUserName(), argsNew.eventData.getAvatarUrl());
					newEmbed.setDescription("------\n__**Sorry, but there is no voice connection that is "
											"currently held by me!**__\n------");
					newEmbed.setTimeStamp(getTimeAndDate());
					newEmbed.setTitle("__**Connection Issue:**__");
					newEmbed.setColor("fefefe");
					RespondToInputEventData dataPackage(newEvent);
					dataPackage.setResponseType(InputEventResponseType::Ephemeral_Interaction_Response);
					dataPackage.addMessageEmbed(newEmbed);
					newEvent = InputEvents::respondToInputEventAsync(dataPackage).get();
					;
					return;
				}

				if (guildMember.voiceData.channelId == "" || guildMember.voiceData.channelId != voiceConnection->getChannelId()) {
					EmbedData newEmbed;
					newEmbed.setAuthor(argsNew.eventData.getUserName(), argsNew.eventData.getAvatarUrl());
					newEmbed.setDescription("------\n__**Sorry, but you need to be in a correct voice channel "
											"to issue those commands!**__\n------");
					newEmbed.setTimeStamp(getTimeAndDate());
					newEmbed.setTitle("__**Playing Issue:**__");
					newEmbed.setColor("fefefe");
					RespondToInputEventData dataPackage(newEvent);
					dataPackage.setResponseType(InputEventResponseType::Ephemeral_Interaction_Response);
					dataPackage.addMessageEmbed(newEmbed);
					newEvent = InputEvents::respondToInputEventAsync(dataPackage).get();
					return;
				}

				if (argsNew.commandData.optionsArgs.size() == 0 && !SongAPI::isThereAnySongs(guild.id)) {
					EmbedData newEmbed;
					newEmbed.setAuthor(argsNew.eventData.getUserName(), argsNew.eventData.getAvatarUrl());
					newEmbed.setDescription("------\n__**Sorry, but there's nothing to play!**__\n------");
					newEmbed.setTimeStamp(getTimeAndDate());
					newEmbed.setTitle("__**Playing Issue:**__");
					newEmbed.setColor("fefefe");
					RespondToInputEventData dataPackage(newEvent);
					dataPackage.setResponseType(InputEventResponseType::Ephemeral_Interaction_Response);
					dataPackage.addMessageEmbed(newEmbed);
					newEvent = InputEvents::respondToInputEventAsync(dataPackage).get();
					return;
				} else if (argsNew.commandData.optionsArgs.size() == 0 && SongAPI::areWeCurrentlyPlaying(guild.id)) {
					EmbedData newEmbed;
					newEmbed.setAuthor(argsNew.eventData.getUserName(), argsNew.eventData.getAvatarUrl());
					newEmbed.setDescription("------\n__**Sorry, but there's already "
											"something playing!**__\n------");
					newEmbed.setTimeStamp(getTimeAndDate());
					newEmbed.setTitle("__**Playing Issue:**__");
					newEmbed.setColor("fefefe");
					RespondToInputEventData dataPackage(newEvent);
					dataPackage.setResponseType(InputEventResponseType::Ephemeral_Interaction_Response);
					dataPackage.addMessageEmbed(newEmbed);
					newEvent = InputEvents::respondToInputEventAsync(dataPackage).get();
					return;
				}

				if (argsNew.eventData.eventType == InteractionType::Application_Command && argsNew.commandData.optionsArgs.size() != 0) {
					RespondToInputEventData dataPackage(argsNew.eventData);
					dataPackage.setResponseType(InputEventResponseType::Deferred_Response);
					newEvent = InputEvents::respondToInputEventAsync(dataPackage).get();
				}

				std::vector<Song> searchResults{};
				if (argsNew.commandData.optionsArgs.size() > 0) {
					searchResults = SongAPI::searchForSong(argsNew.commandData.optionsArgs[0], guild.id);
				}

				std::vector<EmbedData> embedsFromSearch;
				uint32_t x = 0;
				for (Song& value: searchResults) {
					x += 1;
					EmbedData newEmbed;
					newEmbed.setAuthor(argsNew.eventData.getUserName(), argsNew.eventData.getAvatarUrl());
					newEmbed.setDescription("__**Title:**__ [" + value.songTitle + "](" + value.viewUrl + ")" + "\n__**Description:**__ " + value.description +
						"\n__**Duration:**__ " + value.duration);
					newEmbed.setTimeStamp(getTimeAndDate());
					newEmbed.setTitle("__**" + std::to_string(x) + " of " + std::to_string(searchResults.size()) + "**__");
					newEmbed.setImage(value.thumbnailUrl);
					newEmbed.setColor("fefefe");
					embedsFromSearch.push_back(newEmbed);
				}

				if (embedsFromSearch.size() > 0) {
					SongAPI::stop(guild.id);
					Song song = SongAPI::addSongToQueue(guildMember, searchResults[0]);
					auto playlist = SongAPI::getPlaylist(guild.id);

					std::vector<Song> songVector{};
					playlist.songQueue.erase(playlist.songQueue.end() - 1);
					songVector.push_back(song);
					for (auto& value: playlist.songQueue) {
						songVector.push_back(value);
					}
					playlist.songQueue = songVector;
					playlist.currentSong = Song();
					SongAPI::setPlaylist(playlist, guild.id);
				}
				BaseFunctionArguments newArgs = argsNew;
				InputEventData newerEvent = newEvent;
				std::string channelId = newArgs.eventData.getChannelId();
				if (!SongAPI::areWeCurrentlyPlaying(guild.id)) {
					std::function<CoRoutine<void>(SongCompletionEventData)> theTask = [=](SongCompletionEventData eventData) noexcept -> CoRoutine<void> {
						co_await NewThreadAwaitable<void>();
						InputEventData newestEvent = newerEvent;
						if (SongAPI::isThereAnySongs(guild.id)) {
							EmbedData newEmbed{};
							if (!eventData.wasItAFail) {
								if (!SongAPI::sendNextSong(guildMember)) {
									InputEvents::deleteInputEventResponseAsync(std::move(newestEvent));
									SongAPI::play(guildMember.guildId);
									co_return;
								}
								newEmbed.setAuthor(eventData.guildMember.user.userName, eventData.guildMember.user.avatar);
								newEmbed.setDescription("__**Title:**__ [" + SongAPI::getCurrentSong(eventData.guild.id).songTitle + "](" +
									SongAPI::getCurrentSong(eventData.guild.id).viewUrl + ")" + "\n__**Description:**__ " +
									SongAPI::getCurrentSong(eventData.guild.id).description + "\n__**Duration:**__ " + SongAPI::getCurrentSong(eventData.guild.id).duration +
									"\n__**Added By:**__ <@!" + SongAPI::getCurrentSong(eventData.guild.id).addedByUserId + "> (" +
									SongAPI::getCurrentSong(eventData.guild.id).addedByUserName + ")");
								newEmbed.setImage(SongAPI::getCurrentSong(eventData.guild.id).thumbnailUrl);
								newEmbed.setTimeStamp(getTimeAndDate());
								newEmbed.setTitle("__**Now Playing:**__");
								newEmbed.setColor("fefefe");
								if (SongAPI::isLoopAllEnabled(eventData.guild.id) && SongAPI::isLoopSongEnabled(eventData.guild.id)) {
									newEmbed.setFooter("✅ Loop-All, ✅ Loop-Song");
								}
								if (!SongAPI::isLoopAllEnabled(eventData.guild.id) && SongAPI::isLoopSongEnabled(eventData.guild.id)) {
									newEmbed.setFooter("❌ Loop-All, ✅ Loop-Song");
								}
								if (SongAPI::isLoopAllEnabled(eventData.guild.id) && !SongAPI::isLoopSongEnabled(eventData.guild.id)) {
									newEmbed.setFooter("✅ Loop-All, ❌ Loop-Song");
								}
								if (!SongAPI::isLoopAllEnabled(eventData.guild.id) && !SongAPI::isLoopSongEnabled(eventData.guild.id)) {
									newEmbed.setFooter("❌ Loop-All, ❌ Loop-Song");
								}
								CreateMessageData dataPackage02(channelId);
								dataPackage02.addMessageEmbed(newEmbed);
								Messages::createMessageAsync(dataPackage02).get();
							} else {
								SongAPI::sendNextSong(eventData.guildMember);
								newEmbed.setAuthor(eventData.guildMember.user.userName, eventData.guildMember.user.avatar);
								newEmbed.setDescription("__**It appears as though there was an error when trying to "
														"play the following track!**__\n__**Title:**__ [" +
									eventData.previousSong.songTitle + "](" + eventData.previousSong.viewUrl + ")" + "\n__**Description:**__ " +
									eventData.previousSong.description + "\n__**Duration:**__ " + eventData.previousSong.duration + "\n__**Added By:**__ <@!" +
									eventData.previousSong.addedByUserId + "> (" + eventData.previousSong.addedByUserName + ")");
								newEmbed.setImage(eventData.previousSong.thumbnailUrl);
								newEmbed.setTimeStamp(getTimeAndDate());
								newEmbed.setTitle("__**Playing Error:**__");
								newEmbed.setColor("fe0000");
								if (SongAPI::isLoopAllEnabled(eventData.guild.id) && SongAPI::isLoopSongEnabled(eventData.guild.id)) {
									newEmbed.setFooter("✅ Loop-All, ✅ Loop-Song");
								}
								if (!SongAPI::isLoopAllEnabled(eventData.guild.id) && SongAPI::isLoopSongEnabled(eventData.guild.id)) {
									newEmbed.setFooter("❌ Loop-All, ✅ Loop-Song");
								}
								if (SongAPI::isLoopAllEnabled(eventData.guild.id) && !SongAPI::isLoopSongEnabled(eventData.guild.id)) {
									newEmbed.setFooter("✅ Loop-All, ❌ Loop-Song");
								}
								if (!SongAPI::isLoopAllEnabled(eventData.guild.id) && !SongAPI::isLoopSongEnabled(eventData.guild.id)) {
									newEmbed.setFooter("❌ Loop-All, ❌ Loop-Song");
								}
								CreateMessageData dataPackage02(channelId);
								dataPackage02.addMessageEmbed(newEmbed);
								Messages::createMessageAsync(dataPackage02).get();

								if (!SongAPI::areWeCurrentlyPlaying(eventData.guild.id)) {
									newEmbed.setAuthor(eventData.guildMember.user.userName, eventData.guildMember.user.avatar);
									newEmbed.setDescription("__**Title:**__ [" + SongAPI::getCurrentSong(eventData.guild.id).songTitle + "](" +
										SongAPI::getCurrentSong(eventData.guild.id).viewUrl + ")" + "\n__**Description:**__ " +
										SongAPI::getCurrentSong(eventData.guild.id).description + "\n__**Duration:**__ " + SongAPI::getCurrentSong(eventData.guild.id).duration +
										"\n__**Added By:**__ <@!" + SongAPI::getCurrentSong(eventData.guild.id).addedByUserId + "> (" +
										SongAPI::getCurrentSong(eventData.guild.id).addedByUserName + ")");
									newEmbed.setImage(SongAPI::getCurrentSong(eventData.guild.id).thumbnailUrl);
									newEmbed.setTimeStamp(getTimeAndDate());
									newEmbed.setTitle("__**Now Playing:**__");
									newEmbed.setColor("fefefe");
									if (SongAPI::isLoopAllEnabled(eventData.guild.id) && SongAPI::isLoopSongEnabled(eventData.guild.id)) {
										newEmbed.setFooter("✅ Loop-All, ✅ Loop-Song");
									}
									if (!SongAPI::isLoopAllEnabled(eventData.guild.id) && SongAPI::isLoopSongEnabled(eventData.guild.id)) {
										newEmbed.setFooter("❌ Loop-All, ✅ Loop-Song");
									}
									if (SongAPI::isLoopAllEnabled(eventData.guild.id) && !SongAPI::isLoopSongEnabled(eventData.guild.id)) {
										newEmbed.setFooter("✅ Loop-All, ❌ Loop-Song");
									}
									if (!SongAPI::isLoopAllEnabled(eventData.guild.id) && !SongAPI::isLoopSongEnabled(eventData.guild.id)) {
										newEmbed.setFooter("❌ Loop-All, ❌ Loop-Song");
									}
									CreateMessageData dataPackage03(channelId);
									dataPackage03.addMessageEmbed(newEmbed);
									Messages::createMessageAsync(dataPackage03).get();
								}
							}
							SongAPI::play(eventData.guild.id);
						} else {
							EmbedData newEmbed;
							newEmbed.setAuthor(eventData.guildMember.user.userName, eventData.guildMember.user.avatar);
							newEmbed.setDescription("------\n__**Sorry, but there's nothing "
													"left to play here!**__\n------");
							newEmbed.setTimeStamp(getTimeAndDate());
							newEmbed.setTitle("__**Now Playing:**__");
							newEmbed.setColor("fefefe");
							if (SongAPI::isLoopAllEnabled(eventData.guild.id) && SongAPI::isLoopSongEnabled(eventData.guild.id)) {
								newEmbed.setFooter("✅ Loop-All, ✅ Loop-Song");
							} else if (!SongAPI::isLoopAllEnabled(eventData.guild.id) && SongAPI::isLoopSongEnabled(eventData.guild.id)) {
								newEmbed.setFooter("❌ Loop-All, ✅ Loop-Song");
							} else if (SongAPI::isLoopAllEnabled(eventData.guild.id) && !SongAPI::isLoopSongEnabled(eventData.guild.id)) {
								newEmbed.setFooter("✅ Loop-All, ❌ Loop-Song");
							} else if (!SongAPI::isLoopAllEnabled(eventData.guild.id) && !SongAPI::isLoopSongEnabled(eventData.guild.id)) {
								newEmbed.setFooter("❌ Loop-All, ❌ Loop-Song");
							}
							CreateMessageData dataPackage02(channelId);
							dataPackage02.addMessageEmbed(newEmbed);
							Messages::createMessageAsync(dataPackage02).get();
						}
					};
					SongAPI::onSongCompletion(theTask, guild.id);
				}
				if (SongAPI::isThereAnySongs(guild.id)) {
					if (!SongAPI::sendNextSong(guildMember)) {
						SongAPI::play(guild.id);
						InputEvents::deleteInputEventResponseAsync(std::move(newEvent));
						return;
					}

					EmbedData newEmbed;
					newEmbed.setAuthor(argsNew.eventData.getUserName(), argsNew.eventData.getAvatarUrl());
					newEmbed.setDescription("__**Title:**__ [" + SongAPI::getCurrentSong(guild.id).songTitle + "](" + SongAPI::getCurrentSong(guild.id).viewUrl + ")" +
						"\n__**Description:**__ " + SongAPI::getCurrentSong(guild.id).description + "\n__**Duration:**__ " + SongAPI::getCurrentSong(guild.id).duration +
						"\n__**Added By:**__ <@!" + SongAPI::getCurrentSong(guild.id).addedByUserId + "> (" + SongAPI::getCurrentSong(guild.id).addedByUserName + ")");
					newEmbed.setImage(SongAPI::getCurrentSong(guild.id).thumbnailUrl);
					newEmbed.setTimeStamp(getTimeAndDate());
					newEmbed.setTitle("__**Now Playing:**__");
					newEmbed.setColor("fefefe");
					if (SongAPI::isLoopAllEnabled(guild.id) && SongAPI::isLoopSongEnabled(guild.id)) {
						newEmbed.setFooter("✅ Loop-All, ✅ Loop-Song");
					} else if (!SongAPI::isLoopAllEnabled(guild.id) && SongAPI::isLoopSongEnabled(guild.id)) {
						newEmbed.setFooter("❌ Loop-All, ✅ Loop-Song");
					} else if (SongAPI::isLoopAllEnabled(guild.id) && !SongAPI::isLoopSongEnabled(guild.id)) {
						newEmbed.setFooter("✅ Loop-All, ❌ Loop-Song");
					} else if (!SongAPI::isLoopAllEnabled(guild.id) && !SongAPI::isLoopSongEnabled(guild.id)) {
						newEmbed.setFooter("❌ Loop-All, ❌ Loop-Song");
					}
					RespondToInputEventData dataPackage(newEvent);
					dataPackage.setResponseType(InputEventResponseType::Edit_Interaction_Response);
					dataPackage.addMessageEmbed(newEmbed);
					newEvent = InputEvents::respondToInputEventAsync(dataPackage).get();
					SongAPI::play(guild.id);
				}
				return;
			} catch (...) {
				reportException("PlayRN::execute()");
			}
		};
		virtual ~PlayRN(){};
	};

}// namespace DiscordCoreAPI