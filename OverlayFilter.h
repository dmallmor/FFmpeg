#pragma once

extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libavutil/pixdesc.h"
#include "libavfilter/avfilter.h"
#include "libavfilter/buffersrc.h"
#include "libavfilter/buffersink.h"
#include "libavutil/opt.h"

}
#include <stdexcept>

// Posición del overlay dentro de la imagen source
struct OverlayPosition
{
	int x;
	int y;
};


class OverlayFilter
{
public:
	OverlayFilter();
	~OverlayFilter();

	/**
	 * @brief Inicializa los filtros necesarios.
	 * @param time_base Base de tiempos del codec source.
	 * @param src_codec Codec de la imagen source.
	 * @param ovr_codec Codec de la imagen overlay.
	 * @param ovr_pos Posición del overlay con respecto al src.
	 * @param ovr_pos Posición del text con respecto al src.
	 * @param text Texto a introducir.
	 * @param fontsize Tamaño de la fuente.
	 */
	void InitFilter(
		AVRational time_base,
		AVCodecContext* src_codec,
		AVCodecContext* ovr_codec,
		OverlayPosition ovr_pos,
		OverlayPosition ovr_pos_text,
		
		const char* text,
		int fontsize
	);

	/**
	 * @brief realiza el filtrado.
	 * @param src Frame source.
	 * @param ovr Frame overlay.
	 * @param[out] out Frame de salida. Debe ser previamente inicializado con av_frame_alloc().
	 */
	void Filter(AVFrame* src, AVFrame* ovr, AVFrame** out);
	

	void TextFilterInit(AVRational time_base,
		AVCodecContext* src_codec,
		OverlayPosition ovr_pos,
		const char* fontpath, 
		const char* text,
		int fontsize);

private:
	const AVFilter* bufferSrc;
	const AVFilter* bufferOvr;
	const AVFilter* bufferSink;
	const AVFilter* ovrFilter;
	const AVFilter* bufferText;

	AVFilterGraph* graph;

	AVFilterContext* bufSrc0Ctx;
	AVFilterContext* bufSrc1Ctx;
	AVFilterContext* ovrFilterCtx;
	AVFilterContext* bufSinkCtx;
	AVFilterContext* bufTextCtx;
};

