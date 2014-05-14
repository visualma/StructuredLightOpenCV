//
// Copyright (c) 2009-2011  Shuntaro Yamazaki (shun-yamazaki (at) aist.go.jp) 
//  and the National Institute of Advanced Industrial Science and Technology   
//
// $Id: decode.cpp 4546 2011-05-24 12:20:43Z shun $
//

#include "stdafx.h"

#include "decode.h"

void print_usage(const char *basename)
{
	const char *p = strrchr(basename, '\\');
	if (p)
		p++;
	else
		p = basename;
	fprintf(stderr, "Usage: %s <options> <images....>\n", p);
	exit(-1);
}

int main(int argc, char *argv[])
{
	try
	{
		options_t opciones;// = new COptions(1024, 768, 10, 4, true, false, true, false);
		// parse commandline options
	//	if (argc < 2) print_usage(argv[0]);
		std::vector<std::string> files(48);
		opciones.debug = true;
	//	for (int i=0; i<argc-2; i++)
	//		files[i] = argv[i + 2];
		files[0] = "../resources/Captures/V4GP/Capture-00.bmp";
		files[1] = "../resources/Captures/V4GP/Capture-01.bmp";
		files[2] = "../resources/Captures/V4GP/Capture-02.bmp";
		files[3] = "../resources/Captures/V4GP/Capture-03.bmp";
		files[4] = "../resources/Captures/V4GP/Capture-04.bmp";
		files[5] = "../resources/Captures/V4GP/Capture-05.bmp";
		files[6] = "../resources/Captures/V4GP/Capture-06.bmp";
		files[7] = "../resources/Captures/V4GP/Capture-07.bmp";
		files[8] = "../resources/Captures/V4GP/Capture-08.bmp";
		files[9] = "../resources/Captures/V4GP/Capture-09.bmp";
		files[10] = "../resources/Captures/V4GP/Capture-10.bmp";
		files[11] = "../resources/Captures/V4GP/Capture-11.bmp";
		files[12] = "../resources/Captures/V4GP/Capture-12.bmp";
		files[13] = "../resources/Captures/V4GP/Capture-13.bmp";
		files[14] = "../resources/Captures/V4GP/Capture-14.bmp";
		files[15] = "../resources/Captures/V4GP/Capture-15.bmp";
		files[16] = "../resources/Captures/V4GP/Capture-16.bmp";
		files[17] = "../resources/Captures/V4GP/Capture-17.bmp";
		files[18] = "../resources/Captures/V4GP/Capture-18.bmp";
		files[19] = "../resources/Captures/V4GP/Capture-19.bmp";
		files[20] = "../resources/Captures/V4GP/Capture-20.bmp";
		files[21] = "../resources/Captures/V4GP/Capture-21.bmp";
		files[22] = "../resources/Captures/V4GP/Capture-22.bmp";
		files[23] = "../resources/Captures/V4GP/Capture-23.bmp";
		files[24] = "../resources/Captures/V4GP/Capture-24.bmp";
		files[25] = "../resources/Captures/V4GP/Capture-25.bmp";
		files[26] = "../resources/Captures/V4GP/Capture-26.bmp";
		files[27] = "../resources/Captures/V4GP/Capture-27.bmp";
		files[28] = "../resources/Captures/V4GP/Capture-28.bmp";
		files[29] = "../resources/Captures/V4GP/Capture-29.bmp";
		files[30] = "../resources/Captures/V4GP/Capture-30.bmp";
		files[31] = "../resources/Captures/V4GP/Capture-31.bmp";
		files[32] = "../resources/Captures/V4GP/Capture-32.bmp";
		files[33] = "../resources/Captures/V4GP/Capture-33.bmp";
		files[34] = "../resources/Captures/V4GP/Capture-34.bmp";
		files[35] = "../resources/Captures/V4GP/Capture-35.bmp";
		files[36] = "../resources/Captures/V4GP/Capture-36.bmp";
		files[37] = "../resources/Captures/V4GP/Capture-37.bmp";
		files[38] = "../resources/Captures/V4GP/Capture-38.bmp";
		files[39] = "../resources/Captures/V4GP/Capture-39.bmp";
		files[40] = "../resources/Captures/V4GP/Capture-40.bmp";
		files[41] = "../resources/Captures/V4GP/Capture-41.bmp";
		files[42] = "../resources/Captures/V4GP/Capture-42.bmp";
		files[43] = "../resources/Captures/V4GP/Capture-43.bmp";
		files[44] = "../resources/Captures/V4GP/Capture-44.bmp";
		files[45] = "../resources/Captures/V4GP/Capture-45.bmp";
		files[46] = "../resources/Captures/V4GP/Capture-46.bmp";
		files[47] = "../resources/Captures/V4GP/Capture-47.bmp";
		CDecode decode(opciones);
		decode.Decode(files);
		decode.WriteMap(0,"h.map");
		decode.WriteMap(1,"v.map");
		decode.WriteMask("mask.bmp");
		decode.WriteReliable("reliable.bmp");
	}
	catch(const std::exception & e)
	{
		fprintf(stderr, "exception: %s\n", e.what());
		return -1;
	}

	return 0;
}
