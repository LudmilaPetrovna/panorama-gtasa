#ifdef __cplusplus
extern "C" {
#endif

typedef struct{
int inited;
int active;
int delay;
int taken;
char filename[256];
int is_stream;
int ffmpeg;
}SCREENSHOTER;

extern SCREENSHOTER screenshoter;

void screenshot_start(char *filename, int delay, int is_stream);
void screenshot_stop();
void on_frame_screenshot();



#ifdef __cplusplus
}
#endif

