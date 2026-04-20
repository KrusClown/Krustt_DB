#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <numeric>
#include <functional>
#include <iomanip>
#include <sstream>
#include <thread>
#include <chrono>

// ═══════════════════════════════════════════════════════
//  ANSI COLOR CODES
// ═══════════════════════════════════════════════════════
#define RESET       "\033[0m"
#define BOLD        "\033[1m"
#define DIM         "\033[2m"

#define FG_RED      "\033[31m"
#define FG_GREEN    "\033[32m"
#define FG_YELLOW   "\033[33m"
#define FG_BLUE     "\033[34m"
#define FG_MAGENTA  "\033[35m"
#define FG_CYAN     "\033[36m"
#define FG_WHITE    "\033[37m"

#define FG_BRED     "\033[91m"
#define FG_BGREEN   "\033[92m"
#define FG_BYELLOW  "\033[93m"
#define FG_BBLUE    "\033[94m"
#define FG_BMAGENTA "\033[95m"
#define FG_BCYAN    "\033[96m"
#define FG_BWHITE   "\033[97m"

// Simpsons palette aliases
#define HOMER_YELLOW    BOLD FG_BYELLOW
#define SKY_BLUE        BOLD FG_BCYAN
#define DONUT_PINK      BOLD FG_BMAGENTA
#define GRASS_GREEN     BOLD FG_BGREEN
#define SPRINGFIELD_RED BOLD FG_BRED
#define MARGE_BLUE      BOLD FG_BBLUE
#define ACCENT          BOLD FG_YELLOW
#define DIM_WHITE       DIM  FG_WHITE

// ═══════════════════════════════════════════════════════
//  DATA MODEL
// ═══════════════════════════════════════════════════════
struct Character {
    std::string name;
    int         age;
};

// ═══════════════════════════════════════════════════════
//  MAPREDUCE ENGINE
// ═══════════════════════════════════════════════════════
template <typename T, typename K, typename V>
std::map<K, V> mapReduce(
    const std::vector<T>&                    data,
    std::function<std::pair<K,V>(const T&)>  mapFn,
    std::function<V(V,V)>                    reduceFn)
{
    std::vector<std::pair<K,V>> mapped;
    for (const auto& item : data) mapped.push_back(mapFn(item));

    std::map<K, std::vector<V>> grouped;
    for (const auto& kv : mapped) grouped[kv.first].push_back(kv.second);

    std::map<K,V> result;
    for (auto& [key, vals] : grouped)
        result[key] = std::accumulate(
            std::next(vals.begin()), vals.end(), vals.front(), reduceFn);
    return result;
}

// ═══════════════════════════════════════════════════════
//  ASCII ART
// ═══════════════════════════════════════════════════════
void printSplashArt() {
    std::cout << DONUT_PINK;
    std::cout << "  _  _____  _   _  ___  _____ _____   \n";
    std::cout << " | |/ / _ \\| | | |/ __||_   _|_   _|  \n";
    std::cout << " | ' / |_| | |_| |\\__ \\  | |   | |    \n";
    std::cout << " | . \\|   /|  _  |/ __/  | |   | |    \n";
    std::cout << " |_|\\_\\_|\\_\\|_| |_||____| |_|   |_|    \n";
    std::cout << RESET;
    std::cout << HOMER_YELLOW;
    std::cout << "  ____  ____                           \n";
    std::cout << " |  _ \\| __ )                          \n";
    std::cout << " | | | |  _ \\                          \n";
    std::cout << " | |_| | |_) |                         \n";
    std::cout << " |____/|____/                          \n";
    std::cout << RESET;
    std::cout << DONUT_PINK  << "  ====================================\n";
    std::cout << HOMER_YELLOW << "   Hey Hey!  *  MapReduce Edition     \n";
    std::cout << DONUT_PINK  << "  ====================================\n" << RESET;
    std::cout << "\n";
}

void printHomer() {
    std::cout << HOMER_YELLOW;
    std::cout << "\n";
    std::cout << "         .------.\n";
    std::cout << "        /  O  O  \\\n";
    std::cout << "       |    __    |\n";
    std::cout << "       |  /    \\  |\n";
    std::cout << "        \\________/\n";
    std::cout << "      ___|      |___\n";
    std::cout << "     /   |      |   \\\n";
    std::cout << "    / HOMER'S DATABASE\\\n";
    std::cout << "   /____________________\\\n";
    std::cout << RESET << "\n";
}

void printDonut() {
    std::cout << DONUT_PINK;
    std::cout << "       .------.\n";
    std::cout << "      /        \\\n";
    std::cout << "     /  .----.  \\\n";
    std::cout << "    |  /      \\  |\n";
    std::cout << "    |  |      |  |\n";
    std::cout << "    |  \\      /  |\n";
    std::cout << "     \\  '----'  /\n";
    std::cout << "      \\________/\n";
    std::cout << RESET;
    std::cout << FG_BWHITE << "     Mmm... donuts!\n" << RESET;
}

