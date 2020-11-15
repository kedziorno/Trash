#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/hwcontext.h>
#include <libavdevice/avdevice.h>

int main(int argc, char *argv[]) {
	av_register_all();
    avcodec_register_all();
    avdevice_register_all();
av_log_set_level(AV_LOG_ERROR);
	
/*
	enum AVHWDeviceType type = AV_HWDEVICE_TYPE_NONE;
	printf("Supported hardware device types:\n");
	while ((type = av_hwdevice_iterate_types(type)) !=
		AV_HWDEVICE_TYPE_NONE)
			printf("%s\n", av_hwdevice_get_type_name(type));
	printf("\n");

	AVInputFormat *d = NULL;
	AVDictionary *opts = NULL;
	AVDeviceInfoList *device_list = malloc(sizeof(AVDeviceInfoList));
	const char *arg = "v4l2";
	char *dev = NULL;	
	//show_sinks_sources_parse_arg(arg, &dev, &opts);
	do {
		d = av_input_video_device_next(d);
		if(d) {
		avdevice_list_input_sources(d, NULL, opts, &device_list);
		for (int i = 0; i < device_list->nb_devices; i++) {
			printf("%s %s [%s]\n",
				device_list->default_device == i ? "*" : " ",
				device_list->devices[i]->device_name,
				device_list->devices[i]->device_description);
			}
		}
	} while(d); */
//AVOutputFormat * 	av_output_video_device_next (AVOutputFormat *d);
AVOutputFormat *fmt = NULL;
	AVDictionary *opts = NULL;
	AVDeviceInfoList device_list;// = malloc(sizeof(AVDeviceInfoList));
do {
fmt = av_output_video_device_next(fmt);

printf("%s\n", fmt->name);
if(fmt) {
avdevice_list_output_sinks(fmt, fmt->name, opts, &device_list);
		for (int i = 0; i < device_list.nb_devices; i++) {
			printf("%s %s\n",
				device_list.default_device == i ? "*" : " ",
				device_list.devices[i]->device_name);
			}
		}
} while(fmt);

AVFormatContext *avf =  avformat_alloc_context();
AVDeviceInfoList **avl = NULL;
avdevice_list_devices (avf, avl);
}
