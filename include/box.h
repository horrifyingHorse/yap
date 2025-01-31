#ifndef BOX_H
#define BOX_H

#include <ostream>
#include <string>
#include <vector>

enum Align { LEFT, CENTER, RIGHT };

class Box {
 public:
  Box(std::string, int = 40);

  void Chunks();

  friend std::ostream& operator<<(std::ostream&, Box&);

  static std::string& clean(std::string&);
  static std::string padding(std::string, int, enum Align);

 protected:
  int limit = 40;
  int len;
  std::string msg;
  std::vector<std::string> chunks;

  int pushChunk(std::string&);
};

class BoxChar {
 public:
  BoxChar(char c[4]);

  friend std::string operator*(BoxChar&, unsigned int);
  friend std::ostream& operator<<(std::ostream&, BoxChar&);

 protected:
  char c[4];
};

namespace BoxOutline {
extern BoxChar HorizontalSingle;
extern BoxChar VerticalSingle;
extern BoxChar TopLeftCornerSingle;
extern BoxChar TopRightCornerSingle;
extern BoxChar BottomLeftCornerSingle;
extern BoxChar BottomRightCornerSingle;

extern BoxChar EmptyChar;
};  // namespace BoxOutline

#endif  // !BOX_H
