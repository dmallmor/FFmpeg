#include "ImageLoader.h"
#include "OverlayFilter.h"
#include "funciones.h"
#include <iostream>
#include <stdexcept>
#include "VideoDecoder.h"
#include "Logo.h"


static void encode(AVCodecContext* enc_ctx, AVFrame* frame, AVPacket* pkt,
	FILE* outfile)
{
	int ret;
	/* send the frame to the encoder */
	
	ret = avcodec_send_frame(enc_ctx, frame);
	if (ret < 0) {
		fprintf(stderr, "Error sending a frame for encoding\n");
		exit(1);
	}
	while (ret >= 0) {
		ret = avcodec_receive_packet(enc_ctx, pkt);
		if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
			return;
		else if (ret < 0) {
			fprintf(stderr, "Error during encoding\n");
			exit(1);
		}
		
		fwrite(pkt->data, 1, pkt->size, outfile);
		av_packet_unref(pkt);
	}
}



int main(int argc, char** argv) {

	const char* pathImage = "../../logo.png";
	const char* source_path = "../../big_buck_bunny.mp4";
	const char* fileName = "../../big_buck_bunny.mp4";



	AVFormatContext* pFormatCtx = NULL;
	AVFormatContext* formatCtx_out = NULL;

	const AVCodec* codec_video;
	AVCodecContext* codec_ctx_video = NULL;


	if (avformat_open_input(&pFormatCtx, fileName, NULL, NULL))
		return -1; // Couldn't open file


	if (avformat_find_stream_info(pFormatCtx, NULL) < 0)
		return -1; // Couldn't find stream information

	int videoStream = get_stream(pFormatCtx, AVMEDIA_TYPE_VIDEO);
	
	AVCodecContext* c = NULL;
	AVRational time_base;

	AVStream* stream_src = pFormatCtx->streams[videoStream];

	codec_video = avcodec_find_encoder(stream_src->codecpar->codec_id); // No da el mismo resultado
	codec_ctx_video = avcodec_alloc_context3(codec_video);



	time_base = codec_ctx_video->time_base;


	OverlayPosition overlayPosition{};
	OverlayPosition overlayPositionText{};

	overlayPosition.x = 155;
	overlayPosition.y = 155;

	overlayPositionText.x = 50;
	overlayPositionText.y = 50;



	AVCodecContext* codec_ovr = nullptr;
	
	
	AVFrame* frame_ovr = nullptr;
	AVFrame* frame_src = nullptr;
	AVFrame* frame_out = NULL;


	AVFrame* frame_text = nullptr;
	AVFrame* frame_out_final = nullptr;

	//const char* fontpath = "C:/Master/Practicas/Visual Studio/test1";
	const char* text = "UTEK";
	int fontsize = 24;
	

	frame_out = av_frame_alloc();
	frame_out_final = av_frame_alloc();



	ImageLoader imageLoader;
	OverlayFilter overlayFilter;
	


	frame_ovr = imageLoader.LoadImage(pathImage, &codec_ovr);
	AVFormatContext* ctx = avformat_alloc_context();
	if (!ctx)
		throw std::bad_alloc();
	if (avformat_open_input(&ctx, source_path, NULL, NULL) < 0)
		throw std::exception("Cannot open source");


	const char* filename_out, * codec_name;
	const AVCodec* codec_out;
	AVCodecContext* c_out = NULL;
	int ret;
	FILE* f;
	AVFrame* frame;
	AVPacket* pkt;
	uint8_t endcode[] = { 0, 0, 1, 0xb7 };


	filename_out = "video_filtrado.mp4";
	codec_name = "h264";

//	if (avformat_open_input(&formatCtx_out, filename_out, NULL, NULL))
//		return -1; // Couldn't open file

//	codec_out = avcodec_find_encoder(stream_src->codecpar->codec_id);
	//if (!codec_out) {
	//	fprintf(stderr, "Codec '%s' not found\n", codec_name);
	//	exit(1);
	//}
	//c_out = avcodec_alloc_context3(codec_out);


	//pkt = av_packet_alloc();



	//c_out->bit_rate = codec_ctx_video->bit_rate;
	///* resolution must be a multiple of two */
	//c_out->width = 640;
	//c_out->height = 360;
	///* frames per second */
	//c_out->time_base = stream_src->time_base;
	//c_out->framerate = codec_ctx_video->framerate;
	///* emit one intra frame every ten frames
	// * check frame pict_type before passing frame
	// * to encoder, if frame->pict_type is AV_PICTURE_TYPE_I
	// * then gop_size is ignored and the output of encoder
	// * will always be I frame irrespective to gop_size
	// */
	//c_out->gop_size = codec_ctx_video->gop_size;
	//c_out->max_b_frames = codec_ctx_video->max_b_frames;
	//c_out->pix_fmt = AV_PIX_FMT_YUV420P;


	//if (codec_out->id == AV_CODEC_ID_H264)
	//	av_opt_set(c_out->priv_data, "preset", "fast", 0);
	///* open it */
	//ret = avcodec_open2(c_out, codec_out, NULL);
	//errno_t err;

	//err = fopen_s(&f, filename_out, "wb");
	//if (!f) {
	//	fprintf(stderr, "Could not open %s\n", filename_out);
	//	exit(1);
	//}
	//frame = av_frame_alloc();


	//frame->format = c->pix_fmt;
	//frame->width = c->width;
	//frame->height = c->height;
	//ret = av_frame_get_buffer(frame, 32);






	try
	{
		VideoDecoder decoder(ctx);
		decoder.InitializeDecoder();

		AVRational time_base_src;


	//	if (codec_out->id == AV_CODEC_ID_H264)
	//		av_opt_set(c_out->priv_data, "preset", "slow", 0);
		/* open it */
	//	ret = avcodec_open2(c_out, codec_out, NULL);
	//	errno_t err;

	//	err = fopen_s(&f, filename_out, "wb");
	//	if (!f) {
	//		fprintf(stderr, "Could not open %s\n", filename_out);
	//		exit(1);
	//	}
	//	avformat_write_header(formatCtx_out, NULL);
		
		while (true)
		{

			
			AVFrame* frame_src = decoder.DecodeFrame();
			
			AVCodecContext* codec_src = decoder.m_codecCtx;
			
			if (codec_src->time_base.num != 0)
				time_base_src = codec_src->time_base;
			else
				time_base_src = stream_src->time_base;

			

			
			overlayFilter.InitFilter(time_base_src, codec_src, codec_ovr, overlayPosition, overlayPositionText, text, fontsize);

			overlayFilter.Filter(frame_src, frame_ovr, &frame_out);

			
			// -------------------------------------------------------
			// Hacer algo con el frame


			std::cout << "new frame: resolution=" << frame_out_final->width
				<< "x" << frame_out_final->height << std::endl;
			
			// ...
			// -------------------------------------------------------

			ret = av_frame_make_writable(frame_out);
				if (ret < 0)
					exit(1);


		//	encode(c_out, frame_out, pkt, f);

			// Libera los recursos
			av_frame_unref(frame_src);
			av_frame_free(&frame_src);
		}
		/* flush the encoder */
		//encode(c_out, NULL, pkt, f);


		/* add sequence end code to have a real MPEG file */
	//	fwrite(endcode, 1, sizeof(endcode), f);
	/*	fclose(f);
		avcodec_free_context(&c_out);
		av_frame_free(&frame);
		av_packet_free(&pkt);*/
		return 0;




	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return -1;
	}


	avformat_free_context(formatCtx_out);
	avformat_close_input(&formatCtx_out);



}