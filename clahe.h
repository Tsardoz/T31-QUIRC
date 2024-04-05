/******** Prototype of CLAHE function. Put this in a separate include file. *****/
int CLAHE(kz_pixel_t* pImage, unsigned int uiXRes, unsigned int uiYRes, kz_pixel_t Min,
	  kz_pixel_t Max, unsigned int uiNrX, unsigned int uiNrY,
	  unsigned int uiNrBins, float fCliplimit);