#ifdef __cplusplus
extern "C" {
#endif

typedef struct{
int active;
int delay;
int taken;
char filename[256];
char writing_filename[256];
}SCREENSHOTER;

extern SCREENSHOTER screenshoter;

void do_screenshot();
void on_frame_screenshot();



#ifdef __cplusplus
}
#endif

