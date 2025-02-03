#include "box.h"

#include <iostream>
#include <ostream>
#include <string>
#include <vector>

Box::Box(std::string msg, int limit) : msg(Box::clean(msg)), limit(limit) {
  Chunks();
}

void Box::Chunks() {
  std::string msgCopy = this->msg;
  int limit = this->limit;
  int max_chunk_len;
  len = 0;

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
    msgCopy = msgCopy.substr(limit);
    limit = this->limit;

    max_chunk_len = pushChunk(chunk);
    if (max_chunk_len > len)
      len = max_chunk_len;
  }

  max_chunk_len = pushChunk(msgCopy);
  if (max_chunk_len > len)
    len = max_chunk_len;
}

void Box::setBorder(enum Border border) {
  switch (border) {
    case Border::SINGLE: {
      Horizontal = BoxOutline::HorizontalSingle;
      Vertical = BoxOutline::VerticalSingle;
      TopLeftCorner = BoxOutline::TopLeftCornerSingle;
      TopRightCorner = BoxOutline::TopRightCornerSingle;
      BottomLeftCorner = BoxOutline::BottomLeftCornerSingle;
      BottomRightCorner = BoxOutline::BottomRightCornerSingle;
    }
    case Border::DOUBLE: {
      Horizontal = BoxOutline::HorizontalDouble;
      Vertical = BoxOutline::VerticalDouble;
      TopLeftCorner = BoxOutline::TopLeftCornerDouble;
      TopRightCorner = BoxOutline::TopRightCornerDouble;
      BottomLeftCorner = BoxOutline::BottomLeftCornerDouble;
      BottomRightCorner = BoxOutline::BottomRightCornerDouble;
    }
  }
}

std::string& Box::clean(std::string& s) {
  s.erase(0, s.find_first_not_of(' '));
  s.erase(s.find_last_not_of(' ') + 1);
  return s;
}

std::string Box::padding(std::string s, int len, enum Align align) {
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

// handles \n, but what bout \t? (Sigh..)
int Box::pushChunk(std::string& chunk) {
  int max = 0;
  while (chunk.find("\n") != std::string::npos) {
    int idx = chunk.find("\n");
    std::string chunk_part = chunk.substr(0, idx);
    this->chunks.push_back(Box::clean(chunk_part));
    chunk = chunk.substr(idx + 1);

    if (max < chunk_part.size())
      max = chunk_part.size();
  }
  if (chunk != "\0") {
    this->chunks.push_back(Box::clean(chunk));

    if (max < chunk.size())
      max = chunk.size();
  }
  return max;
}

BoxChar::BoxChar(char c[4]) {
  for (int i = 0; i < 4; i++)
    this->c[i] = c[i];
}

// BoxChar
std::ostream& operator<<(std::ostream& os, BoxChar& bc) {
  os << bc.c;
  return os;
}
std::string operator*(BoxChar& bc, unsigned int len) {
  std::string result = "";
  while (len-- != 0) {
    result += bc.c;
  }
  return result;
}
// BoxChar OpOverloading

// Box
std::ostream& operator<<(std::ostream& os, Box& box) {
  int rawWidth = box.len;
  int width = (box.len + 2);
  int height = box.chunks.size();

  if (height == 0)
    return os;

  // clang-format off
  os << box.TopLeftCorner
     << (box.Horizontal * width)
     << box.TopRightCorner
     << '\n';
  for (auto &chunk : box.chunks) {
    os << box.Vertical << BoxOutline::EmptyChar
       << Box::padding(chunk, rawWidth, Align::LEFT)
       << BoxOutline::EmptyChar << box.Vertical
       << "\n";
  }
  os << box.BottomLeftCorner
     << (box.Horizontal * width)
     << box.BottomRightCorner
     << '\n';
  // clang-format on
  return os;
}
// Box OpOverloading

namespace BoxOutline {
BoxChar HorizontalSingle = BoxChar((char*)"─");
BoxChar VerticalSingle = BoxChar((char*)"│");
BoxChar TopLeftCornerSingle = BoxChar((char*)"┌");
BoxChar TopRightCornerSingle = BoxChar((char*)"┐");
BoxChar BottomLeftCornerSingle = BoxChar((char*)"└");
BoxChar BottomRightCornerSingle = BoxChar((char*)"┘");

BoxChar HorizontalDouble = BoxChar((char*)"═");
BoxChar VerticalDouble = BoxChar((char*)"║");
BoxChar TopLeftCornerDouble = BoxChar((char*)"╔");
BoxChar TopRightCornerDouble = BoxChar((char*)"╗");
BoxChar BottomLeftCornerDouble = BoxChar((char*)"╚");
BoxChar BottomRightCornerDouble = BoxChar((char*)"╝");

BoxChar EmptyChar = BoxChar((char*)" ");
};  // namespace BoxOutline
