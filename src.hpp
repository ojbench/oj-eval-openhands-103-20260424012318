#ifndef GAME_H
#define GAME_H

#include <string>

struct Node;
struct GameState;
class Card;
class Pile;

/**
 * Skill card type
 * MULTIPLIER: Increases the score multiplier for the next score card
 * HEAD_BUTT: Moves the card at the bottom (tail) of the discard pile to the top (head) of the draw pile
 */
enum SkillCardType {
    MULTIPLIER,
    HEAD_BUTT,
};

/** 
 * Card base class
 * TODO: Add member variables and functions if needed
 */
class Card {
public:
    explicit Card() = default;
    virtual ~Card() = default;
    // Execute card effect, implemented by derived classes. You need to implement the specific effect of the current card type in each derived class
    virtual void play(GameState& game_state) = 0;
};

/** 
 * Score card class
 * TODO: Add member variables and functions as needed
 * You can modify the destructor if needed
 */
class ScoreCard: public Card {
private:
    int point_;
public:
    explicit ScoreCard(int point);
    void play(GameState &game_state) override;
    ~ScoreCard() override = default;
};

/** 
 * Skill card class
 * TODO: Add member variables and functions as needed
 * You can modify the destructor if needed
 */
class SkillCard: public Card {
private:
    SkillCardType skill_type_;
public:
    explicit SkillCard(SkillCardType skill_card_type);
    void play(GameState &game_state) override;
    ~SkillCard() override = default;
};

/**
 * Power card class
 * TODO: Add member variables and functions as needed
 * You can modify the destructor if needed
 */
class PowerCard: public Card {
private:
    int plus_count_;
public:
    explicit PowerCard(int plus_count);
    void play(GameState &game_state) override;
    ~PowerCard() override = default;
};

/** 
 * Linked list node structure
 * TODO: Add or modify member variables and functions as needed
 */
struct Node {
    Card* card;
    Node* next;
    
    Node(Card* c) : card(c), next(nullptr) {}
};

/** 
 * Pile class
 * All three piles (draw pile, discard pile, hand pile) are represented using this class
 * TODO: Modify as needed, add member variables and functions
 */
class Pile {
private:
    int size_;
    Node* head_;
    Node* tail_;

public:
    friend void outShuffle(GameState&);
    friend void inShuffle(GameState&);
    friend void oddEvenShuffle(GameState&);
    friend class GameController;

    // Constructor and destructor declarations
    // Please implement them in the later part of the file
    Pile();
    ~Pile();

    // Return pile size
    int size() const {
        return size_;
    }

    // Check if pile is empty
    bool empty() const {
        return size_ == 0;
    }

    // Add a card to the end of the pile
    // This function is used to initialize the pile in main.cpp
    // Please implement this function in the later part of the file
    void appendCard(Card* card);
    
    // Helper functions
    Card* removeHead();
    Card* removeTail();
    void appendToTail(Card* card);
    void appendToHead(Card* card);
};

/**
 * Game state structure
 * Stores current game state information such as discard pile, draw pile, hand pile status, total score, current multiplier, etc.
 * TODO: Add member variables and functions as needed
 */
struct GameState {
    Pile hand_{};            // Hand pile
    Pile draw_pile_{};       // Draw pile
    Pile discard_pile_{};    // Discard pile
    int total_score_ = 0;    // Total score
    int multiplier_ = 1;     // Current multiplier
    int power_plus_ = 0;     // Accumulated power plus
};

/** 
 * Shuffle function declarations
 * outShuffle: Shuffle the discard pile into the draw pile in order
 * inShuffle: Shuffle the discard pile into the draw pile in reverse order
 * oddEvenShuffle: Separate the discard pile by odd and even positions, with odd positions first and even positions last
 * Please implement these functions in the later part of this file
*/
void outShuffle(GameState &game_state);
void inShuffle(GameState &game_state);
void oddEvenShuffle(GameState &game_state);

/**
 * Game controller class
 * Used to handle game logic
 */
class GameController {
private:
    GameState game_state_;
    // Shuffle function pointer
    void (*shuffle_)(GameState&);
public:
    explicit GameController(int mode);

    // Draw 5 cards from the draw pile to the hand pile
    // This is a function declaration, please implement this function in the later part of the file
    void draw();

    // Play the card_to_play-th card in the hand pile (counting from 1)
    // This is a function declaration, please implement this function in the later part of the file
    void play(int card_to_play);

    // End the current turn, move all cards in the hand pile to the discard pile
    // This is a function declaration, please implement this function in the later part of the file
    void finish();

    // Shuffle in the specified manner
    // This is a function declaration, please implement this function in the later part of the file
    void shuffle();

    // Query current total score
    // This is a function declaration, please implement this function in the later part of the file
    int queryScore();

    // Query draw pile size
    int queryDrawPileSize() const {
        return game_state_.draw_pile_.size();
    }

    // Query hand pile size
    int queryHandSize() const {
        return game_state_.hand_.size();
    }

    // Query discard pile size
    int queryDiscardPileSize() const {
        return game_state_.discard_pile_.size();
    }

