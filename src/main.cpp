#include <string>
#include <boost/asio.hpp>
#include "cdltypes.hpp"
#include <sstream>

using namespace std;
using timer = std::chrono::system_clock;
timer::time_point startup_time;

// Commands

void help(CMessage, CChannel channel, CDL::cmdargs&) {
    channel->send_embed({
        {"title", "Help Commands for BotWatcher"},
        {"description", "**:gear:Tools** \n"
         "No commands found :/ \n \n"
         "**:speech_balloon:Misc** \n"
         "• `about` \n"
         "Some informations about BotWatcher \n \n"
         "• Whoops! No more commands found :/ \n \n"
         "**Offical DFB.eu Server » [Invite](https://discord.gg/42vDtZxZSt)**"},
        {"color", 0x7289D9},
        {"thumbnail", {{"url", extras::get_avatar_url(env.self)}}},
        {"footer", {{"text", "BotWatcher v0.4"}}},
    });
}

void about(CMessage, CChannel channel, CDL::cmdargs&) {
    ostringstream aboutdata;
    timer::duration uptime = timer::now() - startup_time;
    auto uptime_hours = uptime.count() / 1000000000 / 60 / 60;

    aboutdata << "BotWatcher - Watches bots when they go offline \n \n"
                 "**Memory Usage** \n"  << extras::get_mem::used() << " MB of " << extras::get_mem::total() << " MB \n \n"
                 "**Uptime** \n" << uptime_hours <<"h \n \n"
                 "Important information! \n"
                 "BotWatcher has some bugs at the moment. We will rewrite BotWatcher soon.";

    channel->send_embed({
        {"title", "Informations about BotWatcher"},
        {"description", aboutdata.str()},
        {"color", 0x7289D9},
    });
}

// Commands

int main(int argc, char **argv) {

    startup_time = timer::now();
    CDL::register_command("help", help, NO_ARGS);
    CDL::register_command("about", about, NO_ARGS);

// Timer Function

    CDL::intents::presence_update.push_back([] (Presence *presence) {
        // Check if bot went offline
        if (presence->user->bot and presence->status == PresenceStatus::offline) {
            fetch::channel(769145872113467423, [presence] (CChannel channel) {
                auto user = presence->user;
                // Delay by 30 minutes
                auto t = new boost::asio::steady_timer(*env.aioc, boost::asio::chrono::minutes(30));
                t->async_wait([channel, user, t] (const boost::system::error_code&) {
                    delete t;
                    // Check if bot is still offline
                    if (user->presence->status == PresenceStatus::offline) {
                        // Send warning
                        channel->send("**"+user->get_full_name()+"** went offline 30 minutes ago!!!\n"
                                      "<@&800878791546241044>");
                    }
                });
            });
        }
    });




    CDL::handlers::get_prefix = [] (CChannel) {
        return "w?";
    };


    CDL::intents::ready.push_back([] () {
        env.presence.activities.push_back(Activity(
            "over all Bots in DFB",
            ActivityType::watching
        ));
            env.presence.commit();
        });

    using namespace CDL::intent_vals;
    CDL::main(argc, argv, GUILD_PRESENCES | GUILD_MESSAGES);

   }
