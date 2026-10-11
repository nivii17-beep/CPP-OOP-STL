# Library Management Mini Application — C++17

**Internship Task:** C++ Project — OOP & STL (Mini Application)  
**Project type:** Console-based application  
**Language standard:** C++17

## 1. Project Overview

The Library Management Mini Application is a command-line program developed to demonstrate object-oriented programming (OOP), Standard Template Library (STL) containers, and file-based persistence in C++. It models a small library catalogue containing books and magazines. The application supports adding records, listing catalogue items, searching by title, issuing and returning items, and preserving records between program runs.

The project is intended as an educational mini application. It demonstrates how classes, inheritance, polymorphism, smart pointers, and STL containers can be combined to organise a small information-management system.

## 2. Objectives

- Apply classes and objects to represent library records.
- Demonstrate inheritance through the `LibraryItem`, `Book`, and `Magazine` classes.
- Use virtual functions to support type-specific display and serialization.
- Apply `vector`, `map`, and `set` to manage records and issue status.
- Prevent duplicate identifiers and reject invalid operations.
- Store records in a text file and reload them in a later session.
- Provide a repeatable build process and basic automated checks.

## 3. Features

- Add books and magazines to the catalogue.
- Display the stored catalogue.
- Search for titles using a keyword.
- Issue an available item.
- Return an issued item.
- Reject duplicate or non-positive IDs, empty titles, unknown IDs, and invalid issue/return transitions.
- Save records to a text file and load saved records on a subsequent run.
- Run a basic test executable for important catalogue and issue/return rules.

## 4. Project Structure

```text
internspark_cpp_oop_stl/
├── src/
│   └── main.cpp
├── tests/
│   └── test_library.cpp
├── data/
│   └── demo_library.txt
├── Makefile
├── README.md
└── .gitignore
```

### File descriptions

| File or directory | Purpose |
|---|---|
| `src/main.cpp` | Main application, OOP model, catalogue operations, and file persistence |
| `tests/test_library.cpp` | Standalone basic checks for validation and issue/return logic |
| `data/demo_library.txt` | Sample records demonstrating the persistence format |
| `Makefile` | Repeatable build, run, test, and clean targets |
| `README.md` | Project description, build information, and design notes |
| `.gitignore` | Excludes generated executables and local build artefacts from version control |

## 5. Requirements

- A C++ compiler with C++17 support, such as GCC/G++.
- GNU Make for the Makefile commands (optional).
- Linux, macOS, or Windows with a compatible toolchain such as MinGW-w64/MSYS2.

The source uses standard C++17 library facilities and does not require third-party libraries.

## 6. Build and Execution

### Build with G++

