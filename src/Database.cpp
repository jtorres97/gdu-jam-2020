#include "Database.h"

#include <SDL2/SDL_log.h>

int HighScoreCallback(void *output, int count, char **data, char **columns)
{
    char *v = data[0];
    if (v != NULL)
    {
        *(int *)output = (int)std::atoi(v);
    }
    return 0;
}

Database::Database()
{
}

void Database::Open()
{
    if (sqlite3_open_v2(DB_NAME.c_str(), &m_dbHandle, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL) != SQLITE_OK)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to open SQLite database: %s", sqlite3_errmsg(m_dbHandle));
        exit(1);
    }

    // Create table
    if (sqlite3_exec(m_dbHandle, DB_CREATE_SCORES_TABLE.c_str(), 0, 0, NULL) != SQLITE_OK)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create scores table: %s", sqlite3_errmsg(m_dbHandle));
    }
}

void Database::Close()
{
}

void Database::AddScore(int level, int score)
{
    std::string DB_INSERT_SCORE = "INSERT INTO scores (difficulty, score) VALUES(" + std::to_string(level) + ", " + std::to_string(score) + ");";
    if (sqlite3_exec(m_dbHandle, DB_INSERT_SCORE.c_str(), 0, 0, NULL) != SQLITE_OK)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to insert score: %s", sqlite3_errmsg(m_dbHandle));
    }
}

int Database::GetHighScore(int difficulty)
{
    std::string query = "SELECT MAX(score) FROM scores WHERE difficulty = " + std::to_string(difficulty) + ";";

    int highScore = 0;

    sqlite3_exec(m_dbHandle, query.c_str(), HighScoreCallback, &highScore, NULL);

    return highScore;
}