#include <iostream>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <boost/program_options.hpp>

#include "alldebrid.h"


boost::program_options::variables_map parse_options(int argc, char* argv[]) {
  using namespace boost::program_options;

  options_description desc("Allowed options");
  desc.add_options()("save", "saved link debrid service");
  variables_map vm;
  store(parse_command_line(argc, argv, desc), vm);
  notify(vm);

  return vm;  
}

int main(int argc, char* argv[]) {
  const char* api_key_env = std::getenv("ALLDEBRID_API_KEY");

  if (!api_key_env) {
    std::cerr << "Error: ALLDEBRID_API_KEY environment variable is missing.\n"
	      << "Please set it before running the tool, e.g.:\n"
	      << "export ALLDEBRID_API_KEY='your_api_key_here'\n";
    return 1;
  }

  auto vm = parse_options(argc, argv);
  
  alldebrid::Client client(api_key_env, "my_cpp_cli");

  if (!isatty(STDIN_FILENO)) {
    std::string line;
    while (std::getline(std::cin, line)) {
      auto result = client.unlock_link(line);

      if (result.success) {
	std::cout << result.unlocked_url << "\n";
      } else {
	std::cerr << result.error_message << "\n";
      }
    }
    return 0;
  } else if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <url_to_unlock>\n";
    return 1;
  }

  std::string target_url = argv[1];
  alldebrid::UnlockResult result = client.unlock_link(target_url);

  // Handle the result
  if (result.success) {
    std::cout  << result.unlocked_url << "\n";
    return 0;
  } else {
    std::cerr << result.error_message << "\n";
    return 1;
  }
}
