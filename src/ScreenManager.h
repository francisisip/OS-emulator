#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <optional>

#include "Screen.h"

class ScreenManager {
    private:
        std::vector<Screen> screens;
        std::unordered_map<std::string, int> screenMap;

        std::string generateUniqueName(const std::string& baseName) {
        // Check if the name already exists, even if the user provides it explicitly
        auto it = std::find_if(screens.begin(), screens.end(), [&baseName](const Screen& screen) {
            return screen.getScreenName() == baseName;
        });

        if (it == screens.end()) {
            // If the exact name doesn't exist, allow its use
            if (screenMap[baseName] == 0) {
                screenMap[baseName] = 1;
            }
            return baseName;
        } else {
            // If the base name or explicit name exists, append the next count after a hyphen
            std::string newName = baseName + "-1";

            while (std::find_if(screens.begin(), screens.end(), [&newName](const Screen& screen) {
                       return screen.getScreenName() == newName;
                   }) != screens.end()) {

                // Increment suffix until a unique name is found
                int suffix = screenMap[baseName]++;
                newName = baseName + "-" + std::to_string(suffix + 1);
            }
            return newName;
        }
    }

    public:
    void createScreen(const std::string& baseName) {
        std::string uniqueName = generateUniqueName(baseName);
        screens.push_back(Screen(uniqueName));
    }

    Screen* findScreenByName(const std::string& name) {
        for (int i = 0; i < screens.size(); ++i) {
            if (screens[i].getScreenName() == name) {
                return &(screens[i]);
            }
        }
        return nullptr;
    }

    std::vector<Screen>& getScreens() {
        return screens;
    }

};