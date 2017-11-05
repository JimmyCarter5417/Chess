#include "resmgr.h"
#include <QString>
#include <QPixmap>

ResMgr* ResMgr::getInstance()
{
    static ResMgr mgr;
    return &mgr;
}

ResMgr::ResMgr()
    :board_(nullptr)
{

}

ResMgr::~ResMgr()
{
    if (board_)
    {
        delete board_;
        board_ = nullptr;
    }

    for (auto& elem: pieces_)
    {
        if (elem.second != nullptr)
        {
            delete elem.second;
            elem.second = nullptr;
        }
    }
}

bool ResMgr::loadPieces(EPieceSkin skin)
{
    struct TPieceSkinInfo
    {
        EPieceSkin skin;
        const char* path;
    };

    struct TPieceInfo
    {
        EPiece piece;
        const char* name;
    };

    static TPieceSkinInfo allPieceSkinInfo[] =
    {
        {EPS_Delicate, ":/piece/delicate/res/piece/deicate/"},
        {EPS_Polish,   ":/piece/polish/res/piece/polish/"},
        {EPS_Wood,     ":/piece/wood/res/piece/wood/"},
    };

    static TPieceInfo allPieceInfo[] =
    {
        {EP_RedKing,      "RK.GIF"},
        {EP_RedAdvisor,   "RA.GIF"},
        {EP_RedBishop,    "RB.GIF"},
        {EP_RedKnight,    "RN.GIF"},
        {EP_RedRook,      "RR.GIF"},
        {EP_RedCannon,    "RC.GIF"},
        {EP_RedPawn,      "RP.GIF"},

        {EP_BlackKing,    "BK.GIF"},
        {EP_BlackAdvisor, "BA.GIF"},
        {EP_BlackBishop,  "BB.GIF"},
        {EP_BlackKnight,  "BN.GIF"},
        {EP_BlackRook,    "BR.GIF"},
        {EP_BlackCannon,  "BC.GIF"},
        {EP_BlackPawn,    "BP.GIF"},

        {EP_Empty,        "OO.GIF"},
        {EP_Select,       "OOS.GIF"},
    };

    QString strPrefix;//get prefix first
    for (int i = 0; i < sizeof(allPieceSkinInfo) / sizeof(TPieceSkinInfo); i++)
    {
        if (allPieceSkinInfo[i].skin == i)
        {
            strPrefix = allPieceSkinInfo[skin].path;
            break;
        }
    }

    if (strPrefix.isEmpty())
    {
        return false;
    }

    //初始化棋子
    for (int i = 0; i < sizeof(allPieceInfo) / sizeof(TPieceInfo); i++)
    {
        EPiece piece = allPieceInfo[i].piece;

        if (pieces_[piece] == nullptr)
        {
            pieces_[piece] = new QPixmap;
        }

        if (!pieces_[piece]->load(strPrefix + allPieceInfo[i].name))
        {
            return false;
        }
    }

    return true;
}

bool ResMgr::loadBoard(EBoardSkin skin)
{
    struct TBoardSkinInfo
    {
        EBoardSkin skin;
        const char* path;
    };

    static TBoardSkinInfo allBoardSkinInfo[] =
    {
        {EBS_Canvas,   ":/board/res/board/CANVAS.GIF"},
        {EBS_Drops,    ":/board/res/board/DROPS.GIF"},
        {EBS_Green,    ":/board/res/board/GREEN.GIF"},
        {EBS_Qianhong, ":/board/res/board/QIANHONG.GIF"},
        {EBS_Sheet,    ":/board/res/board/SHEET.GIF"},
        {EBS_Skeleton, ":/board/res/board/SKELETON.GIF"},
        {EBS_White,    ":/board/res/board/WHITE.GIF"},
        {EBS_Wood,     ":/board/res/board/WOOD.GIF"}
    };

    bool ret = false;

    for (int i = 0; i < sizeof(allBoardSkinInfo) / sizeof(TBoardSkinInfo); i++)
    {
        if (allBoardSkinInfo[i].skin == i)
        {
            if (board_ == nullptr)
            {
                board_ = new QPixmap;
            }

            ret = board_->load(allBoardSkinInfo[skin].path);
            break;
        }
    }

    return ret;
}

QPixmap* ResMgr::getBoard()
{
    return board_;
}

QPixmap* ResMgr::getPiece(EPiece piece)
{
    return pieces_[piece];
}
