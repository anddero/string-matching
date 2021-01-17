//
// Created by andde on 07-Jan-21.
//

#include <iostream>
#include "WordDiff.h"

const unsigned long long DEL_SCORE = 12;
const unsigned long long ADD_SCORE = 13;
const unsigned long long REPLACE_SCORE = 14;
const unsigned long long SWAP_SCORE = 11;

const bool DEBUG_LOGS = false;
const bool DEBUG_STEP_BY_STEP = false;

std::vector<Move> get_diff(const std::string& word_one, const std::string& word_two) {
    unsigned long long op_count = 0;
    std::cout << "DEBUG: max length: " << std::max(word_one.length(), word_two.length()) << std::endl;
    if (DEBUG_LOGS) {
        std::cout << std::endl << std::endl << "------------------------------------------------------" << std::endl;
        std::cout << "DEBUG: " << "Word1: " << word_one << std::endl;
        std::cout << "DEBUG: " << "Word2: " << word_two << std::endl;
    }
    unsigned long long index_one = 0; // TODO Continue, run app in DEBUG mode nad see what happens (stack overflow?)
    unsigned long long index_two = 0;
    unsigned long long score = 0;
    std::vector<Move> move_stack;
    unsigned long long best_score = std::numeric_limits<unsigned long long>::max();
    std::vector<Move> best_move_stack;
    Move last_removed_move;
    bool last_removed_move_exists = false;

    while (true) {
        ++op_count;
        if (DEBUG_LOGS) {
            int tmp;
            std::cout << std::endl;
            std::cout << "DEBUG: " << word_one << std::endl;
            std::cout << "DEBUG: " << repeat(" ", index_one) << "^ " << index_one << std::endl;
            std::cout << "DEBUG: " << word_two << std::endl;
            std::cout << "DEBUG: " << repeat(" ", index_two) << "^ " << index_two << std::endl;
            if (DEBUG_STEP_BY_STEP) std::cin >> tmp;
        }
        bool one_end = index_one == word_one.size();
        bool two_end = index_two == word_two.size();
        if (!one_end && !two_end) {
            if (word_one[index_one] == word_two[index_two]) {
                move_stack.emplace_back(score, Move::Operation::Skip, false, false, index_one, index_two);
                ++index_one;
                ++index_two;
                if (DEBUG_LOGS) {
                    std::cout << "DEBUG: " << "Pushing " << move_stack[move_stack.size() - 1] << std::endl;
                }
                continue;
            }
        }
        if (DEBUG_LOGS) {
            std::cout << "DEBUG: " << "Cannot skip" << std::endl;
        }

        // try to remove
        if (!one_end && !last_removed_move_exists) {
            // can remove
            Move move(0, Move::Operation::Remove, false, false, index_one, index_two);
            if (!move_stack.empty() && !move_stack[move_stack.size() - 1].end_of_complex) {
                // possibility to do complex move
                if (move_stack[move_stack.size() - 1].operation == Move::Operation::Insert) {
                    // replace move
                    move_stack[move_stack.size() - 1].part_of_complex = true;
                    score += REPLACE_SCORE - ADD_SCORE;
                    if (DEBUG_LOGS) {
                        std::cout << "\tScore: " << score << std::endl;
                    }
                    move.score_after_move = score;
                    move.end_of_complex = true;
                } else if (move_stack[move_stack.size() - 1].operation == Move::Operation::Skip
                           && move_stack.size() > 1 && move_stack[move_stack.size() - 2].operation == Move::Operation::Insert
                           && word_two[move_stack[move_stack.size() - 2].index_second] == word_one[index_one]) {
                    // potential swap move
                    if (!move_stack[move_stack.size() - 2].end_of_complex) {
                        // swap move with previous non-complex
                        move_stack[move_stack.size() - 2].part_of_complex = true;
                        move_stack[move_stack.size() - 1].part_of_complex = true;
                        score += SWAP_SCORE - ADD_SCORE;
                        if (DEBUG_LOGS) {
                            std::cout << "\tScore: " << score << std::endl;
                        }
                        move.score_after_move = score;
                        move.end_of_complex = true;
                    } else if (move_stack[move_stack.size() - 3].operation == Move::Operation::Remove) {
                        // swap move with previous replace move
                        move_stack[move_stack.size() - 3].part_of_complex = false;
                        move_stack[move_stack.size() - 2].end_of_complex = false;
                        move_stack[move_stack.size() - 2].part_of_complex = true;
                        move_stack[move_stack.size() - 2].score_after_move += DEL_SCORE - REPLACE_SCORE;
                        move_stack[move_stack.size() - 1].part_of_complex = true;
                        move_stack[move_stack.size() - 1].score_after_move += DEL_SCORE - REPLACE_SCORE;
                        score += DEL_SCORE - REPLACE_SCORE + SWAP_SCORE;
                        if (DEBUG_LOGS) {
                            std::cout << "\tScore: " << score << std::endl;
                        }
                        move.score_after_move = score;
                        move.end_of_complex = true;
                    }
                }
            }
            if (move.score_after_move == 0) {
                // non-complex move
                score += DEL_SCORE;
                if (DEBUG_LOGS) {
                    std::cout << "\tScore: " << score << std::endl;
                }
                move.score_after_move = score;
            }
            move_stack.push_back(move);
            ++index_one;
            if (DEBUG_LOGS) {
                std::cout << "DEBUG: " << "Pushing " << move << std::endl;
            }
            continue;
        }
        if (DEBUG_LOGS) {
            std::cout << "DEBUG: " << "Cannot remove" << std::endl;
        }

        // try to insert
        if (!two_end) {
            // move down
            Move move(0, Move::Operation::Insert, false, false, index_one, index_two);
            if (!move_stack.empty() && !move_stack[move_stack.size() - 1].end_of_complex) {
                // possibility to do complex move
                if (move_stack[move_stack.size() - 1].operation == Move::Operation::Remove) {
                    // replace move
                    move_stack[move_stack.size() - 1].part_of_complex = true;
                    score += REPLACE_SCORE - DEL_SCORE;
                    if (DEBUG_LOGS) {
                        std::cout << "\tScore: " << score << std::endl;
                    }
                    move.score_after_move = score;
                    move.end_of_complex = true;
                } else if (move_stack[move_stack.size() - 1].operation == Move::Operation::Skip
                           && move_stack.size() > 1 && move_stack[move_stack.size() - 2].operation == Move::Operation::Remove
                           && word_one[move_stack[move_stack.size() - 2].index_first] == word_two[index_two]) {
                    // potential swap move
                    if (!move_stack[move_stack.size() - 2].end_of_complex) {
                        // swap move with previous non-complex
                        move_stack[move_stack.size() - 2].part_of_complex = true;
                        move_stack[move_stack.size() - 1].part_of_complex = true;
                        score += SWAP_SCORE - DEL_SCORE;
                        if (DEBUG_LOGS) {
                            std::cout << "\tScore: " << score << std::endl;
                        }
                        move.score_after_move = score;
                        move.end_of_complex = true;
                    } else if (move_stack[move_stack.size() - 3].operation == Move::Operation::Insert) {
                        // swap move with previous replace move
                        move_stack[move_stack.size() - 3].part_of_complex = false;
                        move_stack[move_stack.size() - 2].end_of_complex = false;
                        move_stack[move_stack.size() - 2].part_of_complex = true;
                        move_stack[move_stack.size() - 2].score_after_move += ADD_SCORE - REPLACE_SCORE;
                        move_stack[move_stack.size() - 1].part_of_complex = true;
                        move_stack[move_stack.size() - 1].score_after_move += ADD_SCORE - REPLACE_SCORE;
                        score += ADD_SCORE - REPLACE_SCORE + SWAP_SCORE;
                        if (DEBUG_LOGS) {
                            std::cout << "\tScore: " << score << std::endl;
                        }
                        move.score_after_move = score;
                        move.end_of_complex = true;
                    }
                }
            }
            if (move.score_after_move == 0) {
                // non-complex move
                score += ADD_SCORE;
                if (DEBUG_LOGS) {
                    std::cout << "\tScore: " << score << std::endl;
                }
                move.score_after_move = score;
            }
            move_stack.push_back(move);
            ++index_two;
            last_removed_move_exists = false;
            if (DEBUG_LOGS) {
                std::cout << "DEBUG: " << "Pushing " << move << std::endl;
            }
            continue;
        }
        if (DEBUG_LOGS) {
            std::cout << "DEBUG: " << "Cannot insert" << std::endl;
        }

        // update best score and best stack
        if (one_end && score < best_score) {
            best_score = score;
            best_move_stack = move_stack;
            if (DEBUG_LOGS) {
                std::cout << "DEBUG: " << "Updating best score" << std::endl;
            }
        }

        // rollback stack to last place to continue from
        while (!move_stack.empty()) {
            ++op_count;
            last_removed_move = move_stack[move_stack.size() - 1];
            last_removed_move_exists = true;
            move_stack.pop_back();

            if (DEBUG_LOGS) {
                std::cout << "DEBUG: " << "Popping " << last_removed_move << std::endl;
            }

            if (last_removed_move.operation == Move::Operation::Skip) {
                --index_one;
                --index_two;
                if (DEBUG_LOGS) {
                    std::cout << "DEBUG: " << "Decreasing index_one " << index_one << std::endl;
                    std::cout << "DEBUG: " << "Decreasing index_two " << index_two << std::endl;
                }
                continue;
            }

            if (last_removed_move.end_of_complex) {
                // end of a complex move, previous moves must be updated
                Move& sec_last_move = move_stack[move_stack.size() - 1];
                if (sec_last_move.operation == Move::Operation::Skip) {
                    // end of a swap operation
                    Move& third_last_move = move_stack[move_stack.size() - 2];
                    third_last_move.part_of_complex = false;
                    sec_last_move.part_of_complex = false;
                    if (last_removed_move.operation == Move::Operation::Insert) {
                        score += DEL_SCORE - SWAP_SCORE;
                    } else {
                        score += ADD_SCORE - SWAP_SCORE;
                    }
                    if (DEBUG_LOGS) {
                        std::cout << "\tScore: " << score << std::endl;
                    }
                    // check if prev steps can be re-merged into replace
                    if (move_stack.size() > 2) {
                        // stack large enough
                        Move& fourth_last_move = move_stack[move_stack.size() - 3];
                        if (!fourth_last_move.end_of_complex) {
                            // might be possible
                            if (third_last_move.operation == Move::Operation::Insert && fourth_last_move.operation == Move::Operation::Remove) {
                                // REMOVE, then INSERT
                                fourth_last_move.part_of_complex = true;
                                third_last_move.end_of_complex = true;
                                third_last_move.score_after_move += REPLACE_SCORE - DEL_SCORE - ADD_SCORE;
                                score += REPLACE_SCORE - ADD_SCORE - DEL_SCORE;
                                if (DEBUG_LOGS) {
                                    std::cout << "\tScore: " << score << std::endl;
                                }
                            } else if (third_last_move.operation == Move::Operation::Remove && fourth_last_move.operation == Move::Operation::Insert) {
                                // INSERT, then REMOVE
                                fourth_last_move.part_of_complex = true;
                                third_last_move.end_of_complex = true;
                                third_last_move.score_after_move += REPLACE_SCORE - DEL_SCORE - ADD_SCORE;
                                score += REPLACE_SCORE - ADD_SCORE - DEL_SCORE;
                                if (DEBUG_LOGS) {
                                    std::cout << "\tScore: " << score << std::endl;
                                }
                            }
                        }
                    }
                } else {
                    // end of a replace operation
                    sec_last_move.part_of_complex = false;
                    if (last_removed_move.operation == Move::Operation::Insert) {
                        score += DEL_SCORE - REPLACE_SCORE;
                    } else {
                        score += ADD_SCORE - REPLACE_SCORE;
                    }
                    if (DEBUG_LOGS) {
                        std::cout << "\tScore: " << score << std::endl;
                    }
                }
            } else {
                if (last_removed_move.operation == Move::Operation::Remove) {
                    score -= DEL_SCORE;
                } else {
                    score -= ADD_SCORE;
                }
                if (DEBUG_LOGS) {
                    std::cout << "\tScore: " << score << std::endl;
                }
            };

            if (last_removed_move.operation == Move::Operation::Remove) {
                --index_one;
                if (DEBUG_LOGS) {
                    std::cout << "DEBUG: " << "Decreasing index_one " << index_one << std::endl;
                }
            } else if (last_removed_move.operation == Move::Operation::Insert) {
                --index_two;
                if (DEBUG_LOGS) {
                    std::cout << "DEBUG: " << "Decreasing index_two " << index_two << std::endl;
                }
            }

            // check last move on stack to know if should break while loop
            if (last_removed_move.operation == Move::Operation::Remove && index_two < word_two.size()) {
                // can add
                if (DEBUG_LOGS) {
                    std::cout << "DEBUG: " << "Breaking from rollback" << std::endl;
                }
                break;
            }
        }

        if (move_stack.empty() && last_removed_move.operation != Move::Operation::Remove) {
            // cannot make a different first move
            break;
        }
    }

    if (DEBUG_LOGS) {
        std::cout << "DEBUG: " << "Returning answer" << std::endl;
    }
    std::cout << "DEBUG: OP count: " << op_count << std::endl;
    return best_move_stack;
}

void replace(std::string &src, const std::string &what, const std::string &with) {
    auto index = src.find(what);
    while (index != std::string::npos) {
        src.replace(index, what.length(), with);
        index = src.find(what, index + with.length() + what.empty());
    }
}