void printMapReduceDiagram() {
    std::cout << "\n";
    std::cout << SKY_BLUE    << "  +------------------------------------------+\n";
    std::cout                << "  |        MapReduce Pipeline                 |\n";
    std::cout                << "  +------------------------------------------+\n" << RESET;
    std::cout << "\n";
    std::cout << HOMER_YELLOW << "  [Characters]" << RESET;
    std::cout << FG_WHITE     << " --> ";
    std::cout << GRASS_GREEN  << "[MAP]" << RESET;
    std::cout << FG_WHITE     << " --> ";
    std::cout << DONUT_PINK   << "[SHUFFLE]" << RESET;
    std::cout << FG_WHITE     << " --> ";
    std::cout << SPRINGFIELD_RED << "[REDUCE]" << RESET;
    std::cout << FG_WHITE     << " --> ";
    std::cout << SKY_BLUE     << "[Result]\n" << RESET;
    std::cout << "\n";
}

// ═══════════════════════════════════════════════════════
//  UI HELPERS
// ═══════════════════════════════════════════════════════
void clearScreen() { std::cout << "\033[2J\033[H"; }

// Measure visible string length (strip ANSI escape codes)
int visibleLen(const std::string& s) {
    int len = 0; bool esc = false;
    for (char c : s) {
        if (c == '\033') { esc = true; continue; }
        if (esc) { if (c == 'm') esc = false; continue; }
        ++len;
    }
    return len;
}

void printBoxTop(int w = 50) {
    std::cout << SKY_BLUE << "  +" << std::string(w, '-') << "+\n" << RESET;
}
void printBoxBottom(int w = 50) {
    std::cout << SKY_BLUE << "  +" << std::string(w, '-') << "+\n" << RESET;
}
void printBoxDivider(int w = 50) {
    std::cout << SKY_BLUE << "  +" << std::string(w, '-') << "+\n" << RESET;
}
void printBoxRow(const std::string& text, int w = 50) {
    int pad = w - visibleLen(text) - 2;
    if (pad < 0) pad = 0;
    std::cout << SKY_BLUE << "  | " << RESET
              << text
              << std::string(pad, ' ')
              << SKY_BLUE << " |\n" << RESET;
}

void printHeader(const std::string& title) {
    printBoxTop();
    std::string row = std::string(HOMER_YELLOW) + "  * " + RESET
                    + BOLD + FG_BWHITE + title + RESET;
    printBoxRow(row);
    printBoxBottom();
    std::cout << "\n";
}

void waitEnter() {
    std::cout << DIM_WHITE << "\n  [ Press ENTER to continue... ]" << RESET;
    std::cin.ignore(); std::cin.get();
}

// ═══════════════════════════════════════════════════════
//  DATABASE DISPLAY
// ═══════════════════════════════════════════════════════
void printDB(const std::vector<Character>& db) {
    printBoxTop(54);
    std::string title = std::string(HOMER_YELLOW) + "  * " + RESET
                      + BOLD + FG_BWHITE + "THE SIMPSONS DATABASE" + RESET;
    printBoxRow(title, 54);
    printBoxDivider(54);

    std::ostringstream hdr;
    hdr << ACCENT << std::left
        << std::setw(5)  << "ID"
        << std::setw(27) << "Name"
        << std::setw(6)  << "Age" << RESET;
    printBoxRow(hdr.str(), 54);
    printBoxDivider(54);

    for (int i = 0; i < (int)db.size(); ++i) {
        std::ostringstream row;
        std::string nameColor = (i % 2 == 0) ? FG_BWHITE : DIM FG_WHITE;
        std::string ageColor  = (db[i].age < 18) ? GRASS_GREEN
                              : (db[i].age > 60) ? SPRINGFIELD_RED
                              :                    SKY_BLUE;
        row << FG_YELLOW << std::setw(4) << i+1 << "  " << RESET
            << nameColor << std::left << std::setw(27) << db[i].name << RESET
            << ageColor  << std::setw(5) << db[i].age << RESET;
        printBoxRow(row.str(), 54);
    }

    printBoxDivider(54);
    std::ostringstream footer;
    footer << DONUT_PINK << "  * " << RESET
           << FG_WHITE << "Total: " << RESET
           << HOMER_YELLOW << BOLD << db.size() << " characters" << RESET;
    printBoxRow(footer.str(), 54);
    printBoxBottom(54);

    std::cout << "\n  " << GRASS_GREEN  << "[#]" << RESET << FG_WHITE << " Youth(<18)  " << RESET
              <<           SKY_BLUE     << "[#]" << RESET << FG_WHITE << " Adult(18-60)  " << RESET
              <<           SPRINGFIELD_RED << "[#]" << RESET << FG_WHITE << " Senior(60+)\n" << RESET;
    std::cout << "\n";
}

