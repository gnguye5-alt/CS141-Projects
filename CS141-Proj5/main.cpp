/* CS 141 Program 5: The Scrambler
Completed by Hayley Nguyen
*/
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
using namespace std;

class Scrambler {
    private:
        vector<vector<char> > board; // board character hold
        vector<vector<char> > boardIninitial; // hold the initial of the board 
        vector<vector<char> > boardSolution; // for display solution
        vector<string> boardWords; // hold words in the board
        vector<string> jumbleMove; // hold move that were jumble
        int size; // size of the board
        vector<string> words; // hold all words in dictionary
        // private member help jumbling the board
        void jumbleBoard() {
            int x = rand() % 8;
            int nmoves = x + 3;
            for (int i = 1; i <= nmoves; i++) {
                int idx = rand() % this->size;
                if (i % 2 == 0) {
                    // Shift column
                    int dir = rand() % 2 == 0 ? -1 : 1;
                    vector<char> tmp;
                    for (int j = 0; j < size; j++) {
                        tmp.push_back(board[j][idx]);
                    }
                    for (int j = 0; j < size; j++) {
                        int k = (j + dir + size) % size;
                        board[j][idx] = tmp[k];
                    }
                    string move = "c" + to_string(idx + 1) + (dir == -1 ? "u" : "d");
                    jumbleMove.push_back(move);
                } else {
                    // Shift row
                    int dir = rand() % 2 == 0 ? -1 : 1;
                    vector<char> tmp;
                    for (int j = 0; j < size; j++) {
                        tmp.push_back(board[idx][j]);
                    }
                    for (int j = 0; j < size; j++) {
                        int k = (j + dir + size) % size;
                        board[idx][j] = tmp[k];
                    }
                    string move = "r" + to_string(idx + 1) + (dir == -1 ? "l" : "r");
                    jumbleMove.push_back(move);
                }
            }
        }

    public:
        // class Scrambler needs
        // a constructor that accepts two arguments, first string which represents
        // the path to the file containing all the
        // words that could appear in a game, second is the size of the grid which is assumed to be 3 ≤ X ≤ 9.
        Scrambler(const string& path, const int size) {
            this -> size = size;
            // read in the words from dictionary
            ifstream f(path);
            string word;
            while (f >> word) {
                words.push_back(word);
            }
            f.close();

            // resize board with size
            board.resize(size, vector<char>(size, ' '));
            
            // Seed random number generator
            srand(time(nullptr));

            // Fill each row with words
            for (int row = 0; row < size; row++) {
                int remaining_space = size;
                int total_space = size;
                while (total_space > 2) {
                    // Pick a random word from the list of words
                    int word_idx = rand() % words.size();
                    string word = words[word_idx];

                    // Check if the word fits in the remaining space
                    if (word.size() <= remaining_space) {
                        // Insert word into row
                        int word_pos = size - remaining_space;
                        if (word_pos > 0) {
                            if (word.size() + 1 <= remaining_space) {
                                board[row][word_pos] = ' ';
                                word_pos++;
                                total_space--;

                                for (char c : word) {
                                    board[row][word_pos++] = c;
                                }
                                boardWords.push_back(word);
                                total_space -= word.size();
                                remaining_space -= word.size() - 1;
                            }
                        } else {
                            if (size > 5 && (remaining_space - word.size() > 3 || remaining_space - word.size() < 3)) {
                                for (char c : word) {
                                    board[row][word_pos++] = c;
                                }
                                boardWords.push_back(word);
                                remaining_space -= word.size();
                                total_space -= word.size();
                            } else if (size <= 5) {
                              for (char c : word) {
                                    board[row][word_pos++] = c;
                                }
                                boardWords.push_back(word);
                                remaining_space -= word.size();
                                total_space -= word.size();
                            }
                        }
                    }
                }
            }
            // Jumble the board 
            jumbleBoard();
            boardSolution = board;
            boardIninitial = board;
        }
        
        // class Scrambler must have a member function called str
        // which returns the entire board as a string. We will be using this to verify
        // that you are updating the board correctly. So you need to define a member
        // function like so.
        string str() const {
            string res = "";
            res += "    ";
            for (int i = 1; i < size; i++) {
                res += to_string(i) + "   ";
            }
            res += to_string(size);
            int i = 1;
            while (i <= size) {
                res += "\n";
                res += "   ";
                res += string(size * 4, '-');
                res += "\n";
                int j = 0;
                res += to_string(i) + " |";
                while (j < size) {
                    res += " ";
                    res.push_back(board[i - 1][j]);
                    res += " |";
                    j++;
                }
                i++;
            }
            res += "\n";
            res += "   ";
            res += string(size * 4, '-');
            return res;
        }
        // Member function called str_boardSolution() 
        // which returns the entire board as a string. We will be using this to 
        // help displaying the board in display_solution
        string str_boardSolution() const {
            string res = "";
            res += "    ";
            for (int i = 1; i < size; i++) {
                res += to_string(i) + "   ";
            }
            res += to_string(size);
            int i = 1;
            while (i <= size) {
                res += "\n";
                res += "   ";
                res += string(size * 4, '-');
                res += "\n";
                int j = 0;
                res += to_string(i) + " |";
                while (j < size) {
                    res += " ";
                    res.push_back(boardSolution[i - 1][j]);
                    res += " |";
                    j++;
                }
                i++;
            }
            res += "\n";
            res += "   ";
            res += string(size * 4, '-');
            return res;
        }

