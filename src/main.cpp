#include <iostream>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <boost/program_options.hpp>

#include "alldebrid.h"
#include <iostream>


boost::program_options::variables_map parse_options(int argc, char* argv[], bool& save, bool&magnet) {
  using namespace boost::program_options;

  bool is_help{};

  options_description desc("adcli [--save] link");
  desc.add_options()("save,s", bool_switch(&save), "saved link debrid service")("help,h", bool_switch(&is_help), "display a help dialog")("magnet,m", bool_switch(&magnet), "upload magnet link")
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
  bool save{}, magnet{};

  if (!api_key_env) {
    std::cerr << "Error: ALLDEBRID_API_KEY environment variable is missing.\n"
	      << "Please set it before running the tool, e.g.:\n"
	      << "export ALLDEBRID_API_KEY='your_api_key_here'\n";
    return 1;
  }

  auto vm = parse_options(argc, argv, save, magnet);
  
  alldebrid::Client client(api_key_env, "my_cpp_cli");

  if (!isatty(STDIN_FILENO)) {
    std::string line;
    while (std::getline(std::cin, line)) {
      if (magnet) {
        auto magnet_object = client.upload_magnet(line);
	std::cout << magnet_object.name << " " << magnet_object.size << " " << magnet_object.ready << "\n";
      } else if (save) {
	auto result = client.save_link(line);

	if (!result)
	  std::cerr << "Error saving " << line << " to debrid service\n";

      } else {
	auto result = client.unlock_link(line);

	if (result.success) {
	  std::cout << result.unlocked_url << "\n";
	} else {
	  std::cerr << result.error_message << "\n";
	}

      }
      
    }
    return 0;
  }

  std::string target_url = vm["link"].as<std::string>();

  if (magnet) {
    auto magnet_object = client.upload_magnet(target_url);
    std::cout << magnet_object.name << " " << magnet_object.size << " " << magnet_object.ready << "\n";
  } else if (save) {
    auto result = client.save_link(target_url);

    if (!result)
      std::cerr << "Error saving " << target_url << " to debrid service\n";

  } else {
    auto result = client.unlock_link(target_url);

    if (result.success) {
      std::cout << result.unlocked_url << "\n";
    } else {
      std::cerr << result.error_message << "\n";
    }

  }

  return 0;
      
}
