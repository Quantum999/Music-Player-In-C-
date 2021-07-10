#include <bits/stdc++.h>
#include <fstream>
#include "server.h"
#include <cstdlib>
#include <windows.h>
using namespace std;

//**************************MUSIC PLAYER CLASS USING STATE DESIGN PATTERN*********************************//
MusicPlayer::MusicPlayer(int id)
: m_pState(new StoppedState()), song_id_(id){
    fstream fin;
    bool flag = false;
    fin.open("songs.csv", ios::in);
    vector<string> row;
    string temp, word;
    while(getline(fin, temp)){
        row.clear();
        stringstream s(temp);
        while(getline(s, word, ',')){
            row.push_back(word);
        }
        if(stoi(row[0]) == id){
            flag = true;
            break;
        }
    }
    if(flag){
        string path_string = "open \"" + row[5] + "\" type mpegvideo alias mp3";
        const char* comm = path_string.c_str();
        mciSendString(comm, NULL, 0, NULL);
        cout << "valid id\n";
    }
    else{
        cout << "invalid id\n";
    }
}

MusicPlayer::~MusicPlayer() {
	delete m_pState;
}

void MusicPlayer::Play() {
	m_pState->Play(this);
}

void MusicPlayer::Pause() {
	m_pState->Pause(this);
}

void MusicPlayer::Stop() {
	m_pState->Stop(this);
}

void MusicPlayer::SetState(State state)
{
	cout << "changing from " << m_pState->GetName() << " to ";
	delete m_pState;

	if(state == ST_STOPPED)
	{
		m_pState = new StoppedState();
	}
	else if(state == ST_PLAYING)
	{
		m_pState = new PlayingState();
	}
	else
	{
		m_pState = new PausedState();
	}

	cout << m_pState->GetName() << " state\n";
}


MusicPlayerState::MusicPlayerState(string name)
: m_name(name) {}

MusicPlayerState::~MusicPlayerState() {}

void MusicPlayerState::Play(MusicPlayer *)
{
	cout << "Illegal state transition from " << GetName() << " to Playing\n";
}

void MusicPlayerState::Pause(MusicPlayer *)
{
	cout << "Illegal state transition from " << GetName() << " to Paused\n";
}

void MusicPlayerState::Stop(MusicPlayer *)
{
	cout << "Illegal state transition from " << GetName() << " to Stopped\n";
    //cout << "Changing Song\n";
}


PlayingState::PlayingState()
: MusicPlayerState(string("Playing")) {}

PlayingState::~PlayingState() {}

void PlayingState::Pause(MusicPlayer * player)
{
	player->SetState(MusicPlayer::ST_PAUSED);
	mciSendString("pause mp3", NULL, 0, NULL);
}

void PlayingState::Stop(MusicPlayer * player)
{
	player->SetState(MusicPlayer::ST_STOPPED);
	mciSendString("close mp3", NULL, 0, NULL);
}


PausedState::PausedState()
: MusicPlayerState(string("Paused")) {}

PausedState::~PausedState() {}

void PausedState::Play(MusicPlayer * player)
{
	player->SetState(MusicPlayer::ST_PLAYING);
	mciSendString("play mp3", NULL, 0, NULL);
}

void PausedState::Stop(MusicPlayer * player)
{
	player->SetState(MusicPlayer::ST_STOPPED);
	mciSendString("close mp3", NULL, 0, NULL);
}


StoppedState::StoppedState()
: MusicPlayerState(string("Stopped")) {}

StoppedState::~StoppedState() {}

void StoppedState::Play(MusicPlayer * player)
{
	player->SetState(MusicPlayer::ST_PLAYING);
    mciSendString("play mp3", NULL, 0, NULL);
}

//*****************************USERS CLASSES CREATED USING SINGLETON AND FACTORY DESIGN PATTERNS**********************//

