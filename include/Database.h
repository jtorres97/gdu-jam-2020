#pragma once

#include <string>

#include <sqlite3.h>

const std::string DB_NAME = "scores.db";
const std::string DB_CREATE_SCORES_TABLE = "CREATE TABLE IF NOT EXISTS scores (id INT, difficulty INT, score INT);";

class Database
{
public:
    Database();

    void Open();
    void Close();

    void AddScore(int difficulty, int score);
    int GetHighScore(int difficulty);

private:
    sqlite3* m_dbHandle;
};