From the project root, the application can be compiled with:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -O2 src/main.cpp -o library_app
```

Run the resulting executable on Linux or macOS:

```bash
./library_app
```

On Windows with MinGW, the executable is typically named `library_app.exe`:

```powershell
.\library_app.exe
```

### Build and run with Make

The included Makefile provides equivalent targets:

```bash
make
make run
```

To remove generated executables:

```bash
make clean
```

### Persistence file

The application uses `library_data.txt` by default. A different data-file path can be provided as the first command-line argument, for example:

```bash
./library_app data/demo_library.txt
```

The directory containing the selected file must exist and be writable. When a relative path is used, it is interpreted relative to the current working directory.

## 7. Sample Data Format

Each saved record occupies one line. Fields are separated by a vertical bar (`|`).

```text
BOOK|101|The C++ Programming Language|Bjarne Stroustrup|0
MAGAZINE|201|Science Monthly|12|1
```

The record formats are:

- Book: `BOOK|id|title|author|issuedFlag`
- Magazine: `MAGAZINE|id|title|issueNumber|issuedFlag`

The `issuedFlag` value is `0` for available and `1` for issued. Because the vertical bar is the field delimiter, it should not appear inside a title or author field in this simple storage format.

## 8. Object-Oriented Design

### `LibraryItem`

`LibraryItem` is the abstract base class. It stores the common item ID, title, and issue state. It declares virtual functions for item type and serialization, allowing derived classes to provide their own implementations.

### `Book`

`Book` inherits from `LibraryItem` and adds an author field. Its serialization identifies the record as a book and includes the author information.

### `Magazine`

`Magazine` inherits from `LibraryItem` and adds an issue number. Its serialization identifies the record as a magazine and includes the issue number.

### `Library`

`Library` coordinates catalogue operations, maintains the collection, performs ID lookup, tracks issued IDs, and handles persistence. Shared pointers are used to manage polymorphic items through the base-class interface.

## 9. STL Containers and Their Roles

| Container | Role in the application | Main benefit |
|---|---|---|
| `vector` | Maintains the ordered collection of library items | Preserves catalogue order and supports sequential display |
| `map<int, shared_ptr<LibraryItem>>` | Associates each item ID with its object | Provides ordered key lookup with logarithmic complexity |
| `set<int>` | Tracks IDs of issued items | Keeps IDs unique and supports efficient membership operations |
| `shared_ptr` | Manages the lifetime of polymorphic catalogue objects | Avoids manual ownership and deletion of dynamically allocated objects |

The containers serve different purposes: the vector supports ordered presentation, the map supports identifier-based access, and the set records issue status without duplicate IDs.

## 10. Validation and Error Handling

The application checks common invalid cases before changing the catalogue. These include non-positive IDs, empty titles, duplicate IDs, attempts to issue an unknown item, attempts to issue an already-issued item, and attempts to return an item that is not issued.

These checks help preserve consistent state during normal use. The project uses a simple text file for persistence; it is not designed as a multi-user or transaction-safe library database.

## 11. Automated Checks

Compile and execute the basic test program using:

```bash
make test
```

The test target compiles `tests/test_library.cpp` and runs its assertions. The checks cover valid insertion, duplicate IDs, invalid IDs, empty titles, successful issue and return operations, repeated issue/return attempts, unknown IDs, item counts, and issued-item counts.

Expected successful test message:

```text
All 10 basic unit checks passed.
```

**Test scope note:** `tests/test_library.cpp` is a standalone test harness for the core validation and issue/return rules. It exercises a compact test model of those rules; it is not a full integration test of the interactive application or its file-loading code.

## 12. Complexity Notes

Let \(n\) be the number of catalogue items.

| Operation or structure | Typical complexity | Explanation |
|---|---:|---|
| Append an item to `vector` | Amortized \(O(1)\) | Appending is constant time on average |
| Search a title by scanning the catalogue | \(O(n)\) | Each item may need to be checked |
| Find an ID in `map` | \(O(\log n)\) | The map is an ordered tree-based associative container |
| Insert, find, or erase an ID in `set` | \(O(\log n)\) | The set maintains ordered unique keys |
| Display the complete catalogue | \(O(n)\) | Every item is visited |

These are general container-level estimates. Actual operation cost also depends on string comparisons, output, and file I/O.

## 13. Design Decisions

- **Inheritance:** Shared fields and operations are defined once in `LibraryItem`, while books and magazines retain their type-specific data.
- **Polymorphism:** Virtual methods allow the catalogue to work with different item types through a common interface.
- **STL containers:** Separate containers are used according to the access pattern rather than forcing every operation into one data structure.
- **Text persistence:** A readable, line-based format keeps the project small and makes sample data easy to inspect.
- **Makefile:** Standard targets make compilation, execution, testing, and cleanup repeatable.

## 14. Limitations and Future Enhancements

Current limitations include a simple delimiter-based data format, limited validation of text fields, no authentication, no due dates or fine calculation, no concurrent multi-user access, and a basic assertion-based test harness.

Possible future improvements include stronger input validation, escaping or replacing the delimiter-based format, more extensive integration tests, a database-backed storage layer, user accounts and roles, due dates and overdue tracking, and a graphical or web interface.

## 15. Reproducibility Summary

The project is built using standard C++17 tooling. The Makefile defines the application build, run, test, and clean targets. Sample data is included in `data/demo_library.txt`, and the source code and tests are organised in separate directories. These elements make the project structure and its basic checks straightforward to review.

---

**End of README**
