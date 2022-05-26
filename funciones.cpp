#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
using namespace std;


/********************************************//**
 *  Se añaden todas las librerías
 ***********************************************/


#include "funciones.h"


/********************************************//**
 * Se pasan el pFormatCtx y el tipo de stream que se quiere obtener
 ***********************************************/


int get_stream(AVFormatContext* pFormatCtx, char mediaType) {

	int stream = -1;

	for (unsigned int i = 0; i < pFormatCtx->nb_streams; i++)
	{

		if (pFormatCtx->streams[i]->codecpar->codec_type == mediaType)
		{
			stream = i;
			break;
		}

	}

	if (stream == -1)
		cout << "No video stream\n";



	return stream;

}


/********************************************//**
 *  Se pasa por parámetro el stream para obtener la información de este
 ***********************************************/






void get_info_video(AVFormatContext* pFormatCtx, int videoStream, VideoParameters& videoParameters) {
	float result = 0.0;

	const AVCodec* codec;
	AVCodecContext* c = NULL;

	AVStream* streamVideo = pFormatCtx->streams[videoStream];

	int num = streamVideo->r_frame_rate.num;
	int den = (pFormatCtx->streams[videoStream]->r_frame_rate).den;
	AVRational frame_rate = pFormatCtx->streams[videoStream]->avg_frame_rate;
	result = num / den;



	videoParameters.height = streamVideo->codecpar->height;
	videoParameters.width = streamVideo->codecpar->width;
	
//	videoParameters.channels = ;
	videoParameters.bit_rate_video = streamVideo->codecpar->bit_rate;

	videoParameters.codec = streamVideo->codecpar->codec_id;
	string codecName = avcodec_get_name(videoParameters.codec);
	//char codec_name = codecName;

	char buffer[100];
	strcpy_s(buffer, codecName.c_str());
	
	codec = avcodec_find_encoder_by_name(buffer); //"h264"
	//codec = avcodec_find_encoder(videoParameters.codec); // No da el mismo resultado
	c = avcodec_alloc_context3(codec);
	videoParameters.gop_size = c->gop_size;
	videoParameters.pix_fmt = c->pix_fmt;
	//videoParameters.channels = c->channels;
	
	

	videoParameters.codec_timebase = c->time_base;
	videoParameters.stream_timebase = streamVideo->time_base;
	videoParameters.codec_framerate = streamVideo->r_frame_rate;


	


}

void get_info_audio(AVFormatContext* pFormatCtx, int audioStream, AudioParameters& audioParameters) {

	
	const AVCodec* codec;
	AVCodecContext* c = NULL;

	AVStream* streamAudio = pFormatCtx->streams[audioStream];
	

	audioParameters.nb_samples = streamAudio->codecpar->sample_rate;
	audioParameters.bitrate = streamAudio->codecpar->bit_rate;
	audioParameters.channel_layout = streamAudio->codecpar->channel_layout;
	audioParameters.nb_channels = streamAudio->codecpar->channels;

	audioParameters.codec_id = streamAudio->codecpar->codec_id;

	string codecName = avcodec_get_name(audioParameters.codec_id);
	char buffer[100];
	strcpy_s(buffer, codecName.c_str());

	//codec = avcodec_find_encoder_by_name("aac"); //"aac"
	codec = avcodec_find_encoder(audioParameters.codec_id);
	c = avcodec_alloc_context3(codec);

	

	audioParameters.stream_timebase = streamAudio->time_base;
	audioParameters.codec_timebase = c->time_base;
	audioParameters.sample_fmt = c->sample_fmt;
	c->sample_rate;
	
}