// ═══════════════════════════════════════════════════════
//  MENU ACTIONS
// ═══════════════════════════════════════════════════════
void addCharacter(std::vector<Character>& db) {
    printHeader("ADD NEW CHARACTER");
    Character c;
    std::cout << HOMER_YELLOW << "  > " << RESET << FG_BWHITE << "Name: " << RESET;
    std::cin.ignore();
    std::getline(std::cin, c.name);
    std::cout << HOMER_YELLOW << "  > " << RESET << FG_BWHITE << "Age : " << RESET;
    std::cin >> c.age;
    db.push_back(c);

    std::cout << "\n" << GRASS_GREEN << BOLD;
    std::cout << "  +------------------------------+\n";
    std::cout << "  |  [OK] Character Added!       |\n";
    std::cout << "  |                              |\n";
    std::cout << "  |  " << FG_BWHITE << std::left << std::setw(28) << c.name << GRASS_GREEN << "|\n";
    std::cout << "  |  Age: " << FG_BWHITE << c.age
              << GRASS_GREEN << std::string(23 - (int)std::to_string(c.age).size(), ' ') << "|\n";
    std::cout << "  +------------------------------+\n" << RESET << "\n";
    waitEnter();
}

void searchByName(const std::vector<Character>& db) {
    printHeader("SEARCH CHARACTER");
    std::string query;
    std::cout << HOMER_YELLOW << "  > " << RESET << FG_BWHITE << "Search: " << RESET;
    std::cin.ignore();
    std::getline(std::cin, query);
    std::cout << "\n";

    bool found = false;
    for (const auto& c : db) {
        std::string nl = c.name, ql = query;
        std::transform(nl.begin(), nl.end(), nl.begin(), ::tolower);
        std::transform(ql.begin(), ql.end(), ql.begin(), ::tolower);
        if (nl.find(ql) != std::string::npos) {
            std::cout << GRASS_GREEN << "  [OK] " << RESET
                      << FG_BWHITE  << std::left << std::setw(28) << c.name << RESET
                      << SKY_BLUE   << "Age: " << RESET
                      << HOMER_YELLOW << c.age << RESET << "\n";
            found = true;
        }
    }
    if (!found)
        std::cout << SPRINGFIELD_RED << "  [X] No match for \"" << query << "\".\n" << RESET;
    std::cout << "\n";
    waitEnter();
}

void mapReduceAgeGroups(const std::vector<Character>& db) {
    printMapReduceDiagram();
    printHeader("MapReduce > Count by Age Group");

    auto mapFn = [](const Character& c) -> std::pair<std::string,int> {
        if      (c.age <  5) return {"Baby    ( 0- 4) ", 1};
        else if (c.age < 13) return {"Child   ( 5-12) ", 1};
        else if (c.age < 18) return {"Teen    (13-17) ", 1};
        else if (c.age < 40) return {"Adult   (18-39) ", 1};
        else if (c.age < 65) return {"Middle  (40-64) ", 1};
        else                 return {"Senior  (65+)   ", 1};
    };
    auto reduceFn = [](int a, int b) { return a + b; };
    auto result = mapReduce<Character, std::string, int>(
        db,
        std::function<std::pair<std::string,int>(const Character&)>(mapFn),
        std::function<int(int,int)>(reduceFn));

    int maxCount = 0;
    for (auto& [g, c] : result) maxCount = std::max(maxCount, c);

    printBoxTop(50);
    printBoxRow(std::string(ACCENT) + "  Group             Count  Bar" + RESET, 50);
    printBoxDivider(50);

    std::vector<std::string> barColors = {
        HOMER_YELLOW, SKY_BLUE, GRASS_GREEN, DONUT_PINK, SPRINGFIELD_RED, MARGE_BLUE
    };
    int ci = 0;
    for (auto& [group, count] : result) {
        int barLen = (maxCount > 0) ? (count * 20 / maxCount) : 0;
        std::string bar(barLen, '#');
        std::ostringstream row;
        row << FG_BWHITE << group << RESET << " "
            << HOMER_YELLOW << BOLD << std::setw(3) << count << RESET << "  "
            << barColors[ci % barColors.size()] << bar << RESET;
        printBoxRow(row.str(), 50);
        ++ci;
    }
    printBoxBottom(50);
    std::cout << "\n";
    waitEnter();
}

void mapReduceAverageAge(const std::vector<Character>& db) {
    printMapReduceDiagram();
    printHeader("MapReduce > Average Age");

    auto mapFn    = [](const Character& c) -> std::pair<std::string,int> { return {"all", c.age}; };
    auto reduceFn = [](int a, int b) { return a + b; };
    auto result   = mapReduce<Character, std::string, int>(
        db,
        std::function<std::pair<std::string,int>(const Character&)>(mapFn),
        std::function<int(int,int)>(reduceFn));

    double avg = (double)result["all"] / db.size();
    int barLen  = (int)(avg * 28 / 110.0);

    printBoxTop(44);
    auto r1 = std::string(FG_WHITE)  + "  Characters : " + RESET + HOMER_YELLOW + BOLD + std::to_string(db.size()) + RESET;
    auto r2 = std::string(FG_WHITE)  + "  Total age  : " + RESET + SKY_BLUE    + std::to_string(result["all"]) + RESET;
    std::ostringstream avgStr;
    avgStr << std::fixed << std::setprecision(1) << avg;
    auto r3 = std::string(GRASS_GREEN) + BOLD + "  Average age: " + RESET + DONUT_PINK + BOLD + avgStr.str() + " yrs" + RESET;
    printBoxRow(r1, 44);
    printBoxRow(r2, 44);
    printBoxDivider(44);
    printBoxRow(r3, 44);
    printBoxDivider(44);
    std::string bar = "  " + std::string(HOMER_YELLOW) + std::string(barLen, '=') + RESET
                    + std::string(DIM_WHITE) + std::string(28 - barLen, '-') + RESET
                    + " " + avgStr.str();
    printBoxRow(bar, 44);
    printBoxBottom(44);
    std::cout << "\n";
    waitEnter();
}

