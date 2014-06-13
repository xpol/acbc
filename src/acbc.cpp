#include <cstdarg>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <set>
#include <iterator>     // std::ostream_iterator
#include <algorithm>	// std::copy

#pragma warning(disable: 4819)
#include "FreeImage.h"

#pragma comment( lib, "FreeImage" )


static bool is_1d_traversable(FIBITMAP* bmp)
{
	return FreeImage_GetPitch(bmp) == FreeImage_GetWidth(bmp) * FreeImage_GetBPP(bmp);
}

static void get_channel_unique_values(FIBITMAP* channel, std::set<BYTE> &channel_values)
{
	if (is_1d_traversable(channel))
	{
		unsigned w = FreeImage_GetWidth(channel);
		unsigned h = FreeImage_GetHeight(channel);
		BYTE* b = FreeImage_GetBits(channel);
		std::copy(b, b + w*h, std::inserter(channel_values, channel_values.end()));
	}
	else
	{
		unsigned w = FreeImage_GetWidth(channel);
		for (unsigned y = 0; y < FreeImage_GetHeight(channel); y++)
		{
			BYTE* b = FreeImage_GetScanLine(channel, y);
			std::copy(b, b + w, std::inserter(channel_values, channel_values.end()));
		}
	}
}

void load_unique_alpha_values(const std::string& filename, std::set<BYTE>& values)
{
	FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(filename.c_str(), 0);
	if (fif == FIF_UNKNOWN) {
		fif = FreeImage_GetFIFFromFilename(filename.c_str());
	}
	// check that the plugin has reading capabilities ...
	if ((fif == FIF_UNKNOWN) || !FreeImage_FIFSupportsReading(fif))
		return;

	FIBITMAP* im = FreeImage_Load(fif, filename.c_str(), 0);
	auto pal = FreeImage_GetPalette(im);
	if (pal)
	{
		if (FreeImage_IsTransparent(im))
		{
			values.insert(255);
			values.insert(0);
		}
	}
	else
	{
		if (FreeImage_IsTransparent(im))
		{
			FIBITMAP* alpha = FreeImage_GetChannel(im, FICC_ALPHA);
			get_channel_unique_values(alpha, values);
			FreeImage_Unload(alpha);
		}
	}

	FreeImage_Unload(im);
}


static int get_required_bits(const std::set<BYTE>& values)
{
	switch (values.size())
	{
	case 0:// in case all imput image don't have that channel?
		return 0;
	case 1:
		if (values.count(255) == 1)
			return 0;
		break;
	case 2:
		if (values.count(255) == 1 && values.count(0) == 1)
			return 1;
		break;
	default:
		break;
	}
	return 8;
}

static bool usage(const char* message = NULL,...)
{

	if (message)
	{
		std::vector<char> buf(4096);
		va_list args;
		va_start(args, message);
		vsnprintf(&buf.front(), buf.size(), message, args);
		va_end(args);
		std::copy(buf.begin(), buf.end(), std::ostream_iterator<char>(std::cerr, ""));
		std::cerr << std::endl;
	}
	std::cerr << "Usage: mcbc <image>..." << std::endl;
	std::cerr << "Example: mcbc one.png two.bmp" << std::endl;
	return false;
}


int main(int argc, char **argv)
{
	if (argc <= 1)
	{
		usage();
		return EXIT_FAILURE;
	}
	std::vector<std::string> filenames(argv+1, argv+argc);

	std::set<BYTE> values;
	for (auto& f : filenames)
		load_unique_alpha_values(f, values);

	std::cout << get_required_bits(values) << std::endl;
	return EXIT_SUCCESS;
}
