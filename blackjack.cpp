// jackblack.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
/*BLACKJACK
Author: Yutong Wang
Date: 2024-02-26
Purpose: create a blackjack game
Misc.:
3.	HARD: Game of Black Jack (card game)

Simulate the game of blackjack
-The game should be complete with a realistic menu screen and the option to replay as well as an option to read the rules.
-use some simple form of ascii art for card display (need to show a suit and number or KQA etc.)
    Can be simple   |A <3|  but better if more detailed ____
                                                        |AoH|
                                                        |___|
-Use a method to display the card
-assume any card is equally likely to be dealt at any time ( even if it already exists, you don’t need unique cards, you don’t need to remember which cards each person has, just how many) hint: use two counters
-you will be playing with an AI
-A’s are either 1 or 11 depending on the game
-splitting, doubling down and other advanced features (other AIs, etc) of the game are bonus
-the player has 100$ to start and can play multiple rounds (omit this feature for below level 4 program)
-be sure to include an ending condition that stops the game when someone runs out of money

GET THE GAME WORKING IN ITS MOST BASIC FORM FIRST. Do not start with asci.  Start by getting the SIMPLE version working first, no ascii, no menu,
*/

// Include necessary libraries
#include <Windows.h>   // Windows API functions
#include <fcntl.h>     // File control functions
#include <io.h>        // File control functions
#include <shellapi.h>  // Functions to interact with the Windows Shell
#include <stdio.h>     // Standard Input/Output functions

#include <algorithm>  // Algorithms on ranges of elements
#include <cmath>      // Mathematical functions
#include <cstdlib>    // General-purpose functions
#include <ctime>      // Functions to work with time
#include <iostream>   // Input/Output stream functions
#include <string>     // String manipulation functions
#include <vector>     // Vector (dynamic array) functions

// Global array representing the deck of cards
int deck[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207 };  // Declares and initialized array

// Function declarations
void wrong();        // Declare function to output error messages
void shuffle();      // Declare function to rearrange card deck order
void cardview(int);  // Declare function to display a card based on the integer value at deck[]
int cardpoint(int);  // Declare function to add the corresponding point of the card to the user/bot point

