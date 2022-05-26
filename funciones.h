/**
* @file funciones.h
* @version 1.0
* @date 24/02/2022
* @author Diego Mallada
* @title info streams
* @brief funciones de ayuda
* @code
	int main(){
	AVFormatContext* pFormatCtx = NULL;
	const char* fileName = "../../big_buck_bunny.mp4";

	if (avformat_open_input(&pFormatCtx, fileName, NULL, NULL))
		return -1;

	if (avformat_find_stream_info(pFormatCtx, NULL) < 0)
		return -1;

	int videoStream = get_stream(pFormatCtx, AVMEDIA_TYPE_VIDEO);
	int audioStream = get_stream(pFormatCtx, AVMEDIA_TYPE_AUDIO);

	get_info_video(pFormatCtx, videoStream);
	get_info_audio(pFormatCtx, audioStream);

	avformat_close_input(&pFormatCtx);

	return 0;
	}


* @endcode

*/




#ifndef FUNCIONES_H
#define FUNCIONES_H

extern "C" {
#include <libavutil/frame.h>
#include <libavutil/mem.h>
#include <libavcodec/avcodec.h>
#include <libavutil/opt.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#include <libavformat/avformat.h>
#include <libavutil/timestamp.h>
#include <libavfilter/avfilter.h>
#include <libavfilter/buffersink.h>
#include <libavfilter/buffersrc.h>
#include <libswresample/swresample.h>
#include <libavutil/audio_fifo.h>
#include <libavdevice/avdevice.h>
#include <libavutil/avutil.h>
#include <libavutil/time.h>
#include <libavutil/opt.h>
#include <libswscale/swscale.h>
#include <stdio.h>

}



#ifndef STRUCT_H
#define STRUCT_H

struct VideoParameters
{
	int bit_rate_video;
	int channels;
	int height;
	int width;
	int gop_size;


	AVCodecID codec;
	AVPixelFormat pix_fmt;

	AVRational codec_timebase;
	AVRational stream_timebase;
	AVRational codec_framerate;
};

struct AudioParameters
{
	int nb_samples;
	int bitrate;
	int channel_layout;
	int nb_channels;

	AVCodecID codec_id;
	AVSampleFormat sample_fmt;

	AVRational codec_timebase;
	AVRational stream_timebase;
};



#endif // !STRUCT_H


/**
* @brief get_stream obtiene los streams de un archivo de video
* @param pFormatCtx context que contiene la información de los streams
* @param mediaType indica si el tipo de stream es de audio o de video
* @return stream de audio o video
*/
int get_stream(AVFormatContext* pFormatCtx, char mediaType);

/**
* @brief get_info_video Imprime por pantalla la información sobre el stream de video
* @param pFormatContext context que contiene la información de los streams
* @param videoStream stream del que sacar la información
* @param video struct en la que se cargan los datos extraídos del stream
* @return no hay return
*/
void get_info_video(AVFormatContext* pFormatCtx, int videoStream, VideoParameters& video);

/**
* @brief get_info_video Imprime por pantalla la información sobre el stream de audio
* @param pFormatContext context que contiene la información de los streams
* @param audioStream stream del que sacar la información
* @param audio struct en la que se cargan los datos extraídos del stream
* @return no hay return
*/
void get_info_audio(AVFormatContext* pFormatCtx, int audioStream, AudioParameters& audio);







#endif