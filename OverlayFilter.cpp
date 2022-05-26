#include "OverlayFilter.h"

OverlayFilter::OverlayFilter()
{
	bufferSrc = avfilter_get_by_name("buffer");			// source de vídeo
	bufferOvr = avfilter_get_by_name("buffer");			// source de overlay
	bufferSink = avfilter_get_by_name("buffersink");	// sink de filtros
	bufferText = avfilter_get_by_name("drawtext");
	ovrFilter = avfilter_get_by_name("overlay");		// filtro overlay
	

	graph = avfilter_graph_alloc();
	if (!graph)
		throw std::bad_alloc();
}

OverlayFilter::~OverlayFilter()
{
    avfilter_graph_free(&graph);
}

void OverlayFilter::InitFilter(
	AVRational time_base,
	AVCodecContext* src_codec,
	AVCodecContext* ovr_codec,
	OverlayPosition ovr_pos,
	OverlayPosition ovr_pos_text,
	const char* text,
	int fontsize)
{
	int ret = 0;
	char args[512];

	// Inicialización del buffer video source (los frames decodificados se meten aquí)
	snprintf(args, sizeof(args),
		"video_size=%dx%d:pix_fmt=%d:time_base=%d/%d:pixel_aspect=%d/%d",
		src_codec->width, src_codec->height,
		src_codec->pix_fmt,
		time_base.num, time_base.den,
		src_codec->sample_aspect_ratio.num, src_codec->sample_aspect_ratio.den
	);
	ret = avfilter_graph_create_filter(
		&bufSrc0Ctx, 
		bufferSrc, 
		"in0",
		args,
		NULL,
		graph
	);
	if (ret < 0)
		throw std::exception("Cannot create filter");

	// Inicialización del buffer overlay source (la imagen overlay se inserta aquí)
	snprintf(args, sizeof(args),
		"video_size=%dx%d:pix_fmt=%d:time_base=%d/%d:pixel_aspect=%d/%d",
		ovr_codec->width, ovr_codec->height,
		ovr_codec->pix_fmt,
		time_base.num, time_base.den,
		ovr_codec->sample_aspect_ratio.num, ovr_codec->sample_aspect_ratio.den
	);
	ret = avfilter_graph_create_filter(
		&bufSrc1Ctx,
		bufferOvr,
		"in1",
		args,
		NULL,
		graph
	);

	snprintf(args, sizeof(args),
		"text=%s:fontsize=%d:x=%d:y=%d",
		 text, fontsize, ovr_pos_text.x, ovr_pos_text.y
	);
	ret = avfilter_graph_create_filter(
		&bufTextCtx,
		bufferText,
		"text",
		args,
		NULL,
		graph
	);
	if (ret < 0)
		throw std::exception("Cannot create filter");

	// Filtro overlay
	snprintf(args, sizeof(args),
		"x=%d:y=%d:repeatlast=1",
		ovr_pos.x, ovr_pos.y
	);
	ret = avfilter_graph_create_filter(
		&ovrFilterCtx,
		ovrFilter,
		"overlay",
		args,
		NULL,
		graph
	);

	if (ret < 0)
		throw std::exception("Cannot create filter");

	// Bufer sink
	ret = avfilter_graph_create_filter(
		&bufSinkCtx,
		bufferSink,
		"out",
		NULL,
		NULL,
		graph
	);
	if (ret < 0)
		throw std::exception("Cannot create filter");

	// Link all filters
	avfilter_link(bufSrc0Ctx,   0, ovrFilterCtx, 0);
	avfilter_link(bufSrc1Ctx,   0, ovrFilterCtx, 1);
	avfilter_link(ovrFilterCtx,   0, bufTextCtx, 0);
	avfilter_link(bufTextCtx, 0, bufSinkCtx,   0);
	ret = avfilter_graph_config(graph, NULL);
	if (ret < 0)
		throw std::exception("Cannot link filters");
}




void OverlayFilter::Filter(AVFrame* src, AVFrame* ovr, AVFrame** out)
{
	int ret = 0;
	
	ret = av_buffersrc_add_frame_flags(bufSrc0Ctx, src, AV_BUFFERSRC_FLAG_KEEP_REF);
	if (ret < 0)
		throw std::exception("Cannot push frame into filter buffer");

	ret = av_buffersrc_add_frame_flags(bufSrc1Ctx, ovr, AV_BUFFERSRC_FLAG_KEEP_REF);
	if (ret < 0)
		throw std::exception("Cannot push frame into filter buffer");

	while (true)
	{
		ret = av_buffersink_get_frame(bufSinkCtx, *out);
		if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
			break;
		if (ret < 0)
			throw std::exception("Cannot retrieve frame from filter");
	}
}