int main() {  // starts main function of program

    _setmode(_fileno(stdout), _O_U16TEXT);  // Set mode of program to output wchar for Unicode 16 (every std::cout must be std::wcout for the program to run smoothly)

    int action;          // Declare integer variable to store user action
    char again;          // Declare char variable to store user choice
    std::string prompt;  // Declare string variable for user input

    std::wcout << L"Welcome to BlackJack!\n\n";  // Output welcome message

    do {                                                      // Start do-while loop
        do {                                                  // Start do-while loop
            std::wcout << L"\nwhat would you like to do?\n";  // Output message
            std::wcout << L"play the game (1)\n";             // Output option
            std::wcout << L"read the rules (2)\n";            // Output option
            std::wcout << L"quit (3)\n\n";                    // Output option
            std::wcout << L"enter your choice: ";             // Output prompt
            std::cin >> action;                               // Input user action

            if (action < 1 || action > 3) {  // Check if input is invalid
                wrong();                     // Output error message
            }                                //
        } while (action < 1 || action > 3);  // Repeat until a valid input is received

        system("cls");  // Clear console screen

        if (action == 1) {  // Runs if statement to play the game

            int cardcount = 0;             // Declares and initializes integer cardcounter to 0, to sequentially go through the shuffled card deck array
            float user_bet, bot_bet, bet;  // Declares variables as floats for betting amount and balances
            user_bet = 100;                // initializes balances to 100
            bot_bet = 100;                 // initializes balances to 100
            char move;                     // declares char variable
            shuffle();                     // calls to shuffle(); function

            while (true) {  // runs while loop for game mechanism

                int user_point = 0, bot_point = 0, bot_reveal = 0, user_ace = 0, bot_ace = 0;  // declares and initializes integer point, ace and bot reveal point counters as 0
                std::vector<int> user_hand;                                                    // declares user hand vector
                std::vector<int> bot_hand;                                                     // declares bot hand vector

                bool round = false;  // declares and initializes round flag as false
                bool hit = false;    // declares and initializes round flag as false

                while (!round) {  // runs while loop for initial game setup

                    system("cls");  // clears console screen

                    do {                                                                                                                                                            // runs do-while loop for initial game setup
                        std::wcout << L"you have $" << user_bet << std::endl;                                                                                                       // displays user balance
                        std::wcout << L"the maximum you can bet is $" << floor(min(user_bet / 1.5, bot_bet / 1.5)) << " to account for cases where blackjack is hit" << std::endl;  // displays maximum bet
                        std::wcout << L"\nenter the amount you'd like to bet: ";                                                                                                    // prompts for user bet

                        std::cin >> bet;  // takes and reads in user input

                        if (bet > user_bet / 1.5 || bet > bot_bet / 1.5 || bet <= 0) {  // runs if statement for error check
                            wrong();                                                    // calls to error message
                        }                                                               // closes if statement
                    } while (bet > user_bet / 1.5 || bet > bot_bet / 1.5 || bet <= 0);  // closes do-while loop for initial game setup

                    std::wcout << L"dealer's hand:\n";  // outputs dealer hand

                    cardview(deck[cardcount]);  // outputs dealer hand
                    std::wcout << L"\t";        // outputs tab separation
                    cardview(999);              // outputs backcard

                    for (int i = 0; i < 2; i++) {             // runs for loop adding a card to bot hand
                        bot_hand.push_back(deck[cardcount]);  // pushes back index value of card to bot hand

                        if (cardpoint(deck[cardcount]) == 11) {  // runs if statement checking for aces
                            bot_ace++;                           // adds to bot ace count
                        }                                        // closes if statement

                        if (i == 0) {                                  // runs is statement
                            bot_reveal += cardpoint(deck[cardcount]);  // compound operation to add what the bot reveals
                        }                                              // closes if statement

                        bot_point += cardpoint(deck[cardcount]);  // compount operation to add points

                        if (bot_ace == 2) {   // runs if statement
                            bot_point -= 10;  // compound operation subtracting 10 for ace
                            bot_ace--;        // compound operation lowering ace count
                        }                     // closes if statement

                        cardcount++;  // compound operation adding to cardcount to go through deck
                    }                 // closes for loop

                    std::wcout << L"\nthe dealer has revealed " << bot_reveal << " points\n";  // outputs message with bot reveal points

                    std::wcout << L"\n_________________________________\n\n";  // outputs line

                    std::wcout << L"your hand:\n";                 // outputs message
                    for (int i = 0; i < 2; i++) {                  // runs for loop
                        user_hand.push_back(deck[cardcount]);      // pushes back index value to user hand vector
                        if (cardpoint(deck[cardcount]) == 11) {    // runs if statement
                            user_ace++;                            // compound operation for user ace
                        }                                          // closes if statement
                        user_point += cardpoint(deck[cardcount]);  // compound operation for user point
                        if (user_ace == 2) {                       // runs if statement
                            user_point -= 10;                      // compound operation lowering points given ace detected
                            user_ace--;                            // compound operation subtracting ace count
                        }                                          // closes if statement
                        cardview(deck[cardcount]);                 // outputs card
                        std::wcout << L"\t";                       // outputs tab
                        cardcount++;                               // compound operation increases card count
                    }                                              // closes for loop

                    std::wcout << L"\n\nyou have " << user_point << " points\n\n";  // outputs points to user

                    if (user_point == 21) {                                                     // if statement checking if points are 21
                        std::wcout << L"BLACKJACK!\n";                                          // outputs message
                        std::wcout << "you made $" << bet << "x1.5" << std::endl;               // outputs initial balance
                        user_bet += floor(bet * 1.5);                                           // reassigns user balance
                        bot_bet -= floor(bet * 1.5);                                            // reassigns bot balance
                        std::wcout << L"\nyour updated balance is $" << user_bet << std::endl;  // outputs updated balance

                        break;  // breaks out of while loop
                    }           // closes if statement

                    round = true;  // reassigns round flag to true
                }                  // closes while loop

                while (round) {                                                          // runs while loop for hit and stand
                    do {                                                                 // runs do-while loop for char user input
                        std::wcout << L"\nwould you like to hit or stand? (h or s) : ";  // use prompt

                        std::cin >> move;  // takes and reads in user input

                        if (move != 'h' && move != 's') {  // if statement checking for valid input
                            wrong();                       // error message output void function
                        }                                  // closes is statement

                    } while (move != 'h' && move != 's');  // closes do-while loop

                    if (move == 'h') {  // run if statement
                        hit = true;     // reassigns bool hit flag to true
                    }                   // closes if statement
                    else {              // runs else
                        hit = false;    // reassigns bool hit flag to false
                    }                   // closes else

                    if (move == 'h') {  // runs if statement

                        system("cls");                                           // clears console screen
                        std::wcout << L"you have $" << user_bet << std::endl;    // outputs
                        std::wcout << L"you are betting: " << bet << std::endl;  // outputs bet
                        std::wcout << L"dealer's hand:\n";                       // outputs message

                        cardview(bot_hand[0]);  // outputs first card in bot's hand
                        std::wcout << L"\t";    // outputs tab
                        cardview(999);          // outputs backcard

                        std::wcout << L"\nthe dealer has revealed " << bot_reveal << " points\n";  // outputs bot reveal points of first card

                        std::wcout << L"\n_________________________________\n\n";  // outputs line
                        std::wcout << L"your hand:\n";                             // outputs message
                        for (int i = 0; i < user_hand.size(); i++) {               // runs for loop
                            cardview(user_hand[i]);                                // output card
                            std::wcout << L"\t";                                   // outputs tab
                        }                                                          // closes for loop

                        user_hand.push_back(deck[cardcount]);      // pushes back index value in user hand index
                        if (cardpoint(deck[cardcount]) == 11) {    //  if statement checking for ace
                            user_ace++;                            // compound operation increasign ace count
                        }                                          // closes if statement
                        user_point += cardpoint(deck[cardcount]);  // compound operation adding points to user point
                        if (user_point > 21 && user_ace >= 1) {    // runs if statement
                            user_point -= 10;                      // compound operation subtracting points if ace is detected
                            user_ace--;                            // compoun operation subtracting ace count
                        }                                          // closes if statement
                        cardview(deck[cardcount]);                 // display card
                        std::wcout << L"\t";                       // output tab
                        cardcount++;                               // compound operation increasing card count

                        std::wcout << L"\n\nyou have " << user_point << " points\n\n";  // output user points

                        if (user_point >= 21) {  // runs if statement

                            hit = false;  // reassigns bool flag for hit as false
                        }                 // closes if statement

                    }  // closes if statement

                    if (!hit) {  // runs if statement

                        while (bot_point < 17) {                                     // runs while loop
                            system("cls");                                           // clears system console
                            std::wcout << L"you have $" << user_bet << std::endl;    // outputs user amount
                            std::wcout << L"you are betting: " << bet << std::endl;  // outputs bet amount
                            std::wcout << L"dealer's hand:\n";                       // outputs message

                            for (int i = 0; i < bot_hand.size(); i++) {  // runs for loop
                                cardview(bot_hand[i]);                   // displays cards
                                std::wcout << L"\t";                     // outputs tabs
                            }                                            // closes for loop

                            bot_hand.push_back(deck[cardcount]);     // pushes back cards to bot hand vector
                            if (cardpoint(deck[cardcount]) == 11) {  // checks for ace
                                bot_ace++;                           // add to ace count
                            }                                        // closes if statement

                            bot_point += cardpoint(deck[cardcount]);  // compound operation adding to bot point
                            if (bot_point > 21 && bot_ace >= 1) {     // runs if statement
                                bot_point -= 10;                      // compound operation lowering points if ace is detected
                                bot_ace--;                            // compount operation subtracting ace count
                            }                                         // closes if statement
                            cardview(deck[cardcount]);                // displays card
                            std::wcout << L"\t";                      // outputs tab
                            cardcount++;                              // adds to cardcount

                            std::wcout << L"\nthe dealer has " << bot_point << " points\n";  // outputs bot point

                            std::wcout << L"\n_________________________________\n\n";  // outputs line
                            std::wcout << L"your hand:\n";                             // outputs message
                            for (int i = 0; i < user_hand.size(); i++) {               // runs for loop
                                cardview(user_hand[i]);                                // displays card
                                std::wcout << L"\t";                                   // output tab
                            }                                                          // closes for loop

                        }  // closes while loop

                        if (bot_point >= 17) {                                       // runs if statement
                            system("cls");                                           // clears system console
                            std::wcout << L"you have $" << user_bet << std::endl;    // outputs balance
                            std::wcout << L"you are betting: " << bet << std::endl;  // outputs bet amount
                            std::wcout << L"dealer's hand:\n";                       // outputs message

                            for (int i = 0; i < bot_hand.size(); i++) {                      // runs for loop
                                cardview(bot_hand[i]);                                       // function call to display card
                                std::wcout << L"\t";                                         // output tab
                            }                                                                // closes for loop
                            std::wcout << L"\nthe dealer has " << bot_point << " points\n";  // displays bot point

                            std::wcout << L"\n_________________________________\n\n";                   // displays line
                            std::wcout << L"your hand:\n";                                              // outputs message
                            for (int i = 0; i < user_hand.size(); i++) {                                // runs for loop
                                cardview(user_hand[i]);                                                 // calls to functions to display card
                                std::wcout << L"\t";                                                    // outputs tab
                            }                                                                           // closes for loop
                            std::wcout << L"\n";                                                        // outputs new line
                            if (bot_point == 21 && bot_hand.size() == 2) {                              // runs if statement
                                std::wcout << L"the bot hit BLACKJACK!\n";                              // outputs message
                                std::wcout << "it made $" << bet << "x1.5" << std::endl;                // outputs message
                                user_bet -= floor(bet * 1.5);                                           // compound operation to reassign balance
                                bot_bet += floor(bet * 1.5);                                            // compound operation to reassign balance
                                std::wcout << L"\nyour updated balance is $" << user_bet << std::endl;  // outputs updated balance

                                break;                    // breaks out of while loop
                            }
                            else if (bot_point > 21) {  // runs else if

                                std::wcout << L"\nthe bot busted with " << bot_point << " points. you win!\n";  // outputs bot point
                                std::wcout << L"\nyou had $" << user_bet;                                       // outputs balance
                                user_bet += bet;                                                                // compound operation to reassign balance
                                bot_bet -= bet;                                                                 // compound operation to reassign balance

                                std::wcout << L"\nyou bet $" << bet;                                    // outputs bet
                                std::wcout << L"\nyour updated balance is $" << user_bet << std::endl;  // outputs new balance
                                break;                                                                  // breaks out of while loop

                            }                                                                                     // closes else if
                            else if (user_point > 21) {                                                           // runs else if
                                std::wcout << L"\nyou busted with " << user_point << " points. the bot wins.\n";  // outputs user point
                                std::wcout << L"\nyou had $" << user_bet;                                         // otuputs user balance
                                user_bet -= bet;                                                                  // compound operation to reassign balance
                                bot_bet += bet;                                                                   // compound operation to reassign balance
                                std::wcout << L"\nyou bet $" << bet;                                              // outputs bet amount
                                std::wcout << L"\nyour updated balance is $" << user_bet << std::endl;            // outputs new balance
                                round = false;                                                                    // set bool round flag to false
                                break;                                                                            // breaks while loop

                            }                                                                           // closes else if
                            else if (bot_point <= 21 && bot_point >= 17 && bot_point > user_point) {    // runs else if
                                std::wcout << L"\nthe bot wins\n";                                      // outputs message
                                std::wcout << L"\nyou had $" << user_bet;                               // outputs user balance
                                bot_bet += bet;                                                         // compound operation to reassign balance
                                user_bet -= bet;                                                        // compound operation to reassign balance
                                std::wcout << L"\nyou bet $" << bet;                                    // outputs bet
                                std::wcout << L"\nyour updated balance is $" << user_bet << std::endl;  // outputs updated balance
                                round = false;                                                          // reassigns bool flag for round to false
                                break;                                                                  // breaks while loop

                            }                                                                           // closes else if
                            else if (bot_point <= 21 && bot_point >= 17 && bot_point < user_point) {    // runs else if
                                std::wcout << L"\nyou win!\n";                                          // otuputs message
                                std::wcout << L"\nyou had $" << user_bet;                               // outputs user balance
                                user_bet += bet;                                                        // compound operation to reassign balance
                                bot_bet -= bet;                                                         // compound operation to reassign balance
                                std::wcout << L"\nyou bet $" << bet;                                    // outputs bet amoutn
                                std::wcout << L"\nyour updated balance is $" << user_bet << std::endl;  // outputs new balance
                                round = false;                                                          // reassigns bool flag for round to false
                                break;                                                                  // breaks while loop

                            }                                                                           // closes else if
                            else if (bot_point == user_point && bot_point < 21 && bot_point > 17) {     // runs else if
                                std::wcout << L"\ndraw\n";                                              // outputs message
                                std::wcout << L"\nyou had $" << user_bet;                               // outputs intitial amount
                                std::wcout << L"\nyou bet $" << bet;                                    // outputs bet amount
                                std::wcout << L"\nyour updated balance is $" << user_bet << std::endl;  // outputs updated balance
                                round = false;                                                          // reassigns bool round flag to false
                                break;                                                                  // breaks while loop
                            }                                                                           // closes else if

                        }  // closes if statement

                    }  // closes if statement
                }      // closes while loop

                if (floor(user_bet / 1.5) > 0 && floor(bot_bet / 1.5) > 0) {                   // is statement checking for if the balances are enough to bet
                    do {                                                                       // runs do-while loop
                        std::wcout << L"\nwould you like to play another round? (y or n) : ";  // outputs user prompt

                        std::cin >> move;  // takes in and reads user input

                        if (move != 'y' && move != 'n') {  // if statement checking for error
                            wrong();                       // function call to output error message
                        }                                  // closes if statement

                    } while (move != 'y' && move != 'n');  // closes do-while loop

                    if (move == 'n') {                                   // if statement
                        break;                                           // break while loop
                    }                                                    // closes if statement
                }                                                        // closes if statement
                else {                                                   // runs else
                    if (floor(bot_bet / 1.5) <= 0) {                     // runs if statement
                        std::wcout << L"\nthe bot ran out of money.\n";  // outputs message
                    }                                                    // closes if statement
                    else {                                               // runs else
                        std::wcout << L"\nyou ran out of money.\n";      // outputs message
                    }                                                    // closes else
                    break;                                               // breaks out of while loop
                }                                                        // closes else

            }  // closes while loop

        }                        // closes if statement
        else if (action == 2) {  // runs else if
            std::wcout << L"here are the rules:\n"
                << std::endl;                                                                // outputs message
            ShellExecute(0, 0, L"https://bicyclecards.com/how-to-play/blackjack/", 0, 0, SW_SHOW);  // uses ShellExecute(); function to open webpage
        }                                                                                           // closes else if
        do {                                                                                        // Start do-while loop
            std::wcout << L"\nwould you like to go back to the menu? (y or n): ";                   // Output prompt
            std::cin >> again;                                                                      // Input user choice
            if (again != 'y' && again != 'n') {                                                     // runs if statement
                wrong();                                                                            // Output error message
            }                                                                                       // closes if statement
        } while (again != 'y' && again != 'n');                                                     // Repeat until a valid input is received
        if (again == 'n' || action == 3) {                                                          // runs if statement to break if the user elects to leave
            break;                                                                                  // Exit loop if the user chooses not to go back to the menu or selects the quit option
        }                                                                                           // closes if statement
        system("cls");                                                                              // Clear console screen
    } while (again == 'y');                                                                         // Repeat until the user chooses not to go back to the menu
    std::wcout << L"\nleave my casino.";                                                            // Output farewell message

    return 0;  // End program
}  // Close main function

