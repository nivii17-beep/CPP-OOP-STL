#include <cassert>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>
using namespace std;

struct Item {
    int id; string title; bool issued = false;
    Item(int i, string t): id(i), title(t) {}
};
class TestLibrary {
    map<int, Item> items;
    set<int> issued;
public:
    bool add(Item x) {
        if (x.id <= 0 || x.title.empty() || items.count(x.id)) return false;
        items.emplace(x.id, x); return true;
    }
    bool issue(int id) {
        auto it=items.find(id);
        if(it==items.end() || it->second.issued) return false;
        it->second.issued=true; issued.insert(id); return true;
    }
    bool giveBack(int id) {
        auto it=items.find(id);
        if(it==items.end() || !it->second.issued) return false;
        it->second.issued=false; issued.erase(id); return true;
    }
    size_t size() const { return items.size(); }
    size_t issuedCount() const { return issued.size(); }
};
int main() {
    TestLibrary lib;
    assert(lib.add(Item(1,"Algorithms")));
    assert(!lib.add(Item(1,"Duplicate ID")));
    assert(!lib.add(Item(0,"Invalid ID")));
    assert(!lib.add(Item(2,"")));
    assert(lib.issue(1));
    assert(!lib.issue(1));
    assert(!lib.issue(999));
    assert(lib.issuedCount()==1);
    assert(lib.giveBack(1));
    assert(!lib.giveBack(1));
    assert(lib.issuedCount()==0);
    assert(lib.size()==1);
    cout << "All 10 basic unit checks passed.\n";
}
