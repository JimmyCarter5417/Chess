#include "debug.h"
#include <stdio.h>
#include <QString>

void debug::printBoard(const std::vector<std::vector<def::int8>>& board, const std::string& path/* = "D:\\log.txt"*/)
{
    if (board.empty() || board.front().empty() || path.empty())
        return;

    FILE* fp = fopen(path.c_str(), "a+");
    if (fp == nullptr)
        return;

    std::string header = "    00 01 02 03 04 05 06 07 08\n";
    size_t ret = fwrite(header.c_str(), 1, header.size(), fp);

    for (unsigned int i = 0; i < board.size(); i++)
    {
        QString qstr;
        qstr.sprintf("%02d:", i);
        for (unsigned int j = 0; j < board.front().size(); j++)
        {
            char tmp[8] = {0};
            sprintf(tmp, " %02d", board[i][j]);
            qstr.append(tmp);
        }

        ret = fwrite(qstr.toStdString().c_str(), 1, qstr.size(), fp);
        ret = fwrite("\n", 1, 1, fp);
    }

    ret = fwrite("\n", 1, 1, fp);

    fclose(fp);
}