void wrong() {  // Defines function to display error message

    std::wcout << std::endl
        << std::endl;          // Output new lines
    std::wcout << R"(
_________________________________
 _      _____  ____  _  _______
| | /| / / _ \/ __ \/ |/ / ___/
| |/ |/ / , _/ /_/ /    / (_ /
|__/|__/_/|_|\____/_/|_/\___/
try again with valid input
_________________________________)";  // Outputs raw string literal for ASCII art
    std::wcout << std::endl
        << std::endl;  // Output new lines

}  // End of the function block with closing curly brace

void shuffle() {                           // Defines function to shuffle the deck of cards
    srand(time(NULL));                     // Seed the random number generator with the current time
    for (int i = 0; i < 208; i++) {        // Loop through each card in the deck (208 cards)
        int r = i + (rand() % (208 - i));  // Generate a random index within the remaining unshuffled cards
        std::swap(deck[i], deck[r]);       // Swap the current card with the randomly chosen card
    }                                      // End of the loop
}  // End of the function block with closing curly brace

void cardview(int amount) {  // defines function to output card

    if (amount == 999) {              // runs if statement flaging sentinel value for backcard
        std::wcout << L"\U0001F0A0";  // outputs unicode
    }                                 // closes if statement
    else {                            // runs else
        int x = amount % 52;          // declares and initializes x as value%52 to indentify card from 4 decks

        switch (x) {                          // runs switch
        case 0:                           // runs case
            std::wcout << L"\U0001F0A1";  // outputs unicode
            break;                        // breaks case
        case 1:                           // runs case
            std::wcout << L"\U0001F0B1";  // outputs unicode

            break;                        // breaks case
        case 2:                           // runs case
            std::wcout << L"\U0001F0C1";  // outputs unicode

            break;                        // breaks case
        case 3:                           // runs case
            std::wcout << L"\U0001F0D1";  // outputs unicode

            break;                        // breaks case
        case 4:                           // runs case
            std::wcout << L"\U0001F0A2";  // outputs unicode

            break;                        // breaks case
        case 5:                           // runs case
            std::wcout << L"\U0001F0B2";  // outputs unicode

            break;                        // breaks case
        case 6:                           // runs case
            std::wcout << L"\U0001F0C2";  // outputs unicode

            break;                        // breaks case
        case 7:                           // runs case
            std::wcout << L"\U0001F0D2";  // outputs unicode

            break;                        // breaks case
        case 8:                           // runs case
            std::wcout << L"\U0001F0A3";  // outputs unicode

            break;                        // breaks case
        case 9:                           // runs case
            std::wcout << L"\U0001F0B3";  // outputs unicode

            break;                        // breaks case
        case 10:                          // runs case
            std::wcout << L"\U0001F0C3";  // outputs unicode

            break;                        // breaks case
        case 11:                          // runs case
            std::wcout << L"\U0001F0D3";  // outputs unicode

            break;                        // breaks case
        case 12:                          // runs case
            std::wcout << L"\U0001F0A4";  // outputs unicode

            break;                        // breaks case
        case 13:                          // runs case
            std::wcout << L"\U0001F0B4";  // outputs unicode

            break;                        // breaks case
        case 14:                          // runs case
            std::wcout << L"\U0001F0C4";  // outputs unicode

            break;                        // breaks case
        case 15:                          // runs case
            std::wcout << L"\U0001F0D4";  // outputs unicode

            break;                        // breaks case
        case 16:                          // runs case
            std::wcout << L"\U0001F0A5";  // outputs unicode

            break;                        // breaks case
        case 17:                          // runs case
            std::wcout << L"\U0001F0B5";  // outputs unicode

            break;                        // breaks case
        case 18:                          // runs case
            std::wcout << L"\U0001F0C5";  // outputs unicode

            break;                        // breaks case
        case 19:                          // runs case
            std::wcout << L"\U0001F0D5";  // outputs unicode

            break;                        // breaks case
        case 20:                          // runs case
            std::wcout << L"\U0001F0A6";  // outputs unicode

            break;                        // breaks case
        case 21:                          // runs case
            std::wcout << L"\U0001F0B6";  // outputs unicode

            break;                        // breaks case
        case 22:                          // runs case
            std::wcout << L"\U0001F0C6";  // outputs unicode

            break;                        // breaks case
        case 23:                          // runs case
            std::wcout << L"\U0001F0D6";  // outputs unicode

            break;                        // breaks case
        case 24:                          // runs case
            std::wcout << L"\U0001F0A7";  // outputs unicode

            break;                        // breaks case
        case 25:                          // runs case
            std::wcout << L"\U0001F0B7";  // outputs unicode

            break;                        // breaks case
        case 26:                          // runs case
            std::wcout << L"\U0001F0C7";  // outputs unicode

            break;                        // breaks case
        case 27:                          // runs case
            std::wcout << L"\U0001F0D7";  // outputs unicode

            break;                        // breaks case
        case 28:                          // runs case
            std::wcout << L"\U0001F0A8";  // outputs unicode

            break;                        // breaks case
        case 29:                          // runs case
            std::wcout << L"\U0001F0B8";  // outputs unicode

            break;                        // breaks case
        case 30:                          // runs case
            std::wcout << L"\U0001F0C8";  // outputs unicode

            break;                        // breaks case
        case 31:                          // runs case
            std::wcout << L"\U0001F0D8";  // outputs unicode

            break;                        // breaks case
        case 32:                          // runs case
            std::wcout << L"\U0001F0A9";  // outputs unicode

            break;                        // breaks case
        case 33:                          // runs case
            std::wcout << L"\U0001F0B9";  // outputs unicode

            break;                        // breaks case
        case 34:                          // runs case
            std::wcout << L"\U0001F0C9";  // outputs unicode

            break;                        // breaks case
        case 35:                          // runs case
            std::wcout << L"\U0001F0D9";  // outputs unicode

            break;                        // breaks case
        case 36:                          // runs case
            std::wcout << L"\U0001F0AA";  // outputs unicode

            break;                        // breaks case
        case 37:                          // runs case
            std::wcout << L"\U0001F0BA";  // outputs unicode

            break;                        // breaks case
        case 38:                          // runs case
            std::wcout << L"\U0001F0CA";  // outputs unicode

            break;                        // breaks case
        case 39:                          // runs case
            std::wcout << L"\U0001F0DA";  // outputs unicode

            break;                        // breaks case
        case 40:                          // runs case
            std::wcout << L"\U0001F0AB";  // outputs unicode

            break;                        // breaks case
        case 41:                          // runs case
            std::wcout << L"\U0001F0BB";  // outputs unicode

            break;                        // breaks case
        case 42:                          // runs case
            std::wcout << L"\U0001F0CB";  // outputs unicode

            break;                        // breaks case
        case 43:                          // runs case
            std::wcout << L"\U0001F0DB";  // outputs unicode

            break;                        // breaks case
        case 44:                          // runs case
            std::wcout << L"\U0001F0AD";  // outputs unicode

            break;                        // breaks case
        case 45:                          // runs case
            std::wcout << L"\U0001F0BD";  // outputs unicode

            break;                        // breaks case
        case 46:                          // runs case
            std::wcout << L"\U0001F0CD";  // outputs unicode

            break;                        // breaks case
        case 47:                          // runs case
            std::wcout << L"\U0001F0DD";  // outputs unicode

            break;                        // breaks case
        case 48:                          // runs case
            std::wcout << L"\U0001F0AE";  // outputs unicode

            break;                        // breaks case
        case 49:                          // runs case
            std::wcout << L"\U0001F0BE";  // outputs unicode

            break;                        // breaks case
        case 50:                          // runs case
            std::wcout << L"\U0001F0CE";  // outputs unicode

            break;                        // breaks case
        case 51:                          // runs case
            std::wcout << L"\U0001F0DE";  // outputs unicode

            break;  // breaks case
        }               // closes switch
    }                   // closes else
}  // closese function defintion

