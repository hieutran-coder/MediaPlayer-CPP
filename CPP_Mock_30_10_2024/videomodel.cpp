#include "videomodel.h"
extern "C" {
#include <libavformat/avformat.h>
#include<libavcodec/avcodec.h>
}
#include<taglib/fileref.h>
#include<taglib/tag.h>
#include<taglib/audioproperties.h>
#include<string>
#include<fstream>
#include<iomanip>
#include<filesystem>

long getFileSize(const std::string& fileName){
    std::ifstream file(fileName, std::ios::binary|std::ios::ate);
    return file.tellg();

}

VideoModel::VideoModel() {


    std::filesystem::path relativePath("videos");
    std::filesystem::path absolutePath = std::filesystem::path(relativePath);
    std::cout << "Absolute File Path: " << absolutePath << std::endl;

    for (const auto & entry : std::filesystem::recursive_directory_iterator(absolutePath.string())) {
        if(std::filesystem::is_regular_file(entry)){

            std::cout<< entry.path().filename().string()<<std::endl;
            Video* video = new Video();

            // VIDEO Metadata
            avformat_network_init();
            AVFormatContext* formatContext = nullptr;
            std::string fileName =  entry.path().filename().string();

            // Open file input
            if(avformat_open_input(&formatContext, entry.path().c_str(), nullptr, nullptr) != 0){
                std::cerr << "Could not open file: " << fileName << std::endl;
                return;
            }

            // Retrieve stream info
            if(avformat_find_stream_info(formatContext, nullptr)<0){
                std::cerr << "Could not open file stream info: " << fileName << std::endl;
                avformat_close_input(&formatContext);
                return;
            }
            std::cout<<"Video Name: "<<fileName<<std::endl;
            video->setFilePath(entry.path());
            video->setFileName(fileName);
            std::cout<<"Video size: "<<getFileSize(entry.path())<<" bytes"<<std::endl;
            video->setSize(getFileSize(entry.path()));
            std::cout<<"Duration: "<<static_cast<double>(formatContext->duration)/AV_TIME_BASE<<" seconds"<<std::endl;
            video->setDuration(static_cast<double>(formatContext->duration)/AV_TIME_BASE);
            for(unsigned int i = 0; i < formatContext->nb_streams; ++i){
                AVStream* stream = formatContext->streams[i];
                AVCodecParameters* codecParams = stream->codecpar;

                // Kiểm tra nếu stream là video stream
                if(codecParams->codec_type == AVMEDIA_TYPE_VIDEO)
                {
                    std::cout<<"BitRate: "<<codecParams->bit_rate<<" bít per seconds"<<std::endl;
                    video->setBitRate(codecParams->bit_rate);
                    std::cout<<"Codec: "<<avcodec_get_name(codecParams->codec_id)<<std::endl;
                    video->setCodec(avcodec_get_name(codecParams->codec_id));
                    break;
                }
            }

            // Clean
            avformat_close_input(&formatContext);
            avformat_network_deinit();

            listOfVideo.push_back(video);
        }

    }

}


VideoModel *VideoModel::getInstance()
{
    static VideoModel instance;
    return &instance;
}

VideoModel::~VideoModel()
{
    for(int i = 0; i<listOfVideo.size(); i++){
        std::cout<<"Delete video "<<i<<std::endl;
        delete listOfVideo[i];
    }
}


int VideoModel::getSize()
{
    return listOfVideo.size();
}

std::vector<Video *> VideoModel::getListOfVideo() const
{
    return listOfVideo;
}

void VideoModel::setListOfVideo(const std::vector<Video *> &newListOfVideo)
{
    listOfVideo = newListOfVideo;
}





