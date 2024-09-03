#include <drogon/drogon.h>
#include <unistd.h>

int main() {

    std::stringstream ss;
    ss << "open ";
    std::filesystem::path frontend_path = "../../quest_keyer_ui/out/quest_keyer_ui-darwin-arm64/quest_keyer_ui.app";
    ss << std::filesystem::canonical(frontend_path);
    system((ss.str()).c_str());

    //Set HTTP listener address and port
    drogon::app().addListener("0.0.0.0", 5555);
    //Load config file
    //drogon::app().loadConfigFile("../config.json");
    //drogon::app().loadConfigFile("../config.yaml");
    //Run HTTP framework,the method will block in the internal event loop
    drogon::app().run();
    return 0;
}
