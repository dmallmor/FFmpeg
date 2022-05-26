#include "ImageLoader.h"
#include <iostream>

ImageLoader::ImageLoader()
{
    stream   = nullptr;
    codec    = nullptr;
    codecCtx = nullptr;

    fmtCtx = avformat_alloc_context();
    pkt    = av_packet_alloc();
    frame  = av_frame_alloc();

    if (!fmtCtx || !pkt || !frame)
        throw std::bad_alloc();
}

ImageLoader::~ImageLoader()
{
    av_packet_unref(pkt);
    av_packet_free(&pkt);

    av_frame_unref(frame);
    av_frame_free(&frame);

    avcodec_free_context(&codecCtx);
    
    avformat_close_input(&fmtCtx);
    avformat_free_context(fmtCtx);
}

AVFrame* ImageLoader::LoadImage(const char* image_path, AVCodecContext** _codec)
{
    int ret = avformat_open_input(&fmtCtx, image_path, NULL, NULL);
    if (ret < 0)
        return nullptr;

    for (unsigned int i = 0; i < fmtCtx->nb_streams; i++)
    {
        if (fmtCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            stream = fmtCtx->streams[i];
            break;
        }
    }
    if (!stream)
        return nullptr;

    codec = avcodec_find_decoder(stream->codecpar->codec_id);
    if (!codec)
        return nullptr;

    codecCtx = avcodec_alloc_context3(codec);
    if (!codecCtx)
        return nullptr;
    codecCtx->codec_id = stream->codecpar->codec_id;

    ret = avcodec_open2(codecCtx, NULL, NULL);
    if (ret < 0)
        return nullptr;

    ret = av_read_frame(fmtCtx, pkt);
    if (ret == AVERROR_EXIT || ret < 0)
        return nullptr;

    ret = avcodec_send_packet(codecCtx, pkt);
    if (ret < 0)
        return nullptr;

    ret = avcodec_receive_frame(codecCtx, frame);
    if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF || ret < 0)
        return nullptr;

    std::cout << "pix_fmt=" << av_get_pix_fmt_name(codecCtx->pix_fmt) << std::endl;
    *_codec = codecCtx;
    return frame;
}
