#pragma once
#include <stb/stb_image.h>


#include <ReadBarcode.h>
#include <ImageView.h>


ZXing::Barcode parseImg(const std::string& filename) {
	int width;
	int height;
	int channels;

	auto *data = stbi_load(
		filename.c_str(),
		&width,
		&height,
		&channels,
		4
	);

	if (!data)
		throw std::runtime_error("System couldn't read image.");

	ZXing::ImageView image(
		data,
		width,
		height,
		ZXing::ImageFormat::RGBA
	);

	auto result = ZXing::ReadBarcode(image);

	stbi_image_free(data);

	if (!result.isValid()) throw std::runtime_error("System couldn't parse image.");

	return result;
}
