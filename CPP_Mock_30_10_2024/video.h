#ifndef VIDEO_H
#define VIDEO_H
#include<iostream>

class Video
{
public:
    Video();


    std::string getFilePath() const;
    void setFilePath(const std::string &newFilePath);
    std::string getFileName() const;
    void setFileName(const std::string &newFileName);
    int getSize() const;
    void setSize(int newSize);
    double getDuration() const;
    void setDuration(double newDuration);
    int64_t getBitRate() const;
    void setBitRate(int64_t newBitRate);

    std::string getCodec() const;
    void setCodec(const std::string &newCodec);

private:
    std::string filePath;
    std::string fileName;
    int size;
    double duration;
    int64_t bitRate;
    std::string codec;
};

#endif // VIDEO_H