    // Get reference to the draw pile
    // Used to load cards in main.cpp
    Pile& drawPile() {
        return game_state_.draw_pile_;
    }
};

// ================= 洗牌函数实现 ===================

/**
 * Shuffle the discard pile into the draw pile in order
 * TODO: Implement this function
 */
void outShuffle(GameState& game_state) {
    // Move entire discard pile to draw pile tail in order
    if (game_state.discard_pile_.head_ == nullptr) return;
    
    if (game_state.draw_pile_.tail_ == nullptr) {
        game_state.draw_pile_.head_ = game_state.discard_pile_.head_;
        game_state.draw_pile_.tail_ = game_state.discard_pile_.tail_;
    } else {
        game_state.draw_pile_.tail_->next = game_state.discard_pile_.head_;
        game_state.draw_pile_.tail_ = game_state.discard_pile_.tail_;
    }
    
    game_state.draw_pile_.size_ += game_state.discard_pile_.size_;
    game_state.discard_pile_.head_ = nullptr;
    game_state.discard_pile_.tail_ = nullptr;
    game_state.discard_pile_.size_ = 0;
}

/**
 * Shuffle the discard pile into the draw pile in reverse order
 * TODO: Implement this function
 */
void inShuffle(GameState& game_state) {
    // Reverse discard pile and append to draw pile tail
    if (game_state.discard_pile_.head_ == nullptr) return;
    
    Node* prev = nullptr;
    Node* current = game_state.discard_pile_.head_;
    Node* next = nullptr;
    Node* new_tail = game_state.discard_pile_.head_;
    
    while (current != nullptr) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    
    if (game_state.draw_pile_.tail_ == nullptr) {
        game_state.draw_pile_.head_ = prev;
        game_state.draw_pile_.tail_ = new_tail;
    } else {
        game_state.draw_pile_.tail_->next = prev;
        game_state.draw_pile_.tail_ = new_tail;
    }
    
    game_state.draw_pile_.size_ += game_state.discard_pile_.size_;
    game_state.discard_pile_.head_ = nullptr;
    game_state.discard_pile_.tail_ = nullptr;
    game_state.discard_pile_.size_ = 0;
}

/**
 * Separate the discard pile by odd and even positions, with odd positions first and even positions last, and shuffle into the draw pile
 * TODO: Implement this function
 */
void oddEvenShuffle(GameState& game_state) {
    if (game_state.discard_pile_.head_ == nullptr) return;
    
    Node* odd_head = nullptr;
    Node* odd_tail = nullptr;
    Node* even_head = nullptr;
    Node* even_tail = nullptr;
    
    Node* current = game_state.discard_pile_.head_;
    int index = 1;
    
    while (current != nullptr) {
        Node* next = current->next;
        current->next = nullptr;
        
        if (index % 2 == 1) {
            if (odd_head == nullptr) {
                odd_head = current;
                odd_tail = current;
            } else {
                odd_tail->next = current;
                odd_tail = current;
            }
        } else {
            if (even_head == nullptr) {
                even_head = current;
                even_tail = current;
            } else {
                even_tail->next = current;
                even_tail = current;
            }
        }
        
        current = next;
        index++;
    }
    
    // Combine odd and even lists
    Node* new_head = odd_head;
    Node* new_tail = even_tail ? even_tail : odd_tail;
    
    if (odd_tail && even_head) {
        odd_tail->next = even_head;
    }
    
    // Append to draw pile
    if (game_state.draw_pile_.tail_ == nullptr) {
        game_state.draw_pile_.head_ = new_head;
        game_state.draw_pile_.tail_ = new_tail;
    } else {
        game_state.draw_pile_.tail_->next = new_head;
        game_state.draw_pile_.tail_ = new_tail;
    }
    
    game_state.draw_pile_.size_ += game_state.discard_pile_.size_;
    game_state.discard_pile_.head_ = nullptr;
    game_state.discard_pile_.tail_ = nullptr;
    game_state.discard_pile_.size_ = 0;
}
    
// ======================================================




// ================== Game State Structure Implementation ===================
// If you added functions to the GameState structure, you can implement them here





// ======================================================




// ================= Card Class Implementation ===========================

// === Score Card Class Implementation ===
ScoreCard::ScoreCard(int point) : point_(point) {
}

void ScoreCard::play(GameState &game_state) {
    int score = (point_ + game_state.power_plus_) * game_state.multiplier_;
    game_state.total_score_ += score;
    game_state.multiplier_ = 1;
}
// ===================

// === Skill Card Class Implementation ===
SkillCard::SkillCard(SkillCardType skill_card_type) : skill_type_(skill_card_type) {
}

void SkillCard::play(GameState &game_state) {
    if (skill_type_ == MULTIPLIER) {
        game_state.multiplier_++;
    } else if (skill_type_ == HEAD_BUTT) {
        if (!game_state.discard_pile_.empty()) {
            Card* card = game_state.discard_pile_.removeTail();
            game_state.draw_pile_.appendToHead(card);
        }
    }
}
// ===================

// === Power Card Class Implementation ===
PowerCard::PowerCard(int plus_count) : plus_count_(plus_count) {
}