int user_abstract::play_songs(int id, int call){
    int n;
    cout << "Starting Music Player ...\n";
    while(1){
        if(call){
            while(1){
                cout << "1. Play Song\n2. Stop Song\n3. Next Song\n4. Exit Playlist\n";
                cin>>n;
                if(n==1){
                    play_song(id);
                    cout<<"Audio file playing...\n\n";
                    break;
                }
                else if(n==2){
                    //close the file and get out of the loop
                    stop_song();
                    return 0;
                }
                else if(n == 3){
                    stop_song();
                    return 1;
                }
                else if(n == 4){
                    stop_song();
                    return 2;
                }
            }
        }
        else{
            while(1){
                fstream fin;
                bool flag = false;
                fin.open("songs.csv", ios::in);
                vector<string> row;
                string temp, word;
                while(getline(fin, temp)){
                    row.clear();
                    stringstream s(temp);
                    while(getline(s, word, ',')){
                        row.push_back(word);
                    }
                    if(stoi(row[0]) == id){
                        flag = true;
                        break;
                    }
                }
                if(!flag)
                    return 1;
                cout << "1. Play Song\n2. Stop Song\n";
                cin>>n;
                if(n==1){
                    play_song(id);
                    cout<<"Audio file playing...\n\n";
                    break;
                }
                else if(n==2){
                    //close the file and get out of the loop
                    stop_song();
                    return 0;
                }
            }
        }

        if(call){
            while(1){
                cout << "1. Pause Song\n2. Stop Song\n3. Next Song\n4. Exit Music Player\n";
                cin >> n;
                if(n==1){
                    //pause the audio file
                    pause_song();
                    break;
                }
                else if(n==2){
                    //close the audio file
                    stop_song();
                    return 0;
                }
                else if(n == 3){
                    stop_song();
                    return 1;
                }
                else if(n == 4){
                    stop_song();
                    return 2;
                }
            }
        }

        else{
            while(1){
                cout << "1. Pause Song\n2. Stop Song\n";
                cin>>n;

                if(n==1){
                    //pause the audio file
                    pause_song();
                    break;
                }
                else if(n==2){
                    //close the audio file
                    stop_song();
                    return 0;
                }
            }
        }
    }
    return 0;
}

void user_abstract::play_playlist(){
    int n = list_size();
    if(n == 0){
        cout << "no song in playlist/database\n";
        return;
    }
    int status = 1;
    while(1){
        int id;
        if (status == 1){
            id = next_song_id();
            list_song(id);
            status = play_songs(id, 1);
        }
        if(status == 0){
            list_song(id);
            status = play_songs(id, 1);
        }
        if(status == 2)
            break;
    }
}

int user_abstract::utype = 0;

bool compare(vector<string> a, vector<string> b){
    return stoi(a[2]) > stoi(b[2]);
}

listener_concrete* listener_concrete::get_instance(){
    if(!instance_){
        instance_ = new listener_concrete();
        return instance_;
    }
    else
        return instance_;
}

bool listener_concrete::login(){
    string username, password;
    cout << "enter username: ";
    cin >> username;
    cout << "\nenter password: ";
    cin >> password;

    fstream fin;
    fin.open("users.csv", ios::in);
    vector<string> row;
    bool flag = false;
    string line, word, temp;
    while(getline(fin, temp)){
        row.clear();
        stringstream s(temp);
        while(getline(s, word, ',')){
            row.push_back(word);
        }
        if(username.compare(row[1]) == 0 && password.compare(row[2]) == 0){
            flag = true;
            fin.close();
            break;
        }
    }
    if(flag){
        uid_ = stoi(row[0]);
        username_ = row[1];
        password_ = row[2];
        email_ = row[3];
        fav_genre_ = row[4];
        stringstream playlist(row[5]);
        cnt_ = 0;
        for(int j = 0; j < row[5].length(); j++)
            if(row[5].at(j) == '/')    ++cnt_;
        int i = 0;
        playlist_ = new int[cnt_];
        while(getline(playlist, word, '/')){
            if(stoi(word) == 0)
                continue;
            playlist_[i] = stoi(word);
            ++i;
        }
        cout << "\n******************HOME PAGE******************\n";
        cout << "welcome " << username << endl;
        cout << "top 10 songs: \n";

        vector<vector<string>> song_list;
        vector<string> song_details;
        fstream fin_songs;
        fin_songs.open("songs.csv", ios::in);
        i = 0;
        while(getline(fin_songs, line)){
            song_details.clear();
            stringstream s(line);
            while(getline(s, word, ',')){
                song_details.push_back(word);
            }
            song_list.push_back(song_details);
            ++i;
        }

        fin_songs.close();
        sort(song_list.begin(), song_list.end(), compare);
        for(i = 0; i < 10; ++i){
            for(int j = 0; j < 5; ++j){
                cout << song_list[i][j] << " ";
            }
            cout << "\n";
        }
        cout << "\n";
    }
    else{
        fin.close();
    }
    return flag;
}

