#include "server.h"
#include <bits/stdc++.h>
using namespace std;

int main(){
    user_abstract* ptr;
    int ut;
    while(1){
        int choice1;
        while(1){
            cout << "1. Login\n2. Signup\n3. Exit\n";
            cin >> choice1;
            if(cin.fail())
                cout << "enter valid input\n", cin.clear(), cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if(choice1 == 1 || choice1 == 2 || choice1 == 3)
                break;
        }

        if(choice1 == 3)
            break;
        if(choice1 == 2){
            user_abstract* temp;
            temp = userFactory::create_user(2);
            if(temp->signup()){
                cout << "\nSuccess\n";
            }
            else{
                cout << "\nUsername Already Chosen\n";
            }
            userFactory::destroy_user();
            temp = nullptr;
            continue;
        }
        while(1){
            while(1){
                cout << "login as admin(1) or listener(2)?\n";
                cin >> ut;
                if(ut == 1 || ut == 2)
                    break;
                if(cin.fail())
                    cout << "enter valid input\n", cin.clear(), cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            ptr = userFactory::create_user(ut);
            if(ptr->login()){
                break;
            }
            else{
                userFactory::destroy_user();
                ptr = nullptr;
                cout << "wrong credentials\n";
            }
        }
        int page_no = 1;
        while(1){
            int choice, sid;
            cout << "***************PAGE NUMBER: " << page_no << "***************\n";
            ptr->show_functions();
            cin >> choice;
            if(cin.fail())
                cout << "enter valid input\n", cin.clear(), cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if(choice == 1){
                ptr->list_songs();
            }
            if(choice == 2){
                cout << "Enter song id to view details for: ";
                cin >> sid;
                if(!ptr->list_song(sid))
                    cout << "\nThe song does not exist\n";
            }
            if(choice == 3){
                cout << "Enter song id: ";
                cin >> sid;
                int status = ptr->add_song(sid);
                if(status == 1)
                    cout << "\nSuccess\n";
                else if(status == 0)
                    cout << "\nSong already exists in your playlist/Database\n";
                else if(status == -1)
                    cout << "\nInvalid id\n";
            }
            if(choice == 4){
                cout << "Enter song id: ";
                cin >> sid;
                if(ptr->remove_song(sid))
                    cout << "\nSuccess\n";
                else
                    cout << "\nSong does not exist in your playlist/Database\n";
            }
            if(choice == 5){
                ptr->play_playlist();
            }
            if(choice == 6){
                cout << "Enter song id: ";
                cin >> sid;
                ptr->list_song(sid);
                if(ptr->play_songs(sid, 0))
                    cout << "No such song in database\n";
            }
            if(choice == 7){
                cout << "Enter song id: ";
                cin >> sid;
                int status = ptr->like_song(sid);
                if(status == 1)
                    cout << "\nSuccess\n";
                else if(status == -1)
                    cout << "\nSong does not exist\n";
                else if(status == 0)
                    cout << "\nYou already liked the song\n";
            }
            if(choice == 8){
                ptr->recommend_song();
            }
            if(choice == 9){
                cout << "Enter song id: ";
                cin >> sid;
                if(!ptr->recommend_similar_songs(sid))
                    cout << "\nNo recommended Songs\n";
            }
            if(choice == 10){
                ptr->play_genre();
            }
            if(choice == 11){
                userFactory::destroy_user();
                ptr = nullptr;
                break;
            }
            ++page_no;
        }
    }
    return 0;
}

