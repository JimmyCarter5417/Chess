#ifndef ZOBRIST_H
#define ZOBRIST_H

#include <stdint.h>
#include <algorithm>

// RC4密码流生成器
class RC4
{
public:
    RC4()   // 用空密钥初始化密码流生成器
    {
        x = y = 0;

        for (int i = 0; i < 256; i ++)
        {
            s[i] = i;
        }

        for (int i = 0; i < 256; i ++)
        {
            int j = (j + s[i]) & 255;
            /*uint8_t uc = s[i];
            s[i] = s[j];
            s[j] = uc;*/
            std::swap(s[i], s[j]);
        }
    }

    uint8_t NextByte()   // 生成密码流的下一个字节
    {
        x = (x + 1) & 255;
        y = (y + s[x]) & 255;
        /*uint8_t uc = s[x];
        s[x] = s[y];
        s[y] = uc;*/
        std::swap(s[x], s[y]);
        return s[(s[x] + s[y]) & 255];
    }

    uint32_t NextLong()  // 生成密码流的下四个字节
    {
        uint8_t uc0, uc1, uc2, uc3;
        uc0 = NextByte();
        uc1 = NextByte();
        uc2 = NextByte();
        uc3 = NextByte();
        return uc0 + (uc1 << 8) + (uc2 << 16) + (uc3 << 24);
    }

private:
    uint32_t x;
    uint32_t y;
    uint8_t  s[256];
};


// Zobrist结构
class Zobrist
{
public:
    Zobrist()// 用密码流填充Zobrist
    {
        RC4 rc4;

        dwKey = rc4.NextLong();
        dwLock0 = rc4.NextLong();
        dwLock1 = rc4.NextLong();
    }

    uint32_t getKey()
    {
        return dwKey;
    }

    void clear()     // 用零填充Zobrist
    {
        dwKey = dwLock0 = dwLock1 = 0;
    }

    void Xor(const Zobrist& zobr)  // 执行XOR操作
    {
        dwKey ^= zobr.dwKey;
        dwLock0 ^= zobr.dwLock0;
        dwLock1 ^= zobr.dwLock1;
    }

    void Xor(const Zobrist& zobr1, const Zobrist& zobr2)
    {
        dwKey ^= zobr1.dwKey ^ zobr2.dwKey;
        dwLock0 ^= zobr1.dwLock0 ^ zobr2.dwLock0;
        dwLock1 ^= zobr1.dwLock1 ^ zobr2.dwLock1;
    }

private:
    uint32_t dwKey;
    uint32_t dwLock0;
    uint32_t dwLock1;
};

#endif // ZOBRIST_H
