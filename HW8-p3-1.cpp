#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <unordered_set>
#include <string>
#include <set>
#include <map>

using namespace std;

void initialize(unordered_set<string> &dictionary, char &grid[][C], int R, int C){

    fstream inputFile;
    inputFile.open("words.txt");

    for (int i=0; i<R; i++)
    {
        for (int j=0; j<C; j++)
            inputFile>>grid[i][j];
    }

    ifstream file("puzzle.txt");
    string word;
    while( file >> word ) {
        dictionary.insert(word);
    }
}

// This function searches in all 8-direction from point
// (row, col) in grid[][]
bool search_position(char grid[][C], int row, int col, string word, int R, int C)
{
    // For searching in all 8 direction
    int x[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
    int y[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
    // If first character of word doesn't match with
    // given starting point in grid.
    if (grid[row][col] != word[0])
        return false;
    int length = word.length();
    // Search word in all 8 directions starting from (row,col)
    for (int direction = 0; direction < 8; direction++)
    {
        // Initialize starting point for current direction
        int k, row_direction = row + x[direction], column_direction = col + y[direction];
        // First character is already checked, match remaining
        // characters
        for (k = 1; k < length; k++)
        {
            // If out of bound break
            if (row_direction >= R || row_direction < 0 || column_direction >= C || column_direction < 0)
                break;
            // If not matched,  break
            if (grid[row_direction][column_direction] != word[k])
                break;
            //  Moving in particular direction
            row_direction += x[direction], column_direction += y[direction];
        }
        // If all character matched, then value of must
        // be equal to length of word
        if (k == length)
            return true;
    }
    return false;
}

//  Searches given word in a given matrix in all 8 directions
void check_for_word(char grid[][C], string word, set<string> &words_found, multimap<string,int> &word_position, int R, int C)
{
    for (int row = 0; row < R; row++)
        for (int col = 0; col < C; col++)
            if (search_position(grid, row, col, word, R, C))
                words_found.insert(word);
                word_position.insert(pair<string, int>(word,row));
                word_position.insert(pair<string, int>(word,col));
}

void set_to_array(set<string> words_found, string words[]){
    set<string>::iterator it;
    int i = 0;
    for (it = words_found.begin(); it != words_found.end(); ++it)
    {
        words[i] = *it;
        i++;
    }
}


void insertionsort(string words[], int n){
    for(int i = 1; i < n ; i++){
        string temp = words[i];
        int m = i;
        while(temp > words[m-1] && m > 0){
            words[m] = words[m-1];
            m--;
        }
        words[m] = temp;
    }
}

void word_search(int R, int C){
    unordered_set<string> dictionary;
    set<string> words_found;
    string word;
    multimap<string,int> word_position;
    char grid[R][C];

    initialize(dictionary, grid[][C], R, C);

    unordered_set<string>::iterator it;
    for (it = dictionary.begin(); it != dictionary.end(); ++it)
    {
        word = *it;
        check_for_word(grid[][C], word, words_found, word_position, R, C);
    }
    string words[words_found.size()];
    set_to_array(words_found, words);
    insertionsort(words,words->size());

    typedef multimap<string, int>::iterator Iterator;
    for(int i = 0; i < words->size(); i++){
        cout << words[i] << "from (";
        pair<Iterator, Iterator> result = word_position.equal_range(words[i]);
        for (Iterator it = result.first; it != result.second; it++) {
            cout << it->second <<", ";
        }
        cout << ")" <<endl;
    }
}



int main(){
    int R,C;

    cout<<"Enter number of rows of puzzle: ";
    cin >>R;
    cout <<endl;
    cout<<"Enter number of columns of puzzle: ";
    cin >>C;


    word_search(R,C);

    return 0;
}