int cardpoint(int value) {  // Defines function returning point values of cards

    int x, point;  // declares int x and point

    x = value % 52;  // initializes x as value%52 to indentify card from 4 decks

    switch (x) {  // runs switch for int x
    case 0:
    case 1:
    case 2:
    case 3:          // runs for cases
        point = 11;  // initializes point to 11
        break;       // breaks case
    case 4:
    case 5:
    case 6:
    case 7:         // runs for cases
        point = 2;  // initializes point to 2
        break;      // breaks case
    case 8:
    case 9:
    case 10:
    case 11:        // runs for cases
        point = 3;  // initializes point to 3
        break;      // breaks case
    case 12:
    case 13:
    case 14:
    case 15:        // runs for cases
        point = 4;  // initializes point to 4
        break;      // breaks case
    case 16:
    case 17:
    case 18:
    case 19:        // runs for cases
        point = 5;  // initializes point to 5
        break;      // breaks case
    case 20:
    case 21:
    case 22:
    case 23:        // runs for cases
        point = 6;  // initializes point to 6
        break;      // breaks case
    case 24:
    case 25:
    case 26:
    case 27:        // runs for cases
        point = 7;  // initializes point to 7
        break;      // breaks case
    case 28:
    case 29:
    case 30:
    case 31:        // runs for cases
        point = 8;  // initializes point to 8
        break;      // breaks case
    case 32:
    case 33:
    case 34:
    case 35:         // runs for cases
        point = 9;   // initializes point to 9
        break;       // breaks case
    default:         // default case
        point = 10;  // initializes point to 10
    }                    // closes switch

    return point;  // returns integer value point
}  // closes function definetion