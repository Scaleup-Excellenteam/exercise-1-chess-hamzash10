//
// Created by 2022 on 7/14/2024.
//

#include "../include/Myexceptions.h"

std::shared_ptr<IllegalMoveException> IllegalMoveException::instance = nullptr;
std::shared_ptr<BoardStateException> BoardStateException::instance = nullptr;

IllegalMoveException::IllegalMoveException(const string &msg) : message(msg) {}

const char *IllegalMoveException::what() const noexcept {
    return message.c_str();
}

shared_ptr<IllegalMoveException> IllegalMoveException::getInstance(const string &msg) {
    if (instance == nullptr) {
        instance = std::shared_ptr<IllegalMoveException>(new IllegalMoveException(msg));
    } else {
        instance->message = msg;
    }
    return instance;
}

shared_ptr<IllegalMoveException> IllegalMoveException::getInstance() {
    return instance;
}

BoardStateException::BoardStateException(const string &msg) : message(msg) {}

shared_ptr<BoardStateException> BoardStateException::getInstance(const string& msg) {
    if (instance == nullptr) {
        instance = std::shared_ptr<BoardStateException>(new BoardStateException(msg));
    } else {
        instance->message = msg;
    }
    return instance;
}

shared_ptr<BoardStateException> BoardStateException::getInstance() {
    return instance;
}


const char *BoardStateException::what() const noexcept  {
    return message.c_str();
}