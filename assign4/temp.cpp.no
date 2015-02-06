#include <iostream>
#include <vector>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;

int main ()
{
  string s = "11/22/33";
  vector<string> t;
  split(t, s, is_any_of("/"));

  cout << (t[0] + "-") << endl;
}