int listener_concrete::signup(){
    string uname, pword, email, genre;
    cout << "\nEnter username: ";
    cin >> uname;
    cout << "\nEnter password: ";
    cin >> pword;
    cout << "\nEnter email: ";
    cin >> email;
    cout << "\nEnter your favourite genre: ";
    cin >> genre;
    if(uname.compare("") == 0 || pword.compare("") == 0){
            return -1;
    }
    const char newfile[] = "users_new.csv";
    const char oldfile[] = "users.csv";
    fstream fout;
    fstream fin;
    fout.open(newfile, ios::out | ios::app);
    fin.open(oldfile, ios::in);
    string lastuid = "";

    vector<string> row;
    string line, word, temp;
    while(getline(fin, temp)){
        row.clear();
        stringstream s(temp);
        while(getline(s, word, ',')){
            row.push_back(word);
        }
        lastuid = row[0];
        if(row[1].compare(uname) == 0){
            fin.close();
            fout.close();
            remove(newfile);
            return 0;
        }
        for(int i = 0; i < 5; i++){
            fout << row[i] << ",";
        }
        fout << row[5] << "\n";
    }
    fout << to_string(stoi(lastuid) + 1) << "," << uname << "," << pword << "," << email << "," << genre << "," << "0" << "\n";
    fin.close();
    fout.close();
    if(remove(oldfile) != 0)
        perror("error deleting file\n");
    if(rename(newfile, oldfile) != 0)
        perror("error renaming file\n");
    return 1;
}

void listener_concrete::show_functions(){
    cout << "\n Functions:\n";
    cout << "1. Show Playlist\n";
    cout << "2. Display Song Details\n";
    cout << "3. Add a Song to Playlist\n";
    cout << "4. Remove Song from Playlist\n";
    cout << "5. Play Playlist\n";
    cout << "6. Play a Song\n";
    cout << "7. Like Song\n";
    cout << "8. Recommended Songs\n";
    cout << "9. Recommend Similar Songs\n";
    cout << "10. Play songs of a genre\n";
    cout << "11. Logout\n\n";
}

void listener_concrete::play_song(int id){
    if(player_ != nullptr)
        delete player_;
    player_ = nullptr;
    player_ = new MusicPlayer(id);
    player_->Play();
}
void listener_concrete::resume_song(){
    player_->Play();
}
void listener_concrete::pause_song(){
    player_->Pause();
}
void listener_concrete::stop_song(){
    player_->Stop();
}

int listener_concrete::next_song_id(){
    playlist_ptr_ = (playlist_ptr_ + 1) % cnt_;
    return playlist_[playlist_ptr_];
}

int listener_concrete::list_size(){
    return cnt_;
}

bool listener_concrete::list_song(int id){
    fstream fin;
    fin.open("songs.csv", ios::in);
    bool flag = false;
    vector<string> row;
    string line, word, temp;
    while(getline(fin, temp)){
        row.clear();
        stringstream s(temp);
        while(getline(s, word, ',')){
            row.push_back(word);
        }
        if(stoi(row[0]) == id){
            flag = true;
            break;
        }
    }
    if(flag){
        for(int j = 0; j < 5; j++)
            cout << row[j] << " ";
        cout << "\n";
    }
    fin.close();
    return flag;
}

