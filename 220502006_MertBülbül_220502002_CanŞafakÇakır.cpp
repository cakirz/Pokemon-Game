#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include <iomanip>

class Pokemon {
private:
    std::string name;
    int damage;
public:
    Pokemon() : name(""), damage(0) {}
    Pokemon(std::string n, int d) : name(n), damage(d) {}

    std::string getName() const { return name; }
    int getDamage() const { return damage; }
    void setName(const std::string& n) { name = n; }
    void setDamage(int d) { damage = d; }

    void show() const {
        std::cout << name << " (Damage: " << damage << ")";
    }

    void showCard() const {
        std::cout << " +---------------------+" << std::endl;
        std::cout << " | " << std::setw(19) << std::left << name << " |" << std::endl;
        std::cout << " |                     |" << std::endl;
        std::cout << " |   Damage Points: " << std::setw(3) << damage << "    |" << std::endl;
        std::cout << " +---------------------+" << std::endl;
    }
};

class Deck {
private:
    std::vector<Pokemon> cards;
public:
    Deck() {
        cards = {
            Pokemon("Pikachu", 100), Pokemon("Charizard", 60),
            Pokemon("Bulbasaur", 50), Pokemon("Squirtle", 50),
            Pokemon("Tauros", 80), Pokemon("Magmar", 70),
            Pokemon("Eevee", 60), Pokemon("Snorlax", 65),
            Pokemon("Dragonite", 75), Pokemon("Arcanine", 90)
        };
        std::shuffle(cards.begin(), cards.end(), std::default_random_engine(static_cast<unsigned>(time(0))));
    }

    Pokemon drawCard() {
        if (cards.empty()) return Pokemon();
        Pokemon card = cards.back();
        cards.pop_back();
        return card;
    }

    int remainingCards() const {
        return cards.size();
    }

    const std::vector<Pokemon>& getCards() const {
        return cards;
    }
};

class Player {
protected:
    std::string name;
    std::vector<Pokemon> hand;
    int score;
public:
    Player() : name(""), score(0) {}
    Player(std::string n) : name(n), score(0) {}

    std::string getName() const { return name; }
    int getScore() const { return score; }
    void setName(const std::string& n) { name = n; }
    void setScore(int s) { score = s; }

    void drawCard(Deck& deck) {
        hand.push_back(deck.drawCard());
    }

    virtual Pokemon playCard() = 0;

    void showHand() const {
        std::cout << name << "'s hand: ";
        for (const auto& card : hand) {
            card.show();
            std::cout << " ";
        }
        std::cout << std::endl;
    }

    std::vector<Pokemon>& getHand() { return hand; }
};

class HumanPlayer : public Player {
public:
    HumanPlayer(std::string n) : Player(n) {}

    Pokemon playCard() override {
        int choice;
        while (true) {
            std::cout << "Choose a card to play (1-" << hand.size() << "): ";
            std::cin >> choice;
            if (choice == 5) {
                std::cout << "Computer's hand: ";
                for (const auto& card : getHand()) {
                    card.show();
                    std::cout << " ";
                }
                std::cout << std::endl;
            }
            else if (choice > 0 && choice <= hand.size()) {
                Pokemon card = hand[choice - 1];
                hand.erase(hand.begin() + (choice - 1));
                return card;
            }
            else {
                std::cout << "Invalid choice, please try again." << std::endl;
            }
        }
    }
};

class ComputerPlayer : public Player {
public:
    ComputerPlayer(std::string n) : Player(n) {}

    Pokemon playCard() override {
        int choice = rand() % hand.size();
        Pokemon card = hand[choice];
        hand.erase(hand.begin() + choice);
        return card;
    }
};

class Game {
private:
    Deck deck;
    Player* player1;
    Player* player2;
    std::vector<Pokemon> playedCards;
    bool isHumanVsComputer;

