#include "box.h"

#include <iostream>
#include <string>
#include <unordered_map>

typedef struct Flags {
  std::string* modifier = nullptr;
  std::unordered_map<std::string, std::string> flags_str;
  std::unordered_map<std::string, bool> flags_bool;
} Flags;

Flags& initialiseFlags(Flags& flags) {
  flags.flags_str["-f"] = "";
  flags.flags_str["-b"] = "";

  flags.flags_bool["--border-double"] = false;

  flags.modifier = nullptr;

  return flags;
}

void flagCheck(Flags& flags, char* arg) {
  if (flags.flags_str.find(arg) != flags.flags_str.end()) {
    flags.modifier = &flags.flags_str[arg];
    return;
  }
  if (flags.flags_bool.find(arg) != flags.flags_bool.end()) {
    flags.flags_bool[arg] = true;
    return;
  }
  std::cerr << "Unidentified Flag " << arg << "\n";
  exit(1);
}

int main(int argc, char** argv) {
  std::string msg = "";
  bool readingMsg = false;
  Flags flags;

  initialiseFlags(flags);
  for (int i = 1; i < argc; i++) {
    if (!readingMsg && argv[i][0] == '-') {
      flagCheck(flags, argv[i]);
      continue;
    }
    if (!readingMsg && flags.modifier) {
      *flags.modifier = std::string(argv[i]);
      flags.modifier = nullptr;
      continue;
    }

    readingMsg = true;
    msg += argv[i];
    msg += " ";
  }

  Box display(msg);

  std::cout << display;

  return 0;
}