void listener_concrete::list_songs(){
    fstream fin;
    fin.open("songs.csv", ios::in);
    vector<string> row;
    string line, word, temp;
    while(getline(fin, temp)){
        row.clear();
        stringstream s(temp);
        while(getline(s, word, ',')){
            row.push_back(word);
        }
        for(int i = 0; i < cnt_; i++){
            if(playlist_[i] == stoi(row[0])){
                for(int j = 0; j < 5; j++)
                    cout << row[j] << " ";
                cout << "\n";
            }
        }
    }
    fin.close();
    cout << "\n";
}
void listener_concrete::list_all_songs(){
    fstream fin;
    fin.open("songs.csv", ios::in);
    vector<string> row;
    string line, word, temp;
    while(getline(fin, temp)){
        row.clear();
        stringstream s(temp);
        while(getline(s, word, ',')){
            row.push_back(word);
        }
        for(int i = 0; i < 5; i++){
            cout << row[i] << " ";
        }
        cout << "\n";
    }
    fin.close();
    cout << "\n";
}
void listener_concrete::recommend_song(){
    cout << "\nRecommended Songs for You:\n";
    fstream fin;
    fin.open("songs.csv", ios::in);
    vector<string> row;
    string line, word, temp;
    while(getline(fin, temp)){
        row.clear();
        stringstream s(temp);
        while(getline(s, word, ',')){
            row.push_back(word);
        }
        if(row[3].compare(fav_genre_) == 0){
            for(int i = 0; i < 5; i++){
                cout << row[i] << " ";
            }
            cout << "\n";
        }
    }
    fin.close();
    cout << "\n";
}
int listener_concrete::add_song(int id){
    for(int i = 0; i < cnt_; i++)
        if(playlist_[i] == id)
            return 0;

    fstream fin_songs;
    fin_songs.open("songs.csv", ios::in);
    vector<string> row1;
    int lastid = 0;
    string temp1, word1;
    while(getline(fin_songs, temp1)){
        row1.clear();
        stringstream s(temp1);
        getline(s, word1, ',');
        lastid = stoi(word1);
    }
    if(id > lastid) return -1;
    fin_songs.close();

    const char newfile[] = "users_new.csv";
    const char oldfile[] = "users.csv";
    fstream fout;
    fstream fin;
    fout.open(newfile, ios::out | ios::app);
    fin.open(oldfile, ios::in);

    vector<string> row;
    string line, word, temp;
    while(getline(fin, temp)){
        row.clear();
        stringstream s(temp);
        while(getline(s, word, ',')){
            row.push_back(word);
        }
        if(row[1].compare(username_) == 0 && row[2].compare(password_) == 0){
            row[5] = row[5] + '/' + to_string(id);
            int* temp = new int[cnt_ + 1];
            for(int i = 0; i < cnt_; ++i)
                temp[i] = playlist_[i];
            temp[cnt_] = id;
            delete [] playlist_;
            playlist_ = temp;
        }
        for(int i = 0; i < 5; i++){
            fout << row[i] << ",";
        }
        fout << row[5] << "\n";
    }
    fin.close();
    fout.close();
    if(remove(oldfile) != 0)
        perror("error deleting file\n");
    if(rename(newfile, oldfile) != 0)
        perror("error renaming file\n");
    ++cnt_;
    return 1;
}

int listener_concrete::remove_song(int id){
    bool flag = false;
    for(int i = 0; i < cnt_; i++)
        if(playlist_[i] == id){
            flag = true;
            break;
        }
    if(!flag)   return 0;

    const char newfile[] = "users_new.csv";
    const char oldfile[] = "users.csv";
    fstream fout;
    fstream fin;
    fout.open(newfile, ios::out | ios::app);
    fin.open(oldfile, ios::in);

    vector<string> row;
    string line, word, temp;
    while(getline(fin, temp)){
        row.clear();
        stringstream s(temp);
        while(getline(s, word, ',')){
            row.push_back(word);
        }

        if(username_.compare(row[1]) == 0 && password_.compare(row[2]) == 0){
            if(cnt_ == 1){
                row[5].clear();
                row[5] = row[5] + "0";
            }
            else{
                int* temp = new int[cnt_ - 1];
                for(int i = 0, j = 0; i < cnt_; ++i){
                    if(playlist_[i] != id){
                        temp[j] = playlist_[i];
                        ++j;
                    }
                    else if(playlist_[i] == id && playlist_ptr_ > i)
                        --playlist_ptr_;
                }
                delete [] playlist_;
                playlist_ = temp;

                row[5].clear();
                row[5] = row[5] + "0";
                for(int i = 0; i < cnt_ - 2; i++)
                    row[5] = row[5] + '/' + to_string(playlist_[i]);
                row[5] = row[5] + '/' + to_string(playlist_[cnt_ - 2]);
            }
        }

        for(int i = 0; i < 5; i++){
            fout << row[i] << ",";
        }
        fout << row[5] << "\n";
    }
    fin.close();
    fout.close();
    if(remove(oldfile) != 0){
        perror("error deleting file\n");
        return 0;
    }
    if(rename(newfile, oldfile) != 0){
        perror("error renaming file\n");
        return 0;
    }
    --cnt_;
    return 1;
}

