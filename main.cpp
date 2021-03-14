#include <iostream>
#include <cassert>
#include <vector>
#include <fstream>

using namespace std;

//Hans Lous //s1045465
//Leon Zhang //s1042092

struct Length
{
	int minutes;							// #minutes (0..)
	int seconds;							// #seconds (0..59)
};

struct Track
{
	string artist;                          // name of artist
	string cd;                              // cd title
	int    year;                            // year of appearance (if known)
	int    track;							// track number
	string title;                           // track title
	string tags;                            // tags
	Length time;							// playing time
	string country;                         // main countr(y/ies) of artist
};

struct TrackDisplay
{
	bool showArtist;						// if true, show name of artist
	bool showAlbum;							// if true, show cd title
	bool showYear;							// if true, show year of appearance
	bool showTrack;							// if true, show track number
	bool showTitle;							// if true, show track title
	bool showTags;                          // if true, show tags
	bool showLength;						// if true, show playing time
	bool showCountry;                       // if true, show countr(y/ies) of artist
};

struct Slice
{
    int istart ;
    int iend ;
    //inclusive: iend is (intended to be) part of slice
};
vector<Track> tracksDB;

ostream& operator<< (ostream& out, const Length length)
{// Precondition:
    assert (true) ;
/*  Postcondition:
    the value of length is shown via out in the format: minutes, ':', seconds (two digits)
*/
    out << length.minutes << ':';
    if (length.seconds < 10)
        out << '0';
    out << length.seconds ;
    return out ;
}

istream& operator>> (istream& in, Length& length)
{// Precondition:
    assert (true) ;
/*  Postcondition:
    the value of length has been read from in: first minutes, then ':', then seconds
*/
    in >> length.minutes;
    in.ignore();
    in >> length.seconds ;
    length.minutes += (length.seconds)/60;
    length.seconds %= 60;
    return in ;
}

istream& operator>> (istream& in, Track& track)
{// Precondition:
    assert (true) ;
/*  Postcondition:
    the content of the first 8 lines from in have been read and are stored in the corresponding members of track.
    The following (empty) line from in has also been read.
*/
    getline(in, track.artist);
    getline(in, track.cd);
    in >> track.year;
    in.ignore();
    in >> track.track;
    in.ignore();
    getline(in, track.title);
    getline(in, track.tags);
    in >> track.time;
    in.ignore();
    getline(in, track.country);
    in.ignore();
    return in;
}

void show_track (Track track, TrackDisplay lt)
{// Precondition:
    assert (true) ;
/*  Postcondition:
    only the members of track are shown for which the corresponding member in td is true.
*/
    bool first = true;
    if (lt.showArtist)
    {
        cout << track.artist;
        first = false;
    }
    if (lt.showAlbum)
    {
        if (!first)
            cout << " | ";
        cout << track.cd;
        first = false;
    }
    if (lt.showYear)
    {
        if (!first)
            cout << " | ";
        cout << track.year;
        first = false;
    }
    if (lt.showTrack)
    {
        if (!first)
            cout << " | ";
        cout << track.track;
        first = false;
    }
    if (lt.showTitle)
    {
        if (!first)
            cout << " | ";
        cout << track.title;
        first = false;
    }
    if (lt.showTags)
    {
        if (!first)
            cout << " | ";
        cout << track.tags;
        first = false;
    }
    if (lt.showLength)
    {
        if (!first)
            cout << " | ";
        cout << track.time;
        first = false;
    }
    if (lt.showCountry)
    {
        if (!first)
            cout << " | ";
        cout << track.country;
    }
}

string cap(string s)
{//Precondition
    assert(true);
//Postcondition
//string with lowercase letters converted to uppercase
    string n = "";
    for (unsigned int i = 0; i < s.length(); i++){
        if (s[i] >= 'a' && s[i] <= 'z')
            n.push_back(s[i] - 32);
        else
            n.push_back(s[i]);
    }
    return n;
}

bool operator< (const Track& a, const Track& b)
{//Precondition
    assert(true);
//Postcondition
//true if a is (alphabetically) lower than b, false otherwise
//based first on artist, then on album, then on integers
//year and tracknr
    if (a.artist == b.artist){
        if (a.cd == b.cd){
            if (a.year == b.year)
                return (a.track < b.track);
            else return (a.year < b.year);
        }
        else return (cap(a.cd) < cap(b.cd));
    }
    else return (cap(a.artist) < cap(b.artist));
}

bool operator== (const Track& a, const Track& b)
{//Precondition
    assert(true);
//Postcondition
//true if each of artist, album, year and tracknr
//are identical, false otherwise
    return a.artist == b.artist && a.cd == b.cd && a.year == b.year && a.track == b.track;
}

