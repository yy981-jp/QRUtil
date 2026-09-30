#pragma once

#include <ZXingCpp.h>

#include <fstream>
#include <unordered_map>


struct WriteCtx {
	bool textMode = false;
	bool fileMode = false;
	std::string ofile = {};
	ZXing::BarcodeFormat format;
};

struct WriteFormat {
	std::string name;
	ZXing::BarcodeFormat format;
};


void write(const WriteCtx& ctx, const std::string& target) {
	std::string text;
	if (ctx.textMode)
		text = std::move(target);
	else {
		std::ifstream ifs(target);
		if (!ifs) throw std::runtime_error("File open error");

		text.assign((std::istreambuf_iterator<char>(ifs)),
					std::istreambuf_iterator<char>());
	}

	auto barcode = ZXing::CreateBarcodeFromText(
		"Hello, world!",
		ZXing::BarcodeFormat::QRCode
	);

	auto image = ZXing::WriteBarcodeToImage(
		barcode,
		ZXing::WriterOptions()
			.scale(10)
			.addQuietZones(true)
	);



}