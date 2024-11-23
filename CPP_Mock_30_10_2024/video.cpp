#include "video.h"
#include<filesystem>
Video::Video() {





}




int64_t Video::getBitRate() const
{
    return bitRate;
}

void Video::setBitRate(int64_t newBitRate)
{
    bitRate = newBitRate;
}

std::string Video::getCodec() const
{
    return codec;
}

void Video::setCodec(const std::string &newCodec)
{
    codec = newCodec;
}

double Video::getDuration() const
{
    return duration;
}

void Video::setDuration(double newDuration)
{
    duration = newDuration;
}

int Video::getSize() const
{
    return size;
}

void Video::setSize(int newSize)
{
    size = newSize;
}

std::string Video::getFileName() const
{
    return fileName;
}

void Video::setFileName(const std::string &newFileName)
{
    fileName = newFileName;
}

std::string Video::getFilePath() const
{
    return filePath;
}

void Video::setFilePath(const std::string &newFilePath)
{
    filePath = newFilePath;
}