    void showBoard(const Pokemon& p1_card, const Pokemon& p2_card, bool revealCards) const {
        std::cout << "============================================" << std::endl;
        std::cout << player2->getName() << "'s Cards: " << std::endl;
        for (size_t i = 0; i < player2->getHand().size(); ++i) {
            std::cout << "[#] ";
        }
        std::cout << std::endl;
        std::cout << "============================================" << std::endl;
        std::cout << "Played Cards: ";
        for (const auto& card : playedCards) {
            card.show();
            std::cout << " ";
        }
        std::cout << std::endl;
        std::cout << "============================================" << std::endl;
        std::cout << player1->getName() << "'s Cards: ";
        for (const auto& card : player1->getHand()) {
            card.show();
            std::cout << " ";
        }
        std::cout << std::endl;
        std::cout << "============================================" << std::endl;
        std::cout << "Cards Remaining in Deck: " << deck.remainingCards() << std::endl;

        std::cout << player1->getName() << " plays: " << std::endl;
        if (revealCards) {
            p1_card.showCard();
        }
        else {
            std::cout << " +---------------------+" << std::endl;
            std::cout << " |       POKEMON       |" << std::endl;
            std::cout << " |                     |" << std::endl;
            std::cout << " |   Damage Points: ?? |" << std::endl;
            std::cout << " +---------------------+" << std::endl;
        }
        std::cout << " vs. " << std::endl;
        std::cout << player2->getName() << " plays: " << std::endl;
        if (revealCards) {
            p2_card.showCard();
        }
        else {
            std::cout << " +---------------------+" << std::endl;
            std::cout << " |       POKEMON       |" << std::endl;
            std::cout << " |                     |" << std::endl;
            std::cout << " |   Damage Points: ?? |" << std::endl;
            std::cout << " +---------------------+" << std::endl;
        }
        std::cout << std::endl;
    }

public:
    Game(Player* p1, Player* p2, bool humanVsComputer) : player1(p1), player2(p2), isHumanVsComputer(humanVsComputer) {
        startGame();
    }

    void startGame() {
        for (int i = 0; i < 3; ++i) {
            player1->drawCard(deck);
            player2->drawCard(deck);
        }

        while (!player1->getHand().empty() && !player2->getHand().empty()) {
            turn();
            if (deck.remainingCards() > 0) {
                player1->drawCard(deck);
                player2->drawCard(deck);
            }
        }

        endGame();
    }

    void turn() {
        player1->showHand();
        Pokemon p1_card = player1->playCard();
        Pokemon p2_card = player2->playCard();

        if (isHumanVsComputer) {
            showBoard(p1_card, p2_card, false); 
            std::cout << "Press enter to reveal the cards...";
            std::cin.ignore(); 
            std::cin.get(); 
        }

        showBoard(p1_card, p2_card, true); 

        playedCards.push_back(p1_card);
        playedCards.push_back(p2_card);

        if (p1_card.getDamage() > p2_card.getDamage()) {
            player1->setScore(player1->getScore() + 5);
            std::cout << player1->getName() << " wins the round and gets 5 points." << std::endl;
        }
        else if (p2_card.getDamage() > p1_card.getDamage()) {
            player2->setScore(player2->getScore() + 5);
            std::cout << player2->getName() << " wins the round and gets 5 points." << std::endl;
        }
        else
        {
            std::cout << "Round is a tie, no points awarded." << std::endl;
        }

        std::cout << "Scores -> " << player1->getName() << ": " << player1->getScore() << ", "
            << player2->getName() << ": " << player2->getScore() << std::endl;

        std::cout << "\n\n";
    }

    void endGame() {
        std::cout << "Game Over!" << std::endl;
        std::cout << "Final Scores -> " << player1->getName() << ": " << player1->getScore() << ", "
            << player2->getName() << ": " << player2->getScore() << std::endl;
        if (player1->getScore() > player2->getScore()) {
            std::cout << player1->getName() << " wins the game!" << std::endl;
        }
        else if (player2->getScore() > player1->getScore()) {
            std::cout << player2->getName() << " wins the game!" << std::endl;
        }
        else {
            std::cout << "The game is a tie!" << std::endl;
        }
    }
};

int main() {
    srand(static_cast<unsigned>(time(0)));

    int gameMode;
    std::cout << "Select Game Mode:" << std::endl;
    std::cout << "1. Human vs Computer" << std::endl;
    std::cout << "2. Computer vs Computer" << std::endl;
    std::cout << "Enter your choice: ";
    std::cin >> gameMode;

    if (gameMode == 1) {
        HumanPlayer player1("Player 1");
        ComputerPlayer player2("Computer");
        Game game(&player1, &player2, true);
    }
    else if (gameMode == 2) {
        ComputerPlayer player1("Computer 1");
        ComputerPlayer player2("Computer 2");
        Game game(&player1, &player2, false);
    }
    else {
        std::cout << "Invalid choice." << std::endl;
    }

    return 0;
}
