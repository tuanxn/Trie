// Student ID: 20274909

#include <string>
#include <sstream>
#include <math.h>
#include <vector>
#include <iostream>
#include "Trie.h"
#include <queue>

using namespace std;

//int main()
//{
//   Trie test = Trie();
//   //test.lookup("BE");
//   test.insert("A");
//   //cout << test.lookup("BE");
//   test.insert("AB");
//   test.insert("BE");
//   test.insert("GOD");
//   test.insert("GEE");
//   vector<string> temp;
//   test.get_completions("J", temp, 4);
//   for (int i = 0; i < temp.size(); i++) {
//      cout << temp[i] << endl;
//   }
//   //cout << test.lookup("BE");
//   //cout << test.to_string(50);
//   //cout << test.to_string(2);
//}

Trie::Trie() {
   _root = new Trie::Node();
}

void Trie::Node::insert(string s) {
   // iteratively insert the given string into the trie starting at the current node

   Trie::Node* curr = this;
   for (const char* str = s.c_str(); *str; str++) {
      char ch = *str;
      if ((size_t)ch >= curr->next.size())
         curr->next.resize(ch + 1);

      if (curr->next[ch] != nullptr)
         curr = curr->next[ch];
      else
         curr = curr->next[ch] = new Trie::Node;
   }

   // the \0
   if (curr->next.empty()) // TODO
      curr->next.resize(1); // TODO

   if (curr->next[0] == nullptr)
      curr->next[0] = new Trie::Node;

   // TODO watch out for duplicates

}

const Trie::Node* Trie::Node::traverse(string s) const {
   const Trie::Node* curr = this;

   for (const char* str = s.c_str(); *str; str++) {
     char ch = *str;
      if (ch < curr->next.size() && curr->next[ch] != nullptr)
         curr = curr->next[ch];
      else {
         curr = nullptr;
         break;
      }
   }

   return curr;
}

bool Trie::Node::lookup(string s) const {
   const Node* result_node = this->traverse(s);
   
   if (this->next.empty() || result_node == nullptr) {
      return false;
   }
   else if (result_node->next[0] != nullptr) {
      return true;
   }
   return false;
}

Trie::Node::~Node() {

   static int r_depth = 0;
   cout << "~Node() Recursion depth " << ++r_depth << endl;

   Node* curr = this;

   // base case
   if (curr->next.empty()) {
      cout << "~Node() back to depth " << --r_depth << endl;
      return;
   }

   for (size_t i = 0; i < curr->next.size(); i++) {
      if (curr->next[i] != nullptr) {
         cout << "~Node() Descending from depth " << r_depth << endl;
         delete curr->next[i];
         curr->next[i] = NULL;
      }
   }

   if (curr != NULL) {
      curr->next.clear();
      cout << "~Node() back to depth " << --r_depth << endl;
      return;
   }

}

Trie::~Trie() {
   delete _root;
   _root = NULL;
}

size_t Trie::Node::get_completions(vector<string>& completions, size_t limit) const {
   completions.clear();
   if (limit == 0) {
      return completions.size();
   }

   struct Continuation {
      const Trie::Node* node;
      string partial;
      Continuation(const Node* p, string s) : node(p), partial(s) {}
   };
   // All string descendants of this node are completions
   queue<Continuation> unprocessed_nodes;
   unprocessed_nodes.push(Continuation(this, ""));
   while (!unprocessed_nodes.empty()) {
      Continuation cont = unprocessed_nodes.front();
      unprocessed_nodes.pop();
      for (int i = 0; i < cont.node->next.size(); i++) {
         if (i == 0 && cont.node->next[0] != nullptr) {
            completions.push_back(cont.partial);
            if (completions.size() >= limit)
               break;
         }
         else if (cont.node->next[i] != nullptr) {
            string new_partial = cont.partial + (char)i;
            unprocessed_nodes.push(Continuation(cont.node->next[i], new_partial));
         }

      }
      if (completions.size() >= limit)
         break;
   }

   return completions.size();
}

size_t Trie::get_completions(string s, vector<string>& completions, size_t limit) const {
   const Node* result_node = this->traverse(s);
   if (result_node == nullptr) {
      return completions.size();
   }
   return result_node->get_completions(completions, limit);
}

string Trie::to_string(size_t limit) const {
   string output = "# Trie contents\n";
   vector<string> completions;
   size_t completion_num = this->get_completions("", completions, limit + 1);

   if (completion_num < limit) {
      limit = completion_num;
   }

   for (size_t i = 0; i < limit; i++) {
      if(!completions[i].empty()) {
         output += completions[i] + "\n";
      }
   }
   if (completions.size() > limit) {
      output += "...\n";
   }
   return output;
}

size_t Trie::trie_sort(vector<string>& vec) const {
   vec.clear();
   size_t trie_size = this->get_completions("", vec, 10000);
   vec.resize(trie_size);

   return vec.size();
}