#include "box.h"

#include <iostream>
#include <string>

struct Flags {
  bool f = false;
  std::string file = "";

  bool b = false;
  std::string borderStyle = "";
};

std::string* isFlag(struct Flags& f) {
  std::string* setFlag = nullptr;
  setFlag = f.f ? &f.file : setFlag;
  f.f = false;
  setFlag = f.b ? &f.borderStyle : setFlag;
  f.b = false;

  return setFlag;
}

void flagCheck(Flags& flags, char* arg) {
  char* c = arg;
  while (*(c++) != '\0') {
    if (*c == 'f')
      flags.f = true;
    else if (*c == 'b')
      flags.b = true;
  }
}

int main(int argc, char** argv) {
  std::string msg = "";
  bool msgReading = false;
  struct Flags flags;

  for (int i = 1; i < argc; i++) {
    if (!msgReading) {
      if (argv[i][0] == '-') {
        flagCheck(flags, argv[i]);
        continue;
      }

      std::string* modifier = isFlag(flags);
      if (modifier) {
        *modifier = std::string(argv[i]);
        continue;
      }
    }
    msg += argv[i];
    msg += " ";
  }

  Box display(msg);

  std::cout << display;

  return 0;
}
