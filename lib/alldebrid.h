#pragma once
#include <string>
#include <optional>

namespace alldebrid {

    struct UnlockResult {
        bool success;
        std::string unlocked_url;
        std::string error_message;
    };

    class Client {
    public:
        // Requires an API key. Agent is optional but recommended by AllDebrid.
        explicit Client(std::string api_key, std::string agent = "cli_tool");

        // Performs the HTTP request to unlock the link
        UnlockResult unlock_link(const std::string& url);

      bool save_link(const std::string& url);

    private:
        std::string api_key_;
        std::string agent_;
        const std::string base_url_ = "https://api.alldebrid.com/v4/";
    };

} // namespace alldebrid
