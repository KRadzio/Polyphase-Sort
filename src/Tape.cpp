#include "Tape.hpp"

Tape::Tape() { vectorOfRecord = std::vector<std::string>(BLOC_SIZE / RECORD_SIZE); }
Tape::Tape(std::string filename)
{
    vectorOfRecord = std::vector<std::string>(BLOC_SIZE / RECORD_SIZE);
    this->fileName = fileName;
}
Tape::~Tape() { vectorOfRecord.clear(); }