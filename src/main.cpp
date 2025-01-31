#include <iostream>
#include <ostream>
#include <string.h>
#include <string>
#include <vector>

enum Align { LEFT, CENTER, RIGHT };

class Box {
public:
  Box(std::string msg, int limit = 40) : msg(Box::clean(msg)), limit(limit) {
    Chunks();
  }

  void Chunks() {
    std::string msgCopy = this->msg;
    int limit = this->limit;
    len = msgCopy.size();

    while (msgCopy.length() > limit) {
      if (len != this->limit)
        len = this->limit;
      std::string chunk = msgCopy.substr(0, limit);
      if (msgCopy[limit + 1] != '\n' || msgCopy[limit + 1] != ' ') {
        int idx = msgCopy.find_last_of(" ", limit);
        if (idx != std::string::npos) {
          chunk = msgCopy.substr(0, idx);
          limit = idx;
        }
      }
      pushChunk(chunk);
      msgCopy = msgCopy.substr(limit);
      limit = this->limit;
    }
    pushChunk(msgCopy);
  }

  friend std::ostream &operator<<(std::ostream &, Box &);

  static std::string &clean(std::string &s) {
    s.erase(0, s.find_first_not_of(' '));
    s.erase(s.find_last_not_of(' ') + 1);
    return s;
  }

  static std::string padding(std::string s, int len, enum Align align) {
    int to_pad = len - s.length();
    if (to_pad <= 0)
      return s;

    if (align == Align::LEFT) {
      while (to_pad-- != 0) {
        s += " ";
      }
    }

    return s;
  }

protected:
  int limit = 40;
  int len;
  std::string msg;
  std::vector<std::string> chunks;

  // handles \n, but what bout \t? (Sigh..)
  void pushChunk(std::string &chunk) {
    while (chunk.find("\n") != std::string::npos) {
      int idx = chunk.find("\n");
      std::string chunk_part = chunk.substr(0, idx);
      this->chunks.push_back(Box::clean(chunk_part));
      chunk = chunk.substr(idx + 1);
    }
    if (chunk != "\0") {
      this->chunks.push_back(Box::clean(chunk));
    }
  }
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
BoxChar HorizontalSingle = BoxChar((char *)"─");
BoxChar VerticalSingle = BoxChar((char *)"│");
BoxChar TopLeftCornerSingle = BoxChar((char *)"┌");
BoxChar TopRightCornerSingle = BoxChar((char *)"┐");
BoxChar BottomLeftCornerSingle = BoxChar((char *)"└");
BoxChar BottomRightCornerSingle = BoxChar((char *)"┘");

BoxChar EmptyChar = BoxChar((char *)" ");
}; // namespace BoxOutline

std::ostream &operator<<(std::ostream &os, Box &box) {
  int rawWidth = box.len;
  int width = (box.len + 2);
  int height = box.chunks.size();

  // clang-format off
  os << BoxOutline::TopLeftCornerSingle
     << (BoxOutline::HorizontalSingle * width)
     << BoxOutline::TopRightCornerSingle
     << '\n';
  for (auto &chunk : box.chunks) {
    os << BoxOutline::VerticalSingle << BoxOutline::EmptyChar
       << Box::padding(chunk, rawWidth, Align::LEFT)
       << BoxOutline::EmptyChar << BoxOutline::VerticalSingle
       << "\n";
  }
  os << BoxOutline::BottomLeftCornerSingle
     << (BoxOutline::HorizontalSingle * width)
     << BoxOutline::BottomRightCornerSingle
     << '\n';
  // clang-format on
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