        // class Scrambler needs a member function called try_move
        // which accepts a single string as an argument, and that string represents the
        // command the user gave. Some examples of this could be c2d, r1r, or c3u.
        // The user can also type R or r to restart the game from the intial jumbled board state.
        void try_move(const string& cmd) {
            // Convert the command to uppercase for case-insensitive comparison
            string upper_cmd = "";
            for (char c : cmd) {
                upper_cmd += toupper(c);
            }

            if (upper_cmd == "R") {
                // Restart the game from the initial jumbled board state
                board = boardIninitial;
                return;
            } else if (upper_cmd.length() != 3) {
                // invalid command
                return;
            }

            char axis = upper_cmd[0];
            int index = upper_cmd[1] - '0';
            char direction = toupper(upper_cmd[2]);

            if (axis != 'R' && axis != 'C') {
                // Invalid command
                return;
            }

            if (index < 1 || index > size) {
                // Invalid command
                return;
            }

            if (direction != 'U' && direction != 'D' && direction != 'L' && direction != 'R') {
                // Invalid command
                return;
            }

            // Shift the row or column
            if (axis == 'R') {
                index--;
                if (direction == 'L') {
                    // Shift row to the left
                    char tmp = board[index][0];
                    for (int i = 0; i < size - 1; i++) {
                        board[index][i] = board[index][i + 1];
                    }
                    board[index][size - 1] = tmp;
                } else {
                    // Shift row to the right
                    char tmp = board[index][size - 1];
                    for (int i = size - 1; i > 0; i--) {
                        board[index][i] = board[index][i - 1];
                    }
                    board[index][0] = tmp;
                }
            } else {
                index--;
                if (direction == 'U') {
                    // Shift column up
                    char tmp = board[0][index];
                    for (int i = 0; i < size - 1; i++) {
                        board[i][index] = board[i + 1][index];
                    }
                    board[size - 1][index] = tmp;
                } else {
                    // Shift column down
                    char tmp = board[size - 1][index];
                    for (int i = size - 1; i > 0; i--) {
                        board[i][index] = board[i - 1][index];
                    }
                    board[0][index] = tmp;
                }
            }
        }

        // similar function with try_move, but will use on boardSolution for display_solution
        void change_boardSolution(string cmd) {
            // Convert the command to uppercase for case-insensitive comparison
            string upper_cmd = "";
            for (char c : cmd) {
                upper_cmd += toupper(c);
            }
            char axis = upper_cmd[0];
            int index = upper_cmd[1] - '0';
            char direction = toupper(upper_cmd[2]);

            // Shift the row or column
            if (axis == 'R') {
                index--;
                if (direction == 'L') {
                    // Shift row to the left
                    char tmp = boardSolution[index][0];
                    for (int i = 0; i < size - 1; i++) {
                        boardSolution[index][i] = boardSolution[index][i + 1];
                    }
                    boardSolution[index][size - 1] = tmp;
                } else {
                    // Shift row to the right
                    char tmp = boardSolution[index][size - 1];
                    for (int i = size - 1; i > 0; i--) {
                        boardSolution[index][i] = boardSolution[index][i - 1];
                    }
                    boardSolution[index][0] = tmp;
                }
            } else {
                index--;
                if (direction == 'U') {
                    // Shift column up
                    char tmp = boardSolution[0][index];
                    for (int i = 0; i < size - 1; i++) {
                        boardSolution[i][index] = boardSolution[i + 1][index];
                    }
                    boardSolution[size - 1][index] = tmp;
                } else {
                    // Shift column down
                    char tmp = boardSolution[size - 1][index];
                    for (int i = size - 1; i > 0; i--) {
                        boardSolution[i][index] = boardSolution[i - 1][index];
                    }
                    boardSolution[0][index] = tmp;
                }
            }
        }

        // class Scrambler needs a member function called is_over,
        // which accepts no arguments and returns true or false. It will return true
        // if the game is over, which happens if the user has won the game or quit the game.
        bool is_over() const {
            vector <string> current;
            for (int i = 0; i < size; i++) {
                string s = "";
                int j = 0;
                while (j < size) {
                    if (board[i][j] != ' ') {
                        s += board[i][j];
                        j++;
                    } else {
                        j++;
                        if (s != "") {
                            current.push_back(s);
                            s = "";
                        }
                    }
                }
                if (s != "") {
                    current.push_back(s);
                    s = "";
                }
            }

            for (int i = 0; i < current.size(); i++) {
                if (find(boardWords.begin(), boardWords.end(), current[i]) == boardWords.end()) {
                    return false;
                }
            }

            return true;
        }

        // class Scrambler needs a member function display_solution,
        // which accepts no arguments and returns a string showing the solution to
        // the game. This is the solution from the start, not from the current state
        // of the board, by basically reversing the Jumble-Board algorithm.
        string display_solution() {
            string sol = "";
            sol += str_boardSolution();
            int move = 1;            
            for ( int i = jumbleMove.size() - 1; i >= 0; i--) {
                sol += "\n*** Move ";
                sol += to_string(move);
                sol += " (";
                sol += jumbleMove[i][0];
                sol += ",";
                sol += jumbleMove[i][1];
                sol += ",";
                sol += jumbleMove[i][2];
                sol += ")\n";
                change_boardSolution(jumbleMove[i]);
                sol += this -> str_boardSolution();
                move++;
            }
            return sol;
        }

        // class Scrambler needs a member function get_words which returns a vector of strings
        // that contain all the words that are present on the jumbled board.
        vector<string> get_words() const {
            return boardWords;
        }
};

int main() {
    srand(time(NULL));
    Scrambler s("dictionary.txt", 9);
    cout << s.str() << endl << endl;
    string cmd;
    while (!s.is_over()) {
        cout << "Enter a move: ";
        cin >> cmd;
        if (cmd == "S") {
            cout << s.display_solution() << endl;
            return;
        }
        s.try_move(cmd);
        cout << s.str() << endl << endl;
    }

    cout << "Won" << endl;
    return 0;
}