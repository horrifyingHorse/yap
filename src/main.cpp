#include <iostream>
#include <ostream>
#include <string.h>
#include <string>
#include <vector>

class Box {
public:
  Box(std::string msg, int limit = 40) : msg(Box::clean(msg)), limit(limit) {
    Chunks();
  }

  void Chunks() {
    std::string msgCopy = this->msg;
    int limit = this->limit;

    while (msgCopy.length() > limit) {
      std::string chunk = msgCopy.substr(0, limit);
      if (msgCopy[limit + 1] != '\n' || msgCopy[limit + 1] != ' ') {
        int idx = msgCopy.find_last_of(" ", limit);
        if (idx != std::string::npos) {
          chunk = msgCopy.substr(0, idx);
          limit = idx;
        }
      }
      this->chunks.push_back(Box::clean(chunk));
      msgCopy = msgCopy.substr(limit);
      limit = this->limit;
    }
    this->chunks.push_back(Box::clean(msgCopy));
  }

  friend std::ostream &operator<<(std::ostream &, Box &);

  static std::string &clean(std::string &s) {
    s.erase(0, s.find_first_not_of(' '));
    s.erase(s.find_last_not_of(' ') + 1);
    return s;
  }

protected:
  int limit = 40;
  std::string msg;
  std::vector<std::string> chunks;
};

class BoxChar {
public:
  BoxChar(char c[4]) {
    for (int i = 0; i < 4; i++)
      this->c[i] = c[i];
  }
  friend std::string operator*(BoxChar &, unsigned int);
  friend std::ostream &operator<<(std::ostream &, BoxChar &);

protected:
  char c[4];
};
std::ostream &operator<<(std::ostream &os, BoxChar &bc) {
  os << bc.c;
  return os;
}
std::string operator*(BoxChar &bc, unsigned int len) {
  std::string result = "";
  while (len-- != 0) {
    result += bc.c;
  }
  return result;
}

namespace BoxOutline {
inline static BoxChar HorizontalSingle = BoxChar((char *)"─");
inline static BoxChar VerticalSingle = BoxChar((char *)"│");
inline static BoxChar TopLeftCornerSingle = BoxChar((char *)"┌");
inline static BoxChar TopRightCornerSingle = BoxChar((char *)"┐");
inline static BoxChar BottomLeftCornerSingle = BoxChar((char *)"└");
inline static BoxChar BottomRightCornerSingle = BoxChar((char *)"┘");

inline static BoxChar EmptyChar = BoxChar((char *)" ");
}; // namespace BoxOutline

std::ostream &operator<<(std::ostream &os, Box &box) {
  int width = (box.limit + 2);
  int height = box.chunks.size();

  // clang-format off
  os << BoxOutline::TopLeftCornerSingle
     << (BoxOutline::HorizontalSingle * width)
     << BoxOutline::TopRightCornerSingle
     << '\n';
  for (int i = 0; i < height; i++) {
    os << BoxOutline::VerticalSingle << BoxOutline::EmptyChar * width
       << BoxOutline::VerticalSingle << "\n";
  }
  os << BoxOutline::BottomLeftCornerSingle
     << (BoxOutline::HorizontalSingle * width)
     << BoxOutline::BottomRightCornerSingle
     << '\n';
  // clang-format on

  os << "\033[" << height + 1 << "A";

  for (auto &chunk : box.chunks) {
    os << "\033[2C" << chunk << "\n";
  }
  os << "\n";
  return os;
}

struct Flags {
  bool f = false;
  std::string file = "";
};

std::string *isFlag(struct Flags &f) {
  std::string *setFlag = nullptr;
  setFlag = f.f ? &f.file : setFlag;
  f.f = false;
  return setFlag;
}

void flagCheck(Flags &flags, char *arg) {
  char *c = arg;
  while (*(c++) != '\0') {
    if (*c == 'f') {
      flags.f = true;
    }
  }
}

int main(int argc, char **argv) {
  std::string msg = "";
  bool msgReading = false;
  struct Flags flags;

  for (int i = 1; i < argc; i++) {
    if (!msgReading) {
      if (argv[i][0] == '-') {
        flagCheck(flags, argv[i]);
        continue;
      }

      std::string *modifier = isFlag(flags);
      if (modifier) {
        *modifier = std::string(argv[i]);
        continue;
      }
    }

    msg += argv[i];
    switch (argv[i][strlen(argv[i])]) {
    case '\n':
    case '\t':
      break;
    default:
      msg += " ";
    }
  }

  Box display(msg);

  std::cout << display;

  return 0;
}