void mapReduceOldestYoungest(const std::vector<Character>& db) {
    printMapReduceDiagram();
    printHeader("MapReduce > Oldest & Youngest");

    auto mapMax   = [](const Character& c) -> std::pair<std::string,int> { return {"max", c.age}; };
    auto mapMin   = [](const Character& c) -> std::pair<std::string,int> { return {"min", c.age}; };
    auto reduceMax = [](int a, int b) { return std::max(a,b); };
    auto reduceMin = [](int a, int b) { return std::min(a,b); };

    auto rMax = mapReduce<Character,std::string,int>(db,
        std::function<std::pair<std::string,int>(const Character&)>(mapMax),
        std::function<int(int,int)>(reduceMax));
    auto rMin = mapReduce<Character,std::string,int>(db,
        std::function<std::pair<std::string,int>(const Character&)>(mapMin),
        std::function<int(int,int)>(reduceMin));

    int maxAge = rMax["max"], minAge = rMin["min"];

    printBoxTop(46);
    printBoxRow(std::string(ACCENT) + "  Role         Name                   Age" + RESET, 46);
    printBoxDivider(46);

    for (const auto& c : db) {
        if (c.age == maxAge) {
            std::ostringstream r;
            r << SPRINGFIELD_RED << "  [OLDEST]  " << RESET
              << FG_BWHITE << std::left << std::setw(22) << c.name << RESET
              << SPRINGFIELD_RED << BOLD << c.age << RESET;
            printBoxRow(r.str(), 46);
        }
    }
    for (const auto& c : db) {
        if (c.age == minAge) {
            std::ostringstream r;
            r << GRASS_GREEN << "  [YOUNGEST]" << RESET
              << FG_BWHITE << std::left << std::setw(22) << c.name << RESET
              << GRASS_GREEN << BOLD << c.age << RESET;
            printBoxRow(r.str(), 46);
        }
    }
    printBoxBottom(46);
    std::cout << "\n";
    waitEnter();
}

void deleteCharacter(std::vector<Character>& db) {
    printDB(db);
    int id;
    std::cout << HOMER_YELLOW << "  > " << RESET << FG_BWHITE
              << "Enter ID to delete (0 to cancel): " << RESET;
    std::cin >> id;
    if (id < 1 || id > (int)db.size()) {
        std::cout << SPRINGFIELD_RED << "  [X] Cancelled / invalid ID.\n" << RESET;
        waitEnter();
        return;
    }
    std::string removed = db[id-1].name;
    db.erase(db.begin() + id - 1);
    std::cout << SPRINGFIELD_RED << "\n  [X] \"" << removed
              << "\" removed from Springfield!\n" << RESET;
    waitEnter();
}

// ═══════════════════════════════════════════════════════
//  MENU
// ═══════════════════════════════════════════════════════
void printMenu(const std::vector<Character>& db) {
    clearScreen();
    printSplashArt();

    printBoxTop(44);
    auto rec = std::string(DIM_WHITE) + "  Records in DB: " + RESET
             + HOMER_YELLOW + BOLD + std::to_string(db.size()) + RESET;
    printBoxRow(rec, 44);
    printBoxDivider(44);

    auto opt = [](const std::string& num, const std::string& icon, const std::string& label) {
        return std::string(HOMER_YELLOW) + "  " + num + ". " + RESET
             + icon + " " + FG_BWHITE + label + RESET;
    };

    printBoxRow(opt("1", "[LIST]",   "View all characters"),  44);
    printBoxRow(opt("2", "[ +  ]",   "Add a character"),       44);
    printBoxRow(opt("3", "[ X  ]",   "Delete a character"),    44);
    printBoxRow(opt("4", "[FIND]",   "Search by name"),         44);
    printBoxDivider(44);
    printBoxRow(std::string(DONUT_PINK) + "  -- MapReduce Analytics --" + RESET, 44);
    printBoxDivider(44);
    printBoxRow(opt("5", "[BAR ]",   "Count by age group"),    44);
    printBoxRow(opt("6", "[AVG ]",   "Average age"),            44);
    printBoxRow(opt("7", "[TOP ]",   "Oldest & Youngest"),      44);
    printBoxDivider(44);
    printBoxRow(opt("0", "[EXIT]",   "Exit"),                   44);
    printBoxBottom(44);

    std::cout << "\n" << HOMER_YELLOW << "  > " << RESET
              << FG_BWHITE << "Your choice: " << RESET;
}

