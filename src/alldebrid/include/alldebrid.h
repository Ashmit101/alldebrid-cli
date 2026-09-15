#pragma once
#include <string>
#include <optional>
#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>

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

  struct Node{
    std::string n;		// Name
  };

  struct FolderNode : Node {
    std::vector<Node> e;	// Sub nodes

    void addSubNode(Node &node) {
      e.push_back(node);
    }
  };

  struct FileNode : Node {
    int s;			// File Size
    std::string l; 		// Download link
  };

  struct FilesAndLinks {
    int id;
    std::vector<Node> files;

    FilesAndLinks(int id) {
      this->id = id;
    }

    void addNode(Node &node) {
      files.push_back(node);
    }

  };
  
  class Client {
  public:
    explicit Client(std::string api_key, std::string agent = "cli_tool");

    UnlockResult unlock_link(const std::string& url);
    bool save_link(const std::string &url);
    MagnetResult upload_magnet(const std::string &magnet_url);
    FilesAndLinks download_links(const int& id);

  private:
    std::string api_key_;
    std::string agent_;
    const std::string base_url_ = "https://api.alldebrid.com/v4/";
    nlohmann::json send_request(const std::string &url,
		      cpr::Parameters &parameters
		      );
    nlohmann::json send_request(const std::string &url);
  };

  std::ostream& operator<<(std::ostream&, const MagnetResult&);

} // namespace alldebrid
