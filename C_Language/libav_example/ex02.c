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

avcodec_register_all();
    avdevice_register_all();
    AVFormatContext* context = avformat_alloc_context();
    AVInputFormat *fmt = av_find_input_format("video4linux");
    printf("trying to open input");
    int err = avformat_open_input(&context, "/dev/video0", fmt, NULL);
    if(err != 0){
        fprintf(stderr, "ffmpeg failed to open input");
    }
    static struct AVDeviceInfoList* devices_list;
    avdevice_list_devices(context, &devices_list);
    int devices_count = devices_list->nb_devices;
    for(int i = 0; i < devices_count; i++){
        printf("Checking device nr. %d \n", i);
        AVDeviceInfo* current_device_info = devices_list->devices[i];

       printf("Find Device: %s", current_device_info->device_description);
    }

    avformat_free_context(context);
}