void PowerCard::play(GameState &game_state) {
    game_state.power_plus_ += plus_count_;
}
// ===================

// ======================================================




// ================= Pile Class Implementation ===========================
Pile::Pile() : size_(0), head_(nullptr), tail_(nullptr) {
}

Pile::~Pile() {
    Node* current = head_;
    while (current != nullptr) {
        Node* next = current->next;
        delete current->card;
        delete current;
        current = next;
    }
}

/**
  * Add a card to the end of the pile
  * TODO: Implement this function
  */
void Pile::appendCard(Card* card) {
    appendToTail(card);
}

Card* Pile::removeHead() {
    if (head_ == nullptr) return nullptr;
    
    Node* node = head_;
    Card* card = node->card;
    head_ = head_->next;
    if (head_ == nullptr) {
        tail_ = nullptr;
    }
    delete node;
    size_--;
    return card;
}

Card* Pile::removeTail() {
    if (tail_ == nullptr) return nullptr;
    
    if (head_ == tail_) {
        Card* card = tail_->card;
        delete tail_;
        head_ = nullptr;
        tail_ = nullptr;
        size_--;
        return card;
    }
    
    Node* current = head_;
    while (current->next != tail_) {
        current = current->next;
    }
    
    Card* card = tail_->card;
    delete tail_;
    tail_ = current;
    tail_->next = nullptr;
    size_--;
    return card;
}

void Pile::appendToTail(Card* card) {
    Node* node = new Node(card);
    if (tail_ == nullptr) {
        head_ = node;
        tail_ = node;
    } else {
        tail_->next = node;
        tail_ = node;
    }
    size_++;
}

void Pile::appendToHead(Card* card) {
    Node* node = new Node(card);
    if (head_ == nullptr) {
        head_ = node;
        tail_ = node;
    } else {
        node->next = head_;
        head_ = node;
    }
    size_++;
}

// ======================================================




// ================= Game Controller Class Implementation ======================

/**
 * Constructor, initialize game state and shuffle mode
 * @param mode Shuffle mode, 1 for outShuffle, 2 for inShuffle, 3 for oddEvenShuffle
 * TODO: Implement this function
 */
GameController::GameController(int mode){
    if (mode == 1) {
        shuffle_ = outShuffle;
    } else if (mode == 2) {
        shuffle_ = inShuffle;
    } else if (mode == 3) {
        shuffle_ = oddEvenShuffle;
    }
}

/**
 * Draw 5 cards from the draw pile to the hand pile
 * TODO: Implement this function
 */
void GameController::draw() {
    for (int i = 0; i < 5; i++) {
        if (game_state_.draw_pile_.empty()) {
            if (!game_state_.discard_pile_.empty()) {
                shuffle();
            } else {
                break;
            }
        }
        
        if (!game_state_.draw_pile_.empty()) {
            Card* card = game_state_.draw_pile_.removeHead();
            game_state_.hand_.appendToTail(card);
        }
    }
}

/**
 * Play the card_to_play-th card in the hand pile (counting from 1)
 * TODO: Implement this function
 * Reference implementation approach:
    * 1. Find the corresponding hand card node
    * 2. Remove the card from the hand pile
    * 3. Execute the card's effect
    * 4. If it's a score card or skill card, move it to the discard pile; if it's a power card, delete it
 */
void GameController::play(int card_to_play) {
    if (card_to_play < 1 || card_to_play > game_state_.hand_.size_) return;
    
    Node* prev = nullptr;
    Node* current = game_state_.hand_.head_;
    
    for (int i = 1; i < card_to_play; i++) {
        prev = current;
        current = current->next;
    }
    
    // Remove from hand
    if (prev == nullptr) {
        game_state_.hand_.head_ = current->next;
        if (game_state_.hand_.head_ == nullptr) {
            game_state_.hand_.tail_ = nullptr;
        }
    } else {
        prev->next = current->next;
        if (current == game_state_.hand_.tail_) {
            game_state_.hand_.tail_ = prev;
        }
    }
    game_state_.hand_.size_--;
    
    Card* card = current->card;
    delete current;
    
    // Execute card effect
    card->play(game_state_);
    
    // Check card type and handle accordingly
    ScoreCard* score_card = dynamic_cast<ScoreCard*>(card);
    SkillCard* skill_card = dynamic_cast<SkillCard*>(card);
    PowerCard* power_card = dynamic_cast<PowerCard*>(card);
    
    if (score_card != nullptr || skill_card != nullptr) {
        game_state_.discard_pile_.appendToTail(card);
    } else if (power_card != nullptr) {
        delete card;
    }
}

/**
 * Shuffle in the specified manner
 * TODO: Implement this function
 */
void GameController::shuffle() {
    shuffle_(game_state_);
}

/**
 * End the current turn, move all cards in the hand pile to the discard pile
 * TODO: Implement this function
 */
void GameController::finish() {
    while (!game_state_.hand_.empty()) {
        Card* card = game_state_.hand_.removeHead();
        game_state_.discard_pile_.appendToTail(card);
    }
}

/**
 * Query current total score
 * TODO: Implement this function
 */
int GameController::queryScore() {
    return game_state_.total_score_;
}

#endif //GAME_H
