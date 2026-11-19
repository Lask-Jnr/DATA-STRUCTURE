#include <iostream>
using namespace std;

const int MAX_STR = 50;

int strLen(const char* s) {
    int n = 0;
    while (s[n] != '\0') n++;
    return n;
}

void strCopy(char* dest, const char* src) {
    int i = 0;
    while (src[i] != '\0' && i < MAX_STR - 1) {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

char toLowerChar(char c) {
    return (c >= 'A' && c <= 'Z') ? (char)(c + 32) : c;
}

bool strEqualIgnoreCase(const char* a, const char* b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (toLowerChar(a[i]) != toLowerChar(b[i])) return false;
        i++;
    }
    return a[i] == b[i];
}

struct Student {
    char id[MAX_STR];
    char name[MAX_STR];
    char programme[MAX_STR];
    char course[MAX_STR];
    double score;
    char grade[3];
    Student* next;
};

// ---------- Result Management System (linked list) ----------
class ResultSystem {
private:
    Student* head;
    int count;

    static void computeGrade(double score, char* grade) {
        const char* grde;
        if (score >= 80)      grde = "A";
        else if (score >= 75) grde = "B+";
        else if (score >= 70) grde= "B";
        else if (score >= 65) grde = "C+";
        else if (score >= 60) grde = "C";
        else if (score >= 55) grde = "D+";
        else if (score >= 50) grde = "D";
        else                  grde = "E";
        int i = 0;
        while (grde[i] != '\0') { grade[i] = grde[i]; i++; }
        grade[i] = '\0';
    }

    // Finds a record by student ID and course (a student may take many courses)
    Student* findRecord(const char* id, const char* course) {
        Student* cur = head;
        while (cur != nullptr) {
            if (strEqualIgnoreCase(cur->id, id) &&
                strEqualIgnoreCase(cur->course, course))
                return cur;
            cur = cur->next;
        }
        return nullptr;
    }

    static void printHeader() {
        cout << "\n" << "ID\t\tName\t\tProgramme\tCourse\t\tScore\tGrade\n";
        cout << "------------------------------------------------------------------------------\n";
    }

    static void printRecord(const Student* s) {
        cout << s->id << "\t\t" << s->name << "\t\t" << s->programme << "\t\t"
             << s->course << "\t\t" << s->score << "\t" << s->grade << "\n";
    }

public:
    ResultSystem() : head(nullptr), count(0) {}

    ~ResultSystem() {
        while (head != nullptr) {
            Student* tmp = head;
            head = head->next;
            delete tmp;
        }
    }

    // 1. Add student
    bool addStudent(const char* id, const char* name, const char* programme,
                    const char* course, double score) {
        if (score < 0 || score > 100) {
            cout << "Invalid score. Must be between 0 and 100.\n";
            return false;
        }
        if (findRecord(id, course) != nullptr) {
            cout << "A record for this student and course already exists.\n";
            return false;
        }
        Student* node = new Student;
        strCopy(node->id, id);
        strCopy(node->name, name);
        strCopy(node->programme, programme);
        strCopy(node->course, course);
        node->score = score;
        computeGrade(score, node->grade);
        node->next = nullptr;

        // append at tail to keep insertion order
        if (head == nullptr) {
            head = node;
        } else {
            Student* cur = head;
            while (cur->next != nullptr) cur = cur->next;
            cur->next = node;
        }
        count++;
        cout << "Student record added successfully.\n";
        return true;
    }

    // 2. Display results
    void displayResults() {
        if (head == nullptr) {
            cout << "No records available.\n";
            return;
        }
        printHeader();
        for (Student* cur = head; cur != nullptr; cur = cur->next)
            printRecord(cur);
    }

    // 3. Search student (by ID, shows all courses)
    void searchStudent(const char* id) {
        bool found = false;
        for (Student* cur = head; cur != nullptr; cur = cur->next) {
            if (strEqualIgnoreCase(cur->id, id)) {
                if (!found) printHeader();
                printRecord(cur);
                found = true;
            }
        }
        if (!found) cout << "Student with ID " << id << " not found.\n";
    }

    // 4. Update score (grade is recalculated)
    void updateScore(const char* id, const char* course, double newScore) {
        if (newScore < 0 || newScore > 100) {
            cout << "Invalid score. Must be between 0 and 100.\n";
            return;
        }
        Student* rec = findRecord(id, course);
        if (rec == nullptr) {
            cout << "Record not found.\n";
            return;
        }
        rec->score = newScore;
        computeGrade(newScore, rec->grade);
        cout << "Score updated. New grade: " << rec->grade << "\n";
    }

    // 5. Calculate average
    void calculateAverage() {
        if (head == nullptr) {
            cout << "No records available.\n";
            return;
        }
        double sum = 0;
        for (Student* cur = head; cur != nullptr; cur = cur->next)
            sum += cur->score;
        cout << "Average score: " << sum / count << " (from " << count << " records)\n";
    }

    // 6. Identify highest score
    void highestScore() {
        if (head == nullptr) {
            cout << "No records available.\n";
            return;
        }
        double max = head->score;
        for (Student* cur = head; cur != nullptr; cur = cur->next)
            if (cur->score > max) max = cur->score;

        cout << "Highest score: " << max << "\n";
        printHeader();
        for (Student* cur = head; cur != nullptr; cur = cur->next)
            if (cur->score == max) printRecord(cur);   // handles ties
    }

    // 7. Identify lowest score
    void lowestScore() {
        if (head == nullptr) {
            cout << "No records available.\n";
            return;
        }
        double min = head->score;
        for (Student* cur = head; cur != nullptr; cur = cur->next)
            if (cur->score < min) min = cur->score;

        cout << "Lowest score: " << min << "\n";
        printHeader();
        for (Student* cur = head; cur != nullptr; cur = cur->next)
            if (cur->score == min) printRecord(cur);
    }
};

// ---------- Input helpers ----------
void readLine(const char* prompt, char* buf) {
    cout << prompt;
    cin.getline(buf, MAX_STR);
}

double readScore(const char* prompt) {
    double s;
    cout << prompt;
    while (!(cin >> s)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Please enter a number: ";
    }
    cin.ignore(1000, '\n');
    return s;
}

int main() {
    ResultSystem sys;
    int choice;

    do {
        cout << "\n===== STUDENT RESULT MANAGEMENT SYSTEM =====\n"
             << "1. Add student\n"
             << "2. Display results\n"
             << "3. Search student\n"
             << "4. Update score\n"
             << "5. Calculate average\n"
             << "6. Identify highest score\n"
             << "7. Identify lowest score\n"
             << "0. Exit\n"
             << "Choice: ";
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            choice = -1;
        } else {
            cin.ignore(1000, '\n');
        }

        char id[MAX_STR], name[MAX_STR], prog[MAX_STR], course[MAX_STR];

        switch (choice) {
        case 1: {
            readLine("Student ID: ", id);
            readLine("Name: ", name);
            readLine("Programme: ", prog);
            readLine("Course: ", course);
            double score = readScore("Score (0-100): ");
            sys.addStudent(id, name, prog, course, score);
            break;
        }
        case 2:
            sys.displayResults();
            break;
        case 3:
            readLine("Enter Student ID: ", id);
            sys.searchStudent(id);
            break;
        case 4: {
            readLine("Student ID: ", id);
            readLine("Course: ", course);
            double score = readScore("New score (0-100): ");
            sys.updateScore(id, course, score);
            break;
        }
        case 5:
            sys.calculateAverage();
            break;
        case 6:
            sys.highestScore();
            break;
        case 7:
            sys.lowestScore();
            break;
        case 0:
            cout << "Goodbye!\n";
            break;
        default:
            cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 0);

    return 0;
}