int listener_concrete::like_song(int id){
    const char newfile[] = "songs_new.csv";
    const char oldfile[] = "songs.csv";
    fstream fout;
    fstream fin;
    fout.open(newfile, ios::out | ios::app);
    fin.open(oldfile, ios::in);

    bool flag = false;
    vector<string> row;
    string line, word, temp;
    while(getline(fin, temp)){
        row.clear();
        stringstream s(temp);
        while(getline(s, word, ',')){
            row.push_back(word);
        }
        if(stoi(row[0]) == id){
            flag = true;
            string word1;
            stringstream like(row[6]);
            while(getline(like, word1, '/')){
                if(stoi(word1) == uid_){
                    fin.close();
                    fout.close();
                    remove(newfile);
                    return 0;
                }
            }
            row[6] = row[6] + '/' + to_string(uid_);
            row[2] = to_string(stoi(row[2]) + 1);
        }

        for(int i = 0; i < 6; i++){
            fout << row[i] << ",";
        }
        fout << row[6] << "\n";
    }
    fin.close();
    fout.close();
    if(remove(oldfile) != 0){
        perror("error deleting file\n");
        return 0;
    }
    if(rename(newfile, oldfile) != 0){
        perror("error renaming file\n");
        return 0;
    }
    if(!flag)
        return -1;
    return 1;
}

int listener_concrete::recommend_similar_songs(int id){
    cout << "\nSimilar Songs\n";
    fstream fin;
    int flag = 0;
    fin.open("songs.csv", ios::in);
    vector<string> row;
    string line, word, temp, genre;
    while(getline(fin, temp)){
        row.clear();
        stringstream s(temp);
        while(getline(s, word, ',')){
            row.push_back(word);
        }
        if(row[0].compare(to_string(id)) == 0){
            genre = row[3];
            flag = 1;
            break;
        }
    }
    fin.close();
    if(flag){
        fin.open("songs.csv", ios::in);
        while(getline(fin, temp)){
            row.clear();
            stringstream s(temp);
            while(getline(s, word, ',')){
                row.push_back(word);
            }
            if(row[3].compare(genre) == 0){
                for(int i = 0; i < 5; i++){
                    cout << row[i] << " ";
                }
                cout << "\n";
            }
        }
        fin.close();
    }
    else
        return flag;
    return flag;
    cout << "\n";
}

void listener_concrete::play_genre(){
    string genre;
    cout << "Enter the genre you would like to listen to: ";
    cin >> genre;
    fstream fin;
    int flag = 0;
    fin.open("songs.csv", ios::in);
    vector<string> row;
    vector<int> genre_playlist;
    string line, word, temp;
    while(getline(fin, temp)){
        row.clear();
        stringstream s(temp);
        while(getline(s, word, ',')){
            row.push_back(word);
        }
        if(row[3].compare(genre) == 0){
            flag = 1;
            genre_playlist.push_back(stoi(row[0]));
            for(int i = 0; i < 5; i++){
                cout << row[i] << " ";
            }
            cout << "\n";
        }
    }
    fin.close();
    if(!flag){
        cout << "\nNo songs found\n";
        return;
    }
    char choice;
    cout << "Do you want to play the playlist?(y/n)\n";
    cin >> choice;
    if(choice == 'y'){
        int n = genre_playlist.size();
        if(n == 0){
            cout << "no song in playlist/database\n";
            return;
        }
        int idx = -1;
        int status = 1;
        while(1){
            int id;
            if (status == 1){
                idx = (idx + 1) % n;
                id = genre_playlist[idx];
                list_song(id);
                status = play_songs(id, 1);
            }
            if(status == 0){
                list_song(id);
                status = play_songs(id, 1);
            }
            if(status == 2)
                break;
        }
    }
    return;
}

