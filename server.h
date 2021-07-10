#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

#include <string>
#include <iostream>
using namespace std;

//**************************MUSIC PLAYER CLASS USING STATE DESIGN PATTERN*********************************//

class MusicPlayerState;

class MusicPlayer {
public:
	enum State
	{
		ST_STOPPED,
		ST_PLAYING,
		ST_PAUSED
	};

	MusicPlayer(int);
	virtual ~MusicPlayer();

	void Play();
	void Pause();
	void Stop();
	//bool isPlaying();

	void SetState(State state);

private:
	MusicPlayerState * m_pState;
	int song_id_;
};

class MusicPlayerState {
public:
	MusicPlayerState(string name);
	virtual ~MusicPlayerState();

	virtual void Play(MusicPlayer * player);
	virtual void Pause(MusicPlayer * player);
	virtual void Stop(MusicPlayer * player);

	string GetName() { return m_name; }

private:
	string m_name;
};

class PlayingState : public MusicPlayerState {
public:
	PlayingState();
	virtual ~PlayingState();

	virtual void Pause(MusicPlayer * player);
	virtual void Stop(MusicPlayer * player);
};

class PausedState : public MusicPlayerState {
public:
	PausedState();
	virtual ~PausedState();

	virtual void Play(MusicPlayer * player);
	virtual void Stop(MusicPlayer * player);
};

class StoppedState : public MusicPlayerState {
public:
	StoppedState();
	virtual ~StoppedState();

	virtual void Play(MusicPlayer * player);
};


//*****************************USERS CLASSES CREATED USING SINGLETON AND FACTORY DESIGN PATTERNS**********************//

class user_abstract{
private:
    static int utype;
    friend class userFactory;
public:
    virtual bool login() = 0;
    virtual int signup(){cout << "\nNot required for admins\n";}
    virtual void show_functions() = 0;
    virtual void play_song(int id) = 0;
    virtual void pause_song() = 0;
    virtual void resume_song() = 0;
    virtual void stop_song() = 0;
    virtual bool list_song(int) = 0;
    virtual void list_songs() = 0;
    virtual void list_all_songs(){}
    virtual void recommend_song(){cout << "\nNot required for admins\n";}
    virtual int add_song(int id) = 0;
    virtual int remove_song(int id) = 0;
    virtual int like_song(int id){cout << "\nNot required for admins\n";}
    virtual int next_song_id() = 0;
    int play_songs(int, int);
    void play_playlist();
    virtual int list_size() = 0;
    virtual int recommend_similar_songs(int){cout << "\nNot required for admins\n";}
    virtual void play_genre(){cout << "\nNot required for admins\n";}
};


class listener_concrete : public user_abstract{
private:
    static int uid_;
    static string username_;
    static string password_;
    static string email_;
    static string fav_genre_;
    static int* playlist_;
    static int playlist_ptr_;
    static int cnt_;
    static listener_concrete* instance_;
    static MusicPlayer* player_;

    listener_concrete() = default;
    ~listener_concrete() = default;
    listener_concrete& operator=(const listener_concrete&) = default;
    listener_concrete(const listener_concrete&) = default;

public:
    static listener_concrete* get_instance();
    virtual bool login();
    virtual int signup();
    virtual void show_functions();
    virtual void play_song(int id);
    virtual void pause_song();
    virtual void resume_song();
    virtual void stop_song();
    virtual bool list_song(int);
    virtual void list_songs();
    virtual void list_all_songs();
    virtual void recommend_song();
    virtual int add_song(int id);
    virtual int remove_song(int id);
    virtual int like_song(int id);
    virtual int next_song_id();
    virtual int list_size();
    virtual int recommend_similar_songs(int id);
    virtual void play_genre();
    static void destroy_instance();
};


class admin_concrete : public user_abstract{
private:
    static string username_;
    static string password_;
    static string email_;
    static int cnt_;
    static admin_concrete* instance_;
    static MusicPlayer* player_;
    static int playlist_ptr_;

    admin_concrete() = default;
    ~admin_concrete() = default;
    admin_concrete& operator=(const admin_concrete&) = default;
    admin_concrete(const admin_concrete&) = default;
public:
    static admin_concrete* get_instance();
    virtual bool login();
    virtual void show_functions();
    virtual void play_song(int id);
    virtual void pause_song();
    virtual void resume_song();
    virtual void stop_song();
    virtual bool list_song(int);
    virtual void list_songs();
    virtual int add_song(int id);
    virtual int remove_song(int id);
    virtual int next_song_id();
    virtual int list_size();
    static void destroy_instance();
};

class userFactory{
public:
    static user_abstract* create_user(int utype);
    static void destroy_user();
};



#endif // SERVER_H_INCLUDED
