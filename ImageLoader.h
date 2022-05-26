#pragma once

extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libavutil/pixdesc.h"
}
#include <stdexcept>

static class ImageLoader
{
public:
	ImageLoader();
	~ImageLoader();

	/**
	 * @brief Carga una imagen (png, jpg...).
	 * @param image_path Ruta de acceso de la imagen.
	 * @param[out] codec Codec de la imagen.
	 * @return imagen decodificada.
	 */
	AVFrame* LoadImage(const char* image_path, AVCodecContext** codec);
private:
	AVFormatContext* fmtCtx;
	AVStream* stream;
	const AVCodec* codec;
	AVCodecContext* codecCtx;
	AVPacket* pkt;
	AVFrame* frame;
};

