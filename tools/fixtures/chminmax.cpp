// @snippet-definition@
#include <cassert>
#include <string>

int main() {
  int x = 3;
  assert(chmax(x, 5) && x == 5);
  assert(!chmax(x, 5) && !chmax(x, -1) && x == 5);
  assert(chmin(x, 2) && x == 2);
  assert(!chmin(x, 2) && !chmin(x, 9) && x == 2);
  std::string s = "cat";
  assert(chmin(s, std::string("ant")) && s == "ant");
  assert(chmax(s, std::string("dog")) && s == "dog");
}
