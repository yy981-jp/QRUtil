#include <zbar.h>
#include <opencv2/opencv.hpp>
#include <iostream>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Usage: QRR <ImagePath>" << std::endl;
        return -1;
    }

    // QRコードを含む画像を読み込む
    cv::Mat img = cv::imread(argv[1]);
    if (img.empty()) {
        std::cerr << "Could not open or find the image." << std::endl;
        return -1;
    }

    // 画像をグレースケールに変換
    cv::Mat gray;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);

    // ZBarのImageScannerを作成
    zbar::ImageScanner scanner;
    scanner.set_config(zbar::ZBAR_QRCODE, zbar::ZBAR_CFG_ENABLE, 1);

    // OpenCVのMatをZBarのImageに変換
    int width = gray.cols;
    int height = gray.rows;
    zbar::Image image(width, height, "Y800", gray.data, width * height);

    // スキャンを実行
    int n = scanner.scan(image);

    // 認識結果を表示
    if (n > 0) {
        for (zbar::Image::SymbolIterator symbol = image.symbol_begin(); symbol != image.symbol_end(); ++symbol) {
            std::cout << "QR Code data: " << symbol->get_data() << std::endl;
        }
    } else {
        std::cout << "No QR code detected." << std::endl;
    }

    return 0;
}
