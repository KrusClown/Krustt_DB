# Krustt_DB
A colorful C++17 console application that manages a database of The Simpsons characters using a custom MapReduce engine — built entirely for the terminal with ANSI colors and ASCII art.


## Technologies Used

<p align="center">
  <img src="https://skillicons.dev/icons?i=cpp,linux,bash,vscode,hadoop,github&perline=6" />
</p>

  # MapReduce Architecture
  [Characters] --> [MAP] --> [SHUFFLE] --> [REDUCE] --> [Result]


# Getting Started
Requirements

Linux / macOS / WSL (Windows Subsystem for Linux)
g++ with C++17 support


# Compile & Run
g++ -std=c++17 -O2 -o krustt_db krustt_db.cpp
./krusty_db


# Menu Options
  1. [LIST]  View all characters
  2. [ +  ]  Add a character
  3. [ X  ]  Delete a character
  4. [FIND]  Search by name
  ── MapReduce Analytics ──
  5. [BAR ]  Count by age group
  6. [AVG ]  Average age
  7. [TOP ]  Oldest & Youngest
  0. [EXIT]  Exit



#Project Structure
```text
krusty_db/
├── krusty_db.cpp      # Single-file C++ source
└── README.md          # This file

