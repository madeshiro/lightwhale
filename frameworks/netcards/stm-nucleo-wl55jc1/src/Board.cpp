#include "Board.h"

void Board::Init()
{
    if (_Instance)
    {
        delete _Instance;
    }

    _Instance = new Board();
}

void Board::DeInit()
{
    delete _Instance;
    _Instance = nullptr;
}

Board *Board::GetInstance()
{
    return _Instance;
}

HMI *Board::GetHMI()
{
    return _Instance ? _Instance->_pHmi : nullptr;
}


Board::Board():
    _pHmi(new HMI())
{
}

Board::~Board()
{
    delete _pHmi;
}

Board* Board::_Instance = nullptr;