void listener_concrete::destroy_instance(){
    if(instance_ != nullptr){
        uid_ = 0;
        username_ = "";
        password_ = "";
        email_ = "";
        fav_genre_ = "";
        cnt_ = 0;
        if(player_ != nullptr){
            delete player_;
            player_ = nullptr;
        }
        if(playlist_ != nullptr){
            delete [] playlist_;
            playlist_ = nullptr;
        }
        instance_ = nullptr;
    }
}
int listener_concrete::uid_ = 0;
string listener_concrete::username_ = "";
string listener_concrete::password_ = "";
string listener_concrete::email_ = "";
string listener_concrete::fav_genre_ = "";
int listener_concrete::cnt_ = 0;
int* listener_concrete::playlist_ = nullptr;
int listener_concrete::playlist_ptr_ = -1;
listener_concrete* listener_concrete::instance_ = nullptr;
MusicPlayer* listener_concrete::player_ = nullptr;


admin_concrete* admin_concrete::get_instance(){
    if(!instance_){
        instance_ = new admin_concrete();
        return instance_;
    }
    else
        return instance_;
}
bool admin_concrete::login(){
    string username, password;
    cout << "enter username: ";
    cin >> username;
    cout << "\nenter password: ";
    cin >> password;

    fstream fin;
    fin.open("admin.csv", ios::in);
    vector<string> row;
    bool flag = false;
    string line, word, temp;
    while(getline(fin, temp)){
        row.clear();
        stringstream s(temp);
        while(getline(s, word, ',')){
            row.push_back(word);
        }
        if(username.compare(row[0]) == 0 && password.compare(row[1]) == 0){
            flag = true;
            fin.close();
            break;
        }
    }
    if(flag){
        username_ = row[0];
        password_ = row[1];
        email_ = row[2];
        const char file[] = "songs.csv";
        fstream fin_songs;
        fin_songs.open(file, ios::in);

        vector<string> row;
        string line, word, temp;
        while(getline(fin_songs, temp)){
            row.clear();
            stringstream s(temp);
            getline(s, word, ',');
            cnt_ = stoi(word);
        }
        fin_songs.close();
    }
    else{
        fin.close();
        return flag;
        cout << "wrong credentials\n";
    }
    return flag;
}
void admin_concrete::show_functions(){
    cout << "\n Functions:\n";
    cout << "1. Show Database\n";
    cout << "2. Display Song Details\n";
    cout << "3. Add a Song to Database\n";
    cout << "4. Remove Song from Database\n";
    cout << "5. Play All Songs\n";
    cout << "6. Play a Song\n";
    cout << "11. Logout\n\n";
}

void admin_concrete::play_song(int id){
    if(player_ != nullptr)
        delete player_;
    player_ = nullptr;
    player_ = new MusicPlayer(id);
    player_->Play();
}
void admin_concrete::pause_song(){
    player_->Pause();
}
void admin_concrete::resume_song(){
    player_->Play();
}
void admin_concrete::stop_song(){
    player_->Stop();
}