bool valid_slice (vector<Track>& nums, Slice s)
{//Precondition
    assert(true);
//Postcondition
//true if s is within bounds of vector, false otherwise
    return (s.iend <= nums.size()-1 && s.istart >= 0);
}

void swp (vector<Track>& nums, int pos1, int pos2)
{//Precondition
    assert(pos1 >= 0 && pos1 < nums.size() && pos2 >= 0 && pos2 < nums.size());
//Postcondition
//elements at pos1 and pos2 in vector have swapped places
    const Track TEMP = nums[pos2];
    nums[pos2] = nums[pos1];
    nums[pos1] = TEMP;
}

//for part 1
int dnf(vector<Track>& tracks, Slice& orange)
{//Precondition
    assert(valid_slice(tracks, orange));
//Postcondition
//tracks is ordered as follows: red slice < PIVOT == white slice < blue slice
//with PIVOT being the middle element of slice orange
//return: start of blue slice
    int blue = orange.iend + 1;
    const Track PIVOT = tracks[(orange.istart + orange.iend)/2];
    while (orange.iend >= orange.istart){
        if (tracks[orange.iend] < PIVOT)
            swp(tracks, orange.iend, orange.istart++);
        else if (tracks[orange.iend] == PIVOT)
            orange.iend--;
        else
            swp(tracks, orange.iend--, --blue);
    }
    return blue;
}

void quicksort(vector<Track>& tracks, Slice orange)
{//Precondition
    assert(valid_slice(tracks, orange));
//Postcondition
//orange slice of tracks is ordered from low to high
    if (orange.istart >= orange.iend)
        return;
    const int START = orange.istart;
    const int END = orange.iend;
    int blue = dnf(tracks, orange);
    quicksort(tracks, {START, orange.iend}); //sorting the red slice
    quicksort(tracks, {blue, END});          //sorting the blue slice
}

//for part 2
int alt_dnf(vector<Track>& tracks, Slice& orange)
{//Precondition
    assert(valid_slice(tracks, orange));
//Postcondition
//tracks is ordered as follows: red slice < PIVOT == white slice < blue slice
//with PIVOT being the middle element of slice orange
//return: end of red slice
    int red = orange.istart-1;
    const Track PIVOT = tracks[(orange.istart + orange.iend)/2];
    while (orange.istart <= orange.iend){
        if (tracks[orange.istart] < PIVOT){
            swp(tracks, red+1, orange.istart++);
            red++;
        }
        else if (tracks[orange.istart] == PIVOT)
            orange.istart++;
        else{
            swp(tracks, orange.istart, orange.iend--);}
    }
    return red;
}

void alt_quicksort(vector<Track>& tracks, Slice orange)
{//Precondition
    assert(valid_slice(tracks, orange));
//Postcondition
//orange slice of tracks is ordered from low to high
    if (orange.iend <= orange.istart)
        return;
    const int START = orange.istart;
    const int END = orange.iend;
    int red = alt_dnf(tracks, orange);
    alt_quicksort(tracks, {START, red}); //sorting the red slice
    alt_quicksort(tracks, {orange.iend+1, END});          //sorting the blue slice
}

void swp_int (vector<int>& nums, int pos1, int pos2)
{//Precondition
    assert(pos1 >= 0 && pos1 < nums.size() && pos2 >= 0 && pos2 < nums.size());
//Postcondition
//elements at pos1 and pos2 in vector have swapped places
    const int TEMP = nums[pos2];
    nums[pos2] = nums[pos1];
    nums[pos1] = TEMP;
}

int largest(vector<int>& v, int low, int up)
{//Precondition
    assert(low >= 0 && up < v.size() && low <= up);
//Postcondition
//index of largest element in v in range [low, up]
    int pos = up;
    while (low++ < up){
        pos = (v[low] >= v[pos] ? low : pos);
    }
    return pos;
}

void sort_iterative(vector<int>& v)
{//Precondition
    assert(true);
//Postcondition
//v is sorted from low to high
    for (int n = v.size()-1; n > 0; n--){
        const int POS = largest(v, 0, n - 1);
        swp_int(v, POS, n);
    }
}

int main()
{
    //part 1 & 2
    ifstream file;
    Track track;
    file.open("Tracks.txt");
    int cnt = 0;
    while (file >> track){
        tracksDB.push_back(track);
        cnt++;
    }
    cout << cnt << " tracks read\n";
    file.clear();
    file.close();
    alt_quicksort(tracksDB, {0, tracksDB.size()-1});
    for (Track &x : tracksDB){
        show_track(x, {true, true, true, true, true, true, true, true});
        cout << endl;
    }
    //part 3
    vector<int> nums = {1, 3, 5, 77, -55, 6, 101, 77, 40};
    sort_iterative(nums);
    for (int& x : nums){
        cout << x << " ";
    }
    return 0;
}
