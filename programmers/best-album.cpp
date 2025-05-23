#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>

using namespace std;

vector<int> solution(vector<string> genres, vector<int> plays) {
    vector<int> answer;
    map<string, int> genre_plays;
    map<string, vector<pair<int, int>>> genre_songs; 

    for (int i = 0; i < genres.size(); ++i) {
        genre_plays[genres[i]] += plays[i];
        genre_songs[genres[i]].push_back({i, plays[i]});
    }

    vector<pair<string, int>> genre_plays_vec(genre_plays.begin(), genre_plays.end());
    sort(genre_plays_vec.begin(), genre_plays_vec.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
        return a.second > b.second;
    });

    for (auto& genre_play : genre_plays_vec) {
        string genre = genre_play.first;
        
        sort(genre_songs[genre].begin(), genre_songs[genre].end(), [](const pair<int, int>& a, const pair<int, int>& b) {
            if (a.second != b.second) {
                return a.second > b.second;
            } else {
                return a.first < b.first;
            }
        });

        int count = 0;
        for (auto& song : genre_songs[genre]) {
            answer.push_back(song.first);
            count++;
            if (count == 2) break;
        }
    }

    return answer;
}