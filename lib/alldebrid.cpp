#include "alldebrid.h"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <iostream>

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
				  {"links": {url}}
				})
      if (r.status_code == 0) {
	return false;
      }

    try{
      json j = json::parse(r.text);

      if (j.contains("status") && j["status"] == "success") {
	  return true;
	}
    }
  }

} // namespace alldebrid