// ═══════════════════════════════════════════════════════
//  MAIN
// ═══════════════════════════════════════════════════════
int main() {
    std::vector<Character> db = {
        // The Simpson Family
        {"Homer Simpson",           39},
        {"Marge Simpson",           36},
        {"Bart Simpson",            10},
        {"Lisa Simpson",             8},
        {"Maggie Simpson",           1},
        {"Grampa Simpson",          83},
        {"Patty Bouvier",           43},
        {"Selma Bouvier",           43},
        {"Herb Powell",             41},
        {"Mona Simpson",            65},
        {"Sideshow Bob",            38},
        // Neighbors & Friends
        {"Ned Flanders",            60},
        {"Rod Flanders",            10},
        {"Todd Flanders",            8},
        {"Maude Flanders",          46},
        {"Timothy Lovejoy",         50},
        {"Helen Lovejoy",           48},
        {"Jessica Lovejoy",         10},
        // School
        {"Principal Skinner",       44},
        {"Edna Krabappel",          42},
        {"Milhouse Van Houten",     10},
        {"Nelson Muntz",            10},
        {"Martin Prince",           10},
        {"Ralph Wiggum",            10},
        {"Sherri",                  10},
        {"Terri",                   10},
        {"Database",                10},
        {"Jimbo Jones",             13},
        {"Kearney Zzyzwicz",        14},
        {"Dolph Starbeam",          13},
        {"Miss Hoover",             38},
        {"Lunchlady Doris",         55},
        {"Superintendent Chalmers", 52},
        // Springfield figures
        {"Montgomery Burns",       104},
        {"Waylon Smithers",         42},
        {"Chief Wiggum",            43},
        {"Apu Nahasapeemapetilon",  42},
        {"Moe Szyslak",             46},
        {"Barney Gumble",           40},
        {"Lenny Leonard",           38},
        {"Carl Carlson",            38},
        {"Kent Brockman",           45},
        {"Dr. Nick Riviera",        38},
        {"Professor Frink",         50},
        {"Fat Tony",                48},
        {"Louie",                   40},
        {"Legs",                    40},
        {"Lionel Hutz",             42},
        {"Troy McClure",            45},
        {"Squeaky Voiced Teen",     16},
        {"Bumblebee Man",           38},
        {"Dr. Julius Hibbert",      46},
        {"Cookie Kwan",             38},
        {"Gil Gunderson",           55},
        {"Sideshow Raheem",         35},
        // Entertainment
        {"Krusty the Clown",        52},
        {"Sideshow Mel",            48},
        {"Rainier Wolfcastle",      50},
        {"Luann Van Houten",        36},
        {"Kirk Van Houten",         38},
        // Politics & Power
        {"Mayor Quimby",            60},
        {"Agnes Skinner",           80},
        {"Brandine Spuckler",       30},
        {"Cletus Spuckler",         32},
        // Others
        {"Groundskeeper Willie",    42},
        {"Otto Mann",               30},
        {"Snake Jailbird",          32},
        {"Hans Moleman",            75},
        {"Comic Book Guy",          45},
        {"Disco Stu",               42},
        {"Patty Bouvier",           43},
        {"Arnie Pye",               47},
        {"Duffman",                 35},
        {"Radioactive Man",         40},
        {"Fallout Boy",             12},
        {"Santa's Little Helper",    8},
        {"Snowball II",              3},
        // Extended Family & Relatives
        {"Sideshow Bob",            38},
        {"Cecil Terwilliger",       36},
        {"Robert Underdunk Terwilliger Sr.", 70},
        {"Frank Grimes",            35},
        {"Frank Grimes Jr.",        16},
        {"Mindy Simmons",           28},
        {"Karl",                    35},
        {"Lenny's Wife",            37},
        // recurring Springfield Adults
        {"Artie Ziff",              39},
        {"Patty and Selma boss",    55},
        {"Wendell Borton",          10},
        {"Richard",                 10},
        {"Lewis Clark",             10},
        {"Üter Zorker",             10},
        {"Alison Taylor",           10},
        {"Alex Whitney",            10},
        // Religion & Community
        {"Reverend Lovejoy",        50},
        {"Ned Flanders Sr.",        75},
        {"Beatrice Simmons",        82},
        // Crime & Law
        {"Eddie",                   40},
        {"Lou",                     40},
        {"Judge Snyder",            58},
        {"Blue-Haired Lawyer",      50},
        {"Freddy Quimby",           22},
        // Media & Culture
        {"Brockman Anchor",         45},
        {"Smooth Jimmy Apollo",     42},
        {"Bill",                    38},
        {"Marty",                   36},
        {"Scott Christian",         40},
        // Business & Industry
        {"Lyle Lanley",             45},
        {"Hank Scorpio",            40},
        {"Don Vittorio",            70},
        {"Rex Banner",              42},
        {"Russ Cargill",            50},
        // Historical & Legendary Springfield
        {"Jebediah Springfield",    60},
        {"Shelbyville Manhattan",   60},
        // Kids (extended)
        {"Bart's Elephant Stampy",  2},
        {"Patches",                 10},
        {"Poor Violet",             10},
        {"Brendan Dorsey",          10},
        {"Weasel",                  13},
        // Celebrities & One-timers
        {"Michael Jackson (Leon)",  35},
        {"Conan O'Brien",           30},
        {"Bob Hope",                90},
        {"Hugh Jass",               45},
        {"Larry Burns",             45},
        {"Surly Duff",              35},
        {"Duff Gardner",            40},
        // Animals & Pets
        {"Pinchy the Lobster",       1},
        {"Duncan the Horse",         8},
        {"Princess the Poodle",      4},
        // More Springfield Residents
        {"Herman Hermann",          50},
        {"Sanjay Nahasapeemapetilon",38},
        {"Dr. Marvin Monroe",       50},
        {"Lionel Hutz",             42},
        {"Dave Shutton",            45},
        {"Allison Taylor",          10},
        {"Elizabeth Hoover",        38},
        {"Agnes Skinner",           80},
        {"Brandine Spuckler",       30},
        {"Cletus Spuckler",         32},
        {"Baby Gerald",              1},
        {"Gavin",                    8},
        {"Database",                10},
        // International Characters
        {"Adil Hoxha",              10},
        {"Bart's Penpal Cesar",     10},
        {"Ugolin",                  40},
        {"Larry the Barfly",        45},
        // More Criminals
        {"Big House",               45},
        {"Don Brodka",              40},
        {"Remy Beauregard Brandine", 8},
        {"The Rich Texan",          55},
        {"Cowboy Bob",              50},
        // Springfield Elementary Staff
        {"Otto's Bus",               0},
        {"Jimbo's Mom",             38},
        {"Brandine's Baby",          0},
        // TV & Pop Culture (in-universe)
        {"Itchy",                    5},
        {"Scratchy",                 5},
        {"Poochie",                  3},
        {"Worker and Parasite",      5},
        {"Stampy the Elephant",      4},
        // More Adults
        {"Patty's Girlfriend",      43},
        {"Sideshow Bob's Wife",     35},
        {"Francesca Terwilliger",    34},
        {"Gino Terwilliger",          2},
        {"Manjula Nahasapeemapetilon",36},
        {"Sanjay's Wife",           36},
        // Power Plant Workers
        {"Charlie",                 40},
        {"Lenny's Bowling Team",    38},
        {"Julio",                   35},
        {"Grady",                   34},
        // Retirement Castle
        {"Jasper Beardley",         82},
        {"Old Jewish Man",          80},
        {"Crazy Old Cat Lady",      70},
        {"Leon Kompowsky",          35},
        // Famous Visitors to Springfield
        {"Larry Burns",             45},
        {"Sideshow Luke Perry",     35},
        {"John Waters",             45},
        {"Patrick Farrelly",        40},
        // Season characters
        {"Arnie Pye in the Sky",    47},
        {"Wendell's Dad",          42},
        {"Chester J. Lampwick",     65},
        {"Brad Goodman",            45},
        {"Lester",                  10},
        {"Eliza",                   10},
        // Ancillary Springfield
        {"Luigi Risotto",           50},
        {"Sven Golly",              42},
        {"Bleeding Gums Murphy",    45},
        {"Joshua",                  10},
        {"Sheldon Skinner",         20},
        // More Springfield Elementary Kids
        {"Becky",                   10},
        {"Nikki McKenna",           10},
        {"Laura Powers",            13},
        {"Jessica Lovejoy",         10},
        {"Sherri",                  10},
        {"Terri",                   10},
        {"Tina",                    10},
        {"Gordie",                  10},
        {"Freddie",                 10},
        {"Warren",                  10},
        {"Wendell Borton",          10},
        {"Patches",                 10},
        {"Poor Violet",             10},
        // More Flanders Family
        {"Ned Flanders Sr.",        75},
        {"Nedsel Flanders",         78},
        {"Marge's Mom Jacqueline",  72},
        {"Clancy Bouvier",          74},
        // More Government & Law
        {"Bob Arnold",              55},
        {"Sideshow Bob's Lawyer",  50},
        {"Judge Constance Harm",    52},
        {"Alex",                    38},
        {"Brandine's Mom",         55},
        // Springfield Church & Community
        {"Reverend Lovejoy Sr.",    78},
        {"Lenny's Priest",         62},
        {"Sister Mary-Elephant",    55},
        // Bars & Nightlife
        {"Sam",                     48},
        {"Larry",                   50},
        {"Barney's Girlfriend",    38},
        {"Lurleen Lumpkin",         30},
        {"Princess Kashmir",        25},
        {"Belle",                   35},
        // Sports & Competition
        {"Ronaldo",                 28},
        {"Don Mattingly",           35},
        {"Darryl Strawberry",       32},
        {"Jose Canseco",            33},
        {"Wade Boggs",              36},
        {"Roger Clemens",           34},
        {"Mike Scioscia",           35},
        {"Steve Sax",               32},
        // Medical
        {"Dr. Zweig",               50},
        {"Dr. Riviera's Nurse",    35},
        {"Nurse",                   40},
        {"Dr. Foster",              52},
        // Food & Service Industry
        {"Squeaky Voiced Teen",     16},
        {"Pimple-Faced Teen",       16},
        {"Donut Shop Owner",        45},
        {"Kwik-E-Mart Robber",      28},
        // More One-Episode Legends
        {"Llewellyn Sinclair",      48},
        {"Don Brodka",              40},
        {"Sherry Bobbins",          30},
        {"Adil Hoxha",              10},
        {"Bart's Robot Buddy",     10},
        {"Cooder",                  35},
        {"Spud",                    35},
        {"Baron von Kissalot",      40},
        // Treehouse of Horror specials
        {"Kang",                   500},
        {"Kodos",                  500},
        {"Serak the Preparer",     500},
        {"Pazuzu",                 999},
        {"The Devil Ned Flanders",  62},
        {"Count Burns",            104},
        // Future / Alternate Universe
        {"Adult Bart",              40},
        {"Adult Lisa",              38},
        {"Adult Maggie",            30},
        {"Zia Simpson",             12},
        {"Picard Simpson",          10},
        // Extended Bouvier family
        {"Bambi Bouvier",           40},
        {"Aunt Hortense",           70},
        // More Celebrities in-universe
        {"Michael Jackson (Voice)", 35},
        {"James Brown",             55},
        {"Aerosmith",               48},
        {"The Red Hot Chili Peppers",35},
        {"Barry White",             52},
        {"Tom Jones",               55},
        {"Tony Bennett",            68},
        {"Linda Ronstadt",          48},
        {"Hugh Hefner",             70},
        // Misc unique residents
        {"Sideshow Raheem",         35},
        {"Gabbo",                   10},
        {"Arthur Crandall",         50},
        {"Leopold",                 45},
        {"Lunch Lady Doris",        55},
        {"Dr. J. Loren Pryor",      52},
        {"Brandine's Kids",         8},
        {"Baby Oscar",               1},
        {"Amber Simpson",           28},
        {"Amber Dempsey",           10},

        // === INTERNATIONAL / TRAVEL EPISODES ===
        {"Akira",                   35},
        {"Miss Matsumura",          30},
        {"Greta Wolfcastle",        10},
        {"Costas Becker",           45},
        {"Declan Desmond",          42},
        {"Tobias Drundridge",       38},
        {"Brendan",                 10},
        {"Angel",                   10},
        {"Pepito",                  10},
        {"Marge's French Lover",   38},
        {"Sylvia Winfield",         45},
        {"Jim Winfield",            48},
        {"Bart's Australian Host", 45},
        {"Tobias Drundridge Jr.",    8},
        {"Tokyo Host Family Dad",   42},
        {"Tokyo Host Family Mom",   40},
        {"African Safari Guide",    38},
        {"Rio Tour Guide",          35},
        {"Paris Chef",              50},
        {"London Bobby",            40},

        // === HISTORICAL FIGURES (Simpsons versions) ===
        {"Abraham Lincoln",         56},
        {"George Washington",       67},
        {"Benjamin Franklin",       70},
        {"Richard Nixon",           81},
        {"Elvis Presley",           42},
        {"Babe Ruth",               53},
        {"Albert Einstein",         76},
        {"Thomas Edison",           65},
        {"Nikola Tesla",            60},
        {"William Shakespeare",     52},
        {"Julius Caesar",           55},
        {"Napoleon Bonaparte",      51},
        {"Leonardo da Vinci",       67},
        {"Christopher Columbus",    54},

        // === MORE CELEBRITY CAMEOS ===
        {"Stephen Hawking",         76},
        {"Alec Baldwin",            45},
        {"Kim Basinger",            45},
        {"Ron Howard",              48},
        {"Simon Cowell",            44},
        {"Ricky Gervais",           44},
        {"Anne Hathaway",           30},
        {"Seth Rogen",              30},
        {"Kelsey Grammer",          50},
        {"Albert Brooks (Brad)",    50},
        {"Patrick Stewart",         60},
        {"Mel Gibson",              45},
        {"Pierce Brosnan",          52},
        {"Michelle Pfeiffer",       45},
        {"Jon Hamm",                42},
        {"Tina Fey",                38},
        {"Ed Sheeran",              25},
        {"Lady Gaga",               27},
        {"Elon Musk (parody)",      45},
        {"Mark Zuckerberg (parody)",35},

        // === MORE TREEHOUSE OF HORROR ===
        {"Zombie Flanders",         62},
        {"Homerzilla",              39},
        {"King Homer",              39},
        {"Witch Marge",             36},
        {"Robot Homer",             39},
        {"The Shinning Homer",      39},
        {"Dracula Burns",          104},
        {"Werewolf Flanders",       62},
        {"Frankenstein Homer",      39},
        {"Space Mutant Lisa",        8},
        {"Zombie Bart",             10},
        {"Devil Flanders",          62},
        {"Gremlin on Plane",        20},
        {"The Jury of the Damned",  80},
        {"Paul Anka",               45},
        {"Giant Lard Lad",          25},
        {"Evil Krusty Doll",        10},
        {"Groundskeeper Willie Ghost",42},
        {"Nightmare Babysitter",    25},
        {"The Strangler",           40},

        // === SPRINGFIELD UNIVERSITY ===
        {"Dean Bobby Peterson",     55},
        {"Professor John Frink Sr.",80},
        {"Boobarella",              28},
        {"College Roommate 1",      18},
        {"College Roommate 2",      18},
        {"College Roommate 3",      18},
        {"Professor Lombardo",      50},
        {"Professor Peyton",        52},
        {"Tad",                     20},
        {"Noel",                    20},
        {"Jojo",                    20},
        {"Stubby",                  20},

        // === EMERGENCY SERVICES ===
        {"Fire Chief Clancy",       48},
        {"Firefighter 1",           35},
        {"Firefighter 2",           36},
        {"Firefighter 3",           34},
        {"Paramedic 1",             32},
        {"Paramedic 2",             30},
        {"Officer Marge",           36},
        {"Officer Steve",           38},
        {"Prison Guard",            40},
        {"SWAT Team Leader",        42},
        {"Detective Don Brodka",    40},

        // === CIRCUS & CARNIVAL ===
        {"Freak Show Barker",       50},
        {"The Bearded Lady",        35},
        {"Conjoined Twin 1",        30},
        {"Conjoined Twin 2",        30},
        {"Tom the Sword Swallower", 40},
        {"Lobster Boy",             28},
        {"The Strong Man",          32},
        {"Carnie 1",                38},
        {"Carnie 2",                36},
        {"Carnie 3",                34},
        {"The Clown (not Krusty)",  45},
        {"Tatum",                   30},
        {"The Acrobat",             25},

        // === MORE RETIREMENT CASTLE ===
        {"Beatrice Simmons",        82},
        {"Morty",                   80},
        {"Shlomo",                  83},
        {"Cornelius Chapman",       85},
        {"Nate",                    79},
        {"Charley",                 81},
        {"Lionel Budgie",           84},
        {"Arnie",                   80},
        {"Old Jewish Man 2",        78},
        {"Cat Lady Resident",       72},
        {"Retirement Nurse",        45},
        {"Retirement Doctor",       50},
        {"Ida",                     76},
        {"Mabel",                   80},

        // === SHELBYVILLE CHARACTERS ===
        {"Shelbyville Kid 1",       10},
        {"Shelbyville Kid 2",       10},
        {"Shelbyville Kid 3",       10},
        {"Shelbyville Principal",   46},
        {"Shelbyville Bully",       13},
        {"Shelbyville Milhouse",    10},
        {"Shelbyville Nelson",      10},
        {"Shelbyville Mayor",       58},
        {"Shelbyville Apu",         42},
        {"Shelbyville Ned",         60},

        // === MORE ANIMALS ===
        {"Stampy the Elephant",      4},
        {"Duncan the Horse",         8},
        {"Pinchy the Lobster",       1},
        {"Furious D",                5},
        {"Stinky the Rabbit",        3},
        {"Rex the Dog",              4},
        {"Sprinkles the Hamster",    2},
        {"Ladybug the Cat",          3},
        {"Jub-Jub",                  5},
        {"Spider-Pig",               1},
        {"Plopper the Pig",          1},
        {"Babe the Sheep",           2},
        {"Bart's Frog Froggy",      1},
        {"Nibbles the Guinea Pig",   1},
        {"Lolo the Koala",           3},
    };

    // Splash screen
    clearScreen();
    printHomer();
    std::cout << HOMER_YELLOW << "\n  Loading Springfield...\n" << RESET;
    std::this_thread::sleep_for(std::chrono::milliseconds(900));
    printDonut();
    std::this_thread::sleep_for(std::chrono::milliseconds(700));

    int choice = -1;
    while (true) {
        printMenu(db);
        std::cin >> choice;
        std::cout << "\n";

        switch (choice) {
            case 1: clearScreen(); printDB(db);              waitEnter(); break;
            case 2: clearScreen(); addCharacter(db);                      break;
            case 3: clearScreen(); deleteCharacter(db);                   break;
            case 4: clearScreen(); searchByName(db);                      break;
            case 5: clearScreen(); mapReduceAgeGroups(db);                break;
            case 6: clearScreen(); mapReduceAverageAge(db);               break;
            case 7: clearScreen(); mapReduceOldestYoungest(db);           break;
            case 0:
                clearScreen();
                std::cout << HOMER_YELLOW;
                std::cout << "\n  +==============================+\n";
                std::cout <<   "  |                              |\n";
                std::cout <<   "  |   D'oh!  See you later!      |\n";
                std::cout <<   "  |       ~ Homer Simpson ~       |\n";
                std::cout <<   "  |                              |\n";
                std::cout <<   "  +==============================+\n" << RESET;
                std::cout << "\n";
                return 0;
            default:
                std::cout << SPRINGFIELD_RED
                          << "  [X] Invalid option. Try again!\n" << RESET;
                std::this_thread::sleep_for(std::chrono::milliseconds(700));
        }
    }
}