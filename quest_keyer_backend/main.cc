#include <drogon/drogon.h>
#include <unistd.h>

int main(int argc, char** argv) {
    if (argc < 2 || strcmp(argv[1], "-debug") != 0) {
        std::cout << "Running in production mode\n";

        std::stringstream ss;
        ss << "cd ";
        const std::filesystem::path frontend_path = "../../quest_keyer_ui";
        ss << std::filesystem::canonical(frontend_path);
        ss << " && npm start run &";
        if (system((ss.str()).c_str())) {
            std::cerr << "Couldn't open frontend application";
            return 1;
        }
    } else {
        std::cout << "Running in development mode\n";
    }

    //Set HTTP listener address and port
    drogon::app().addListener("0.0.0.0", 5555);
    //Load config file
    //drogon::app().loadConfigFile("../config.json");
    //drogon::app().loadConfigFile("../config.yaml");
    //Run HTTP framework,the method will block in the internal event loop
    drogon::app().run();

    return 0;
}
