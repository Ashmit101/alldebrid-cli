#include <iostream>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <boost/program_options.hpp>

#include "alldebrid.h"
#include <iostream>


boost::program_options::variables_map parse_options(int argc, char* argv[], bool& save) {
  using namespace boost::program_options;

  bool is_help{};

  options_description desc("adcli [--save] link");
  desc.add_options()("save,s", bool_switch(&save), "saved link debrid service")("help,h", bool_switch(&is_help), "display a help dialog")
    ("link", value<std::string>(), "link to save or unlock");

  positional_options_description positional{};
  positional.add("link", 1);
    
  command_line_parser parser{ argc, argv };
  parser.options(desc);
  parser.positional(positional);
  auto parsed_result = parser.run();

  variables_map vm;
  store(parsed_result, vm);
  notify(vm);

  if (is_help) {
    std::cout << desc;
    exit(0);
  }
  
  return vm;  
}

int main(int argc, char* argv[]) {
  const char* api_key_env = std::getenv("ALLDEBRID_API_KEY");
  bool save{};

  if (!api_key_env) {
    std::cerr << "Error: ALLDEBRID_API_KEY environment variable is missing.\n"
	      << "Please set it before running the tool, e.g.:\n"
	      << "export ALLDEBRID_API_KEY='your_api_key_here'\n";
    return 1;
  }

  auto vm = parse_options(argc, argv, save);
  
  alldebrid::Client client(api_key_env, "my_cpp_cli");

  if (!isatty(STDIN_FILENO)) {
    std::string line;
    while (std::getline(std::cin, line)) {
      if (!save) {
	auto result = client.unlock_link(line);

	if (result.success) {
	  std::cout << result.unlocked_url << "\n";
	} else {
	  std::cerr << result.error_message << "\n";
	}
      } else {
	auto result = client.save_link(line);

	if (!result)
	  std::cerr << "Error saving " << line << " to debrid service\n";
      }
      
    }
    return 0;
  }

  std::string target_url = vm["link"].as<std::string>();

  if (!save) {
    alldebrid::UnlockResult result = client.unlock_link(target_url);

    if (result.success) {
      std::cout  << result.unlocked_url << "\n";
      return 0;
    } else {
      std::cerr << result.error_message << "\n";
      return 1;
    }
  } else {
    bool result = client.save_link(target_url);
    if (!result) {
      std::cerr << "Error saving " << target_url << " to debrid service.\n";
      return 2;
    }

    return 0;
      
  }
}
