#include "include/alldebrid.h"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <stdexcept>
#include <string>

using json = nlohmann::json;

namespace alldebrid {

    Client::Client(std::string api_key, std::string agent)
        : api_key_(std::move(api_key)), agent_(std::move(agent)) {}

    UnlockResult Client::unlock_link(const std::string& url) {
        // Build the request according to AllDebrid v4 API docs
        cpr::Response r = cpr::Get(cpr::Url{base_url_ + "link/unlock"},
                                   cpr::Parameters{
                                       {"agent", agent_},
                                       {"apikey", api_key_},
                                       {"link", url}
                                   });

        // Check for underlying network/cURL errors
        if (r.status_code == 0) {
            return {false, "", "Network error: " + r.error.message};
        }

        try {
            json j = json::parse(r.text);

            if (j.contains("status") && j["status"] == "success") {
                return {true, j["data"]["link"], ""};
            } else {
                std::string err_msg = "Unknown API Error";
                if (j.contains("error") && j["error"].contains("message")) {
                    err_msg = j["error"]["message"];
                }
                return {false, "", err_msg};
            }
        } catch (const json::exception& e) {
            return {false, "", "JSON parse error: " + std::string(e.what())};
        }
    }

  bool Client::save_link(const std::string& url) {
    cpr::Response r = cpr::Post(cpr::Url{base_url_ + "user/links/save"},
				cpr::Parameters{
				  {"agent", agent_},
				  {"apikey", api_key_},
				  {"links", url}
				});
      if (r.status_code == 0) {
	return false;
      }

    try{
      json j = json::parse(r.text);

      if (j.contains("status") && j["status"] == "success") {
	  return true;
	}
    } catch (const json::exception& e) {
      return false;
    }

    return false;
  }

  MagnetResult Client::upload_magnet(const std::string &magnet_link) {
    cpr::Url url{base_url_ + "magnet/upload"};
    cpr::Parameters parameters{
        {"magnets", magnet_link}, {"agent", agent_}, {"apikey", api_key_}};
    cpr::Response r = cpr::Post(url, parameters);

      if (r.status_code == 0) {
	throw std::runtime_error("Error uploading magnet!");
      }

    try{
      json j = json::parse(r.text);

      if (j.contains("status") && j["status"] == "success") {
        auto magnets = j["data"]["magnets"];
        auto magnet = magnets[0];
	return MagnetResult{
          magnet["magnet"], magnet["name"], magnet["id"], magnet["hash"],
              magnet["size"],
magnet["ready"]              
          };
      } else {
	throw std::runtime_error("Error getting the data!");
        }
    } catch (const json::exception& e) {
      throw std::runtime_error("Error parsing json response!");
    } catch (const std::runtime_error &e) {
      std::cerr << "Error uploading magnet!";
    }
    throw std::runtime_error("SHould not have come this far!")    ;
  }

  std::vector<Node*> parse_files(const json& files) {
    std::vector<Node*> nodes;

    for (auto file: files) {
      auto name = file["n"].get<std::string>();
      if (file.contains("s")) {
	FileNode* fileNode = new FileNode(name,
					  file["s"].get<int>(),
					  file["l"].get<std::string>());

	nodes.push_back(fileNode);
      } else {
	FolderNode* folderNode = new FolderNode(name,
						parse_files(file["e"]));
	nodes.push_back(folderNode);
      }

    }
    return nodes;
  }


  FilesAndLinks Client::download_links(const int &id) {
    cpr::Parameters parameters{{"id[]", std::to_string(id)}};
    auto response = send_request("magnet/files",
				 parameters);

    auto files = response["data"]["magnets"][0]["files"];

    auto filesAndFolders = parse_files(files);
    return FilesAndLinks(id, filesAndFolders);
  }


  json Client::send_request(const std::string &url,
		    cpr::Parameters &parameters) {
    cpr::Url fullUrl{base_url_ + url};
    parameters.Add({"agent", agent_});
    parameters.Add({"apikey", api_key_});

    cpr::Response r = cpr::Post(fullUrl, parameters);

    try {
      json j = json::parse(r.text);
      return j;
    } catch (const json::exception& e) {
      throw std::runtime_error("Invalid json");
    }
  }

  json Client::send_request(const std::string &url) {
    cpr::Url fullUrl{base_url_ + url};
    cpr::Parameters parameters{};
    parameters.Add({"agent", agent_});
    parameters.Add({"apikey", api_key_});
    
    cpr::Response r = cpr::Post(fullUrl, parameters);

    try {
      json j = json::parse(r.text);
      return j;
    } catch (const json::exception& e) {
      throw std::runtime_error("Invalid json");
    }
  }

  std::ostream& operator<<(std::ostream& s, const MagnetResult& result) {
    s << "ID: " << result.id << "\n";
    s << "Name: " << result.name << "\n";
    s << "Size: " << result.size << "\n";
    s << "Ready: ";

    if (result.ready) {
      s << "True";
    } else {
      s << "False";
    }
    s << "\n";

    return s;
  }

  std::ostream& operator<<(std::ostream& s, const Node* node) {
    s << node->n << "\n";
    if (auto* file = dynamic_cast<const FileNode*>(node)) {
      s << "Link: " << file->l << "\n";
      s << "Size: " << file->s << "\n";
    } else if (auto* folder = dynamic_cast<const FolderNode*>(node)) {
      for (const auto* file : folder->e) {
	s << file << "\n";
      }
    }

    return s;
  }

} // namespace alldebrid