bool admin_concrete::list_song(int id){
    fstream fin;
    fin.open("songs.csv", ios::in);
    bool flag = false;
    vector<string> row;
    string line, word, temp;
    while(getline(fin, temp)){
        row.clear();
        stringstream s(temp);
        while(getline(s, word, ',')){
            row.push_back(word);
        }
        if(stoi(row[0]) == id){
            flag = true;
            break;
        }
    }
    if(flag){
        for(int j = 0; j < 5; j++)
            cout << row[j] << " ";
        cout << "\n";
    }
    fin.close();
    return flag;
}
void admin_concrete::list_songs(){
    fstream fin;
    fin.open("songs.csv", ios::in);
    vector<string> row;
    string line, word, temp;
    while(getline(fin, temp)){
        row.clear();
        stringstream s(temp);
        while(getline(s, word, ',')){
            row.push_back(word);
        }
        for(int i = 0; i < 5; i++){
            cout << row[i] << " ";
        }
        cout << "\n";
    }
    fin.close();
    cout << "\n";
}
int admin_concrete::add_song(int id){
    string name, genre, artist, location;
    cout << "\nEnter name: ";
    cin >> name;
    cout << "\nEnter genre: ";
    cin >> genre;
    cout << "\nEnter artist: ";
    cin >> artist;
    cout << "\nEnter location: ";
    cin >> location;
    id = cnt_ + 1;
    const char newfile[] = "songs_new.csv";
    const char oldfile[] = "songs.csv";
    fstream fout;
    fstream fin;
    fout.open(newfile, ios::out | ios::app);
    fin.open(oldfile, ios::in);

    vector<string> row;
    string line, word, temp;
    while(getline(fin, temp)){
        row.clear();
        stringstream s(temp);
        while(getline(s, word, ',')){
            row.push_back(word);
        }
        for(int i = 0; i < 6; i++){
            fout << row[i] << ",";
        }
        fout << row[6] << "\n";
    }
    fout << id << "," << name << ",0," << genre << "," << artist << "," << location << "," << "0" << "\n";
    fin.close();
    fout.close();
    if(remove(oldfile) != 0){
        perror("error deleting file\n");
        return 0;
    }
    if(rename(newfile, oldfile) != 0){
        perror("error renaming file\n");
        return 0;
    }
    ++cnt_;
    return 1;
}
int admin_concrete::remove_song(int id){
    if(id > cnt_ || id < 1)   return 0;
    if(playlist_ptr_ > id)
        --playlist_ptr_;
    const char newfile[] = "songs_new.csv";
    const char oldfile[] = "songs.csv";
    fstream fout;
    fstream fin;
    fout.open(newfile, ios::out | ios::app);
    fin.open(oldfile, ios::in);

    vector<string> row;
    string line, word, temp;
    for(int i = 1; i <= cnt_ - 1; i++){
        row.clear();
        getline(fin, temp);
        if(i == id)
            getline(fin, temp);
        stringstream s(temp);
        while(getline(s, word, ',')){
            row.push_back(word);
        }
        row[0] = to_string(i);
        for(int j = 0; j < 6; ++j)
            fout << row[j] << ",";
        fout << row[6] << "\n";
    }
    fin.close();
    fout.close();
    if(remove(oldfile) != 0){
        perror("error deleting file\n");
        return 0;
    }
    if(rename(newfile, oldfile) != 0){
        perror("error renaming file\n");
        return 0;
    }
    --cnt_;
    return 1;
}


int admin_concrete::next_song_id(){
    if(playlist_ptr_ == cnt_)
        playlist_ptr_ = 1;
    else
        playlist_ptr_ = (playlist_ptr_ + 1);
    return playlist_ptr_;
}
int admin_concrete::list_size(){
    return cnt_;
}
void admin_concrete::destroy_instance(){
    if(instance_ != nullptr){
        username_ = "";
        password_ = "";
        email_ = "";
        delete instance_;
        instance_ = nullptr;
        if(player_ != nullptr){
            delete player_;
            player_ = nullptr;
        }
        cnt_ = 0;
        playlist_ptr_ = 0;
    }
}
string admin_concrete::username_ = "";
string admin_concrete::password_ = "";
string admin_concrete::email_ = "";
int admin_concrete::cnt_ = 0;
MusicPlayer* admin_concrete::player_ = nullptr;
admin_concrete* admin_concrete::instance_ = nullptr;
int admin_concrete::playlist_ptr_ = 0;


user_abstract* userFactory::create_user(int utype){
    if(utype == 1 && user_abstract::utype == 0){
        user_abstract::utype = utype;
        admin_concrete* admin_ptr = admin_concrete::get_instance();
        return admin_ptr;
    }
    else if(utype == 2 && user_abstract::utype == 0){
        user_abstract::utype = utype;
        listener_concrete* listener_ptr = listener_concrete::get_instance();
        return listener_ptr;
    }
    else    return nullptr;
}

void userFactory::destroy_user(){
    if(user_abstract::utype == 1){
        admin_concrete::destroy_instance();
        user_abstract::utype = 0;
    }
    else if(user_abstract::utype == 2){
        listener_concrete::destroy_instance();
        user_abstract::utype = 0;
    }
}

