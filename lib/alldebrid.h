#pragma once
#include <string>
#include <optional>

namespace alldebrid {

    struct UnlockResult {
        bool success;
        std::string unlocked_url;
        std::string error_message;
    };

    struct MagnetResult {
      std::string magnet;
      std::string name;
      int id;
      std::string hash;
      int size;
      bool ready;      
    };

    class Client {
    public:
      explicit Client(std::string api_key, std::string agent = "cli_tool");

      UnlockResult unlock_link(const std::string& url);
      bool save_link(const std::string &url);
      MagnetResult upload_magnet(const std::string &magnet_url);

    private:
        std::string api_key_;
        std::string agent_;
        const std::string base_url_ = "https://api.alldebrid.com/v4/";
    };

} // namespace alldebrid
