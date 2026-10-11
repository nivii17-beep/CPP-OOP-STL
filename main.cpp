#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class LibraryItem {
protected:
    int id;
    string title;
    bool issued;
public:
    LibraryItem(int itemId, string itemTitle) : id(itemId), title(std::move(itemTitle)), issued(false) {}
    virtual ~LibraryItem() = default;
    int getId() const { return id; }
    string getTitle() const { return title; }
    bool isIssued() const { return issued; }
    void setIssued(bool value) { issued = value; }
    virtual string type() const = 0;
    virtual string serialize() const = 0;
    virtual void display() const {
        cout << left << setw(7) << id << setw(24) << title
             << setw(14) << type() << (issued ? "Issued" : "Available") << '\n';
    }
};

class Book : public LibraryItem {
    string author;
public:
    Book(int itemId, string itemTitle, string itemAuthor)
        : LibraryItem(itemId, std::move(itemTitle)), author(std::move(itemAuthor)) {}
    string getAuthor() const { return author; }
    string type() const override { return "Book"; }
    string serialize() const override {
        return "BOOK|" + to_string(id) + "|" + title + "|" + author + "|" + (issued ? "1" : "0");
    }
    void display() const override {
        cout << left << setw(7) << id << setw(24) << title << setw(14) << type()
             << setw(22) << author << (issued ? "Issued" : "Available") << '\n';
    }
};

class Magazine : public LibraryItem {
    int issueNumber;
public:
    Magazine(int itemId, string itemTitle, int issue)
        : LibraryItem(itemId, std::move(itemTitle)), issueNumber(issue) {}
    string type() const override { return "Magazine"; }
    string serialize() const override {
        return "MAGAZINE|" + to_string(id) + "|" + title + "|" +
               to_string(issueNumber) + "|" + (issued ? "1" : "0");
    }
};

class Library {
    vector<shared_ptr<LibraryItem>> items;
    map<int, shared_ptr<LibraryItem>> byId;
    set<int> issuedIds;
    const string fileName;

    static vector<string> split(const string& line, char delimiter) {
        vector<string> fields;
        string field;
        stringstream stream(line);
        while (getline(stream, field, delimiter)) fields.push_back(field);
        return fields;
    }

public:
    explicit Library(string file = "library_data.txt") : fileName(std::move(file)) {}

    bool add(const shared_ptr<LibraryItem>& item) {
        if (!item || item->getId() <= 0 || item->getTitle().empty() || byId.count(item->getId()))
            return false;
        items.push_back(item);
        byId[item->getId()] = item;
        return true;
    }

    bool issue(int id) {
        auto found = byId.find(id);
        if (found == byId.end() || found->second->isIssued()) return false;
        found->second->setIssued(true);
        issuedIds.insert(id);
        return true;
    }

    bool returnItem(int id) {
        auto found = byId.find(id);
        if (found == byId.end() || !found->second->isIssued()) return false;
        found->second->setIssued(false);
        issuedIds.erase(id);
        return true;
    }

    shared_ptr<LibraryItem> find(int id) const {
        auto found = byId.find(id);
        return found == byId.end() ? nullptr : found->second;
    }

    vector<shared_ptr<LibraryItem>> searchTitle(const string& query) const {
        vector<shared_ptr<LibraryItem>> result;
        for (const auto& item : items)
            if (item->getTitle().find(query) != string::npos) result.push_back(item);
        return result;
    }

    const vector<shared_ptr<LibraryItem>>& all() const { return items; }
    size_t issuedCount() const { return issuedIds.size(); }

    bool save() const {
        ofstream out(fileName);
        if (!out) return false;
        for (const auto& item : items) out << item->serialize() << '\n';
        return static_cast<bool>(out);
    }

    bool load() {
        ifstream in(fileName);
        if (!in) return false;
        items.clear(); byId.clear(); issuedIds.clear();
        string line;
        while (getline(in, line)) {
            auto f = split(line, '|');
            if (f.size() != 5) continue;
            try {
                int itemId = stoi(f[1]);
                shared_ptr<LibraryItem> item;
                if (f[0] == "BOOK") item = make_shared<Book>(itemId, f[2], f[3]);
                else if (f[0] == "MAGAZINE") item = make_shared<Magazine>(itemId, f[2], stoi(f[3]));
                else continue;
                bool state = (f[4] == "1");
                item->setIssued(state);
                if (add(item) && state) issuedIds.insert(itemId);
            } catch (const exception&) {
                // Ignore malformed records; valid records remain loadable.
            }
        }
        return true;
    }
};

static void printItems(const vector<shared_ptr<LibraryItem>>& items) {
    if (items.empty()) { cout << "No matching items found.\n"; return; }
    cout << left << setw(7) << "ID" << setw(24) << "Title" << setw(14)
         << "Type" << "Status\n";
    cout << string(58, '-') << '\n';
    for (const auto& item : items) item->display();
}

int main(int argc, char* argv[]) {
    string file = (argc > 1) ? argv[1] : "library_data.txt";
    Library library(file);
    library.load();
    if (library.all().empty()) {
        library.add(make_shared<Book>(101, "Clean Code", "Robert C. Martin"));
        library.add(make_shared<Book>(102, "The Pragmatic Programmer", "Andrew Hunt"));
        library.add(make_shared<Magazine>(201, "C++ Monthly", 18));
    }

    while (true) {
        cout << "\n=== LIBRARY MANAGEMENT MINI APPLICATION ===\n"
             << "1. View all items\n2. Add a book\n3. Search by title\n"
             << "4. Issue an item\n5. Return an item\n6. View summary\n"
             << "7. Save and exit\nChoose an option: ";
        int choice;
        if (!(cin >> choice)) { cin.clear(); cin.ignore(10000, '\n'); cout << "Enter a number.\n"; continue; }
        cin.ignore(10000, '\n');
        if (choice == 1) printItems(library.all());
        else if (choice == 2) {
            int id; string title, author;
            cout << "Book ID: "; cin >> id; cin.ignore(10000, '\n');
            cout << "Title: "; getline(cin, title);
            cout << "Author: "; getline(cin, author);
            if (library.add(make_shared<Book>(id, title, author))) cout << "Book added.\n";
            else cout << "Could not add book. Check ID and title.\n";
        } else if (choice == 3) {
            string query; cout << "Title keyword: "; getline(cin, query);
            printItems(library.searchTitle(query));
        } else if (choice == 4 || choice == 5) {
            int id; cout << "Item ID: "; cin >> id; cin.ignore(10000, '\n');
            bool ok = (choice == 4) ? library.issue(id) : library.returnItem(id);
            cout << (ok ? "Operation successful.\n" : "Operation rejected: check ID and status.\n");
        } else if (choice == 6) {
            cout << "Total items: " << library.all().size()
                 << "\nCurrently issued: " << library.issuedCount()
                 << "\nAvailable: " << library.all().size() - library.issuedCount() << '\n';
        } else if (choice == 7) {
            if (library.save()) cout << "Data saved to " << file << ". Goodbye!\n";
            else cout << "Could not save data. Check file permissions.\n";
            break;
        } else cout << "Invalid option. Choose 1-7.\n";
    }
    return 0;
}
