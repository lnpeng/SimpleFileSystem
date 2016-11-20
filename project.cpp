#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

vector<string> split(const string &s, char delim) {
  vector<string> elems;
  stringstream ss(s);
  string item;
  while (getline(ss, item, delim)) {
    elems.push_back(item);
  }
  return elems;
}

string join(const vector<string>& input) {
  string output = "";
  bool has_data = false;
  for (size_t i = 0; i < input.size(); ++i) {
    has_data = true;
    output += input[i];
    output += ", ";
  }
  if (has_data) {
    output = output.substr(0, output.length() - 2);
  }
  return output;
}

class FileSystem {
    map<string, vector<string>*> directories;
    map<string, string> files;
 public:
    FileSystem() {
        pair<string, vector<string>*> root("/", new vector<string>());
        directories.insert(root);
    }
    
  vector<string> Ls(const string& path) {
      map<string, vector<string>*>::iterator p = directories.find(path);
      if (p == directories.end()) {
          vector<string> rs;
          rs.push_back(files.find(path)->second);
          return rs;
      }
      else
          return *(p->second);
  }

  void MkdirP(const string& dir_path) {
      //add
      vector<string> tokens = split(dir_path, '/');
      string parent = "/";
      for (size_t i = 1; i < tokens.size(); i++) {
          string cur = parent + (parent.rfind('/') == parent.size() - 1 ? "" : "/") + tokens[i];
          map<string, vector<string>*>::iterator p = directories.find(cur);
          if (p == directories.end()) {
              pair<string, vector<string>*> dir(cur, new vector<string>());
              directories.insert(dir);
              map<string, vector<string>*>::iterator p1 = directories.find(parent);
              if (p1 != directories.end()) {
                  vector<string> * v = p1->second;
                  if (v != NULL)
                      v->push_back(tokens[i]);
              }
              
          }
          parent = cur;
      }
  }

  void AddFileWithContent(const string& file_path, const string& content) {
      pair<string, string> file(file_path, content);
      files.insert(file);
      string path = file_path.substr(0, file_path.rfind('/'));
      string fileName = file_path.substr(path.size() + 1);
      vector<string> * t = directories.find(path)->second;
      t->push_back(fileName);
  }

  string GetFileContent(const string& file_path) {
    return files.find(file_path)->second;
  }
};


#ifndef __main__
#define __main__

int main(int argc, char* argv[]) {
  FileSystem fs;

  // should print ""
  cout << join(fs.Ls("/")) << endl;
  fs.MkdirP("/a/b/c");
  fs.AddFileWithContent("/a/b/c/d", "hello world");
  // should print a
  cout << join(fs.Ls("/")) << endl;
  // should print d
  cout << join(fs.Ls("/a/b/c")) << endl;
  // should print d
  cout << join(fs.Ls("/a/b/c/d")) << endl;
  // should print hello world
  cout << fs.GetFileContent("/a/b/c/d") << endl;
}

#endif


