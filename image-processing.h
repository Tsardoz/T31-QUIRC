#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include <imp/imp_framesource.h> // Include for IMPFSChnAttr definition

#ifdef __cplusplus
extern "C" {
#endif

int process_frame(void* arg, IMPFSChnAttr fsChnAttr);
//int process_frame(IMPFrameInfo* frame, IMPFSChnAttr attr);

#ifdef __cplusplus
}
#endif

#endif